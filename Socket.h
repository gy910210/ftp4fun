#ifndef LINUXSOCKET_SOCKET_H
#define LINUXSOCKET_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

typedef unsigned char Byte;
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket {
private:
    int m_sock;
    sockaddr_in m_addr;

public:
    Socket();
    virtual ~Socket();

    bool create();
    bool bind(int port);
    bool listen() const;
    bool accept(Socket &) const;
    bool connect(const std::string host, int port);
    bool send(const std::string) const;
    bool recv(std::string &) const;
    bool SendFile(Byte *buf, std::size_t buf_size);
    bool RecvFile(Byte *buf, std::size_t buf_size);
    void set_non_blocking(bool);
    bool is_valid() const { return m_sock != -1; }
};

#endif //LINUXSOCKET_SOCKET_H
