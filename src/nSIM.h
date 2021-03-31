#ifndef NSIM_H
#define NSIM_H
#include"instruction.h"
#include "instOp3.h"
#include "instOp2.h"
#include "instOp1.h"
#include "instOp0.h"
#include <vector>
#include <thread>
#include <mutex>


class nSIM{
private:
	instruction*** inst;
	int mem[1024]; //can be turned into an array of registers(a possible seperate class) if necessary. 
	std::mutex memMut[1024];
	std::mutex ReadWriteMut;
	int* pc;
	int* p;
	int n;
	std::vector<std::thread> T;
	std::string getType(std::string);
	void runThread(int);
public:
	nSIM(int);
	void getInst(std::string, int i);
	void run();
	void readFile(std::string[]);
	~nSIM();
};
#endif