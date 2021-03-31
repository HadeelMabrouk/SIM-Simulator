#include <string>
#include <ctype.h>
#include "instruction.h"


instruction::instruction(){}
std::string instruction::getOperand(int i, std::string str)
{
	std::string num = "";
	for (int j = i; ((isdigit(str[j]) || str[j]=='-') && j<str.size()); j++)
	{
		num += str[j];
	}
	return num;
}
std::string instruction::getType()
{
	return type;
}
void instruction::validateJAddress(int pc, int size) {}
instruction::~instruction() {}


