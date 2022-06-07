#ifndef GRAPH_TRANSLATOR_H
#define GRAPH_TRANSLATOR_H

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

//Конвертер из листа смежности в матрицу смежности.
vector<vector<int>> FromAdjListToAdjMatrix(int kolV, vector<vector<int>> adjList) {
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    for (auto i = 0; i < kolV; ++i) {
        for (auto vertex: adjList[i]) {
            adjMatrix[i][vertex - 1] = 1;
        }
    }
    return adjMatrix;
}

//Конвертер из матрицы инциндентности в матрицу смежности.
vector<vector<int>> FromIncMatrixToAdjMatrix(int kolV, int kolE, vector<vector<int>> incMatrix, bool isOrient) {
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    if (isOrient) {
        for (int i = 0; i < kolE; ++i) {
            pair<int, int> Edge;
            Edge.first = -1;
            Edge.second = -1;
            for (int j = 0; j < kolV; ++j) {
                if (incMatrix[j][i] == -1) {
                    Edge.first = j;
                }
                if (incMatrix[j][i] == 1) {
                    Edge.second = j;
                }
                if (Edge.first != -1 && Edge.second != -1) break;
            }
            adjMatrix[Edge.first][Edge.second] = 1;
        }
    } else {
        for (int i = 0; i < kolE; ++i) {
            pair<int, int> Edge;
            Edge.first = -1;
            Edge.second = -1;
            for (int j = 0; j < kolV; ++j) {
                if (incMatrix[j][i] == 1 && Edge.first == -1) {
                    Edge.first = j;
                } else if (incMatrix[j][i] == 1) {
                    Edge.second = j;
                }
                if (Edge.first != -1 && Edge.second != -1) break;
            }
            adjMatrix[Edge.first][Edge.second] = 1;
            adjMatrix[Edge.second][Edge.first] = 1;
        }
    }
    return adjMatrix;
}

//Конвертер из матрицы смежности в список ребер.
vector<pair<int, int>> GetListOfEdges(int kolV, vector<vector<int>> adjMatrix, bool isOrient) {
    vector<pair<int, int>> listEdges;
    for (int i = 0; i < kolV; ++i) {
        for (int j = 0; j < kolV; ++j) {
            if (!isOrient && i >= j)
                continue;
            if (adjMatrix[i][j]) {
                listEdges.push_back({i + 1, j + 1});
            }
        }
    }
    return listEdges;
}

//Конвертер из матрицы смежности в список смежности.
vector<vector<int>> GetAdjList(int kolV, vector<vector<int>> adjMatrix) {
    vector<vector<int>> adjList(kolV);
    for (int i = 0; i < kolV; ++i) {
        for (int j = 0; j < kolV; ++j) {
            if (adjMatrix[i][j]) {
                adjList[i].push_back(j);
            }
        }
    }
    return adjList;
}

//Конвертер из матрицы смежности в матрицу инциндентности.
vector<vector<int>> GetIncMatrix(int kolV, int kolE, const vector<vector<int>> adjMatrix, bool isOrient) {
    vector<vector<int>> incMatrix(kolV, vector<int>(kolE, 0));
    for (int i = 0; i < kolV; ++i) {
        for (int j = 0; j < kolE; ++j) {
            incMatrix[i][j] = 0;
        }
    }
    if (isOrient) {
        int Counter = 0;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            for (int j = 0; j < adjMatrix.size(); ++j) {
                if (adjMatrix[i][j] == 1) {
                    incMatrix[i][Counter] = -1;
                    incMatrix[j][Counter] = 1;
                    Counter++;
                }
            }
        }
    } else {
        int Counter = 0;
        for (int i = 0; i < adjMatrix.size(); ++i) {
            for (int j = i; j < adjMatrix.size(); ++j) {
                if (adjMatrix[i][j] == 1) {
                    incMatrix[i][Counter] = 1;
                    incMatrix[j][Counter] = 1;
                    Counter++;
                }
            }
        }
    }
    return incMatrix;
}

#endif //GRAPH_TRANSLATOR_H
