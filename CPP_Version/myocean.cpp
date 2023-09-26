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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

void print_mesh(std::vector<std::vector<float>>);
void print_mesh_csv(std::vector<std::vector<float>> mesh);

int main()
{
    std::string line;
    int X_max, Y_max, time;
    std::vector<std::vector<float>> mesh;

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

    //printf("Inital Matrix:\n");
    //print_mesh(mesh);

    int i, j, r, b;
    r = 1;
    b = 2;
    for(i = 0; i < time; ++i)
    {
        if(i % 2 == 0)
        {
            for(j = 1; j < (int)mesh.size()-1; ++j)
            {
                if(j % 2 == 0) r = 1;
                else           r = 2;
                for(; r < (int)mesh[j].size()-1; r += 2)
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
            for(j = 1; j < (int)mesh.size()-1; ++j)
            {
                if(j % 2 == 0) b = 2;
                else           b = 1;
                for(; b < (int)mesh[j].size()-1; b += 2)
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

    printf("\nFinalized Matrix:\n");
    print_mesh(mesh);
    print_mesh_csv(mesh);

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
    std::fstream fs;
    fs.open("mesh.csv", std::fstream::out);
    for(int i = 0; i < (int)mesh.size(); ++i)
    {
        printf("%.2f,", mesh[i][0]);
        fs << std::setprecision(3) << mesh[i][0];
        for(int j = 1; j < (int)mesh[i].size(); ++j)
            fs << std::setprecision(3) << "," << mesh[i][j];
        printf("\n");
        fs << "\n";
    }
    fs.close();
    return;
}