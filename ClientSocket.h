#ifndef LINUXSOCKET_CLIENTSOCKET_H
#define LINUXSOCKET_CLIENTSOCKET_H

#include "Socket.h"
#include <string>

class ClientSocket : public Socket {
public:
    ClientSocket(std::string host, int port);
    virtual ~ClientSocket() {}
    const ClientSocket& operator << (const std::string) const;
    const ClientSocket& operator >> (std::string&) const;
};

#endif //LINUXSOCKET_CLIENTSOCKET_H
