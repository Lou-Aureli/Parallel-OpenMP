#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("usage: ./myocean [X-Dimension] [Y-Dimension] [Number of Timesteps]");
        exit(0);
    }
    return 0;
}