#include "clientSocket.h"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024

struct sockaddr_in server_address;
struct sockaddr_in client_address;
char buffer[BUFFER_SIZE];

clientSocket::clientSocket(std::string ipAddress, int port)
    : m_ipAddress(ipAddress),
      m_port(port){};

void clientSocket::Run()
{
    while (true)
    {
        int client_socket = CreateClientSocket();
        int value_from_read = 0;
        while (true)
        {
            std::cout << "Client is sending: ";
            std::cin.getline(buffer, BUFFER_SIZE);
            write(client_socket, buffer, BUFFER_SIZE);
            std::cout << "Server responded: ";
            read(client_socket, buffer, BUFFER_SIZE);
            std::cout << buffer;
            std::cout << std::endl;
        }
        close(client_socket);
        std::cout << "Closing...." << std::endl;
    }
};

int clientSocket::CreateClientSocket()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        std::cerr << "Accept failed with error " << strerror(errno) << std::endl;
        return -1;
    }
    memset(&server_address, '0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(m_port);

    if (inet_pton(AF_INET, m_ipAddress.c_str(), &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address:  " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Client socket was created." << std::endl;

    /**
     * Here we connect 
     *
     * @param socket - client_socket
     * @param sockaddr *address - is a pointer to a struct sockaddr that contains information about port and IP address
     * @param address_len - the length in bytes of address
     * @return 
     */

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Connection failed with error:  " << strerror(errno) << std::endl;
        return -1;
    }

    return client_socket;
};