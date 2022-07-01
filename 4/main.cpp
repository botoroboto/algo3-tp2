#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <bits/stdc++.h>
using namespace std;
#define INF INT_MAX

int TYPE_B = 0;
int TYPE_C = 1;

int const EMPTY_INTERVAL_ID = -1000;

class AdjListNode
{
    int v;
    int weight;
public:
    AdjListNode(int _v, int _w)  { v = _v;  weight = _w;}
    int getV()       {  return v;  }
    int getWeight()  {  return weight; }
};

// Class to represent a graph using adjacency
// list representation
class Graph
{
    int V;    // No. of vertices'

    // Pointer to an array containing adjacency lists
    list<AdjListNode> *adj;

    // A function used by shortestPath
    void topologicalSortUtil(int v, bool visited[], stack<int> &Stack);
public:
    Graph(int V);   // Constructor

    // function to add an edge to graph
    void addEdge(int u, int v, int weight);

    // Finds shortest paths from given source vertex
    vector<int> shortestPath(int s);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<AdjListNode>[V];
}

void Graph::addEdge(int u, int v, int weight)
{
    AdjListNode node(v, weight);
    adj[u].push_back(node); // Add v to u's list
}

void Graph::topologicalSortUtil(int v, bool visited[], stack<int> &Stack)
{
    // Mark the current node as visited
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    list<AdjListNode>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        AdjListNode node = *i;
        if (!visited[node.getV()])
            topologicalSortUtil(node.getV(), visited, Stack);
    }

    // Push current vertex to stack which stores topological sort
    Stack.push(v);
}

vector<int> Graph::shortestPath(int s)
{
    stack<int> Stack;
    int dist[V];
    int parent[V];

    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);

    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[s] = 0;

    while (Stack.empty() == false)
    {
        int u = Stack.top();
        Stack.pop();

        list<AdjListNode>::iterator i;
        if (dist[u] != INF)
        {
            for (i = adj[u].begin(); i != adj[u].end(); ++i)
                if (dist[i->getV()] > dist[u] + i->getWeight()) {
                    dist[i->getV()] = dist[u] + i->getWeight();
                    parent[i->getV()] = u;
                }

        }
    }

    int i = V-1;
    vector<int> path = {};
    while(i>0) {
        path.push_back(parent[i]);
        i = parent[i];
    }

    return path;
}

struct link {
    int vertex, type, length;

    bool operator==(const link & l) const {
        return l.vertex == vertex && l.type == type && l.length == length;
    }

};

struct cdtVertex{
    int d, pi, id;
};

struct graph {
    vector<list<link>> adjacents;
    vector<cdtVertex> vertexes;
};

struct interval{
    int a, b;
    bool contained;
    int realIndex;
};

int n,a,b;
vector<interval> I;
vector<interval> INOUT_MAPPER = {};

// remueve los intervalos contenidos
// O(n^2) pues por cada n recorre todos los n verificando cual está contenido
vector<interval> removeContainedIntervals(vector<interval> &I){
    vector<interval> retval;

    //add I0
    int min = I[0].a;
    int max = I[0].b;
    for (int i = 0; i < I.size(); ++i) {
        if(I[i].b > max) {
           max = I[i].b;
        }
    }

    interval I0, In;
    I0.b = min - 1;
    I0.a = min - 2;
    I0.contained = false;
    I0.realIndex = n+4;

    In.a = max + 1;
    In.b = max + 2;
    In.contained = false;
    In.realIndex = n+5;

    retval.push_back(I0);

    for(int j=0;j<I.size();j++){
        bool contained = false;
        for(int k=0; k<I.size();k++){
            if(I[k].a <= I[j].a && I[k].b >= I[j].b && j!=k){
                contained = true;
                break;
            }
        }

        if(!contained){
            retval.push_back(I[j]);
        }
        I[j].contained=contained;
    }

    retval.push_back(In);
    return retval;
}

