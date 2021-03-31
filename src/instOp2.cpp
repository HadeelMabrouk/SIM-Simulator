#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <limits.h>
#include "instOp2.h"

void instOp2::setInst(std::string str, int mem[],std::string t)
{
	type = t;

	std::string operand;
	int i = 0;
	if (type == "JMP0") i = 4; //to avoid taking "0" in "JMP0" as an operand
	while (!(isdigit(str[i])||str[i]=='-') && str[i] != '#') //to reach the operands part
	{
		if (!isalpha(str[i]))
			throw std::logic_error("Syntax error.");
		i++;
		if (i >= str.size())
			throw std::logic_error("Too few operands.");
	}
	if (str[i] == '#')
	{
		inMemAccess = true;
		i++;
	}
	else
		inMemAccess = false;
	operand = getOperand(i, str);
	in = atoll(operand.c_str());
	i += operand.size();


	if (str[i] != ',')
		throw std::logic_error("Syntax error.");
	i++;
	if (i >= str.size())
		throw std::logic_error("Too few operands.");
	if ((str[i] == '#' && type != "JMP0") || (str[i] == '$' && type == "JMP0")) i++;
	else
		throw std::logic_error("Syntax error.");
	if (i >= str.size())
		throw std::logic_error("Too few operands.");
	if (!(isdigit(str[i]) || str[i] == '-'))
		throw std::logic_error("Syntax error.");

	operand = getOperand(i, str);
	out = atoll(operand.c_str());
	i += operand.size();

	if (i < str.size())
		throw std::logic_error("too many operands, might be a syntax error.");

	if (inMemAccess && (in < 0 || in>1023))
		throw std::out_of_range("First Operand: address out of range, should be between 0 and 1023.");
	if (out < 0 || out>1023)
		throw std::out_of_range("Second Operand: address out of range, should be between 0 and 1023.");
	if (in < INT_MIN || in>INT_MAX)
		throw std::out_of_range("First Operand: value out of range, should be an integer.");
	if (out < INT_MIN || out>INT_MAX)
		throw std::out_of_range("Second Operand: value out of range, should be an integer.");
}
void instOp2::execute(int mem[], int& pc, std::mutex mut[]) 
{
	int n;
	if (inMemAccess)
	{
		std::unique_lock<std::mutex> lck1{ mut[in] ,std::defer_lock };
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		std::lock(lck1, lck3);
		n = mem[in];
		executeUtil(mem[out], n, pc);
	}
	else
	{
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		lck3.lock();
		n = in;
		executeUtil(mem[out], n, pc);
	}
}


