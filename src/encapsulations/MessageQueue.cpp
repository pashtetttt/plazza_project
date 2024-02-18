/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** MessageQueue.cpp
*/

#include "MessageQueue.hpp"
#include "Process.hpp"

MessageQueue::MessageQueue(key_t key) : _key(key), id(1)
{
    _msgId = msgget(_key, IPC_CREAT | 0666);

    if (_msgId == -1)
        throw EncapsulationError("Failed to create message queue.");
}

MessageQueue::MessageQueue(std::string path)
{
    _key = ftok(path.c_str(), 'A');
    _msgId = msgget(_key, IPC_CREAT | 0666);

    if (_msgId == -1)
        throw EncapsulationError("Failed to create message queue.");
}

void MessageQueue::destroy()
{
    msgctl(_msgId, IPC_RMID, nullptr);
}

void convertStringToCharArray(const std::string &str, char (&charArray)[MESSAGE_SIZE])
{
    str.copy(charArray, sizeof(charArray) - 1);
    charArray[str.size()] = '\0';
}

void MessageQueue::sendTermination()
{
    Message msg;
    msg.isQueueEnding = true;

    if (msgsnd(_msgId, &msg, sizeof(msg), 0) == -1)
        throw EncapsulationError("Failed to send message.");
}

void MessageQueue::sendMessage(std::string content, long int recipientId)
{
    Message msg;
    convertStringToCharArray(content, msg.content);
    msg.isQueueEnding = false;
    msg.mtype = recipientId;

    if (msgsnd(_msgId, &msg, sizeof(msg), 0) == -1)
        throw EncapsulationError("Failed to send message.");
}

std::string MessageQueue::receiveMessage(int flag)
{
    Message msg;
    if (msgrcv(_msgId, &msg, sizeof(msg), this->id, flag) != -1)
    {
        /* was it the `stop' instruction? */
        if (msg.isQueueEnding == true)
            return "stop";
        if (msg.mtype != (long int)this->id)
            return "";
        return std::string(msg.content);
    }
    else
        throw EncapsulationError("Failed to receive message.");
}

int MessageQueue::getAwaitingLen()
{
    struct msqid_ds queueInfo;
    if (msgctl(_msgId, IPC_STAT, &queueInfo) == -1)
        throw EncapsulationError("Failed to get message queue information.");
    return queueInfo.msg_qnum;
}

MessageQueue &MessageQueue::operator<<(std::string content)
{
    this->sendMessage(content);
    return *this;
}
MessageQueue &MessageQueue::operator>>(std::string &value)
{
    value = this->receiveMessage();
    return *this;
}