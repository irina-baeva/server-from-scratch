#include <string>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>

// declaration of the serverListener socket class
// class serverListener;

// callback to data recieved ?

// typedef void (*MessageRecievedHandler)(serverListener *listener, int socketId, std::string msgRecieved);

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