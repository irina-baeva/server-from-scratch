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
#define MAX_NUM_CLIENTS 10

int new_client_socket, client_socket[MAX_NUM_CLIENTS], selectValue, i, client_socket_descriptor, max_socket_descriptor;
int value_from_read = 0;

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

    //set of socket descriptors
    fd_set readfds;

    char *wellcomeMessage = "Hey new client \n";

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < MAX_NUM_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }
    int server_socket = CreateSocket();

    while (true)
    {
        FD_ZERO(&readfds); // clear
        FD_SET(server_socket, &readfds);
        max_socket_descriptor = server_socket;

        //add client sockets to fd set
        for (i = 0; i < MAX_NUM_CLIENTS; i++)
        {
            client_socket_descriptor = client_socket[i];
            if (client_socket_descriptor > 0)
            {
                FD_SET(client_socket_descriptor, &readfds);
            }
            if (client_socket_descriptor > max_socket_descriptor)
            {
                max_socket_descriptor = client_socket_descriptor;
            }
        }

        selectValue = select(max_socket_descriptor + 1, &readfds, NULL, NULL, NULL);
        if ((selectValue < 0) && (errno != EINTR))
        {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
        }

        // accept new connection
        if (FD_ISSET(server_socket, &readfds))
        {
            new_client_socket = WaitForConnection(server_socket);
            if (send(new_client_socket, wellcomeMessage, strlen(wellcomeMessage), 0) != strlen(wellcomeMessage))
            {
                std::cerr << "Sending wellcome message error: " << strerror(errno) << std::endl;
            }
            std::cout << "Sending wellcome message  was successful" << std::endl;

            //add a new connection to list of sockets
            for (i = 0; i < MAX_NUM_CLIENTS; i++)
            {
                //check if there is free space
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_client_socket;
                    std::cout << "Adding new client to the set of sockets with index " << i << std::endl;
                    break;
                }
            }
        }
        // else recieve a new message
        for (i = 0; i < MAX_NUM_CLIENTS; i++)
        {
            client_socket_descriptor = client_socket[i];
            if (FD_ISSET(client_socket_descriptor, &readfds))
            {
                memset(buffer, 0, BUFER_SIZE);
                value_from_read = read(client_socket_descriptor, buffer, BUFER_SIZE);

                if (value_from_read == 0)
                {
                    close(client_socket_descriptor);
                    client_socket[i] = 0; // so it could be reused
                }
                else if (value_from_read < 0)
                {
                    std::cerr << "Something wrong " << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Client with index " << i << " says: " << std::string(buffer, 0, value_from_read) << std::endl;
                    if (MessageRecieved != NULL)
                    {
                        MessageRecieved(this, client_socket_descriptor, std::string(buffer, 0, value_from_read));
                    }
                }
            }
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
    std::cout << "Accepting new connection was successful" << std::endl;
    return client_socket;
};