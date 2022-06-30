#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;
#define INF INT_MAX

string path;
int stream;

int TYPE_B = 0;
int TYPE_C = 1;
int TYPE_ZERO = 5;
int TYPE_IDENTITY = 2;

// Graph is represented using adjacency list. Every node
// of adjacency list contains vertex number of the vertex
// to which edge connects. It also
// contains weight of the edge
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

// A recursive function used by shortestPath.
// See below link for details
// https://www.geeksforgeeks.org/topological-sorting/
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

// The function to find shortest paths from given vertex.
// It uses recursive topologicalSortUtil() to get topological
// sorting of given graph.
vector<int> Graph::shortestPath(int s)
{
    stack<int> Stack;
    int dist[V];
    int parent[V];

    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to store
    // Topological Sort starting from all vertices
    // one by one
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);

    // Initialize distances to all vertices as
    // infinite and distance to source as 0
    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[s] = 0;

    // Process vertices in topological order
    while (Stack.empty() == false)
    {
        // Get the next vertex from topological order
        int u = Stack.top();
        Stack.pop();

        // Update distances of all adjacent vertices
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

    // Print the calculated shortest distances
    int i = V-1;
    vector<int> path = {};
    while(i>0) {
        cout << parent[i] << " | ";
        path.push_back(parent[i]);
        i = parent[i];
    }

    cout << endl;

    return path;
}

struct link {
    int vertex, type, length;

    bool operator==(const link & l) const {
        return l.vertex == vertex && l.type == type && l.length == length;
    }

};

struct linkInterval {
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

Graph buildCdtGraph (vector<interval> I,vector<interval> noContained){
    graph cdtGraph;

    // coloca una lista vacia por cada nodo
    list<link> emptyList;
    for(int i=0;i<noContained.size();i++){
        cdtGraph.adjacents.push_back(emptyList); //add vertex of the noContained interval
    }

//    // aristas tipo B
//    for (int i = 0; i < noContained.size(); ++i) {
//        for (int j = i; j < noContained.size() ; ++j) {
//            if(
//            j!=i && noContained[i].a < noContained[j].a &&
//                noContained[i].b > noContained[j].a &&
//                noContained[i].b < noContained[j].b
//            ) {
//                link dummy;
//                dummy.vertex = j;
//                dummy.length= 1;
//                dummy.type= TYPE_B;
//
//                cdtGraph.adjacents[i].push_back(dummy);
//            }
//        }
//    }

    crearAristasB(cdtGraph, noContained);

    interval emptyInterval;
    emptyInterval.a = -1000;
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
        if (buckets[i].a != -1000) {
            ordenadoPorInicio.push_back(buckets[i]);
        }

        if (buckets_fin[i].a != -1000) {
            ordenadoPorFin.push_back(buckets_fin[i]);
        }
    }

    for (int i = 0; i < ordenadoPorFin.size(); ++i) {
        interval candidate = emptyInterval;
        for (int j = 0; j < ordenadoPorInicio.size(); ++j) {
            if(ordenadoPorInicio[j].a > ordenadoPorFin[i].b &&
            ordenadoPorInicio[j].realIndex != ordenadoPorFin[i].realIndex &&
            candidate.a == -1000
            ) {
                candidate = ordenadoPorInicio[j];
            }
        }

        if (candidate.a != -1000){
            link dummy;
            dummy.vertex = candidate.realIndex;
            dummy.length = 1;
            dummy.type = TYPE_C;

            cdtGraph.adjacents[ordenadoPorFin[i].realIndex].push_back(dummy);
        }
    }

    //inout graph step
    graph cdtGraph_inout;

    cdtGraph_inout.adjacents.push_back(emptyList); // add vertex 0 no-duplicated
    interval zero_interval;
    zero_interval.a = noContained[0].a;
    zero_interval.b = noContained[0].b;
    zero_interval.realIndex = 0;
    zero_interval.contained = false;
    INOUT_MAPPER.push_back(zero_interval);

