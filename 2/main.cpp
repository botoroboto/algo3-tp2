#include <iostream>
#include <list>
#include <vector>
#include <sstream>

using namespace std;
class Graph {
    int vertexQuantity = 0;
    vector<vector<int>> adjList;

public:
    Graph(int vertex);
    void addEdge(int src, int dest);
    vector<vector<int>> returnEdges();
};

Graph::Graph(int vertex) {
    vertexQuantity = vertex;
    adjList.resize(vertex);
}

void Graph::addEdge(int src, int dest) {
    adjList[src].push_back(dest);
    adjList[dest].push_back(src);
}

vector<vector<int>> Graph::returnEdges(){
    return adjList;
}

class DisjointSet {
    int *rank, *parent, n, numberOfSets;

public:
    // Con esto se crea un conjunto de n items
    DisjointSet(int n)
    {
        rank = new int[n];
        parent = new int[n];
        this->n = n;
        this->numberOfSets = n;
        makeSet();
    }

    // Con esto se crean conjuntos consistente en ese unico elemento
    void makeSet()
    {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int findSet(int x)
    {
        // Se busca el parent, representativo, del conjunto al que pertenezca x
        if (parent[x] != x) {

            // Si x no es representativo del conjunto, entonces debera buscarse el elemento que lo sea
            parent[x] = findSet(parent[x]);
        }

        return parent[x];
    }

    // Une dos conjuntos disjuntos representados por x e y
    void UnionByRank(int x, int y)
    {
        // encuentra los conjuntos a los que pertenecen x e y
        int xset = findSet(x);
        int yset = findSet(y);

        // Si pertenecen al mismo conjunto no se hace nada nuevo
        if (xset == yset)
            return;

        // Se chequea la diferencia de rank tal como propone el libro de Cormen para mejorar la complejidad.
        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
        }
        else if (rank[xset] > rank[yset]) {
            parent[yset] = xset;
        }
        // Si tienen el mismo rank, se debera aumentar
        else {
            parent[yset] = xset;
            rank[xset] = rank[xset] + 1;
        }
        numberOfSets = numberOfSets-1;
    }
    int returnSets(){
        return this->numberOfSets;
    }
};


int main() {

    int M, N;
    cin >> M >> N;
    vector<vector<pair<int, int>>> input(M, vector<pair<int, int>>());
    string line;
    pair<int,int> p;
    // Esto permite diferenciarlo e identificarlo.
    int vertexId = 0;
    for (int i = -1; i < M; ++i) {
        getline(cin, line);
        std::istringstream iss(line);
        string s;
        while(getline(iss, s, ' ')){
            if(stoi(s)==1){
                p = make_pair(stoi(s), vertexId);
                input[i].push_back(p);
                vertexId++;
            }else{
                p = make_pair(stoi(s), -1);
                input[i].push_back(p);
            }

        }

    }
    int totalVertex = vertexId;

    DisjointSet ds(totalVertex);
    ds.makeSet();

    Graph g = Graph(totalVertex);

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[0].size(); ++j) {
            if(input[i][j].first==1){
                int vertex = input[i][j].second;
                if(j+1 < input[0].size() && input[i][j+1].first == 1){
                    g.addEdge(vertex, input[i][j+1].second);
                }
                if( i+1 < input.size() && input[i+1][j].first == 1){
                    g.addEdge(vertex, input[i+1][j].second);
                }
                if(j-1 >=0 && input[i][j-1].first == 1){
                    g.addEdge(vertex, input[i][j-1].second);
                }
                if( i-1 >=0 && input[i-1][j].first == 1){
                    g.addEdge(vertex, input[i-1][j].second);
                }
            }
        }
    }


    for (int i = 0; i < g.returnEdges().size(); ++i) {
        for (int j = 0; j < g.returnEdges()[i].size(); ++j) {

            if(ds.findSet(i) != ds.findSet(g.returnEdges()[i][j])){
                ds.UnionByRank(i, g.returnEdges()[i][j]);
            }
        }
    }
    cout << ds.returnSets() << endl;

    return 0;
}