// This file was refactored to class in serverSocker.cpp, serverSocket.h main.cpp

#include <string>
#include <iostream>
#include <hello.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>

#define BUFER_SIZE 1024
#define EXIT_WORD 'end'

bool is_client_connection_closed(const char *msg);

int main()
{
    int server_socket, client_socket;
    const char *hello = PrintHello("server");
    long value_from_read;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    const int DEFAULT_PORT = 8088;

    /**
     * Create a TCP socket
     *
     * @param domain parameters domain,
     * @param type type of service,
     * @param protocol,
     * @return socket descriptor, or -1 on error.
     */

    server_socket = socket(AF_INET, SOCK_STREAM, 0); // listening socket

    // Check if socket is created without error
    if (server_socket < 0)
    {
        std::cerr << "Error with" << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Socket descriptor was created: " << server_socket << std::endl;

    server_address.sin_family = AF_INET; // use IPv4 or IPv6, whichever
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // IP

    /**
     *  Binding an address - assigning a transport address to the socket
     *
     * @param socket - socket that was created with the socket system call,
     * @param sockaddr *address - is a pointer to a struct sockaddr that contains information about port and IP address
     * @param address_len - the length in bytes of address
     * @return 
     */

    int bind_result = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Binding was successful: " << bind_result << std::endl;

    /**
     *  Listen
     *
     * @param socket - socket that was created with the socket system call,
     * @param backlog - maximum number of pending connections that can be queued up before connections are refused
     * @return 
     */
    if (listen(server_socket, 3) < 0)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Starting to listen.... " << std::endl;

    /**
     *   Accept connections will be blocked until a connection is present on the queue (socket operations are synchronous)
     *
     * @param socket -  is the server socket that was set for accepting connections with listen
     * @param sockaddr is client_address
     * @param address_len the length of client address (should be a pointer to a socklen_t)
     * @return 
     */
    socklen_t address_size = sizeof(server_address);
    client_socket = accept(server_socket, (struct sockaddr *)&server_address, &address_size);
    if (client_socket < 0)
    {
        std::cerr << "Accept failed with error " << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Accepting was successful: " << std::endl;
    bool isExit = false;
    char buffer[BUFER_SIZE]; // clearing up buffer on each loop

    while (!isExit)
    {

        value_from_read = read(client_socket, buffer, BUFER_SIZE); // read() is similar to recv()
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
            // Send the same message
            if (send(client_socket, buffer, BUFER_SIZE, 0) < 0)
            {
                std::cerr << "Something wrong ";
                break;
            }
        }
    }
    close(server_socket);
    std::cout << "Closing socket... " << std::endl;
    return 0;
}

// TODO: fix it and use it
/**
     *  Check if client sends closing session symbol
     *
     * @param msg 
     * @return boolean
    */

bool is_client_connection_closed(const char *msg)
{
    for (int i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == EXIT_WORD)
        {
            return true;
        }
    }
}