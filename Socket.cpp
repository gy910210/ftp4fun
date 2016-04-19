#include "Socket.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

Socket::Socket() : m_sock(-1) {
    memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket() {
    if (is_valid()) ::close(m_sock);
}

bool Socket::create() {
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    return is_valid();
}

bool Socket::bind(int port) {
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY; // listen any address
    m_addr.sin_port = htons(port); // to big-endian

    int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
    return bind_return != -1;
}

bool Socket::listen() const {
    if (!is_valid()) return false;

    int listen_return = ::listen(m_sock, MAXCONNECTIONS);
    return listen_return != -1;
}

bool Socket::accept(Socket &new_socket) const {
    int addr_length = sizeof(m_addr);
    new_socket.m_sock = ::accept(m_sock, (struct sockaddr *) &m_addr, (socklen_t *) &addr_length);
    return new_socket.m_sock > 0;
}

bool Socket::send(const std::string s) const {
    int status = ::send(m_sock, s.c_str(), s.size(), MSG_NOSIGNAL);

    if (status == -1) {
        std::cout << "status == -1  errno == " << errno << "  in Socket::send\n";
        return false;
    }
    else return status != 0;
}

bool Socket::recv(std::string &s) const {
    char buf[MAXRECV + 1];
    s = "";

    memset(buf, 0, MAXRECV + 1);
    int status = ::recv(m_sock, buf, MAXRECV, 0);

    if (status == -1) {
        std::cout << "status == -1  errno == " << errno << "  in Socket::recv\n";
        return false;
    }
    else if (status == 0) {
        return false;
    }
    else {
        s = buf;
        return true;
    }
}

bool Socket::SendFile(Byte *buf, std::size_t buf_size) {
    int status = ::send(m_sock, buf, buf_size, MSG_NOSIGNAL);

    if (status == -1) {
        std::cout << "status == -1  errno == " << errno << "  in Socket::SendFile\n";
        return false;
    }
    else return status != 0;
}

bool Socket::RecvFile(Byte *buf, std::size_t buf_size) {
    int status = ::recv(m_sock, buf, buf_size, 0);

    if (status == -1) {
        std::cout << "status == -1  errno == " << errno << "  in Socket::RecvFile\n";
        return false;
    }
    else return status != 0;
}

bool Socket::connect(const std::string host, int port) {
    if (!is_valid()) return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);
    if (errno == EAFNOSUPPORT) return false;
    status = ::connect (m_sock, (struct sockaddr *) &m_addr, sizeof (m_addr));

    return status == 0;
}

void Socket::set_non_blocking(const bool b) {
    int opts;
    opts = fcntl(m_sock, F_GETFL);

    if (opts < 0) return;
    if (b) opts = (opts | O_NONBLOCK);
    else opts = (opts & ~O_NONBLOCK);

    fcntl (m_sock, F_SETFL, opts);
}
