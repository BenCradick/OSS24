#ifndef MESSAGE_H
#define MESSAGE_H

#include <unistd.h>
#include <list>

enum {
    PARENT = 1,
    CHILD = 2
} typedef messageTypes;
struct messageBuffer // Add 'struct' keyword here
{
    long mtype;
    char mtext[100];
    pid_t pid;
};

class Message
{
    public:
        Message(messageTypes type);
        Message();
        ~Message();
        void sendMessage(pid_t pid, const char* message);
        messageBuffer getMessage(pid_t pid, int flags);
        void cleanUp();
        
    protected: 
        static std::list<int> messageIds;

    private:
        
        messageTypes sends;
        messageTypes recieves;

        messageTypes opposite(messageTypes type);
        
};
#endif