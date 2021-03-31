How to run: On running main.cpp, the console will ask for the numbers of programs to be run concurrently first (MAX is 1000, can be changed) 
then, for each program, it will ask for the test file name, and then instructions will be read and concurrently exectued if no exceptions get thrown.
A sample input/output screenshot is attached in the main directory.


# Note: 
1. The following prefixes are added prior to memory addresses as follows:
(#) before data memory addresses.
($) before instruction memory addresses.
2. For the data memory, I created a simple array of integers for the purpose of this assignment. A seperate register class can be implemented if necessary.
3. A few printing lines of some data memory elements are added at lines 108-119 SIM.cpp for testing purposes only, can be commented.

The following list of errors are handled:
* accessing data or instruction memory out of range.
* assigning or returning a value out of the integer range.
* too few/many operands for a certain instruction.
* instruction name misspelled.
* wrong instruction operand type, e.x. giving a memory address instead of a instruction address.
* wrong jumping address, whether outside the given instructions range of addresses or when it's a jump to the same address.
* the file doesn't end with HALT.
* any other typos, unexpected characters.

For access synchronization:
I created 1024 mutex (these are for the data memory synchronization), and just one mutex for READ/WRITE instructions 
to prevent printing data on the console while it's expecting an input or vice versa.