// O(n^2) pues por cada n recorre todos los n verificando cuales se solapan
void crearAristasB(graph &cdtGraph, vector<interval> &noContained) {
    for (int i = 0; i < noContained.size(); ++i) {
        for (int j = i; j < noContained.size() ; ++j) {
            if(
                    j!=i && noContained[i].a < noContained[j].a &&
                    noContained[i].b > noContained[j].a &&
                    noContained[i].b < noContained[j].b
                    ) {
                link dummy;
                dummy.vertex = j;
                dummy.length= 1;
                dummy.type= TYPE_B;

                cdtGraph.adjacents[i].push_back(dummy);
            }
        }
    }
}

// O(n^2)
void crearAristasC(graph &cdtGraph, vector<interval> &noContained) {

    // ORDENAMIENTO POR INICIO Y POR FIN CON BUCKET SORT
    interval emptyInterval;
    emptyInterval.realIndex = EMPTY_INTERVAL_ID;
    vector<interval> buckets(2*n +2, emptyInterval);
    vector<interval> buckets_fin(2*n +2, emptyInterval);

    buckets_fin[0] = noContained[0];
    buckets_fin[0].realIndex = 0;
    buckets[0] = noContained[0];
    buckets[0].realIndex = 0;

    for (int i = 1; i < noContained.size() - 1; ++i) {
        buckets_fin[noContained[i].b + 1] = noContained[i];
        buckets_fin[noContained[i].b +1].realIndex = i;

        buckets[noContained[i].a + 1] = noContained[i];
        buckets[noContained[i].a + 1].realIndex = i;
    }

    buckets_fin[buckets_fin.size() - 1] = noContained[noContained.size() -1];
    buckets_fin[buckets_fin.size() - 1].realIndex = noContained.size()-1;
    buckets[buckets_fin.size() - 1] = noContained[noContained.size()-1];
    buckets[buckets_fin.size() - 1].realIndex = noContained.size()-1;

    vector<interval> ordenadoPorFin;
    vector<interval> ordenadoPorInicio;

    for (int i = 0; i < buckets.size(); ++i) {
        if (buckets[i].realIndex != EMPTY_INTERVAL_ID) {
            ordenadoPorInicio.push_back(buckets[i]);
        }

        if (buckets_fin[i].realIndex != EMPTY_INTERVAL_ID) {
            ordenadoPorFin.push_back(buckets_fin[i]);
        }
    }

    // TERMINA EL ORDENAMIENTO POR INICIO Y POR FIN CON BUCKET SORT


    for (int i = 0; i < ordenadoPorFin.size(); ++i) {
        interval candidate = emptyInterval;
        for (int j = 0; j < ordenadoPorInicio.size(); ++j) {
            if(ordenadoPorInicio[j].a > ordenadoPorFin[i].b &&
               ordenadoPorInicio[j].realIndex != ordenadoPorFin[i].realIndex &&
               candidate.realIndex == EMPTY_INTERVAL_ID
               ) {
                candidate = ordenadoPorInicio[j];
            }
        }

        if (candidate.realIndex != EMPTY_INTERVAL_ID){
            link dummy;
            dummy.vertex = candidate.realIndex;
            dummy.length = 1;
            dummy.type = TYPE_C;

            cdtGraph.adjacents[ordenadoPorFin[i].realIndex].push_back(dummy);
        }
    }
}

