#ifndef INSTOP1_H
#define INSTOP1_H

#include "instruction.h"
#include <stdexcept>


class instOp1 : public instruction {
protected:
	long long x; //the operand is of type long long for range validation purposes only
	bool xImmediate;
public:
	virtual void setInst(std::string, int[], std::string);
	virtual void execute(int[], int&, std::mutex []);
};

class JMP : public instOp1 {
public:
	virtual void execute(int mem[], int& pc, std::mutex mut[])
	{
		pc = x - 1;
	};
	virtual void validateJAddress(int pc, int size) //to throw an exception in case of a jumping error.
	{
		if (pc == x) throw std::logic_error("JMP is not allowed to jump to the same instruction.");
		if (x > size-1) throw std::logic_error("jumping address out of range.");
	};
};



class READ : public instOp1 {
	
public:
	virtual void execute(int mem[], int& pc, std::mutex mut[])
	{
		int in;
		std::cout << "Please, enter the number to be stored at location " << x <<": ";
		std::cin >> in;

		std::unique_lock<std::mutex> lck3{ mut[x] ,std::defer_lock };
		lck3.lock();
		mem[x] = in;
	};
};

class WRITE : public instOp1 {
public:
	virtual void execute(int mem[], int& pc, std::mutex mut[])
	{
		if (!xImmediate)
		{
			std::unique_lock<std::mutex> lck3{ mut[x] ,std::defer_lock };
			lck3.lock();
			std::cout << "The value stored at location " << x << " in memory = " << mem[x] << std::endl;
		}	
		else
		{
			std::cout << "The immediate value stored in the instruction = " << x << std::endl;
		}	
	};
};
#endif

