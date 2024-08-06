#include <iostream>
#include <string>
#include "../messaging/queue.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring> 

int main(int argc, char **argv) {
    int publisherSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in messageQueueAddr;
    messageQueueAddr.sin_family = AF_INET;
    messageQueueAddr.sin_port = htons(8879);
    messageQueueAddr.sin_addr.s_addr = INADDR_ANY;

    connect(publisherSocket, (struct sockaddr*)&messageQueueAddr, sizeof(messageQueueAddr));

    while (1) {
        int id;
        std::cout << "id: ";
        std::cin >> id;
        std::cout << "mensagem: ";
        std::string msg;
        std::cin >> msg;
        Message msgStruct;
        msgStruct.id = id;
        msgStruct.msg = msg;
        send(publisherSocket, msgStruct.msg.c_str(), strlen(msgStruct.msg.c_str()), 0);
    }
}