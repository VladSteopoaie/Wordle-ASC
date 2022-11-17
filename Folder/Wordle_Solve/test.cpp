#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    while(1){
        int fdr = open("guess", O_RDONLY);
        if(fdr == -1)
        {
            printf("1");
            return -1;
        }

        char c[6] ;
        if(read(fdr, c, sizeof(char) * 5) == -1){
            printf("2");
            return -1;
        }
        c[5] = '\0';
        printf("%s\n", c);
        close(fdr);
        int fdw = open("guess", O_WRONLY);
        if(fdw == -1)
        {
            printf("3");
            perror("Sal");
            return -1;
        }

        char v[6];
        std::cin.getline(v, 6);
        bool ghicit = true;
        
        if(write(fdw, v, sizeof(char) * 5) == -1)
        {
            printf("4");
            return -1;
        }
        close(fdw);
        for(int i = 0; i < 5; i ++)
            if(v[i] != 'G')
                ghicit = false;
        if(ghicit) break;
    }
    return 0;
}

//VREAU