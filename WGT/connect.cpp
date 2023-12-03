//connect.cpp
#include "connect.h"

LIBSSH2_SESSION* SSHConnector::getSession() {
    return session;
}

SSHConnector::SSHConnector() : sock(INVALID_SOCKET), session(nullptr) {
    // Initializing Winsock
    WSAStartup(MAKEWORD(2, 0), &wsadata);
}

SSHConnector::~SSHConnector() {
    disconnectToSSH();

    // Cleaning libssh2 library
    libssh2_exit();

    // Completing Winsock
    WSACleanup();
}

bool SSHConnector::connectToSSH(const std::string& hostname,
                                const std::string& username,
                                const std::string& password,
                                int port) {
    // Creating socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, hostname.c_str(), &(sin.sin_addr));

    connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in));

    // Initializing the libssh2 library
    libssh2_init(0);

    // Creating a new session
    session = libssh2_session_init();

    // Connecting to SSH server
    return libssh2_session_handshake(session, sock) == 0 &&
           libssh2_userauth_password(session, username.c_str(), password.c_str()) == 0;
}

void SSHConnector::disconnectToSSH() {
    if (session) {
        // Closing the session
        libssh2_session_disconnect(session, "Bye bye, cruel world");
        libssh2_session_free(session);
        session = nullptr;
    }

    if (sock != INVALID_SOCKET) {
        // Closing a socket
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}
