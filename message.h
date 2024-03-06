
enum {
    PARENT = 0,
    CHILD = 1
}typedef messageTypes;

class Message
{
    public:
        Message(messageTypes type);
        ~Message();
        void sendMessage(int msgId, const char* message);
        const char* getMessage(int msgId);
        
    private:
        struct msgbuf{
            long mtype;
            char mtext[100];
        }; 
        messageTypes sends;
        messageTypes recieves;

        messageTypes opposite(messageTypes type);
};