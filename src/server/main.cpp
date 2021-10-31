#include <string>
#include <iostream>
#include "serverSocket.h"
#pragma once
using namespace std;

void Listener_MessageRecieved(serverListener *listener, int client, string msg);

int main()
{
    serverListener server("127.0.0.1", 8080, Listener_MessageRecieved);
    server.Run();
};

void Listener_MessageRecieved(serverListener *listener, int client, string msg)
{
    listener->SendMessage(client, msg);
};