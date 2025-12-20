#include "MQClient.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void testBasicPublishSubscribe() {
    std::cout << "\n=== Test 1: Basic Publish/Subscribe ===" << std::endl;
    
    MQClient client("tcp://localhost:5555");
    client.subscribe("test_queue", 1, 30);
    client.publish("test_queue", "Test message 1", 1);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "✓ Test 1 passed" << std::endl;
}

void testMultipleMessages() {
    std::cout << "\n=== Test 2: Multiple Messages ===" << std::endl;
    
    MQClient client("tcp://localhost:5555");
    client.subscribe("test_queue", 1, 30);
    
    for (int i = 0; i < 5; i++) {
        std::string msg = "Message #" + std::to_string(i + 1);
        client.publish("test_queue", msg, 1);
        std::cout << "Published: " << msg << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "✓ Test 2 passed" << std::endl;
}

void testDifferentQoS() {
    std::cout << "\n=== Test 3: Different QoS Levels ===" << std::endl;
    
    MQClient client("tcp://localhost:5555");
    client.subscribe("test_queue", 2, 30);
    
    client.publish("test_queue", "QoS 0 message", 0);
    client.publish("test_queue", "QoS 1 message", 1);
    client.publish("test_queue", "QoS 2 message", 2);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "✓ Test 3 passed" << std::endl;
}

void testHighPriorityQueue() {
    std::cout << "\n=== Test 4: High Priority Queue ===" << std::endl;
    
    MQClient client("tcp://localhost:5555");
    client.subscribe("HighPriorityQueue", 1, 30);
    
    client.publish("HighPriorityQueue", "High priority message", 1);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "✓ Test 4 passed" << std::endl;
}

void testEventsQueue() {
    std::cout << "\n=== Test 5: Events Queue ===" << std::endl;
    
    MQClient client("tcp://localhost:5555");
    client.subscribe("EventsQueue", 1, 30);
    
    client.publish("EventsQueue", "Event notification", 1);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "✓ Test 5 passed" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   BabadzakiMQ Integration Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testBasicPublishSubscribe();
        testMultipleMessages();
        testDifferentQoS();
        testHighPriorityQueue();
        testEventsQueue();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "   All tests passed! ✓" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
