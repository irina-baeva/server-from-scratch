#include "./hello.h"
#include "string"

char *PrintHello(char *socket_type)
{
    if (socket_type == "server")
    {

        return "Hello from server!!!";
    }
    else
    {
        return "Hello From client!!!";
    }
}