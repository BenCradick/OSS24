
enum {
    PARENT = 0,
    CHILD = 1
}typedef messageTypes;
struct msgbuf
{
    long mtype;
    char mtext[100];
    pid_t pid;
};

class Message
{
    public:
        Message(messageTypes type);
        ~Message();
        void sendMessage(int pid, const char* message);
        const char* getMessage(int pid, int flags);
        
    private:
        
        messageTypes sends;
        messageTypes recieves;

        messageTypes opposite(messageTypes type);
};