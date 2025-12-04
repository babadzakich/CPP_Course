#pragma once

#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

#include <storageLib/DTO/Pallet.hpp>

struct Truck {
  static constexpr int MAX_LOADERS = 3;
  int slotId;
  std::vector<Pallet> pallets;
  std::counting_semaphore<MAX_LOADERS> loaderSemaphore;
  std::mutex mtx;
  Truck(int id) : slotId(id), loaderSemaphore(MAX_LOADERS) {}

  // Грузчик пытается зайти в машину
  bool tryEnter() { return loaderSemaphore.try_acquire(); }

  // Грузчик выходит из машины
  void exit() { loaderSemaphore.release(); }
};