
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
    if (argc < 2)
        return 0;
    char arg[] = "/tmp/test.sh";
    char *args[] = {arg};
    chdir("/tmp/");
    if (execve("/bin/bash", ++argv, envp) == -1)
        perror("execve");
    std::cout << "HELLO" << std::endl;
}
