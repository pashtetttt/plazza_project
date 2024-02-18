/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** MessageQueue.hpp
*/

#pragma once

#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MESSAGE_SIZE 4096

struct Message
{
    long int mtype;
    char content[4096];
    bool isQueueEnding;
};

class MessageQueue
{
private:
    key_t _key;
    int _msgId;

public:
    size_t id;

    MessageQueue(key_t);
    MessageQueue(std::string path = "msg_plazza");
    ~MessageQueue() = default;

    void destroy();
    int getAwaitingLen();
    void sendTermination();
    void sendMessage(std::string content, long int recipientId = 0);
    std::string receiveMessage(int flag = IPC_NOWAIT);

    MessageQueue &operator<<(std::string);
    MessageQueue &operator>>(std::string &);
};
