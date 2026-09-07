#include "floyd_warshall.h"

#include <fstream>
#include <iostream>
#include <limits>

bool FloydWarshallGraph::loadFromFile(const std::string& path)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        std::cerr << "Could not open file: "
                  << path << '\n';
        return false;
    }

    if (!(in >> numVertices))
    {
        std::cerr << "Invalid vertex count.\n";
        return false;
    }

    if (numVertices <= 0)
    {
        std::cerr << "Invalid number of vertices.\n";
        return false;
    }

    matrix.assign(
        numVertices,
        std::vector<double>(numVertices)
    );

    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = 0; j < numVertices; ++j)
        {
            std::string value;

            if (!(in >> value))
            {
                std::cerr
                    << "Invalid matrix: expected "
                    << numVertices * numVertices
                    << " values.\n";

                return false;
            }

            if (value == "INF")
            {
                matrix[i][j] =
                    std::numeric_limits<double>::infinity();
            }
            else
            {
                try
                {
                    matrix[i][j] = std::stod(value);
                }
                catch (...)
                {
                    std::cerr
                        << "Invalid matrix value: "
                        << value << '\n';

                    return false;
                }
            }
        }
    }

    return true;
}


std::vector<std::vector<double>> floydWarshall(
    const FloydWarshallGraph& graph
)
{
    const double INF =
        std::numeric_limits<double>::infinity();

    const int n = graph.numVertices;

    std::vector<std::vector<double>> dist =
        graph.matrix;

    /*
        Floyd-Warshall:
        dist[i][j] = shortest distance from i to j.
    */

    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            if (dist[i][k] == INF)
            {
                continue;
            }

            for (int j = 0; j < n; ++j)
            {
                if (dist[k][j] == INF)
                {
                    continue;
                }

                double throughK =
                    dist[i][k] + dist[k][j];

                if (throughK < dist[i][j])
                {
                    dist[i][j] = throughK;
                }
            }
        }
    }

    return dist;
}


bool hasNegativeCycle(
    const std::vector<std::vector<double>>& dist
)
{
    const int n =
        static_cast<int>(dist.size());

    for (int i = 0; i < n; ++i)
    {
        if (dist[i][i] < 0)
        {
            return true;
        }
    }

    return false;
}