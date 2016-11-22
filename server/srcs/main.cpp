#include <stdlib.h>
#include "Packet.hh"
#include "Server.hh"
#include "ISocketManager.hh"

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Usage: ./server port" << std::endl;
        return (404);
    }
    Server server(atoi(av[1]));
    server.launch();
    return (0);
}
