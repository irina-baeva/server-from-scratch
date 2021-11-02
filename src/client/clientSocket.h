// this is a header so you need a header guard or pragma once
// https://en.wikipedia.org/wiki/Include_guard
#ifndef client_clientSocket_h_
#define client_clientSocket_h_

#include <string>
// You do not need these headers. 
// You do not use the features they provide in the header
// #include <iostream>
// #include <sys/socket.h>
// #include <errno.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <string>
// #include <unistd.h>

// declaration of the serverListener socket class
//class serverListener;

// callback to data recieved ?

// typedef void (*MessageRecievedHandler)(serverListener *listener, int socketId, std::string msgRecieved);
// I prefer the modern `using` syntax:
//using MessageRecievedHandler = void (*)(serverListener *listener, int socketId, std::string msgRecieved);


class clientSocket
{
public:
    clientSocket(std::string ipAddress, int port);

    // send a message to the client
    void SendMessage(int clientSocket, std::string msg);

    void Run();

private:
    // create socket
    int CreateClientSocket();
    // wait for connection
    int WaitForConnection(int server_socket);

    std::string m_ipAddress;
    int m_port;
};

#endif // client_clientSocket_h_
