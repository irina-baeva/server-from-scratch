#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>

int main()
{
    std::cout << "Hello World\n";

    int server_fd;
    int bind_result;
    struct sockaddr_in server_address; // consists of info about server address
    const int DEFAULT_PORT = 8080;

    /**
     * Create a TCP socket
     *
     * @param domain parameters domain,
     * @param type type of service,
     * @param protocol,
     * @return socket descriptor, or -1 on error.
     */

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Error with" << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Socket descriptor was created: " << server_fd << std::endl;
    
    /*
    struct sockaddr_in 
    { 
        __uint8_t         sin_len; 
        sa_family_t       sin_family - The address family we used when we set up the socket: AF_INET
        in_port_t         sin_port - The port number (the transport address)
        struct in_addr    sin_addr - machine’s IP address: INADDR_ANY
        char              sin_zero[8]; 
    }
    */
    server_address.sin_family = AF_INET; // use IPv4 or IPv6, whichever
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // fill in IP

    /**
     *  Binding an address - assigning a transport address to the socket
     *
     * @param socket - socket that was created with the socket system call,
     * @param sockaddr *address - is a pointer to a struct sockaddr that contains information about port and IP address
     * @param address_len - the length in bytes of address
     * @return 
     */

    bind_result = bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
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

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        return 0;
    }
    std::cout << "Starting to listen.... " << std::endl;
}