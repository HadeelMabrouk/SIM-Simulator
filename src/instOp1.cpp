#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <limits.h>
#include "instOp1.h"

void instOp1::setInst(std::string str, int mem[], std::string t)
{
	type = t;

	std::string operand;
	int i = 0;

	while (!(isdigit(str[i]) || str[i] == '-') && str[i] != '#' && str[i] != '$') //to reach the first operand
	{
		if (!isalpha(str[i]))
			throw std::logic_error("Syntax error.");
		i++;
		if (i >= str.size())
			throw std::logic_error("Too few operands.");
	}
	xImmediate = (str[i] != '#');
	if ((str[i] == '#' && type != "JMP") || (str[i] == '$' && type == "JMP")) i++;
	else if (!xImmediate)
		throw std::logic_error("Syntax error."); //error with respect to the expected operand type

	operand = getOperand(i, str);
	x = atoll(operand.c_str());
	i += operand.size();

	if (i < str.size())
		throw std::logic_error("too many operands, might be a syntax error.");

	if ((x < 0 || x>1023) && !xImmediate)
		throw std::out_of_range("First Operand: address out of range, should be between 0 and 1023.");
	if (x < INT_MIN || x>INT_MAX)
		throw std::out_of_range("First Operand: value out of range, should be an integer.");

}

void instOp1::execute(int mem[], int& pc, std::mutex mut[]) {}

