#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv)
{
    if (argc < 4) return -1;
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    float h = atoi(argv[3]);

    printf("%d %d 1.0 1.0\n", x, y);
    for (int i = 0; i < x; i ++) {
        for (int j = 0; j < y; j ++) {
            printf(" %f", 0.001f * h * cosf(i*i/360.0 + j*j/360.0));
        }
        printf("\n");
    }
}
