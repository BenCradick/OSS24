#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#include "message.h"
#include <string>

Message::Message(messageTypes type){
    this->sends = type;
    this->recieves = opposite(type);
}

Message::~Message(){
}

void Message::sendMessage(int pid, const char* message){
    struct msgbuf buf;
    buf.mtype = sends;


    //apparently is bad practice to just use the pid as the key
    std::string pidStr = std::to_string(pid);
    const char* pidChar = pidStr.c_str();

    key_t key = ftok(pidChar, 539);

    int msgId = msgget(key, 0666 | IPC_CREAT);
    strcpy(buf.mtext, message);
    msgsnd(msgId, &buf, sizeof(buf.mtext), 0);
}

const char* Message::getMessage(int pid){
    struct msgbuf buf;
    buf.mtype = recieves;

    std::string pidStr = std::to_string(pid);
    const char* pidChar = pidStr.c_str();

    key_t key = ftok(pidChar, 539);

    std::string message;
    char char_message[100];

    int msgId = msgget(key, 0666 | IPC_CREAT);
    msgrcv(msgId, &buf, sizeof(buf.mtext), 1, 0);
    strcpy(char_message, buf.mtext);
    return char_message;
}

messageTypes Message::opposite(messageTypes type){
    if(type == PARENT){
        return CHILD;
    }
    else{
        return PARENT;
    }
}