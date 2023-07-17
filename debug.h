#ifndef DEBUG_H
#define DEBUG_H

#include<fstream>
#include<iostream>

using namespace std;

//DebugLogger class for convenience to log information
struct DebugLogger
{
	static ofstream debugLog; //Used to log debug information for your convenience
	static void writeToDebugLog(string message);
	static void openDebugFile(string output_filename);
	static void closeDebugFile();
};

#endif
