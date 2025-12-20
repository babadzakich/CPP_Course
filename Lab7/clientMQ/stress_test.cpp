#include "MQClient.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <atomic>
#include <iomanip>

class RandomGenerator {
public:
    std::mt19937 gen;
    
    RandomGenerator() : gen(std::random_device{}()) {}
    
    int getRandomDelay(int min_ms, int max_ms) {
        std::uniform_int_distribution<> dist(min_ms, max_ms);
        return dist(gen);
    }
    
    int getRandomQoS() {
        std::uniform_int_distribution<> dist(0, 2);
        return dist(gen);
    }
    
    int getRandomMessageSize(int min_size, int max_size) {
        std::uniform_int_distribution<> dist(min_size, max_size);
        return dist(gen);
    }
    
    bool shouldFail(double failure_rate) {
        std::uniform_real_distribution<> dist(0.0, 1.0);
        return dist(gen) < failure_rate;
    }
    
    std::string getRandomQueue(const std::vector<std::string>& queues) {
        std::uniform_int_distribution<> dist(0, static_cast<int>(queues.size()) - 1);
        return queues[dist(gen)];
    }
};

std::string generateRandomMessage(RandomGenerator& rng, int size) {
    static const char charset[] = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    std::string message;
    message.reserve(size);
    
    std::uniform_int_distribution<> dist(0, static_cast<int>(sizeof(charset)) - 2);
    for (int i = 0; i < size; i++) {
        message += charset[dist(rng.gen)];
    }
    
    return message;
}

struct TestStats {
    std::atomic<int> messages_sent{0};
    std::atomic<int> messages_received{0};
    std::atomic<int> failures{0};
    std::atomic<int> timeouts{0};
    
    void print() {
        std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
        std::cout << "║         Test Statistics              ║" << std::endl;
        std::cout << "╠══════════════════════════════════════╣" << std::endl;
        std::cout << "║ Messages sent:     " << std::setw(10) << messages_sent.load() << "      ║" << std::endl;
        std::cout << "║ Messages received: " << std::setw(10) << messages_received.load() << "      ║" << std::endl;
        std::cout << "║ Failures:          " << std::setw(10) << failures.load() << "      ║" << std::endl;
        std::cout << "║ Timeouts:          " << std::setw(10) << timeouts.load() << "      ║" << std::endl;
        double success_rate = messages_sent.load() > 0 
            ? (100.0 * messages_received.load() / messages_sent.load()) 
            : 0.0;
        std::cout << "║ Success rate:      " << std::setw(9) << std::fixed << std::setprecision(2) 
                  << success_rate << "%     ║" << std::endl;
        std::cout << "╚══════════════════════════════════════╝\n" << std::endl;
    }
};

