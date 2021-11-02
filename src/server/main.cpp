// as a general rule put your own headers before external/standard lib ones
// it prevents makeing code brittle
#include "serverSocket.h"

#include <string>
#include <iostream>


// See client code for comments
//#pragma once
//using namespace std;

void Listener_MessageRecieved(serverListener *listener, int client, std::string msg);

int main()
{
    serverListener server("127.0.0.1", 8080, Listener_MessageRecieved);
    server.Run();
};

void Listener_MessageRecieved(serverListener *listener, int client, std::string msg)
{
    listener->SendMessage(client, msg);
};