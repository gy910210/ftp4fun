#ifndef LINUXSOCKET_SERVERSOCKET_H
#define LINUXSOCKET_SERVERSOCKET_H

#include "Socket.h"
#include <string>

class ServerSocket : public Socket {
public:
    ServerSocket (int port);
    ServerSocket () {};
    virtual ~ServerSocket();
    const ServerSocket& operator << (const std::string) const;
    const ServerSocket& operator >> (std::string &) const;
    void accept (ServerSocket &);
};

#endif //LINUXSOCKET_SERVERSOCKET_H
