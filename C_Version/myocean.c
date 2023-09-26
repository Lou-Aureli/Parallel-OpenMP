#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_mesh(float **, int, int);

int main()
{
    char line[1000];
    int X_max, Y_max, time;
    float** mesh;

    printf("Please enter all in one line: X_max, Y_max, time_steps> ");
    fscanf(stdin, "%d %d %d", &X_max, &Y_max, &time);

    printf("%d %d %d\n", X_max, Y_max, time);

    mesh = malloc(sizeof(float*) * X_max);
    for(int i = 0; i < X_max; ++i) mesh[i] = malloc(sizeof(float) * Y_max);

    float tmp;
    int row, column;
    row = column = 0;
    printf("Please enter each row of the matrix (ctrl+D to stop):\n");
    while(fscanf(stdin, "%f", &tmp) != EOF)
    {
        column = 0;
        mesh[row][column] = tmp;
        for(column = 1; column < Y_max; ++column)
        {
            fscanf(stdin, "%f", &tmp);
            mesh[row][column] = tmp;
        }
        row += 1;
    }

    int i, j, r, b;
    r = 1;
    b = 2;
    for(i = 0; i < time; ++i)
    {
        if(i % 2 == 0)
        {
            for(j = 1; j < X_max-1; ++j)
            {
                if(j % 2 == 0) r = 1;
                else           r = 2;
                for(; r < Y_max-1; r += 2)
                {
                    float north, south, east, west;
                    north = mesh[j-1][r];
                    south = mesh[j+1][r];
                    east = mesh[j][r+1];
                    west = mesh[j][r-1];
                    mesh[j][r] = (north + south + east + west + mesh[j][r])/5.0;
                }
            }
        }
        else
        {
            for(j = 1; j < X_max-1; ++j)
            {
                if(j % 2 == 0) b = 2;
                else           b = 1;
                for(; b < Y_max-1; b += 2)
                {
                    float north, south, east, west;
                    north = mesh[j-1][b];
                    south = mesh[j+1][b];
                    east = mesh[j][b+1];
                    west = mesh[j][b-1];
                    mesh[j][b] = (north + south + east + west + mesh[j][b])/5.0;
                }
            }
        }
    }

    print_mesh(mesh, X_max, Y_max);

    for(int i = 0; i < X_max; ++i) free(mesh[i]);
    free(mesh);
    return 0;
}

void print_mesh(float** mesh, int X_max, int Y_max)
{
    for(int i = 0; i < X_max; ++i)
    {
        for(int j = 0; j < Y_max; ++j)
            printf("%.2f ", mesh[i][j]);
        printf("\n");
    }
}

/*
        // If no initial grid is specified then make a default one.
    if((int)mesh.size() == 0)
    {
        for(int i = 0; i < X_max; ++i)
        {
            std::vector<float> tmp;
            for(int j = 0; j < Y_max; ++j)
            {
                if(j == Y_max - 1 || i == X_max - 1)
                    tmp.push_back(100.0);
                else
                    tmp.push_back(1.0);
            }
            mesh.push_back(tmp);
        }
    }
*/