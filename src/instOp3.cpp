#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <limits.h>
#include "instOp3.h"

void instOp3::setInst(std::string str, int mem[],std::string t)
{
	type = t;

	std::string operand;
	int i = 0;

	while (!(isdigit(str[i]) || str[i] == '-') && str[i] != '#')
	{
		if (!isalpha(str[i]))
			throw std::logic_error("Syntax error.");
		i++;
		if (i >= str.size())
			throw std::logic_error("Too few operands.");
	}
	if (str[i] == '#')
	{
		in1MemAccess = true;
		i++;
	}
	else
		in1MemAccess = false;
	operand = getOperand(i, str);
	in1 = atoll(operand.c_str());
	i+= operand.size();


	if (str[i]!=',')
		throw std::logic_error("Syntax error.");
	i++;
	if (i >= str.size())
		throw std::logic_error("Too few operands.");
	if (!(isdigit(str[i]) || str[i] == '-') && str[i] != '#')
		throw std::logic_error("Syntax error.");
	if (str[i] == '#')
	{
		in2MemAccess = true;
		i++;
	}
	else
		in2MemAccess = false;

	operand = getOperand(i, str);
	in2 = atoll(operand.c_str());
	in2MemAccess = (str[i - 1] == '#');
	i += operand.size();

	
	if (str[i] != ',')
		throw std::logic_error("Syntax error.");
	i++;
	if (i >= str.size())
		throw std::logic_error("Too few operands.");
	if (str[i] != '#')
		throw std::logic_error("Syntax error.");
	i++;
	if (i >= str.size())
		throw std::logic_error("Too few operands.");
	if (!(isdigit(str[i]) || str[i] == '-'))
		throw std::logic_error("Syntax error.");

	operand = getOperand(i, str);
	out = atoll(operand.c_str());
	i += operand.size();

	if (i < str.size())
		throw std::logic_error("Too many operands, might be a syntax error.");

	if(in1MemAccess && (in1<0 || in1>1023))
		throw std::out_of_range("First Operand: address out of range, should be between 0 and 1023.");
	if (in2MemAccess && (in2 < 0 || in1>1023))
		throw std::out_of_range("Second Operand: address out of range, should be between 0 and 1023.");
	if (out < 0 || out>1023)
		throw std::out_of_range("Third Operand: address out of range, should be between 0 and 1023.");
	if (in1 < INT_MIN || in1 >INT_MAX)
		throw std::out_of_range("First Operand: value out of range, should be an integer.");
	if (in2 < INT_MIN || in2 >INT_MAX)
		throw std::out_of_range("Second Operand: value out of range, should be an integer.");
	if (out < INT_MIN || out>INT_MAX)
		throw std::out_of_range("Third Operand: value out of range, should be an integer.");
}

void instOp3::execute(int mem[], int& pc, std::mutex mut[]) 
{
	int n1, n2;
	if (in1MemAccess && in2MemAccess)
	{
		std::unique_lock<std::mutex> lck1{ mut[in1] ,std::defer_lock };
		std::unique_lock<std::mutex> lck2{ mut[in2] ,std::defer_lock };
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		std::lock(lck1, lck2, lck3);
		n1 = mem[in1];
		n2 = mem[in2];
		executeUtil(mem[out], n1, n2);
	}
	else if (in1MemAccess)
	{
		std::unique_lock<std::mutex> lck1{ mut[in1] ,std::defer_lock };
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		std::lock(lck1, lck3);
		n1 = mem[in1];
		n2 = in2;
		executeUtil(mem[out], n1, n2);
	}
	else if (in2MemAccess)
	{
		std::unique_lock<std::mutex> lck2{ mut[in2] ,std::defer_lock };
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		std::lock(lck2, lck3);
		n2 = mem[in2];
		n1 = in1;
		executeUtil(mem[out], n1, n2);
	}
	else
	{
		std::unique_lock<std::mutex> lck3{ mut[out] ,std::defer_lock };
		lck3.lock();
		n2 = in2;
		n1 = in1;
		executeUtil(mem[out], n1, n2);
	}
}
