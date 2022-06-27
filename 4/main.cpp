#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;

string path;
int stream;

int TYPE_B = 0;
int TYPE_C = 1;
int TYPE_ZERO = 5;
int TYPE_IDENTITY = 2;

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


vector<interval> removeContainedIntervals(vector<interval> &I){
    vector<interval> retval;

    //add I0

    interval min = I[0];
    interval max = I[n-1];

    interval I0, In;
    I0.b = min.a - 1;
    I0.a = min.a - 2;
    I0.contained = false;
    I0.realIndex = n+4;

    In.a = max.b + 1;
    In.b = max.b + 2;
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

graph buildCdtGraph (vector<interval> I,vector<interval> noProperlyContained){
    graph cdtGraph;

    //fill the graph with vertex
    list<link> emptyList;
    for(int i=0;i<noProperlyContained.size();i++){
        cdtGraph.adjacents.push_back(emptyList); //add vertex of the noProperlyContained interval
    }

    // aristas tipo B
    for (int i = 0; i < noProperlyContained.size(); ++i) {
        for (int j = i; j < noProperlyContained.size() ; ++j) {
            if(j!=i && noProperlyContained[i].a < noProperlyContained[j].a &&
                noProperlyContained[i].b > noProperlyContained[j].a &&
                noProperlyContained[i].b < noProperlyContained[j].b
            ) {
                link dummy;
                dummy.vertex = j;
                dummy.length= 1;
                dummy.type= TYPE_B;
                if(i==0){
                    dummy.length=0;
                }

                cout << "["<< noProperlyContained[i].a << "," << noProperlyContained[i].b << "]" << "->" << "["<< noProperlyContained[dummy.vertex].a << "," << noProperlyContained[dummy.vertex].b << "]" << "link B" << endl;
                cdtGraph.adjacents[i].push_back(dummy);
            }
        }
    }

    cout << "---------------------" << endl;


    //aristas tipo c
    int flag;
    for(int i=0;i < noProperlyContained.size();i++){
        link dummy;
        dummy.vertex=0;
        dummy.length=1;
        dummy.type=TYPE_C;
        interval actual = noProperlyContained[i];
        int next;
        int toAdd = i;
        for(int k=0; k < noProperlyContained.size();k++){ // find the next that doesnt overlap with the i interval
            if(actual.b < noProperlyContained[k].a){
                next = k;
                break;
            }
            if(noProperlyContained[k].a > actual.a && !noProperlyContained[k].contained) toAdd++;
        }
        flag = noProperlyContained[next].b;
        for(int j=next;j<noProperlyContained.size();j++){
            if(!noProperlyContained[j].contained) toAdd++;
            if(noProperlyContained[j].a > noProperlyContained[i].b && noProperlyContained[j].a < flag && !noProperlyContained[j].contained){
                dummy.vertex = toAdd;
                if(i==0){
                    dummy.length=0;
                    continue;
                }
                cout << "["<< noProperlyContained[i].a << "," << noProperlyContained[i].b << "]" << "->" << "["<< noProperlyContained[toAdd].a << "," << noProperlyContained[toAdd].b << "]" << "link C" << endl;

                cdtGraph.adjacents[i].push_back(dummy);
            }
            if(noProperlyContained[j].b<flag){
                flag = noProperlyContained[j].b;
            }
        }
    }

    //inout graph step
    graph cdtGraph_inout;

    cdtGraph_inout.adjacents.push_back(emptyList); // add vertex 0 no-duplicated

    for(int i=1;i<noProperlyContained.size();i++) { // add vertex from cdtGraph
        link dummy;
        dummy.vertex = 2*i - 1;
        dummy.length=0;
        dummy.type=TYPE_IDENTITY;

        cdtGraph_inout.adjacents.push_back(emptyList);
        cdtGraph_inout.adjacents.push_back(emptyList);

        cout << "[" << 2*i<< ","<< dummy.vertex << "] TYPE Identity" << endl;

        cdtGraph_inout.adjacents[2*i].push_back(dummy);
    }

    cdtGraph_inout.adjacents.push_back(emptyList); // add vertex n+1 no-duplicated

    for (int i = 1; i < noProperlyContained.size() - 1; ++i) {  // build 0 adjacents
        link l;
        l.vertex = 2*i;
        l.length = 0;
        l.type = TYPE_ZERO;

        cdtGraph_inout.adjacents[0].push_back(l);
    }


    for (int i = 0; i < cdtGraph.adjacents.size(); ++i) {
        for (auto it = cdtGraph.adjacents[i].begin(); it != cdtGraph.adjacents[i].end(); ++it){
            if (it->type == TYPE_B && i!=0 && it->vertex != n+1) {
                link l;
                l.vertex = it->vertex * 2;
                l.length = it->length;
                l.type = it->type;

                cout << "[" << 2*i<< ","<< it->vertex << "] TYPE b" << endl;

                cdtGraph_inout.adjacents[2*i].push_back(l); // iOut -> jIn
            }

            // i -> j  swap to j -> i
            if (it->type == TYPE_C && i!=0 && it->vertex != n+1) {
                link l;
                l.vertex = i*2;
                l.length = it->length;
                l.type = it->type;

                cout << "[" << it->vertex << ","<< 2*i - 1 << "] TYPE C" << endl;

                cdtGraph_inout.adjacents[2*(it->vertex)].push_back(l); // JOut -> iIn
            }
        }
    }

    cout << "---------------------" << endl;
    cout << "---------------------" << endl;

    for (int j = 0; j < cdtGraph_inout.adjacents.size(); j+=1) {
        for (std::list<link>::iterator it = cdtGraph_inout.adjacents[j].begin(); it != cdtGraph_inout.adjacents[j].end(); ++it) {
            cout << "---------------------" << endl;
            cout << j << " "<< it->vertex << endl;
            cout << it->type <<"["<< noProperlyContained[j / 2].a << "," << noProperlyContained[j / 2].b << "]" << "->" << "["<< noProperlyContained[it->vertex / 2].a << "," << noProperlyContained[it->vertex / 2].b << "]" << "Inout" << endl;
        }
    }

    return cdtGraph_inout;
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

vector<interval> shortestPathCdt (graph cdtGraph,vector<interval> noProperlyContained) {
    //notice interval vector is alrready sorted
    initShortestPathVertexes(cdtGraph);

    for (int i=0;i<cdtGraph.adjacents.size();i++){
        for(link l : cdtGraph.adjacents[i]){
            relajar(cdtGraph.vertexes[i],cdtGraph.vertexes[l.vertex], l.length);
        }
    }
    vector<interval> result;
    int i = cdtGraph.vertexes.size()-1;
    while(i>0){
        i = cdtGraph.vertexes[i].pi;
        result.push_back(noProperlyContained[(i / 2)]);
    }
    return result;
}

vector<interval> cdt (vector<interval> I) {
    for(int i=0;i<I.size();i++){  //sort I by the first component of each interval
        for(int j=0;j<I.size();j++){
            if(I[j].a<I[i].a && j>i){
                swap(I[j],I[i]);
            }
        }
    }

    vector<interval> noProperlyContained = removeContainedIntervals(I);
    graph cdtGraph = buildCdtGraph(I, noProperlyContained);
    vector<interval> result = shortestPathCdt(cdtGraph, noProperlyContained);
    return result;
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

    vector<interval> result = cdt(I);

    cout << result.size() << endl;
    for(interval i : result){
        cout << i.a << " a" << endl;
        cout << i.b << " b"<< endl;
        cout << i.realIndex << " real index "<< endl;
    }
    cout << endl;

    return 0;
}
