#include <iostream>
#include <sys/socket.h>
#include <errno.h>

int main()
{
    std::cout << "Hello World\n";

    int server_fd;
    const struct sockaddr *address;
    const int PORT = 8080;

    /**
     * Create a TCP socket
     *
     * @param domain parameters domain,
     * @param type type of service,
     * @param protocol,
     * @return socket descriptor, or -1 on error.
     */

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << server_fd << std::endl;
    if (server_fd < 0)
    {
        std::cerr << "Error with" << strerror(errno) << std::endl;
        return 0;
    }
}