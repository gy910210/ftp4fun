#ifndef LINUXSOCKET_SOCKETEXCEPTION_H
#define LINUXSOCKET_SOCKETEXCEPTION_H

#include <string>

class SocketException {
public:
    SocketException ( std::string s ) : m_s ( s ) {};
    ~SocketException (){};
    std::string description() { return m_s; }

private:
    std::string m_s;
};

#endif //LINUXSOCKET_SOCKETEXCEPTION_H
