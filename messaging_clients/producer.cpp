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
        strncpy(msgStruct.msg, msg.c_str(), sizeof(msgStruct.msg) - 1);
        msgStruct.msg[sizeof(msgStruct.msg) - 1] = '\0';

        char buffer[sizeof(int) + sizeof(msgStruct.msg)];
        memcpy(buffer, &msgStruct.id, sizeof(int));
        memcpy(buffer + sizeof(int), msgStruct.msg, sizeof(msgStruct.msg));

        send(publisherSocket, buffer, sizeof(buffer), 0);

    }
}