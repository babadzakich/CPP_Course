#pragma once
#include <storageLib/DTO/Pallet.hpp>
#include <vector>
#include <mutex>
#include <optional>
#include <memory>

class StorageZone {
private:
    struct Shelf {
        std::vector<std::unique_ptr<Pallet>> slots;
        std::mutex mtx;
        Shelf(int numSlots) : slots(numSlots) {}
    };
    
    struct Rack {
        std::vector<Shelf> shelves;
        Rack(int numShelves, int slotsPerShelf);
    };
    
    std::vector<Rack> racks;
    std::mutex inventoryMtx;
    
public:
    StorageZone(int numRacks);
    std::optional<Address> findSlotForPallet(const Pallet& pallet);
    bool storePallet(const Address& addr, std::unique_ptr<Pallet> pallet);
    std::unique_ptr<Pallet> retrievePallet(const Address& addr);
    bool returnPallet(const Address& addr, std::unique_ptr<Pallet> pallet);
};