#ifndef WORKPLACE_H
#define WORKPLACE_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Workspace_Type.h"

class Workplace {
 private:
  Workspace_Type workplaceType;
  Number branchSeats;

 public:
  Workplace(const Workspace_Type& type, Number seats);
  Workplace() : workplaceType(Workspace_Type::CASH_DESK), branchSeats(0) {};

  Workspace_Type getWorkplaceType() const;
  Number getBranchSeats() const;
};

#endif  // WORKPLACE_H