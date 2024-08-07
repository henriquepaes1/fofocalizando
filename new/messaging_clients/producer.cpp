#include <iostream>
#include <string>
#include "../messaging/queue.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int main(int argc, char **argv) {
    int publisherSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (publisherSocket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in messageQueueAddr;
    messageQueueAddr.sin_family = AF_INET;
    messageQueueAddr.sin_port = htons(8889);
    messageQueueAddr.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Tentando conectar à fila...\n";

    if (connect(publisherSocket, (struct sockaddr*)&messageQueueAddr, sizeof(messageQueueAddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectado à fila...\n";

    while (true) {
        int id;
        std::cout << "id: ";
        std::cin >> id;
        std::cout << "mensagem: ";
        std::string msg;
        std::cin >> msg;
        Message msgStruct;
        msgStruct.id = id;
        msgStruct.msg = msg;

        std::string message = std::to_string(msgStruct.id) + msgStruct.msg;

        send(publisherSocket, message.c_str(), message.size(), 0);

    }
}