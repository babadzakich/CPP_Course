#include "banklib/classes/Workplace.h"

Workplace::Workplace(const Workspace_Type& type, Number seats) : workplaceType(type), branchSeats(seats) {}

Workspace_Type Workplace::getWorkplaceType() const {
  return workplaceType;
}

Number Workplace::getBranchSeats() const {
  return branchSeats;
}
