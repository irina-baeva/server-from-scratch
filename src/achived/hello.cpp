#include "./hello.h"
#include "string"

char *PrintHello(char *socket_type)
{
    if (socket_type == "server")
    {

        return "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    }
    else
    {
        return "Hello From client!!!";
    }
}