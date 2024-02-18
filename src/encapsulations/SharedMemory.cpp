/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** SharedMemory.hpp
*/

#include "SharedMemory.hpp"

SharedMemory::SharedMemory(std::string name)
{
    _key = ftok(name.c_str(), 'A');
    _shmId = shmget(_key, sizeof(Communication_t), IPC_CREAT | 0666);
    _semId = semget(_key, 1, IPC_CREAT | 0666);

    if (_shmId == -1)
        throw EncapsulationError("Failed to create shared memory segment.");

    if (_semId == -1)
        throw EncapsulationError("Failed to create semaphore.");

    communication = static_cast<Communication_t *>(shmat(_shmId, nullptr, 0));

    if ((void *)communication == (void *)-1)
        throw EncapsulationError("Failed to attach shared memory segment.");
    semctl(_semId, 0, SETVAL, 1);

    for (int i = 0; i < MAX_PROCESS; i++)
    {
        this->communication->process_ids[i] = -1;
        this->communication->process_count[i] = 0;
        this->communication->process_to_receive[i] = 0;
    }
}

SharedMemory::~SharedMemory()
{
    shmdt(communication);
}

void SharedMemory::destroy()
{
    semctl(_semId, 0, IPC_RMID);
    shmctl(_shmId, IPC_RMID, nullptr);
}

void SharedMemory::setIsAlive(bool isAlive)
{
    semLock(_semId);
    this->communication->isAlive = isAlive;
    semUnlock(_semId);
}

bool SharedMemory::isAlive()
{
    return this->communication->isAlive;
}

int SharedMemory::getAttribute(std::string attr)
{
    return this->getAttribute(attr, 0);
}

void SharedMemory::incAttribute(std::string attr)
{
    this->incAttribute(attr, 0);
}

void SharedMemory::decAttribute(std::string attr)
{
    this->decAttribute(attr, 0);
}

void SharedMemory::setAttribute(std::string attr, int value)
{
    this->setAttribute(attr, 0, value);
}

int SharedMemory::getAttribute(std::string attr, int offset)
{
    if (offset < 0 || offset > MAX_PROCESS)
        throw EncapsulationError("SharedMemory can't access specified process");
    int value;

    semLock(_semId);
    if (attr == "id")
        value = this->communication->process_ids[offset];
    else if (attr == "count")
        value = this->communication->process_count[offset];
    else if (attr == "receive")
        value = this->communication->process_to_receive[offset];
    else if (attr == "display")
        value = this->communication->displayCount;
    else
    {
        semUnlock(_semId);
        throw EncapsulationError("Attribute is not registered");
    }
    semUnlock(_semId);
    return value;
}

void SharedMemory::setAttribute(std::string attr, int offset, int value)
{
    if (offset < 0 || offset > MAX_PROCESS)
        throw EncapsulationError("SharedMemory can't access specified process");

    semLock(_semId);
    if (attr == "id")
        this->communication->process_ids[offset] = value;
    else if (attr == "count")
        this->communication->process_count[offset] = value;
    else if (attr == "receive")
        this->communication->process_to_receive[offset] = value;
    else if (attr == "display")
        this->communication->displayCount = value;
    else
    {
        semUnlock(_semId);
        throw EncapsulationError("Attribute is not registered");
    }
    semUnlock(_semId);
}

void SharedMemory::incAttribute(std::string attr, int offset)
{
    if (offset < 0 || offset > MAX_PROCESS)
        throw EncapsulationError("SharedMemory can't access specified process");

    semLock(_semId);
    if (attr == "id")
        this->communication->process_ids[offset]++;
    else if (attr == "count")
        this->communication->process_count[offset]++;
    else if (attr == "receive")
        this->communication->process_to_receive[offset]++;
    else if (attr == "display")
        this->communication->displayCount++;
    else
    {
        semUnlock(_semId);
        throw EncapsulationError("Attribute is not registered");
    }
    semUnlock(_semId);
}

void SharedMemory::decAttribute(std::string attr, int offset)
{
    if (offset < 0 || offset > MAX_PROCESS)
        throw EncapsulationError("SharedMemory can't access specified process");

    semLock(_semId);
    if (attr == "id")
        this->communication->process_ids[offset]--;
    else if (attr == "count")
        this->communication->process_count[offset]--;
    else if (attr == "receive")
        this->communication->process_to_receive[offset]--;
    else if (attr == "display")
        this->communication->displayCount--;
    else
    {
        semUnlock(_semId);
        throw EncapsulationError("Attribute is not registered");
    }
    semUnlock(_semId);
}

SharedMemory &SharedMemory::operator<<(bool value)
{
    this->setIsAlive(value);
    return *this;
}

SharedMemory &SharedMemory::operator>>(bool &value)
{
    value = this->isAlive();
    return *this;
}

void semLock(int semId)
{
    struct sembuf semOp
    {
    };
    semOp.sem_num = 0;
    semOp.sem_op = -1; // Lock operation
    semOp.sem_flg = SEM_UNDO;

    semop(semId, &semOp, 1);
}

void semUnlock(int semId)
{
    struct sembuf semOp
    {
    };
    semOp.sem_num = 0;
    semOp.sem_op = 1; // Unlock operation
    semOp.sem_flg = SEM_UNDO;

    semop(semId, &semOp, 1);
}
