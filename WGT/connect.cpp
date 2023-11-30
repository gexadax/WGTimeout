//connect.cpp
#include "connect.h"

LIBSSH2_SESSION* SSHConnector::getSession() {
    return session;
}

SSHConnector::SSHConnector() : sock(INVALID_SOCKET), session(nullptr) {
    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 0), &wsadata);
}

SSHConnector::~SSHConnector() {
    disconnectToSSH();

    // Очистка библиотеки libssh2
    libssh2_exit();

    // Завершение Winsock
    WSACleanup();
}

bool SSHConnector::connectToSSH(const std::string& hostname, const std::string& username, const std::string& password, int port) {
    // Создание сокета
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, hostname.c_str(), &(sin.sin_addr));

    connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in));

    // Инициализация библиотеки libssh2
    libssh2_init(0);

    // Создание новой сессии
    session = libssh2_session_init();

    // Подключение к серверу SSH
    return libssh2_session_handshake(session, sock) == 0 &&
           libssh2_userauth_password(session, username.c_str(), password.c_str()) == 0;
}

void SSHConnector::disconnectToSSH() {
    if (session) {
        // Закрытие сессии
        libssh2_session_disconnect(session, "Bye bye, cruel world");
        libssh2_session_free(session);
        session = nullptr;
    }

    if (sock != INVALID_SOCKET) {
        // Закрытие сокета
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}
