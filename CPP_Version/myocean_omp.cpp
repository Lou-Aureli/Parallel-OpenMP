/*
    Author: Fazrian Prawiranata
    Date: 09/26/23
    Notes:  usage: ./myocean_c < [RECOMENDED myocean.in[.short]]
            - RECOMENDED TO PIPE IN AN INPUT FILE OF THE FOLLOWING FORMAT
            |-> XMAX YMAX STEPS 
                I00 I01 I02 ...
                I10 I11 I12 ...
                ...
            - This is the sequential version of the program
            - In the specifed number of time steps, the program will average the
              NON-border values with itself and the values to the north south east
              and west.
            - However, we will do this with the Red-Black approach that will help
              parallelize it later. On even time step, red values will be averaged
              and on odd time steps, black values will be averaged.
*/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include "omp.h"

void print_mesh(std::vector<std::vector<float>>);
void print_mesh_csv(std::vector<std::vector<float>> mesh);

int main(int argc, char** argv)
{
    std::string line;
    double start, end;
    int num_thread = 2;
    int X_max, Y_max, time;
    std::vector<std::vector<float>> mesh;

    if(argc > 2)
    {
        printf("usage: ./myocean_omp [num_threads, DEFAULT=2] [OPT: < myocean.in]");
        exit(0);
    }
    else
        num_thread = std::stoi(argv[1]);
    
    omp_set_num_threads(num_thread);

    start = omp_get_wtime();
    printf("Please enter all in one line: X_max, Y_max, time_steps> ");
    getline(std::cin, line);
    std::stringstream ss(line);
    ss >> X_max >> Y_max >> time;
    
    printf("Please enter each row of the matrix (ctrl+D to stop):\n");
    while(getline(std::cin, line))
    {
        float m;
        std::vector<float> tmp;
        std::istringstream is(line);
        while(is >> m)
            tmp.push_back(m);
        mesh.push_back(tmp);
    }

        // If no initial grid is specified then make a default one.
    if((int)mesh.size() < X_max)
    {
        for(int i = (int)mesh.size(); i < X_max; ++i)
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
    end = omp_get_wtime();
    printf("Initialization TIME %.5fs\n", end - start);

    //printf("Inital Matrix:\n");
    //print_mesh(mesh);

    int i, j, r, b;
    start = omp_get_wtime();
    for(i = 0; i < time; ++i)
    {
        if(i % 2 == 0)
        {
            #pragma omp parallel for
            for(j = 1; j < (int)mesh.size()-1; ++j)
            {
                #pragma omp parallel for
                for(r = j % 2 + 1; r < (int)mesh[j].size()-1; r += 2)
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
            for(j = 1; j < (int)mesh.size()-1; ++j)
            {
                #pragma omp parallel for
                for(b = 2 - j % 2; b < (int)mesh[j].size()-1; b += 2)
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

        if(i % 30 == 0)
        {
            printf("\nMatrix at %d/%d:\n", i, time);
            print_mesh(mesh);
        }
    }
    end = omp_get_wtime();
    printf("\nFinalized Matrix:\n");
    print_mesh(mesh);

    printf("Red+Black TIME %.5fs\n", end - start);

    return 0;
}

void print_mesh(std::vector<std::vector<float>> mesh)
{
    for(int i = 0; i < (int)mesh.size(); ++i)
    {
        for(int j = 0; j < (int)mesh[i].size(); ++j)
            std::cout << std::setw(5) << std::setprecision(3) << mesh[i][j] << " ";
        printf("\n");
    }
    return;
}

void print_mesh_csv(std::vector<std::vector<float>> mesh)
{
    for(int i = 0; i < (int)mesh.size(); ++i)
    {
        printf("%.2f,", mesh[i][0]);
        for(int j = 1; j < (int)mesh[i].size(); ++j)
            printf(",%.2f", mesh[i][j]);
        printf("\n");
    }
    return;
}