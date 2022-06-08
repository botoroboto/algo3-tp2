#include <iostream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

int const UNDEFINED_VALUE = -1;
int const NO_MULTIPLES_PATHS = -1;

struct Edge {
    int v1 = UNDEFINED_VALUE;
    int v2 = UNDEFINED_VALUE;
};

Edge EDGE_NO_MULTIPLE_PATHS;

class Graph {
    int numVertices = 0;
    vector<vector<int>> adjList;
    vector<bool> visited;
    vector<vector<bool>> visitedEdges;
    vector<int> d;
    vector<int> parent;

public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    Edge BFS(int startVertex, vector<vector<int>> &resMatrix);
    void esGeodesico();
    void clear();
};

Graph::Graph(int vertices) {
    numVertices = vertices;
    adjList.resize(numVertices);
    visited.resize(numVertices, false);
    parent.resize(numVertices);
    visitedEdges.resize(numVertices, vector<bool>(numVertices, false));
}

void Graph::clear() {
    vector<bool> v(numVertices, false);
    vector<int> distance(numVertices);
    vector<int> p(numVertices);
    visitedEdges.resize(numVertices, vector<bool>(numVertices, false));
    visited = v;
    d = distance;
    parent = p;
}

void Graph::addEdge(int src, int dest) {
    adjList[src].push_back(dest);
    adjList[dest].push_back(src);
}

// BFS modificado, devuelve EDGE_NO_MULTIPLE_PATHS si salió bien y sino devuelve el vértice que genera conflicto y al cual podemos encontrar 2 caminos desde startVertex mínimos
Edge Graph::BFS(int startVertex, vector<vector<int>> &resMatrix) {
    this->clear();
    queue<int> q;
    q.push(startVertex);
    visited[startVertex] = true;
    parent[startVertex] = UNDEFINED_VALUE;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : adjList[v]) {
            if (!visited[u]) {
                visited[u] = true;
                q.push(u);
                d[u] = d[v] + 1;
                parent[u] = v;
                resMatrix[startVertex][u] = v; // O(1)
                visitedEdges[u][v] = true;  // O(1)
                visitedEdges[v][u] = true;   // O(1)
            } else if (
                    !visitedEdges[u][v] &&   // verifica que no se pase por la misma arista para que no se produzcan ciclos
                    !visitedEdges[v][u] &&   // verifica que no se pase por la misma arista para que no se produzcan ciclos
                    d[v] + 1 == d[u]  // ambos son caminos mínimos
            ) {
                Edge problem;
                problem.v1 = v;
                problem.v2 = u;

                return problem;
            }
        }
    }

    return EDGE_NO_MULTIPLE_PATHS;
}

bool operator==(const Edge& a, const Edge& b)
{
    return a.v1 == b.v1 && a.v2 == b.v2;
}

void Graph::esGeodesico() {
    vector<vector<int> > resMatrix(numVertices, vector<int> (numVertices, UNDEFINED_VALUE));
    Edge lastEdge = EDGE_NO_MULTIPLE_PATHS;
    int i=0;
    while (i < numVertices && lastEdge==EDGE_NO_MULTIPLE_PATHS) {   // esto cuesta O(n)
        lastEdge = BFS(i, resMatrix);   // esto cuesta O(n + m) ya que es BFS y lo que se agrega aporta O(1) que son las comparaciones en el else que se fija si un nodo ya fue visitado
        i++;
    }

    if (!(lastEdge == EDGE_NO_MULTIPLE_PATHS)) {
        int lastVertexPath1 = lastEdge.v2;
        int lastVertexPath2 = lastEdge.v1;
        int source = i - 1;
        vector<int> path1;
        vector<int> path2;
        path2.push_back(lastVertexPath1);

        while (lastVertexPath1 != source) {   // reconstruye un camino
            path1.push_back(lastVertexPath1);
            lastVertexPath1 = resMatrix[i - 1][lastVertexPath1];
        }

        while (lastVertexPath2 != source) {   // reconstruye el otro camino
            path2.push_back(lastVertexPath2);
            lastVertexPath2 = resMatrix[i - 1][lastVertexPath2];
        }

        path1.push_back(source);
        path2.push_back(source);

        for (int j = 0; j < path1.size(); ++j) {
            cout << path1[j] << " ";
        }
        cout<<"----"<<endl;

        for (int j = 0; j < path2.size(); ++j) {
            cout<<path2[j]<< " ";
        }
    }else {
        cout << "1" <<endl;
        for (int j = 0; j < resMatrix.size(); ++j) {
            for (int k = 0; k < resMatrix[0].size(); ++k) {
                int value =  resMatrix[j][k];
                if (value != UNDEFINED_VALUE) {
                    cout << resMatrix[j][k] << "  ";
                }
            }
            cout << "" << endl;
        }
    }
}

int main() {
    Graph g(10);
    g.addEdge(5,8);
    g.addEdge(4,3);
    g.addEdge(3,8);
    g.addEdge(8,2);
    g.addEdge(8,9);
    g.addEdge(5,9);
    g.addEdge(9,7);

    g.esGeodesico();

    return 0;
}