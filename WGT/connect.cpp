// connect.cpp
#include "connect.h"
#include <qlogging.h>

SSHConnector::SSHConnector() : sock(INVALID_SOCKET), session(nullptr) {
    // Initializing Winsock
    WSAStartup(MAKEWORD(2, 0), &wsadata);
}

SSHConnector::~SSHConnector() {
    // disconnectAndCleanup();
}

bool SSHConnector::connectToSSH(const std::string& hostname,
                                const std::string& username,
                                const std::string& password,
                                int port) {
    // Creating socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        return false;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, hostname.c_str(), &(sin.sin_addr));

    if (connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        return false;
    }

    // Initializing the libssh2 library
    libssh2_init(0);

    // Creating a new session
    session = libssh2_session_init();

    // Connecting to SSH server
    if (libssh2_session_handshake(session, sock) != 0 ||
        libssh2_userauth_password(session, username.c_str(), password.c_str()) != 0) {
        std::cerr << "Error connecting to SSH server or authenticating" << std::endl;
        disconnectToSSH();
        return false;
    }

    // Connection successful
    return true;
}

void SSHConnector::disconnectToSSH() {
    if (session) {
        libssh2_session_disconnect(session, "The WireGuard Timeout application has terminated.");
        libssh2_session_free(session);
        session = nullptr;
    }

    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}

void SSHConnector::disconnectAndCleanup() {
    disconnectToSSH();

    // Cleaning libssh2 library
    libssh2_exit();

    // Completing Winsock
    WSACleanup();
}

LIBSSH2_SESSION* SSHConnector::getSession() {
    return session;
}

std::string SSHConnector::executeCommand(const std::string& command) const {
    if (!session) {
        // Handle error: SSH session is not initialized
        return "";
    }

    // Create a new channel
    LIBSSH2_CHANNEL* channel = libssh2_channel_open_session(session);
    if (!channel) {
        // Handle error: Unable to open a channel
        return "";
    }

    // Execute the command on the channel
    if (libssh2_channel_exec(channel, command.c_str()) != 0) {
        // Handle error: Unable to execute the command
        libssh2_channel_free(channel);
        return "";
    }

    // Read the command output
    std::string result;
    char buffer[1024];
    int bytesRead;

    do {
        bytesRead = libssh2_channel_read(channel, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            result.append(buffer, bytesRead);
        }
    } while (bytesRead > 0);

    // Close the channel
    libssh2_channel_free(channel);

    return result;
}
