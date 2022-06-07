#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "ClassGraph.h"

using namespace std;

//Подсчет максимального возможного количества ребер.
int MaxEdges(int kolV, bool isOrient) {
    if (isOrient)
        return kolV * (kolV - 1);
    return kolV * (kolV - 1) / 2;
}

//ВВод списка ребер.
bool ListOfEdges(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the edges of the graph in the format - two vertices separated by a space" << endl;
    vector<vector<int>> adjacency_matrix(kolV, vector<int>(kolV, 0));
    int v, u;
    for (int i = 0; i < kolE; ++i) {
        cin >> v >> u;
        if (v <= 0 || v > kolV || u <= 0 || u > kolV) {
            cerr << "Incorrect format, try again!" << endl;
            return false;
        }
        if (adjacency_matrix[v - 1][u - 1] == 1 || v == u) {
            cerr << "Multiple edges and loops aren`t allowed" << endl;
            return false;
        }
        adjacency_matrix[v - 1][u - 1] = 1;
        if (!isOrient)
            adjacency_matrix[u - 1][v - 1] = 1;
    }
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод матрицы смежности.
bool AdjMatrix(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the adjacency matrix (V*V)" << endl;
    vector<vector<int>> adjacency_matrix;
    int v;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        for (auto j = 0; j < kolV; ++j) {
            cin >> v;
            if (v != 0 && v != 1) {
                cerr << "Incorrect format, try again!" << endl;
                return false;
            }
            adj.push_back(v);
        }
        adjacency_matrix.push_back(adj);
    }
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод матрицы инциндентности.
bool IncMatrix(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "enter the incidence matrix V*E (vertices horizontally)" << endl;
    vector<vector<int>> incendMatrix;
    int v;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        for (auto j = 0; j < kolE; ++j) {
            cin >> v;
            if (v != 0 && v != 1 && v != -1) {
                cerr << "Incorrect format, try again!" << endl;
                return false;
            }
            adj.push_back(v);
        }
        incendMatrix.push_back(adj);
    }
    auto adjacency_matrix = FromIncMatrixToAdjMatrix(kolV, kolE, incendMatrix, isOrient);
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод списка смежности.
bool AdjList(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the adjacency list in the format:" << endl
         << "on the i-th line, the number of vertices, then the vertices themselves" << endl
         << "Example: 3 1 2 3" << endl;
    vector<vector<int>> adjList;
    int v, kol;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        cin >> kol;
        for (auto j = 0; j < kol; ++j) {
            cin >> v;
            adj.push_back(v);
        }
        adjList.push_back(adj);
    }
    auto adjacency_matrix = FromAdjListToAdjMatrix(kolV, adjList);
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод в виде Fo.
bool Fo(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the graph in the format (FO):" << endl
         << "number of vertices and then all vertices connected to the i-th through the separator 0" << endl
         << "Example: 3 2 3 0 1 0 1 0" << endl;
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    vector<vector<int>> fifo;
    int n, elem, j = 0;
    cin >> n;
    if (isOrient == 0) {
        for (auto i = 0; i < 2 * kolE + kolV; ++i) {
            cin >> elem;
            if (elem != 0) { adjMatrix[j][elem - 1] = 1; }
            else { j++; }
        }
    } else {
        for (auto i = 0; i < kolE + kolV; ++i) {
            cin >> elem;
            if (elem != 0) { adjMatrix[j][elem - 1] = 1; }
            else { j++; }
        }
    }
    graph = Graph(kolV, kolE, adjMatrix, isOrient);
    return true;
}

//ВВод в виде Fi.
bool Fi(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the graph in the format (FI):" << endl
         << "ONLY FOR ORIENTED GRAPH" << endl;
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    vector<vector<int>> fifo;
    int n, elem, j = 0;
    cin >> n;
    for (auto i = 0; i < kolE + kolV; ++i) {
        cin >> elem;
        if (elem != 0) { adjMatrix[elem - 1][j] = 1; }
        else { j++; }
    }
    graph = Graph(kolV, kolE, adjMatrix, isOrient);
    return true;
}

//ВВод в виде MFo.
bool MFo(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the graph in the format MFO:" << endl;
    cout << "Enter ME" << endl;
    int size = isOrient == 1 ? kolE : 2 * kolE;
    int elem;
    vector<int> ME;
    for (int i = 0; i < size; ++i) {
        cin >> elem;
        ME.push_back(elem);
    }
    cout << "Enter MV (V+1)" << endl;
    vector<int> MV;
    for (int i = 0; i < 1 + kolV; ++i) {
        cin >> elem;
        MV.push_back(elem);
    }
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    for (int i = 0; i < kolV; ++i) {
        for (int j = MV[i]; j < MV[i + 1]; ++j) {
            adjMatrix[i][ME[j] - 1] = 1;
        }
    }
    graph = Graph(kolV, kolE, adjMatrix, isOrient);
    return true;
}

//ВВод в виде MFi.
bool MFi(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the graph in the format MFI:" << endl;
    cout << "Enter ME" << endl;
    int size = isOrient == 1 ? kolE : 2 * kolE;
    int elem;
    vector<int> ME;
    for (int i = 0; i < size; ++i) {
        cin >> elem;
        ME.push_back(elem);
    }
    cout << "Enter MV (V+1)" << endl;
    vector<int> MV;
    for (int i = 0; i < 1 + kolV; ++i) {
        cin >> elem;
        MV.push_back(elem);
    }
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    for (int i = 0; i < kolV; ++i) {
        for (int j = MV[i]; j < MV[i + 1]; ++j) {
            adjMatrix[ME[j] - 1][i] = 1;
        }
    }
    graph = Graph(kolV, kolE, adjMatrix, isOrient);
    return true;
}

//ВВод в виде BFo.
bool BFo(Graph &graph, int kolV, int kolE, bool isOrient) {
    cout << "Enter the graph in the format (BFO):" << endl;
    vector<vector<int>> adjMatrix(kolV, vector<int>(kolV, 0));
    int n, elem, j = 0;
    cin >> n;
    for (auto i = 0; i < kolE + kolV; ++i) {
        cin >> elem;
        if (elem != 0) {
            adjMatrix[j][elem - 1] = 1;
            adjMatrix[elem - 1][j] = 1;
        } else { j++; }
    }
    graph = Graph(kolV, kolE, adjMatrix, isOrient);
    return true;
}

//Метод для чтения с консоли.
bool ReadFromConsole(Graph &graph) {
    int kolV = -1, kolE = -1;
    bool isOrient;
    cout << "If you want undirected(neorientir) graph press 0, else press 1" << endl;
    cin >> isOrient;
    cout << "Enter the number of vertices and the number of edges" << endl;
    cin >> kolV >> kolE;
    if (kolV < 0 || kolE < 0 || kolV > 100 || kolE > MaxEdges(kolV, isOrient)) {
        cerr << "Incorrect numbers, try again!" << endl;
        return false;
    }
    cout << "Select the type of graph representation" << endl << "1. adjacency matrix" << endl
         << "2. adjacency list" << endl << "3. list of edges" << endl << "4. incidence matrix" << endl
         << "5. FO presentation" << endl << "6. FI presentation (ONLY FOR ORIENTED)" << endl
         << "7. MFO presentation" << endl << "8. MFI presentation (ONLY FOR ORIENTED)" << endl
         << "9. BFO presentation (ONLY FOR UNDERIVED)" << endl;
    char type;
    cin >> type;
    switch (type) {
        case '1':
            return AdjMatrix(graph, kolV, kolE, isOrient);
        case '2':
            return AdjList(graph, kolV, kolE, isOrient);
        case '3':
            return ListOfEdges(graph, kolV, kolE, isOrient);
        case '4':
            return IncMatrix(graph, kolV, kolE, isOrient);
        case '5':
            return Fo(graph, kolV, kolE, isOrient);
        case '6':
            return Fi(graph, kolV, kolE, isOrient);
        case '7':
            return MFo(graph, kolV, kolE, isOrient);
        case '8':
            return MFi(graph, kolV, kolE, isOrient);
        case '9':
            return BFo(graph, kolV, kolE, isOrient);
        default:
            cerr << "Undefined command, try again!" << endl;
            break;
    }
}

//ВВод списка ребер из файла.
bool ListOfEdgesFile(Graph &graph, int kolV, int kolE, bool isOrient) {
    string path;
    cout << "Enter the path";
    cin >> path;
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Sorry, we can`t open your file" << endl;
        return false;
    }
    vector<vector<int>> adjacency_matrix(kolV, vector<int>(kolV, 0));
    int v, u;
    for (int i = 0; i < kolE; ++i) {
        fin >> v >> u;
        cout << v << " " << u << endl;
        if (v <= 0 || v > kolV || u <= 0 || u > kolV) {
            cerr << "Incorrect format, try again!" << endl;
            return false;
        }
        if (adjacency_matrix[v - 1][u - 1] == 1 || u == v) {
            cerr << "Multiple edges and loops aren`t allowed" << endl;
            return false;
        }
        adjacency_matrix[v - 1][u - 1] = 1;
        if (!isOrient)
            adjacency_matrix[u - 1][v - 1] = 1;
    }
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод матрицы смежности из файла.
bool AdjMatrixFile(Graph &graph, int kolV, int kolE, bool isOrient) {
    string path;
    cout << "Enter the path";
    cin >> path;
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Sorry, we can`t open your file" << endl;
        return false;
    }
    vector<vector<int>> adjacency_matrix;
    int v;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        for (auto j = 0; j < kolV; ++j) {
            fin >> v;
            if (v != 0 && v != 1) {
                cerr << "Incorrect format, try again!" << endl;
                return false;
            }
            adj.push_back(v);
        }
        adjacency_matrix.push_back(adj);
    }
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод матрицы инциндентности из файла.
bool IncMatrixFile(Graph &graph, int kolV, int kolE, bool isOrient) {
    string path;
    cout << "Enter the path";
    cin >> path;
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Sorry, we can`t open your file" << endl;
        return false;
    }
    vector<vector<int>> incendMatrix;
    int v;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        for (auto j = 0; j < kolE; ++j) {
            fin >> v;
            if (v != 0 && v != 1 && v != -1) {
                cerr << "Incorrect format, try again!" << endl;
                return false;
            }
            adj.push_back(v);
        }
        incendMatrix.push_back(adj);
    }
    auto adjacency_matrix = FromIncMatrixToAdjMatrix(kolV, kolE, incendMatrix, isOrient);
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//ВВод списка смежности из файла.
bool AdjListFile(Graph &graph, int kolV, int kolE, bool isOrient) {
    string path;
    cout << "Enter the path";
    cin >> path;
    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Sorry, we can`t open your file" << endl;
        return false;
    }
    vector<vector<int>> adjList;
    int v, kol;
    for (auto i = 0; i < kolV; ++i) {
        vector<int> adj;
        fin >> kol;
        for (auto j = 0; j < kol; ++j) {
            fin >> v;
            adj.push_back(v);
        }
        adjList.push_back(adj);
    }
    auto adjacency_matrix = FromAdjListToAdjMatrix(kolV, adjList);
    graph = Graph(kolV, kolE, adjacency_matrix, isOrient);
    return true;
}

//Метод для чтения из файла.
bool ReadFromFile(Graph &graph) {
    int kolV = -1, kolE = -1;
    bool isOrient;
    cout << "If you want undirected(neorientir) graph press 0, else press 1" << endl;
    cin >> isOrient;
    cout << "Enter the number of vertices and the number of edges" << endl;
    cin >> kolV >> kolE;
    if (kolV < 0 || kolE < 0 || kolV > 100 || kolE > MaxEdges(kolV, isOrient)) {
        cerr << "Incorrect numbers, try again!" << endl;
        return false;
    }
    cout << "Select the type of graph representation" << endl << "1. adjacency matrix" << endl
         << "2. adjacency list" << endl << "3. list of edges" << endl << "4. incidence matrix" << endl
         << "5. FO presentation" << endl << "6. FI presentation (ONLY FOR ORIENTED)" << endl
         << "7. MFO presentation" << endl << "8. MFI presentation (ONLY FOR ORIENTED)" << endl
         << "9. BFO presentation (ONLY FOR UNDERIVED)" << endl;
    char type;
    cin >> type;
    switch (type) {
        case '1':
            return AdjMatrixFile(graph, kolV, kolE, isOrient);
        case '2':
            return AdjListFile(graph, kolV, kolE, isOrient);
        case '3':
            return ListOfEdgesFile(graph, kolV, kolE, isOrient);
        case '4':
            return IncMatrixFile(graph, kolV, kolE, isOrient);
        case '5':
            return Fo(graph, kolV, kolE, isOrient);
        case '6':
            return Fi(graph, kolV, kolE, isOrient);
        case '7':
            return MFo(graph, kolV, kolE, isOrient);
        case '8':
            return MFi(graph, kolV, kolE, isOrient);
        case '9':
            return BFo(graph, kolV, kolE, isOrient);
        default:
            cerr << "Undefined command, try again!" << endl;
            break;
    }
}

//Метод для определения нового графа.
bool ReadNewGraph(Graph &graph) {
    cout << "If you want to read from the console -> press 0" << endl
         << "If you want to read from the file -> press another symbol" << endl;
    string type;
    cin >> type;
    if (type == "0")
        return ReadFromConsole(graph);
    return ReadFromFile(graph);
}

//Метод для печати меню.
void PrintMenu() {
    cout << "Menu" << endl;
    cout << "1. Convert to an adjacency matrix" << endl;
    cout << "2. Convert to an adjacency list" << endl;
    cout << "3. Convert to a list of edges" << endl;
    cout << "4. Convert to an incidence matrix" << endl;
    cout << "5. Bypass the graph using dfs (recursion)" << endl;
    cout << "6. Bypass the graph using dfs (linear)" << endl;
    cout << "7. Bypass the graph using bfs" << endl;
    cout << "8. Counting the number of degrees" << endl;
    cout << "9. Counting the number of edges" << endl;
    cout << "m. Print menu" << endl;
    cout << "0. Finish working with the graph" << endl;
}

//Метод регулирующий работу программы.
void Working(Graph &graph) {
    PrintMenu();
    string command;
    int i = 0;
    while (true) {
        cin >> command;
        if (i++ >= 2) {
            cin.clear();
            i = 0;
        }
        switch (command[0]) {
            case '0':
                return;
            case '1':
                graph.PrintAdjacencyMatrix();
                break;
            case '2':
                graph.PrintAdjacencyList();
                break;
            case '3':
                graph.PrintListOfEdges();
                break;
            case '4':
                graph.PrintIncidenceMatrix();
                break;
            case '5':
                graph.traversalAlgo(0);
                break;
            case '6':
                graph.traversalAlgo(1);
                break;
            case '7':
                graph.traversalAlgo(2);
                break;
            case '8':
                graph.PrintVertexDegree();
                break;
            case '9':
                graph.PrintCountOfEdges();
                break;
            case 'm':
                PrintMenu();
                break;
            default:
                cerr << "Undefined command, try again!" << endl;
                break;
        }
    }
}

//Метод запуска.
int main() {
    string flag = "";
    while (true) {
        Graph graph;
        if (ReadNewGraph(graph)) {
            Working(graph);
        }
        cout << "If you want to exit -> press 0, else press on any symbol" << endl;
        cin >> flag;
        if (flag == "0") {
            cout << "Good bye, dear user!";
            break;
        }
    }
}