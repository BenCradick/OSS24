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

void Message::sendMessage(messageBuffer buf){

    //apparently is bad practice to just use the pid as the key
    std::string pidStr = std::to_string(buf.pid);
    const char* pidChar = pidStr.c_str();

    key_t key = ftok(pidChar, 539);

    int msgId = msgget(key, 0666 | IPC_CREAT);
    messageIds.push_back(msgId);
    

    msgsnd(msgId, &buf, sizeof(buf.mtext)+ sizeof(buf.blocked) 
    + sizeof(buf.pid) + sizeof(buf.terminate) + sizeof(buf.timeUsed), 0);

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
    if( -1 == msgrcv(msgId, &buf, sizeof(buf.mtext)+ sizeof(buf.blocked) 
    + sizeof(buf.pid) + sizeof(buf.terminate) + sizeof(buf.timeUsed), recieves, 0)){
        buf.blocked = 0;
        buf.terminate = 0;
        buf.timeUsed = 0;
        buf.mtext[0] = '0';
        buf.pid = 0;

        switch(errno){
            case ENOMSG:
                break;
            case E2BIG:
                std::cout << "E2BIG" << std::endl;
                break;
            case EACCES:
                std::cout << "EACCES" << std::endl;
                break;
            case EAGAIN:
                std::cout << "EAGAIN" << std::endl;
                break;
            case EFAULT:
                std::cout << "EFAULT" << std::endl;
                break;
            case EIDRM:
                std::cout << "EIDRM" << std::endl;
                break;
            case EINTR:
                std::cout << "EINTR" << std::endl;
                break;

            default:
                std::cout << "Unknown error" << std::endl;
                break;
        }
        return buf;

    }
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