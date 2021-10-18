#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <hello.h>

#define DEFAULT_PORT 8088
#define SERVER_IP "127.0.0.1"
#define EXIT_WORD_FROM_SERVER '#'
#define BUFFER_SIZE 1024

bool is_client_connection_closed(const char *msg);

int main(int argc, char const *argv[])
{
    int client_socket;
    long value_from_read;
    struct sockaddr_in server_address;

    const char *hello = PrintHello("client");
    char buffer[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        std::cerr << "Accept failed with error " << strerror(errno) << std::endl;
        return -1;
    }
    memset(&server_address, '0', sizeof(server_address)); //TODO: read about memset

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEFAULT_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
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
    send(client_socket, hello, strlen(hello), 0);
    std::cout << "Client sends hello message" << std::endl;
    value_from_read = read(client_socket, buffer, BUFFER_SIZE);
    std::cout << "Server says: " << buffer << std::endl;
    while (true)
    {
        std::cout << "Client is sending: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        write(client_socket, buffer, BUFFER_SIZE);
        if (is_client_connection_closed(buffer))
        {
            break;
        }
        std::cout << "Server responded: ";
        read(client_socket, buffer, BUFFER_SIZE);
        std::cout << buffer;
        if (is_client_connection_closed(buffer))
        {
            break;
        }
        std::cout << std::endl;
    }
    close(client_socket);
    std::cout << "Closing...." << std::endl;

    return 0;
}

bool is_client_connection_closed(const char *msg)
{
    for (int i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == EXIT_WORD_FROM_SERVER)
        {
            return true;
        }
    }
}