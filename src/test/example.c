#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    int fd = open("/dev/test", O_RDWR);

    char buffer[100];
    strcpy(buffer, "Bonjour, je suis un test");
    write(fd, buffer, strlen(buffer));

    memset(buffer, 0, 100);
    read(fd, buffer, 100);
    printf("Received: %s\n",buffer);

    int param = 0x123456; 
    ioctl(fd, 0x1337, &param);
    
}