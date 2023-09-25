#include <iostream>
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

    getline(std::cin, line);
    std::stringstream ss(line);
    ss >> X_max >> Y_max >> time;
    while(getline(std::cin, line))
    {
        float m;
        std::vector<float> tmp;
        std::istringstream is(line);
        while(is >> m)
            tmp.push_back(m);
        mesh.push_back(tmp);
    }

    printf("Inital Matrix:\n");
    print_mesh(mesh);

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

    return 0;
}

void print_mesh(std::vector<std::vector<float>> mesh)
{
    for(int i = 0; i < (int)mesh.size(); ++i)
    {
        for(int j = 0; j < (int)mesh[i].size(); ++j)
            printf("%.2f ", mesh[i][j]);
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