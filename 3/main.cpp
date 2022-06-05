#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <climits>

using namespace std;
using Edge = pair<int, int>;  // (v, peso)


// Variables globales
const int INF = 1e9;
int N, M;

struct Arista {
    int u;  // origen
    int v;  // destino
    int w;  // peso de la arista (u,v)
};

struct Grafo {
    int V;
    int E;
    vector<Arista> aristas;
};


struct Grafo* createGraph(int V, int E) {
    struct Grafo* miGrafo = new Grafo;
    miGrafo->V = V;
    miGrafo->E = E;

    miGrafo->aristas = {};
    return miGrafo;
}

void imprimirCiclo(vector<int>& v){
    cout << 0 << endl;
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
}


void imprimirSolucion(vector<vector<int>> D, int n){
    cout << 1 << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n ; ++j) {
            if (D[i][j] == INF){
                cout << "INF" << " ";
            } else {
                cout << D[i][j] << " ";
            }

            if (j == n-1) {
                cout << endl;
            }
        }
    }
    cout << endl;
}


void dijkstra(vector<vector<Edge>>& G, int s, vector<int>& d) {
    int n = G.size();
    d.assign(n, INF);

    d[s] = 0;
    priority_queue<Edge, vector<Edge>, greater<Edge>> q;
    q.push({0, s});
    while (!q.empty()) {
        int v = q.top().second;
        int d_v = q.top().first;
        q.pop();
        if (d_v != d[v])
            continue;

        for (auto edge : G[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                d[to] = d[v] + len;
                q.push({d[to], to});
            }
        }
    }
}

void BellmanFord(struct Grafo* grafo, int s, vector<int>& dist, vector<int>& ciclo) {
    int V = grafo->V;
    int E = grafo->E;
    bool hayCicloNeg = false;
    int parent[V];

    // Poner distancias de todos los nodos a infinito y parents en -1

    for (int i = 0; i < V; i++) {
        dist.push_back(INT_MAX);
        parent[i] = -1;
    }

    // Distancia al origen es cero
    dist[s] = 0;

    // Relajar las aristas |V| - 1 times
    for (int i = 0; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = grafo->aristas[j].u;
            int v = grafo->aristas[j].v;
            int w = grafo->aristas[j].w;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }

    // Paso 3: Detectar ciclo negativo y guardarlo si hay
    int vcn;
    for (int i = 0; i < E; i++) {
        int u = grafo->aristas[i].u;
        int v = grafo->aristas[i].v;
        int w = grafo->aristas[i].w;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {

            hayCicloNeg = true;
            vcn = u;
            break;

        }
    }

    if (hayCicloNeg){
        for (int i = vcn;  ; i = parent[i]) {
            ciclo.push_back(i);
            if (i == vcn && ciclo.size() > 1)
                break;
        }
       
    }
}

void johnson(struct Grafo* grafo, vector<vector<int>>& D, vector<int>& ciclo) {
    //Armar G1 para Bellman-Ford
    int V = grafo->V;
    int E = grafo->E;
    struct Grafo* G1 = createGraph(V+1,E+V);

    G1->aristas = grafo ->aristas;
    for (int i = 0; i < V; ++i) {
        struct Arista* e = new Arista;
        e->u = V+1;
        e->v = i;
        e->w = 0;

        G1->aristas.push_back(*e);

        //delete(e);
    }
    vector<int> h;
    BellmanFord(G1, V, h, ciclo);

    //Si hay ciclo negativo se imprime y termina
    if (ciclo.size() != 0) {
        imprimirCiclo(ciclo);
        return;
    }

    //Si no hay ciclo negativo, pueden haber aristas de peso negativo
    //Primero repesamos las aristas para que sean todas positivas
    //Sabiendo que 0 <= w(u,v) + h(u) - h(v) = w'(u,v)

    for (int i = 0; i < E; ++i) {
        int u = grafo->aristas[i].u;
        int v = grafo->aristas[i].v;
        int w = grafo->aristas[i].w;

        grafo->aristas[i].w = w + h[u] - h[v];
    }

    //inicializo D
    //si no habia arista  ponemos en d_uv = INF
    D.assign(V, vector<int>(V,INF));

    //diagonal en 0
    for (int i = 0; i < V; ++i) {
        D[i][i] = 0;
    }
    // si hay arista de u a v ponemos en d_uv el peso de la arista
    for (int i = 0; i < E; ++i) {
        int u = grafo->aristas[i].u;
        int v = grafo->aristas[i].v;
        int w = grafo->aristas[i].w;

        D[u][v] = w;
    }

    // Armo G2 como lista de adyacencias en O(E) para dijkstra
    vector<vector<Edge>> G2(V);
    for (int i = 0; i < E; ++i) {
        int u = grafo->aristas[i].u;
        int v = grafo->aristas[i].v;
        int w = grafo->aristas[i].w;

        G2[u].push_back(make_pair(v,w));
    }


    vector<int> distDijkstra;
    for (int i = 0; i < V; ++i) {
        dijkstra(G2, i, distDijkstra);
        for (int j = 0; j < V; ++j) {
            D[i][j] = distDijkstra[j] + h[j] - h[i];
        }
    }

    imprimirSolucion(D,V);
}


int main() {

   /* int V = 2;
    int E = 2;

    struct Grafo* G = createGraph(V, E);

    Arista e1,e2;
    e1.u = 0;
    e1.v = 1;
    e1.w = 1;

    e2.u = 1;
    e2.v = 0;
    e2.w = -2;

    G->aristas.push_back(e1);
    G->aristas.push_back(e2);
*/
    int V = 4;
    int E = 5;

    struct Grafo* G = createGraph(V, E);

    Arista e1,e2,e3,e4,e5;
    e1.u = 0;
    e1.v = 1;
    e1.w = 3;

    e2.u = 1;
    e2.v = 2;
    e2.w = 2;

    e3.u = 2;
    e3.v = 3;
    e3.w = 6;

    e4.u = 3;
    e4.v = 0;
    e4.w = -1;

    e5.u = 1;
    e5.v = 3;
    e5.w = -1;

    G->aristas.push_back(e1);
    G->aristas.push_back(e2);
    G->aristas.push_back(e3);
    G->aristas.push_back(e4);
    G->aristas.push_back(e5);

    vector<vector<int> > distancias(V, vector<int> (V));
    vector<int> cicloNeg;
    johnson(G,distancias, cicloNeg);

    return 0;
}
