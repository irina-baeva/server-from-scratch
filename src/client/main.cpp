#include <string>
#include <iostream>
#include "clientSocket.h"
#pragma once
using namespace std;


int main()
{
    clientSocket client("127.0.0.1", 8088);
    client.Run();
};