    for(int i=1;i<noContained.size() - 1;i++) { // add vertex from cdtGraph
        interval dummyInterval;
        link dummy;
        dummy.vertex = 2*i;
        dummy.length=0;
        dummy.type=TYPE_IDENTITY;

        dummyInterval.a = noContained[i].a;
        dummyInterval.b = noContained[i].b;
        dummyInterval.realIndex = i;
        dummyInterval.contained = false;
        INOUT_MAPPER.push_back(dummyInterval);
        INOUT_MAPPER.push_back(dummyInterval);
        cdtGraph_inout.adjacents.push_back(emptyList);
        cdtGraph_inout.adjacents.push_back(emptyList);

        cdtGraph_inout.adjacents[2*i - 1].push_back(dummy);
    }

    interval n_1_interval;
    n_1_interval.a = noContained[noContained.size() - 1].a;
    n_1_interval.b = noContained[noContained.size() - 1].b;
    n_1_interval.realIndex = noContained.size() - 1;
    n_1_interval.contained = false;
    INOUT_MAPPER.push_back(n_1_interval);
    cdtGraph_inout.adjacents.push_back(emptyList); // add vertex n+1 no-duplicated

    Graph g(cdtGraph_inout.adjacents.size());

    vector<interval> pr = INOUT_MAPPER;
    for (int i = 0; i < cdtGraph.adjacents.size(); ++i) {
        if(i >0 && i!= cdtGraph.adjacents.size()-1) {
            g.addEdge(2*i -1, 2*i, 0);
        }

        for (auto it = cdtGraph.adjacents[i].begin(); it != cdtGraph.adjacents[i].end(); ++it){
            int vertex_out = 2*i;
            int vertex_in = (it->vertex * 2) - 1;
            int type = it->type;
            if (type == TYPE_B && i!=0 && (vertex_in!=INOUT_MAPPER.size() - 1)) {
                link l;
                l.vertex = vertex_in;
                l.length = it->length;
                l.type = type;

                g.addEdge(vertex_out, vertex_in, it->length);
                cdtGraph_inout.adjacents[vertex_out].push_back(l); // iOut -> jIn
            }

            // Iin -> Jout
            vertex_out = (2*it->vertex);
            vertex_in = (i*2) - 1;
            if (type == TYPE_C && i!=0 && (vertex_out-1 != INOUT_MAPPER.size() - 1)) {
                link l;
                l.vertex = vertex_out;
                l.length = it->length;
                l.type = it->type;

                g.addEdge(vertex_in, vertex_out, it->length);
                cdtGraph_inout.adjacents[vertex_in].push_back(l);
            }


            // si llega a n+1
            bool isN_1_link = (type == TYPE_C || type == TYPE_B) && ((it->vertex * 2) - 1 == INOUT_MAPPER.size() - 1);
            if (isN_1_link && i!=0) {
                link l;
                l.vertex = (it->vertex * 2) - 1;
                l.length = 1;
                l.type = it->type;

                g.addEdge(2*i - 1, (it->vertex * 2) - 1, 1);
                cdtGraph_inout.adjacents[2*i - 1].push_back(l); // JOut -> iIn
            }

            // si sale de 0
            if (i==0) {
                link l;
                l.vertex = (it->vertex * 2);
                l.length = 0;
                l.type = TYPE_ZERO;

                g.addEdge(0, (it->vertex * 2), 0);
                cdtGraph_inout.adjacents[0].push_back(l); // O -> IOut
            }
        }
    }

    vector<int> path = g.shortestPath(0);
    cout << path.size() - 1 << endl;

    for (int i = 0; i < path.size(); ++i) {
        cout << INOUT_MAPPER[path[i]].a << "," << INOUT_MAPPER[path[i]].b << endl;
    }

    return g;
}

void initShortestPathVertexes(graph &cdtGraph){
    for(int i=0;i<cdtGraph.adjacents.size();i++){
        cdtVertex dummy;
        dummy.d = INT32_MAX;
        dummy.pi = -1;
        dummy.id = i;
        cdtGraph.vertexes.push_back(dummy);
    }
    cdtGraph.vertexes[0].d = 0;
}

void relajar (cdtVertex &u, cdtVertex &v,int w){
    if(v.d > u.d + w){
        v.d = u.d + w;
        v.pi = u.id;
    }
}

Graph cdt (vector<interval> I) {
    vector<interval> noContained = removeContainedIntervals(I);
    Graph cdtGraph = buildCdtGraph(I, noContained);
    return cdtGraph;
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

    Graph result = cdt(I);

    cout << endl;

    return 0;
}
