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
vector<interval> INOUT_MAPPER = {};

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
            if(
            j!=i && noProperlyContained[i].a < noProperlyContained[j].a &&
                noProperlyContained[i].b > noProperlyContained[j].a &&
                noProperlyContained[i].b < noProperlyContained[j].b
            ) {
                link dummy;
                dummy.vertex = j;
                dummy.length= 1;
                dummy.type= TYPE_B;
                // cout << " " << " ["<< noProperlyContained[i].a << "," <<noProperlyContained[i].b << "]" << " ["<< noProperlyContained[j].a << "," << noProperlyContained[j].b << "]"<< endl;

                cdtGraph.adjacents[i].push_back(dummy);
            }
        }
    }

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
                //cout << " " << " ["<< noProperlyContained[i].a << "," <<noProperlyContained[i].b << "]" << " ["<< noProperlyContained[toAdd].a << "," << noProperlyContained[toAdd].b << "]"<< endl;

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
    interval zero_interval;
    zero_interval.a = noProperlyContained[0].a;
    zero_interval.b = noProperlyContained[0].b;
    zero_interval.realIndex = 0;
    zero_interval.contained = false;
    INOUT_MAPPER.push_back(zero_interval);

    for(int i=1;i<noProperlyContained.size() - 1;i++) { // add vertex from cdtGraph
        interval dummyInterval;
        link dummy;
        dummy.vertex = 2*i;
        dummy.length=0;
        dummy.type=TYPE_IDENTITY;

        dummyInterval.a = noProperlyContained[i].a;
        dummyInterval.b = noProperlyContained[i].b;
        dummyInterval.realIndex = i;
        dummyInterval.contained = false;
        INOUT_MAPPER.push_back(dummyInterval);
        INOUT_MAPPER.push_back(dummyInterval);
        cdtGraph_inout.adjacents.push_back(emptyList);
        cdtGraph_inout.adjacents.push_back(emptyList);

        cdtGraph_inout.adjacents[2*i - 1].push_back(dummy);
    }

    interval n_1_interval;
    n_1_interval.a = noProperlyContained[noProperlyContained.size() - 1].a;
    n_1_interval.b = noProperlyContained[noProperlyContained.size() - 1].b;
    n_1_interval.realIndex = noProperlyContained.size() - 1;
    n_1_interval.contained = false;
    INOUT_MAPPER.push_back(n_1_interval);
    cdtGraph_inout.adjacents.push_back(emptyList); // add vertex n+1 no-duplicated

   // for (int i = 1; i < noProperlyContained.size() - 1; ++i) {  // build 0 adjacents
     //   link l;
       // l.vertex = 2*i;  // 0 -> iOut
       // l.length = 0;
       // l.type = TYPE_ZERO;

    //    cdtGraph_inout.adjacents[0].push_back(l);
    // }

    vector<interval> pr = INOUT_MAPPER;
    for (int i = 0; i < cdtGraph.adjacents.size(); ++i) {
        for (auto it = cdtGraph.adjacents[i].begin(); it != cdtGraph.adjacents[i].end(); ++it){
            int vertex_out = 2*i;
            int vertex_in = (it->vertex * 2) - 1;
            int type = it->type;
            if (type == TYPE_B && i!=0 && (vertex_in!=INOUT_MAPPER.size() - 1)) {
                link l;
                l.vertex = vertex_in;
                l.length = it->length;
                l.type = type;

                cdtGraph_inout.adjacents[vertex_out].push_back(l); // iOut -> jIn
            }

            // i -> j  swap to j -> i
            vertex_out = (2*it->vertex);
            vertex_in = (i*2) - 1;
            if (type == TYPE_C && i!=0 && (vertex_out-1 != INOUT_MAPPER.size() - 1)) {
                link l;
                l.vertex = vertex_out;
                l.length = it->length;
                l.type = it->type;

                cdtGraph_inout.adjacents[vertex_in].push_back(l); // Iin -> Jout
            }


            // si llega a n+1
            bool isN_1_link = (type == TYPE_C || type == TYPE_B) && ((it->vertex * 2) - 1 == INOUT_MAPPER.size() - 1);
            if (isN_1_link && i!=0) {
                link l;
                l.vertex = (it->vertex * 2) - 1;
                l.length = 1;
                l.type = 32;

                cdtGraph_inout.adjacents[2*i - 1].push_back(l); // JOut -> iIn
            }

            // si sale de 0
            if (i==0) {
                link l;
                l.vertex = (it->vertex * 2);
                l.length = 0;
                l.type = TYPE_ZERO;

                cdtGraph_inout.adjacents[0].push_back(l); // O -> IOut
            }
        }
    }

    for (int j = 0; j < cdtGraph_inout.adjacents.size(); j+=1) {
        for (std::list<link>::iterator it = cdtGraph_inout.adjacents[j].begin(); it != cdtGraph_inout.adjacents[j].end(); ++it) {
            cout << it->type << " " << " ["<< INOUT_MAPPER[j].a << "," <<INOUT_MAPPER[j].b << "]" << " ["<< INOUT_MAPPER[it->vertex].a << "," <<INOUT_MAPPER[it->vertex].b << "]"<< endl;
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
        result.push_back(INOUT_MAPPER[i]);
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

    for (int i = 0; i < result.size(); ++i) {
        cout << result[i].a << "," << result[i].b << endl;
    }

    cout << endl;

    return 0;
}
