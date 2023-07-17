#include "debug.h"
#include <fstream>
#include <iostream>

using namespace std;

//------------------------ Debug log functions ------------------------
//Writes a message to the debug log
void DebugLogger::writeToDebugLog(string message)
{
    //Time logging on non-Windows platforms
    #ifndef _WIN32
    time_t t = time(0);   //Get the current time
    tm* now;
    now = localtime(&t); //Convert to local time on machine

    //Write the timestamp and message
    debugLog << "[" << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'
        << now->tm_mday << " " << now->tm_hour << ":" << now->tm_min << ":" <<
        now->tm_sec << "] " << message << endl;
    #else //Windows printing
    time_t t = time(0);   //Get the current time
    tm now;
    localtime_s(&now, &t); //Convert to local time on machine

    //Write the timestamp and message
    debugLog << "[" << (now.tm_year + 1900) << '-' << (now.tm_mon + 1) << '-'
        << now.tm_mday << " " << now.tm_hour << ":" << now.tm_min << ":" <<
        now.tm_sec << "] " << message << endl;
    #endif

}

//Opens the debug file
void DebugLogger::openDebugFile(string output_filename)
{
    debugLog.open(output_filename);
    if (!debugLog.good())
    {
        cout << "Could not open debug log: " << output_filename << endl;
        return;
    }
}

//Closes the debug file
void DebugLogger::closeDebugFile()
{
    debugLog.close();
}