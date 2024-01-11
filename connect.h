// connect.h
#ifndef CONNECT_H
#define CONNECT_H

#include <ws2tcpip.h>
#include <libssh2.h>
#include <iostream>

// Connecting the ws2_32.lib library
#pragma comment(lib, "ws2_32.lib")

class SSHConnector {
public:
    SSHConnector();
    ~SSHConnector();

    bool connectToSSH(const std::string& hostname,
                      const std::string& username,
                      const std::string& password,
                      int port = 22);
    void disconnectToSSH();
    void disconnectAndCleanup();
    LIBSSH2_SESSION* getSession() const;
    std::string executeCommand(const std::string& command) const;
private:
    SOCKET sock;
    WSADATA wsadata;
    struct sockaddr_in sin;
    LIBSSH2_SESSION* session;
};

#endif // CONNECT_H
