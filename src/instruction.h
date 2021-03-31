#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include<mutex>

class instruction {
protected:
	std::string type;
	std::string getOperand(int, std::string); //to parse the operand and return it as a substring
public:
	instruction();
	virtual void setInst(std::string, int[], std::string)=0; //to read each instruction and set its operands and type accordingly
	virtual void execute(int[], int&, std::mutex mut[]) =0; //to execute the instruction depending on its type
	std::string getType(); //to retrieve instruction type
	virtual void validateJAddress(int pc, int size); //to validate jump address
	virtual ~instruction();
};
#endif
