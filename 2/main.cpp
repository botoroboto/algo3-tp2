#include <iostream>
#include <list>
#include <vector>
#include <sstream>

using namespace std;
class Graph {
    int numVertices = 0;
    vector<vector<int>> adjList;

public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    vector<vector<int>> devolverAristas();
};

Graph::Graph(int vertices) {
    numVertices = vertices;
    adjList.resize(vertices);
}

void Graph::addEdge(int src, int dest) {
    adjList[src].push_back(dest);
    adjList[dest].push_back(src);
}

vector<vector<int>> Graph::devolverAristas(){
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
    vector<vector<int>> entrada(M, vector<int>());
    string line;

    for (int i = -1; i < M; ++i) {
        getline(cin, line);
        std::istringstream iss(line);
        string s;
        while(getline(iss, s, ' ')){
            entrada[i].push_back(stoi(s));
        }

    }

     vector<vector<pair<int, int>>> entradaModificada(entrada.size(),
                                                      vector<pair<int, int>>(entrada[0].size(), make_pair(0, -1)));
    int totalVertices = 0;
    //esto sirve para identificar univocamente al vertice y no agregar sus aristas repetidas veces.
    int vertice = 0;
    for (int i = 0; i < entrada.size(); ++i) {
        for (int j = 0; j < entrada[0].size(); ++j) {
            if(entrada[i][j]==1){
                totalVertices += entrada[i][j];
                entradaModificada[i][j].first= entrada[i][j];
                entradaModificada[i][j].second = vertice;
                vertice++;
            }
        }
    }

    DisjointSet ds(totalVertices);

    Graph g = Graph(totalVertices);

    for (int i = 0; i < entradaModificada.size(); ++i) {
        for (int j = 0; j < entradaModificada[0].size(); ++j) {
            if(entradaModificada[i][j].first==1){
                int vertice = entradaModificada[i][j].second;
                if(j+1 < entradaModificada[0].size() && entradaModificada[i][j+1].first == 1){
                    g.addEdge(vertice, entradaModificada[i][j+1].second);
                }
                if( i+1 < entradaModificada.size() && entradaModificada[i+1][j].first == 1){
                    g.addEdge(vertice, entradaModificada[i+1][j].second);
                }
                if(j-1 >=0 && entradaModificada[i][j-1].first == 1){
                    g.addEdge(vertice, entradaModificada[i][j-1].second);
                }
                if( i-1 >=0 && entradaModificada[i-1][j].first == 1){
                    g.addEdge(vertice, entradaModificada[i-1][j].second);
                }
            }
        }
    }
    ds.makeSet();

    for (int i = 0; i < g.devolverAristas().size(); ++i) {
        for (int j = 0; j < g.devolverAristas()[i].size(); ++j) {

            if(ds.findSet(i) != ds.findSet(g.devolverAristas()[i][j])){
                ds.UnionByRank(i, g.devolverAristas()[i][j]);
            }
        }
    }
    cout << ds.returnSets() << endl;

    return 0;
}