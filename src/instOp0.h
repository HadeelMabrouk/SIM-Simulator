#ifndef INSTOP0_H
#define INSTOP0_H

#include "instruction.h"

class instOp0 : public instruction {
public:
	virtual void setInst(std::string, int[], std::string);
	virtual void execute(int[], int&, std::mutex[]);
};


class HALT : public instOp0 {
public:
	virtual void execute(int mem[], int& pc, std::mutex mut[])
	{
		pc = -2;
	};
};
#endif

