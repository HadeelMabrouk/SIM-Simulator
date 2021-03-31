#include <iostream>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <vector>
#include "nSIM.h"

using namespace std;
nSIM::nSIM(int n=1) : n(n)
{
	inst = new instruction** [n];
	p = new int[n];
	pc = new int[n];
	for (int i = 0; i < n; i++)
	{
		inst[i] = new instruction* [1024];
		p[i] = 0;
		pc[i] = 0;
	}
	for (int i = 0; i < 1024; i++)
		mem[i] = 0;
}

std::string nSIM::getType(string s)
{
	//to check the instruction type given that they are of various lengths.
	string type5 = s.substr(0, 5);
	string type4 = s.substr(0, 4);
	string type3 = s.substr(0, 3);
	string type2 = s.substr(0, 2);
	if (type5 == "WRITE")
	{
		if (!isspace(s[type5.size()]))
			throw std::logic_error("Syntax error, please insert a space after the instruction type.");
		return type5;
	}
	else if (type4 == "READ" || type4 == "JMP0")
	{
		if (!isspace(s[type4.size()]))
			throw std::logic_error("Syntax error, please insert a space after the instruction type.");
		return type4;
	}
	else if (type4 == "HALT")
	{
		return type4;
	}
	else if (type3 == "ADD" || type3 == "MUL" || type3 == "JMP" || type3 == "NEG" || type3 == "ASS")
	{
		if (!isspace(s[type3.size()]))
			throw std::logic_error("Syntax error, please insert a space after the instruction type.");
		return type3;
	}
	else if (type2 == "LE")
	{
		if (!isspace(s[type2.size()]))
			throw std::logic_error("Syntax error, please insert a space after the instruction type.");
		return type2;
	}
	else
		throw std::logic_error("Instruction type is incorrect.");
}
void nSIM::getInst(string s, int i)
{
	string type;
	try
	{
		type = getType(s);
	}
	catch (std::logic_error& e)
	{
		string str = "Error In file " + to_string(i + 1) + " at line " + to_string(p[i] +1) + ": " + e.what();
		throw std::logic_error(str);
	}
	if (type == "ADD") inst[i][p[i]] = new ADD;
	else if (type == "MUL") inst[i][p[i]] = new MUL;
	else if (type == "LE") inst[i][p[i]] = new LE;
	else if (type == "ASS") inst[i][p[i]] = new ASS;
	else if (type == "JMP0") inst[i][p[i]] = new JMP0;
	else if (type == "NEG") inst[i][p[i]] = new NEG;
	else if (type == "READ") inst[i][p[i]] = new READ;
	else if (type == "WRITE") inst[i][p[i]] = new WRITE;
	else if (type == "JMP") inst[i][p[i]] = new JMP;
	else if (type == "HALT") inst[i][p[i]] = new HALT;

	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	if (type == "HALT" && s.size() > 4)
	{
		string str = "Error In file " + to_string(i + 1) + " at line " + to_string(p[i] + 1) + ": Syntax Error";
		throw std::logic_error(str);
	}
	try
	{
		inst[i][p[i]]->setInst(s, mem, type);
		p[i]++;
	}
	catch (std::logic_error& e)
	{
		string str = "Error In file " + to_string(i + 1) + " at line " + to_string(p[i] + 1) + ": " + e.what();
		throw std::logic_error(str);
	}
	catch (std::out_of_range& e)
	{
		string str = "Error In file " + to_string(i + 1) + " at line " + to_string(p[i] + 1) + ": " + e.what();
		throw std::logic_error(str);
	}
}

void nSIM::runThread(int i)
{
	while (pc[i] != -1 && pc[i] < 1024)
	{
		if (inst[i][pc[i]]->getType() == "READ" || inst[i][pc[i]]->getType() == "WRITE")
		{
			std::unique_lock<std::mutex> lck{ ReadWriteMut ,std::defer_lock };
			lck.lock();
			inst[i][pc[i]]->execute(mem, pc[i], memMut);
		}
		else
			inst[i][pc[i]]->execute(mem, pc[i], memMut);
		pc[i]++;
	}
}

void nSIM::run()
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < p[i]; j++)
		{
			try
			{
				inst[i][j]->validateJAddress(j,p[i]);
			}
			catch (std::logic_error& e)
			{
				string str = "Error In file " + to_string(i + 1) + " at line " + to_string(p[i] + 1) + ": " + e.what();
				throw std::logic_error(str);
			}
		}
	for (int i = 0; i < n; i++)
	{
		T.push_back(
			std::thread([=]() {
				runThread(i);
				})
		);
	}
	for (std::thread& t : T) t.join();
	std::cout << "The values of the first 10 data memory entries are: [For testing purposes]" << endl;
	std::cout << mem[0] << endl; //for testing purposes, can be commented
	std::cout << mem[1] << endl;
	std::cout << mem[2] << endl;
	std::cout << mem[3] << endl;
	std::cout << mem[4] << endl;
	std::cout << mem[5] << endl;
	std::cout << mem[6] << endl;
	std::cout << mem[7] << endl;
	std::cout << mem[8] << endl;
	std::cout << mem[9] << endl;
	std::cout << mem[10] << endl;
	std::cout << "The values of the second 10 data memory entries are: [For testing purposes]" << endl;
	std::cout << mem[10] << endl; //for testing purposes, can be commented
	std::cout << mem[11] << endl;
	std::cout << mem[12] << endl;
	std::cout << mem[13] << endl;
	std::cout << mem[14] << endl;
	std::cout << mem[15] << endl;
	std::cout << mem[16] << endl;
	std::cout << mem[17] << endl;
	std::cout << mem[18] << endl;
	std::cout << mem[19] << endl;
	std::cout << mem[20] << endl;
}

void nSIM::readFile(std::string fname[])
{
	ifstream inFile;

	for (int i = 0; i < n; i++)
	{
		inFile.open(fname[i]);
		if (inFile)
		{
			string inst;
			while (!inFile.eof())
			{
				getline(inFile, inst);
				try
				{
					getInst(inst,i);
				}
				catch (std::logic_error& e)
				{
					inFile.close();
					throw;
				}
			}
			if (inst != "HALT") throw std::logic_error("The program should end with a HALT instruction.");
			inFile.close();
		}
		else
			throw std::invalid_argument("Unable to open file");
	}
	
}

nSIM::~nSIM()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < p[i]; j++)
			delete inst[i][j];
		delete inst[i];
	}
}
