#include <string>
#include "instOp0.h"

void instOp0::setInst(std::string str, int mem[], std::string t)
{
	type = t;
}
void instOp0::execute(int mem[], int& pc, std::mutex mut[]) {}
