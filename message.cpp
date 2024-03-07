/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "message.h"
#include <string>

std::list<int> Message::messageIds;

Message::Message(messageTypes type){
    this->sends = type;
    this->recieves = opposite(type);
}
Message::Message(){};
Message::~Message(){
}

void Message::sendMessage(pid_t pid, const char* message){
    struct messageBuffer buf;
    buf.mtype = sends;
    buf.pid = pid;


    //apparently is bad practice to just use the pid as the key
    std::string pidStr = std::to_string(pid);
    const char* pidChar = pidStr.c_str();

    key_t key = ftok(pidChar, 539);

    int msgId = msgget(key, 0666 | IPC_CREAT);
    messageIds.push_back(msgId);
    strcpy(buf.mtext, message);
    msgsnd(msgId, &buf, sizeof(buf.mtext), 0);
    std::cout << "Message sent: " << buf.mtext[0] <<  " type: "<< buf.mtype <<  " pid:" << buf.pid << std::endl;
}

messageBuffer Message::getMessage(pid_t pid, int flags){
    struct messageBuffer buf;
    buf.mtype = recieves;
    buf.pid = pid;

    std::string pidStr = std::to_string(pid);
    const char* pidChar = pidStr.c_str();

    key_t key = ftok(pidChar, 539);

    std::string message;
    char char_message[100];

    int msgId = msgget(key, 0666 | IPC_CREAT);
    msgrcv(msgId, &buf, sizeof(buf.mtext), recieves, 0);
    std::cout << "Message recieved: " << buf.mtext[0] <<  " type: "<< buf.mtype <<  " pid:" << buf.pid << std::endl;
    
    return buf;
}

messageTypes Message::opposite(messageTypes type){
    if(type == PARENT){
        return CHILD;
    }
    else{
        return PARENT;
    }
}

void Message::cleanUp(){
    for(int i : messageIds){
        msgctl(i, IPC_RMID, NULL);
    }
}