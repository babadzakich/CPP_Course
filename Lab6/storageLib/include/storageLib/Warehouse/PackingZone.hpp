#pragma once
#include <storageLib/DTO/Pallet.hpp>
#include <storageLib/DTO/Order.hpp>
#include <vector>
#include <mutex>
#include <memory>

class PackingTable {
private:
    int tableId;
    std::vector<std::unique_ptr<Pallet>> inputSlots;
    std::vector<std::unique_ptr<Pallet>> outputSlots;
    std::mutex mtx;
    bool reserved;
    
public:
    PackingTable(int id, int inputSlotCount);
    bool tryReserve(std::chrono::milliseconds timeout);
    void release();
    bool addInputPallet(std::unique_ptr<Pallet> pallet);
    std::unique_ptr<Pallet> removeOutputPallet(int idx);
    void packProducts(const Order& order);
};

class PackingZone {
private:
    std::vector<PackingTable> tables;
    
public:
    PackingZone(int numTables);
    PackingTable* getAvailableTable();
};