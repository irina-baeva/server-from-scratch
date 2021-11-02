#include <string>
#include <iostream>
#include "clientSocket.h"

// Pragma once is only for header files. do not use it in .cpp files
//#pragma once

// in this case it is not needed. 
// In general you should not use it at all 
// ... qualifing stuff with std:: is not that much of a hussle
using namespace std;


int main()
{
    clientSocket client("127.0.0.1", 8088);
    client.Run();
};
