//
//  carguments.c
//
//  An example about the use of main() arguments.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){    
    int arg1, arg2;
    
    if (argc < 3)
    {	printf("Usage: %s arg1 arg2 [where arg1 is ... and arg2 is ... ]\n", argv[0]);
        return 0;
    }
    
    arg1 = atoi(argv[1]);
    arg2 = atoi(argv[2]);
    
	printf("%s program with arg1=%d and arg2=%d\n",argv[0], arg1, arg2);
    
    return 0;
}