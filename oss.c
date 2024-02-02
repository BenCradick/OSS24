#include <stdio.h>
#include <unistd.h>

int main(int argc,  char* argv[]){
    int opt;

    while(opt = getopt(argc, argv, "nst") != -1){
        // opt arguments -n, -s, -t
        switch(opt){
            case 'n':
                printf("Option n\n");
                break;
            case 's':
                printf("Option s\n");
                break;
            case 't':
                printf("Option t\n");
                break;
            default:
                printf("Option ?\n");
                break;
        }
    }

     printf("Hello, World!\n");
    return 0;
}