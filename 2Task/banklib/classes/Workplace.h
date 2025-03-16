#ifndef WORKPLACE_H
#define WORKPLACE_H

#include "BankClasses.h"

class Workplace {
private:
    Workspace_Type workplaceType;
    Number branchSeats;

public:
    Workplace(const Workspace_Type& type, Number seats);
    
    Workspace_Type getWorkplaceType() const;
    Number getBranchSeats() const;
};

#endif // WORKPLACE_H