#ifndef INSTOP3_H
#define INSTOP3_H
#include "instruction.h"



class instOp3 : public instruction {
protected:
	long long in1, in2, out; //the operands are of type long long for range validation purposes only
	bool in1MemAccess, in2MemAccess;
	virtual void executeUtil(int&, int, int) = 0;
public:
	virtual void setInst(std::string, int[], std::string);
	virtual void execute(int[], int&, std::mutex []);
};



class ADD : public instOp3 {
private:
	virtual void executeUtil(int& out, int in1, int in2)
	{
		out = in1 + in2;
	}
};

class MUL : public instOp3 {
private:
	virtual void executeUtil(int& out, int in1, int in2)
	{
		out = in1 * in2;
	}
};

class LE : public instOp3 {
private:
	virtual void executeUtil(int& out, int in1, int in2)
	{
		out = (in1 < in2);
	}
};
#endif

