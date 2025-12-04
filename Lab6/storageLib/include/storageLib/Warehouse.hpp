#pragma once
#include <storageLib/Warehouse/ReceivingDock.hpp>
#include <storageLib/Warehouse/StorageZone.hpp>
#include <storageLib/Warehouse/PackingZone.hpp>
#include <storageLib/Warehouse/ShippingZone.hpp>
#include <storageLib/Warehouse/workers/Loader.hpp>
#include <storageLib/Warehouse/workers/Manager.hpp>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>

struct Task {
    enum class Type {
        UNLOAD_TRUCK,
        PACK_ORDER,
        INVENTORY
    };
    
    Type type;
    int targetId;
    std::shared_ptr<Order> order;
};

class Warehouse {
private:
    ReceivingDock receivingDock;
    StorageZone storageZone;
    PackingZone packingZone;
    ShippingZone shippingZone;
    
    std::vector<std::unique_ptr<Loader>> loaders;
    std::vector<std::unique_ptr<Manager>> managers;
    
    std::queue<Task> taskQueue;
    std::mutex taskMtx;
    std::condition_variable taskCv;
    
    std::atomic<bool> running;
    
public:
    Warehouse(int numReceivingSlots, int numRacks, 
              int numPackingTables, int numShippingSlots,
              int numLoaders, int numManagers);
    
    void start();
    void stop();
    
    void addTask(const Task& task);
    bool getTask(Task& task);
    
    ReceivingDock& getReceivingDock() { return receivingDock; }
    StorageZone& getStorageZone() { return storageZone; }
    PackingZone& getPackingZone() { return packingZone; }
    ShippingZone& getShippingZone() { return shippingZone; }
};