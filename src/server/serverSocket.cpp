#include "serverSocket.h"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFER_SIZE 1024

struct sockaddr_in server_address;
struct sockaddr_in client_address;
char buffer[BUFER_SIZE];

serverListener::serverListener(std::string ipAddress, int port, MessageRecievedHandler handler)
    : m_ipAddress(ipAddress),
      m_port(port),
      MessageRecieved(handler){};

// send a message to specified client
void serverListener::SendMessage(int clientSocket, std::string msg)
{
    send(clientSocket, buffer, BUFER_SIZE, 0);
};

// the main loop
void serverListener::Run()
{
    while (true)
    {
        // create server_socket socket
        int server_socket = CreateSocket();
        if (server_socket < 0)
        {
            break;
        }
        // if all good -> try to get client socket
        int client_socket = WaitForConnection(server_socket);
        if (client_socket < 0)
        {
            break;
        }
        else
        {
            close(server_socket);
            int value_from_read = 0;
            do
            {
                memset(buffer, 0, BUFER_SIZE);
                value_from_read = read(client_socket, buffer, BUFER_SIZE);
                if (value_from_read == 0)
                {
                    std::cout << "No bytes are there to read " << std::endl;
                    break;
                }
                else if (value_from_read < 0)
                {
                    std::cerr << "Something wrong " << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Client says: " << std::string(buffer, 0, value_from_read) << std::endl;
                    // Send the same message though message recived function as send(client_socket, buffer, BUFER_SIZE, 0)
                    if (MessageRecieved != NULL)
                    {
                        MessageRecieved(this, client_socket, std::string(buffer, 0, value_from_read));
                    }
                }
            } while (value_from_read > 0);
            close(client_socket);
        }
    }
};

int serverListener::CreateSocket()
{
    // create server_socket socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error with" << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Socket descriptor was created: " << std::endl;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;     // use IPv4 or IPv6, whichever
    server_address.sin_port = htons(m_port); // port we are getting now from constructor
    inet_pton(AF_INET, m_ipAddress.c_str(), &server_address.sin_addr);

    //binding
    int bind_result = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Binding was successful: " << bind_result << std::endl;
    // listening
    if (listen(server_socket, 3) < 0)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Starting to listen.... " << std::endl;

    return server_socket;
};

int serverListener::WaitForConnection(int server_socket)
{
    socklen_t address_size = sizeof(server_address);

    int client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_size);
    if (client_socket < 0)
    {
        std::cerr << "Accept failed with error " << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Accepting was successful: " << std::endl;
    return client_socket;
};