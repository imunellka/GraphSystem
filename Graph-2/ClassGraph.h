
#ifndef GRAPH_CLASSGRAPH_H
#define GRAPH_CLASSGRAPH_H

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <filesystem>
#include "translator.h"

using namespace std;

//Определение класса для работы с графом.
class Graph {
private:
    int kolV;
    int kolE;
    bool isOrient;
    vector<vector<int>> adjMatrix;
public:
    Graph() {}
    //Конструктор.
    Graph(int kolv, int kole, vector<vector<int>> adjmatrix, bool isorient) {
        kolV = static_cast<size_t>(kolv);
        kolE = static_cast<size_t>(kole);
        adjMatrix = move(adjmatrix);
        isOrient = isorient;
    }
    //Определение потока вывода.
    string GetStream() {
        cout << "If you want to output the graph to console press 0, to file press - another symbol" << endl;
        string type;
        cin >> type;
        return type;
    }

    //Вывод количества ребер.
    void PrintCountOfEdges() {
        if (isOrient)
            cout << "Number of arcs = " << kolE << endl;
        else
            cout << "Number of edges = " << kolE << endl;
    }

    //Вывод степеней матрицы.
    void PrintVertexDegree() {
        for (auto i = 0; i < kolV; ++i) {
            cout << "Vertex #" << i + 1;
            int out = 0, in = 0;
            for (auto j = 0; j < kolV; ++j) {
                if (adjMatrix[i][j] != 0)
                    out++;
                if (adjMatrix[j][i] != 0)
                    in++;
            }
            if (isOrient)
                cout << " Outgoing = " << out << ", Incoming = " << in << endl;
            else
                cout << " Gegree = " << (out + in) / 2 << endl;
        }
    }

    //Вывод матрицы смежности.
    void PrintAdjacencyMatrix() {
        string path = "output.txt";
        ofstream fout(path);
        string type = GetStream();
        ostream &out = type == "0" ? cout : fout;
        out << "\t";
        for (auto i = 0; i < kolV; ++i) {
            out << i + 1 << "\t";
        }
        out << endl;
        for (auto i = 0; i < kolV; ++i) {
            out << i + 1 << "\t";
            for (auto j = 0; j < kolV; ++j) {
                out << adjMatrix[i][j] << "\t";
            }
            out << endl;
        }
        cout << "Next command,please" << endl;
    }

    //Вывод списка смежности.
    void PrintAdjacencyList() {
        string path = "output.txt";
        ofstream fout(path);
        string type = GetStream();
        ostream &out = type == "0" ? cout : fout;
        vector<vector<int>> adjacency_list = GetAdjList(kolV, adjMatrix);
        for (size_t i = 0; i < kolV; ++i) {
            out << i + 1 << " | ";
            for (int v: adjacency_list[i]) {
                out << v + 1 << " ";
            }
            out << endl;
        }
        cout << "Next command,please" << endl;
    }

    //Вывод списка ребер.
    void PrintListOfEdges() {
        string path = "output.txt";
        ofstream fout(path);
        string type = GetStream();
        ostream &out = type == "0" ? cout : fout;
        vector<pair<int, int>> list_of_edges = GetListOfEdges(kolV, adjMatrix, isOrient);
        for (auto edge: list_of_edges) {
            out << edge.first << " " << edge.second << endl;
        }
        cout << "Next command,please" << endl;
    }

    //Вывод матрицы инциндентности.
    void PrintIncidenceMatrix() {
        string path = "output.txt";
        ofstream fout(path);
        string type = GetStream();
        ostream &out = type == "0" ? cout : fout;
        vector<vector<int>> incidence_matrix = GetIncMatrix(kolV, kolE, adjMatrix, isOrient);
        out << "\t";
        for (size_t i = 0; i < kolE; ++i) {
            out << i + 1 << "\t";
        }
        out << endl;
        for (size_t i = 0; i < kolV; ++i) {
            out << i + 1 << "\t";
            for (size_t j = 0; j < kolE; ++j) {
                out << incidence_matrix[i][j] << "\t";
            }
            out << endl;
        }
        cout << "Next command,please" << endl;
    }

    //Рекурсивный обход Dfs.
    void RecursionDfs(vector<bool> &used, int v) {
        used[v] = true;
        cout << "V #" << v + 1 << " ";
        for (size_t i = 0; i < kolV; ++i)
            if (adjMatrix[v][i] && !used[i])
                RecursionDfs(used, i);
    }

    //Прямой обход Dfs.
    void StackDfs(vector<bool> &used, int v) {
        cout << "V #" << v + 1 << " ";
        stack<int> que;
        que.push(v);
        while (!que.empty()) {
            bool flag = false;
            int x = que.top();
            used[x] = true;
            for (size_t i = 0; i < kolV; ++i) {
                if (adjMatrix[x][i] && !used[i]) {
                    cout << "V #" << i + 1 << " ";
                    que.push(i);
                    flag = true;
                    break;
                }
            }
            if (flag)
                continue;
            que.pop();
        }
    }

    //обход bfs.
    void Bfs(int v, vector<bool> &used) {
        queue<int> que;
        que.push(v);
        while (!que.empty()) {
            int x = que.front();
            que.pop();
            if (used[x])
                continue;
            cout << "V #" << x + 1 << " ";
            used[x] = true;
            for (size_t i = 0; i < kolV; ++i) {
                if (adjMatrix[x][i] && !used[i]) {
                    que.push(i);
                }
            }
        }
    }

    //Определение типа обхода.
    void traversalAlgo(int type) {
        vector<bool> used(kolV, false);
        int component = 0;
        for (size_t i = 0; i < kolV; ++i)
            if (!used[i]) {
                cout << component + 1 << "-th component:" << endl;
                switch (type) {
                    case 0:
                        RecursionDfs(used, i);
                        break;
                    case 1:
                        StackDfs(used, i);
                        break;
                    case 2:
                        Bfs(i, used);
                        break;
                }
                component++;
                cout << endl << "=======================" << endl;
            }
    }
};

#endif //GRAPH_CLASSGRAPH_H
