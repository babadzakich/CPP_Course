#pragma once

#include <storageLib/DTO/Truck.hpp>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

class ReceivingDock {
private:
    std::vector<std::unique_ptr<Truck>> slots;
    std::queue<int> activeTrucks;
    std::mutex dockMtx;
    std::condition_variable cv;
    
public:
    ReceivingDock(int numSlots);
    void addTruck(std::vector<Pallet> pallets);
    bool tryUnloadPallet(int slotId, Pallet& pallet);
    bool isTruckEmpty(int slotId);
};