#ifndef WORKPLACE_H
#define WORKPLACE_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Workspace_Type.h"

class Workplace {
 private:
  Workspace_Type workplaceType;
  uint64_t branchSeats;

 public:
  Workplace(const Workspace_Type& type, uint64_t seats);
  Workplace() : workplaceType(Workspace_Type::CASH_DESK), branchSeats(0) {};

  Workspace_Type getWorkplaceType() const;
  uint64_t getBranchSeats() const;
};

#endif  // WORKPLACE_H