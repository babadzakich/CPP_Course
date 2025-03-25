#include "banklib/classes/Workplace.h"

Workplace::Workplace(const Workspace_Type& type, uint64_t seats) : workplaceType(type), branchSeats(seats) {}

Workspace_Type Workplace::getWorkplaceType() const {
  return workplaceType;
}

uint64_t Workplace::getBranchSeats() const {
  return branchSeats;
}
