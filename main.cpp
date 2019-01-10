#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

int readFromFile(char* filename, std::vector<int>& vec, int &n)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in);

    if(!inputFile.is_open())
    {
        std::cout << filename << " can not be opened\n";
        return 0;
    }

    int value;
    while (inputFile >> value)
    {
        vec.push_back(value);
    }

    n = vec.size();
    std::cout << n << " numbers have been read successfully from " << filename << std::endl;
    inputFile.close();
    return 1;
}

void launchGenerator(std::vector<int>& vec, int n)
{
    std::cout << "Generator launched\n";

    srand (time(NULL));
    for (int i = 0; i < n; i++)
        vec.push_back(rand() % 900 + 101); // generating distances [101; 1000]

    std::cout << "Generated distance:\n";
    for (int i = 0; i < n; i++)
        std::cout << vec[i] << " ";

    std::cout << std::endl;
}

int sumVector (std::vector<int>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0);
}

void algorithm(std::vector<int>& distances, int pairedVertice[], int res[], int n, bool &flag) {

    int totalDistance = sumVector(distances); // sum of all distances
    int length = 0; // current length (analyzed)
    int verticeHead = 0; // destination mast for the analyzed path
    int backwardDistance, forwardDistance; // distances for the analyzed path

    for(int i = 0; i < n; i++)
    {
        while (length < (int) totalDistance/2) // crossing the half of the total distance
        {
            length += distances[verticeHead];
            ++verticeHead;
            verticeHead %= n;
        }

        if ((length == (int)(totalDistance/2)) || ((length == (int)(totalDistance/2) + 1) && (totalDistance % 2 == 1))) // beforetime algorithm termination, answer found
        {
            std::cout << "Vertices: " << i << " " << verticeHead << ". Distance: " << (int) totalDistance/2 << std::endl;
            flag = true;
            break;
        }

        backwardDistance = totalDistance - length; // calculating backward distance
        forwardDistance = (verticeHead != 0) ? length - distances[verticeHead-1] : length - distances[n-1]; // calculating forward distance

        res[i] = std::max(backwardDistance, forwardDistance);
        pairedVertice[i] = res[i] == backwardDistance ? verticeHead  : (verticeHead + n - 1) % n; // choosing the destination vertice
        length -= distances[i]; // current path length
    }
}

int main(int argc, char** argv)
{

    int n; // number of masts
    std::vector<int> distances; // vector for input to be stored

    if(argv[1][0] == '-' && argv[1][1] == 'h')
    {
        std::cout << "Generator: AAL_Tomas_Suchodolskij [number of vertices]\nReading from file: AAL_Tomas_Suchodolskij [file name]\n";
        return 0;
    }

    if (argc == 2)
    {
        if (atoi(argv[1]))
        {
            n = atoi(argv[1]);
            launchGenerator(distances, n);
        }
        else if (readFromFile(argv[1], distances, n) == 1)
            std::cout << argv[1] << " has been closed successfully\n";
        else
            return 0;
    }
    else
    {
        std::cout << "Invalid arguments.\nUse -h for help.";
        return 0;
    }

    int pairedVertice[n] = {}; // array for vertice pairs to be stored (index - first vertice, value - second vertice)
    int res[n] = {}; // array for path distances to be stored
    bool flag = false; // true = beforetime algorithm termination

    algorithm(distances, pairedVertice, res, n, flag);

    if(!flag)
    {
        int maxDistanceVertice = 0;
        for (int i = 1; i < n; i++)
            if(res[i] > res[maxDistanceVertice]) {
                maxDistanceVertice = i;
            }

        std::cout << "Vertices: " << maxDistanceVertice << " " << pairedVertice[maxDistanceVertice] << ". Distance: " << res[maxDistanceVertice] << std::endl;
    }

    return 0;
}
