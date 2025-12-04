#pragma once
#include <storageLib/DTO/Pallet.hpp>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>

class ShippingTruck {
private:
    int slotId;
    std::string destination;
    int capacity;
    int loaded;
    std::mutex mtx;
    std::queue<int> loaderQueue;
    std::condition_variable cv;
    
public:
    ShippingTruck(int id, const std::string& dest, int cap);
    bool tryLoadPallet(Pallet& pallet, std::chrono::milliseconds timeout);
    bool isFull() const { return loaded >= capacity; }
    std::string getDestination() const { return destination; }
};

class ShippingZone {
private:
    std::vector<std::unique_ptr<ShippingTruck>> slots;
    std::mutex zoneMtx;
    
public:
    ShippingZone(int numSlots);
    void addTruck(const std::string& destination, int capacity);
    ShippingTruck* findTruckForCity(const std::string& city);
    void removeTruck(int slotId);
};