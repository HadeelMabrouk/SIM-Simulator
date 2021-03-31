#ifndef INSTOP2_H
#define INSTOP2_H

#include "instruction.h"

class instOp2 : public instruction {
protected:
	long long in, out; //the operands are of type long long for range validation purposes only
	bool inMemAccess;
	virtual void executeUtil(int&, int,int&)=0;
public:
	virtual void setInst(std::string, int[], std::string);
	virtual void execute(int[], int&, std::mutex []);
};


class NEG : public instOp2 {
private:
	virtual void executeUtil(int& out, int in, int& pc)
	{
		out = -in;
	}
};

class ASS : public instOp2 {
private:
	virtual void executeUtil(int& out, int in, int& pc)
	{
		out = in;
	}
};


class JMP0 : public instOp2 {
private:
	virtual void executeUtil(int& out, int in,int& pc)
	{
		if (in == 0) pc = out - 1;
	}
public:
	virtual void validateJAddress(int pc, int size) //to throw an exception in case of a jumping error.
	{
		if (pc == out) throw std::logic_error("JMP is not allowed to jump to the same instruction.");
		if (out > size - 1) throw std::logic_error("jumping address out of range.");
	};
};


#endif

