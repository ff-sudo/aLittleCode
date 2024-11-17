#ifndef TASKMANAGER_H
#define TASKMANAGER_H

class TaskManager
{
public:
    TaskManager();
    ~TaskManager();

    void InitNetwork();
    void InitCommandGenter();
    void InitCommandParser();
    void InitMessageHandler();
};

#endif