void testRandomDelays(TestStats& stats) {
    std::cout << "\n=== Test 1: Random Delays Simulation ===" << std::endl;
    RandomGenerator rng;
    
    try {
        MQClient client("tcp://localhost:5555");
        client.subscribe("test_queue", 1, 30);
        
        for (int i = 0; i < 20; i++) {
            int delay = rng.getRandomDelay(10, 500);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            
            std::string msg = "Delayed message #" + std::to_string(i + 1) + 
                            " (delay: " + std::to_string(delay) + "ms)";
            client.publish("test_queue", msg, 1);
            stats.messages_sent++;
            
            std::cout << "  [" << i + 1 << "/20] Sent after " << delay << "ms delay" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "✓ Test 1 completed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test 1 failed: " << e.what() << std::endl;
        stats.failures++;
    }
}

void testHighLoad(TestStats& stats) {
    std::cout << "\n=== Test 2: High Load Stress Test ===" << std::endl;
    RandomGenerator rng;
    
    std::vector<std::string> queues = {"test_queue", "HighPriorityQueue", "EventsQueue"};
    
    auto worker = [&](int worker_id, int message_count) {
        try {
            MQClient client("tcp://localhost:5555");
            
            std::string queue = rng.getRandomQueue(queues);
            client.subscribe(queue, 1, 30);
            
            for (int i = 0; i < message_count; i++) {
                int msg_size = rng.getRandomMessageSize(10, 1000);
                std::string msg = generateRandomMessage(rng, msg_size);
                
                int qos = rng.getRandomQoS();
                
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(rng.getRandomDelay(1, 50))
                );
                
                client.publish(rng.getRandomQueue(queues), msg, qos);
                stats.messages_sent++;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "  Worker " << worker_id << " error: " << e.what() << std::endl;
            stats.failures++;
        }
    };
    
    std::vector<std::thread> workers;
    int num_workers = 5;
    int messages_per_worker = 50;
    
    std::cout << "  Starting " << num_workers << " workers..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_workers; i++) {
        workers.emplace_back(worker, i + 1, messages_per_worker);
    }
    
    for (auto& w : workers) {
        w.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "  Completed in " << duration.count() << "ms" << std::endl;
    std::cout << "  Throughput: " << (num_workers * messages_per_worker * 1000.0 / duration.count()) 
              << " msg/sec" << std::endl;
    std::cout << "✓ Test 2 completed" << std::endl;
}

void testFailureSimulation(TestStats& stats) {
    std::cout << "\n=== Test 3: Network Failure Simulation ===" << std::endl;
    RandomGenerator rng;
    
    const double FAILURE_RATE = 0.15;
    
    try {
        MQClient client("tcp://localhost:5555");
        client.subscribe("test_queue", 2, 10);
        
        for (int i = 0; i < 30; i++) {
            if (rng.shouldFail(FAILURE_RATE)) {
                std::cout << "  [" << i + 1 << "/30] ⚠ Simulated failure!" << std::endl;
                stats.failures++;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            int network_delay = rng.getRandomDelay(5, 200);
            std::this_thread::sleep_for(std::chrono::milliseconds(network_delay));
            
            std::string msg = "Message #" + std::to_string(i + 1);
            client.publish("test_queue", msg, 2);
            stats.messages_sent++;
            
            if (network_delay > 150) {
                std::cout << "  [" << i + 1 << "/30] ⏱ High latency: " << network_delay << "ms" << std::endl;
            } else {
                std::cout << "  [" << i + 1 << "/30] ✓ Sent (" << network_delay << "ms)" << std::endl;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "✓ Test 3 completed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test 3 failed: " << e.what() << std::endl;
        stats.failures++;
    }
}

void testVariableMessageSizes(TestStats& stats) {
    std::cout << "\n=== Test 4: Variable Message Sizes ===" << std::endl;
    RandomGenerator rng;
    
    try {
        MQClient client("tcp://localhost:5555");
        client.subscribe("test_queue", 1, 30);
        
        std::vector<int> size_ranges = {10, 100, 500, 1000, 5000, 10000};
        
        for (int size : size_ranges) {
            std::string msg = generateRandomMessage(rng, size);
            client.publish("test_queue", msg, 1);
            stats.messages_sent++;
            
            std::cout << "  Sent " << size << " byte message" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "✓ Test 4 completed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test 4 failed: " << e.what() << std::endl;
        stats.failures++;
    }
}

void testChaos(TestStats& stats) {
    std::cout << "\n=== Test 5: Chaos Engineering Test ===" << std::endl;
    
    std::vector<std::string> queues = {"test_queue", "HighPriorityQueue", "EventsQueue"};
    
    auto chaos_worker = [&](int worker_id) {
        RandomGenerator rng;
        
        try {
            MQClient client("tcp://localhost:5555");
            
            for (int i = 0; i < 30; i++) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(rng.getRandomDelay(1, 300))
                );
                
                if (rng.shouldFail(0.1)) {
                    std::cout << "  Worker " << worker_id << ": Simulated failure!" << std::endl;
                    stats.failures++;
                    continue;
                }
                
                std::string queue = rng.getRandomQueue(queues);
                
                if (rng.shouldFail(0.3)) {
                    client.subscribe(queue, rng.getRandomQoS(), 30);
                }
                
                int msg_size = rng.getRandomMessageSize(10, 2000);
                std::string msg = "Chaos[" + std::to_string(worker_id) + "]: " + 
                                generateRandomMessage(rng, msg_size);
                
                client.publish(queue, msg, rng.getRandomQoS());
                stats.messages_sent++;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "  Worker " << worker_id << " error: " << e.what() << std::endl;
            stats.failures++;
        }
    };
    
    std::vector<std::thread> workers;
    int num_workers = 8;
    
    std::cout << "  Unleashing chaos with " << num_workers << " workers... 🔥" << std::endl;
    
    for (int i = 0; i < num_workers; i++) {
        workers.emplace_back(chaos_worker, i + 1);
    }
    
    for (auto& w : workers) {
        w.join();
    }
    
    std::cout << "✓ Test 5 completed (survived chaos!)" << std::endl;
}

int main() {
    std::cout << "\n╔════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   BabadzakiMQ Stress & Chaos Tests        ║" << std::endl;
    std::cout << "║   Random Variables & Failure Simulation    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════╝\n" << std::endl;
    
    TestStats stats;
    
    try {
        testRandomDelays(stats);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        testHighLoad(stats);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        testFailureSimulation(stats);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        testVariableMessageSizes(stats);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        testChaos(stats);
        
        stats.print();
        
        std::cout << "╔════════════════════════════════════════════╗" << std::endl;
        std::cout << "║   All stress tests completed! 🎉           ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════╝\n" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Critical failure: " << e.what() << std::endl;
        stats.print();
        return 1;
    }
    
    return 0;
}
