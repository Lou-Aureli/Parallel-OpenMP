/*
    Author: Fazrian Prawiranata
    Date: 09/26/23
    Notes:  usage: ./myocean_c [num_threads, default = 2] < [RECOMENDED myocean.in[.short]]
            - RECOMENDED TO PIPE IN AN INPUT FILE OF THE FOLLOWING FORMAT
            |-> XMAX YMAX STEPS 
                I00 I01 I02 ...
                I10 I11 I12 ...
                ...
            - This is the parallelized version of the program
            - In the specifed number of time steps, the program will average the
              NON-border values with itself and the values to the north south east
              and west.
            - However, we will do this with the Red-Black approach that will help
              parallelize it later. On even time step, red values will be averaged
              and on odd time steps, black values will be averaged.
            - This will utilize OpenMP to parallelize the program with the user
              specified (or default of 2) number of threads. This will also time
              how long each part of the process takes to complete.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

void print_mesh(float **, int, int);

int main(int argc, char** argv)
{
    int num_threads = 2;

    if(argc > 2)
    {
        printf("usage: ./myocean_omp [num_threads, DEFAULT=2] [REC: < myocean.in]");
        exit(0);
    }
    else
        num_threads = atoi(argv[1]);
    
    omp_set_num_threads(num_threads);

    char line[1000];
    int X_max, Y_max, time;
    float** mesh;
    double start, end;

    start = omp_get_wtime();
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
    end = omp_get_wtime();
    printf("Initialization TIME %.5fs\n", end - start);

    int i, j, r, b;
    r = 1;
    b = 2;
    start = omp_get_wtime();
    for(i = 0; i < time; ++i)
    {
        if(i % 2 == 0)
        {
            #pragma omp parallel for
            for(j = 1; j < X_max-1; ++j)
            {
                #pragma omp parallel for
                for(r = j % 2 + 1; r < Y_max-1; r += 2)
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
            #pragma omp parallel for
            for(j = 1; j < X_max-1; ++j)
            {
                #pragma omp parallel for
                for(b = 2 - j % 2; b < Y_max-1; b += 2)
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
    end = omp_get_wtime();
    printf("Red+Black TIME %.5fs\n", end - start);

    //print_mesh(mesh, X_max, Y_max);

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