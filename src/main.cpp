#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "nSIM.h"

using namespace std;


int main()
{
	string filename[100];
	int n;
	cout << "Please, enter the number of concurrent programs n: ";
	cin >> n;
	nSIM mySIM(n);
	for (int i = 0; i < n; i++)
	{
		cout << "Please, enter the name of file number " << i+1 << " : ";
		cin >> filename[i];
	}
	try
	{
		mySIM.readFile(filename);
		cout << "Instructions are read successfully!" << endl;
		mySIM.run();
		cout << "Instructions ran successfully!" << endl;
	}
	catch (std::invalid_argument& e)
	{
		cout << e.what() << endl;
	}
	catch (std::logic_error& e)
	{
		cout << e.what() << endl;
	}
	return 0;
}