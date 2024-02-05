# OSS24 

## Project 1

Targeting Ubuntu:focal with C99


Runing project goes as follows:
````

#builds project
make

#prints help menu
./oss -h

# run the program
./oss -n NumberOfProcesses -s MaxConcurrentChildren -t Iterations
````

I will hard cap the number of concurrent children at 20 to be in line with opsys limitations.


Otherwise nothing special was done in this project, may implement signalling at later date to clean up potential zombies. You can safely ignore everything in the .github, .devcontainer, and .vscode folders. I will still explain what they do later.

Youtube video of me walking you through the code can be found [here](https://www.youtube.com/watch?v=dQw4w9WgXcQ).

One weird thing to note is that the main process loops starts by calling on line 79 :
````
ChildExited = waitpid(-1, &status, WNOHANG);
````
which on the first pass, which is expected but the errno needs to be reset. So it is on lines 94-96.

````
else if (errno == ECHILD && CurrentChildren == 0){
            errno = 0;
        }
````


Explanation of weird folders.

.vscode folder: Contains instructions for the [VSCode](https://code.visualstudio.com/) editor to use for building and running project. Ultimately I did everythign from the command line as this wasn't quite complicated enough for me to bust out GDB.

.devcontainer folder: Folder that contains image instructions for the VSCode extension [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) there is a more in depth explanation [here](https://code.visualstudio.com/docs/devcontainers/containers) but its basically instructions for VSCode to spin up a [Docker](https://www.docker.com/) container then SSH into it, then pretend as if the entire editor is sitting inside the container. I'm developing across 3 machines and wanted a consistend development environment that was also identical to opsys. So containers.

.github: I don't know it just added it at somepoint.


###### BenCradick
###### February 4 2024