Graph buildCdtGraph (vector<interval> I,vector<interval> noContained){
    graph cdtGraph;

    // coloca una lista vacia por cada nodo
    list<link> emptyList;
    for(int i=0;i<noContained.size();i++){
        cdtGraph.adjacents.push_back(emptyList); //add vertex of the noContained interval
    }

    crearAristasB(cdtGraph, noContained);  // O(n)
    crearAristasC(cdtGraph, noContained);

    interval zero_interval;
    zero_interval.a = noContained[0].a;
    zero_interval.b = noContained[0].b;
    zero_interval.realIndex = 0;
    zero_interval.contained = false;
    INOUT_MAPPER.push_back(zero_interval);

    for(int i=1;i<noContained.size() - 1;i++) { // add vertex from cdtGraph
        interval dummyInterval;

        dummyInterval.a = noContained[i].a;
        dummyInterval.b = noContained[i].b;
        dummyInterval.realIndex = i;
        dummyInterval.contained = false;
        INOUT_MAPPER.push_back(dummyInterval);
        INOUT_MAPPER.push_back(dummyInterval);
    }

    interval n_1_interval;
    n_1_interval.a = noContained[noContained.size() - 1].a;
    n_1_interval.b = noContained[noContained.size() - 1].b;
    n_1_interval.realIndex = noContained.size() - 1;
    n_1_interval.contained = false;
    INOUT_MAPPER.push_back(n_1_interval);

    Graph g(((noContained.size() - 2) * 2) + 2);

    for (int i = 0; i < cdtGraph.adjacents.size(); ++i) {
        if(i >0 && i!= cdtGraph.adjacents.size()-1) {
            g.addEdge(2*i -1, 2*i, 0);
        }

        // iOut -> jIn
        for (auto it = cdtGraph.adjacents[i].begin(); it != cdtGraph.adjacents[i].end(); ++it){
            int vertex_out = 2*i;
            int vertex_in = (it->vertex * 2) - 1;
            int type = it->type;
            if (type == TYPE_B && i!=0 && (vertex_in!=INOUT_MAPPER.size() - 1)) {
                g.addEdge(vertex_out, vertex_in, it->length);
            }

            // Iin -> Jout
            vertex_out = (2*it->vertex);
            vertex_in = (i*2) - 1;
            if (type == TYPE_C && i!=0 && (vertex_out-1 != INOUT_MAPPER.size() - 1)) {
                g.addEdge(vertex_in, vertex_out, it->length);
            }


            // si llega a n+1
            bool isN_1_link = (type == TYPE_C || type == TYPE_B) && ((it->vertex * 2) - 1 == INOUT_MAPPER.size() - 1);
            if (isN_1_link && i!=0) {
                g.addEdge(2*i - 1, (it->vertex * 2) - 1, 1);
            }

            // si sale de 0
            if (i==0) {
                g.addEdge(0, (it->vertex * 2), 0);
            }
        }
    }

    return g;
}


void cdt (vector<interval> I) {
    vector<interval> noContained = removeContainedIntervals(I);
    vector<interval> res = {};


    if(noContained.size() ==3) { // caso que hay un intervalo que contiene a todos (es igual a 3 porque siempre pushea I_0 y I_n+1)
        interval contenedor = noContained[1];
        interval otro = noContained[1];

        for (int i = 0; i < I.size(); ++i) {  // busca otro cualquiera diferente al contenedor
            if(I[i].a != contenedor.a || I[i].b != contenedor.b) {
                otro = I[i];
            }
        }

        res.push_back(contenedor);
        res.push_back(otro);
    } else { // caso en el que no hay un intervalo que contiene a todos
        Graph g = buildCdtGraph(I, noContained);

        vector<int> path = g.shortestPath(0);

        interval last;
        for (int i = 0; i < path.size() - 1; ++i) {
            interval j = INOUT_MAPPER[path[i]];
            if(last.a != j.a && last.b != j.b) {
                res.push_back(j);
                last = j;
            }
        }
    }


    cout << res.size() << endl;
    for (int i = res.size() - 1; i >= 0; --i) {
        for (int j = 0; j < I.size(); ++j) {
            if (I[j].a == res[i].a && I[j].b == res[i].b) {
                cout << j << " ";
            }
        }
    }
}

int main() {
    cin >> n;
    for (int i= 0; i< n; i++){
        interval dummy{};
        cin >> a >> b;
        dummy.a = a;
        dummy.b = b;
        dummy.realIndex=i;
        I.push_back(dummy);
    }

    auto start = chrono::steady_clock::now(); // Empieza el clock

    cdt(I);
    cout << endl;

    auto end = chrono::steady_clock::now(); // Termina el clock
    double total_time = chrono::duration<double, milli>(end - start).count();
    cout << "Tiempo de ejecucion: " << total_time << " ms" << endl;

    return 0;
}
