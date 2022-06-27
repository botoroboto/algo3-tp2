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


vector<interval> removeContainedIntervals(vector<interval> &I){
    vector<interval> retval;

    //add I0

    interval min = I[0];
    interval max = I[n-1];

    interval I0, In;
    I0.b = min.a - 1;
    I0.a = min.a - 2;
    I0.contained = false;
    I0.realIndex = 0;

    In.a = max.b + 1;
    In.b = max.b + 2;
    In.contained = false;
    In.realIndex = n+1;

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
    cdtGraph.adjacents.push_back(emptyList); // add vertex 0
    for(int i=0;i<noProperlyContained.size();i++){
        cdtGraph.adjacents.push_back(emptyList); //add vertex of the noProperlyContained interval
    }
    cdtGraph.adjacents.push_back(emptyList); // add n+1 vertex

    // aristas tipo B

    for (int i = 0; i < noProperlyContained.size(); ++i) {
        for (int j = i; j < noProperlyContained.size() ; ++j) {
            if(noProperlyContained[i].a < noProperlyContained[j].a < noProperlyContained[i].b < noProperlyContained[j].b) {
                link dummy;
                dummy.vertex = j;
                dummy.length= 1;
                dummy.type= TYPE_B;
                if(i==0){
                    dummy.length=0;
                }
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
        for(int j=next;j<I.size();j++){
            if(!noProperlyContained[j].contained) toAdd++;
            if(noProperlyContained[j].a > noProperlyContained[i].b && noProperlyContained[j].a < flag && !noProperlyContained[j].contained){
                dummy.vertex = toAdd;
                if(toAdd==0){
                    dummy.length=0;
                }
                cdtGraph.adjacents[i].push_back(dummy);
            }
            if(noProperlyContained[j].b<flag){
                flag = noProperlyContained[j].b;
            }
        }
    }


/*
    //fill the vertex with links
    // type B links
    for(int i=0;i<noProperlyContained.size();i++){
        link dummy;
        dummy.vertex=0;
        dummy.length=1;
        dummy.type=0;
        for( int j=i; j<noProperlyContained.size() ; j++ ){
            if ( noProperlyContained[i].b > noProperlyContained[j].a && i!=j ){
                dummy.vertex = j+1;
                cdtGraph.adjacents[i+1].push_back(dummy);
            }
        }
    }

    // type C links
    interval last;
    last.a = I[I.size()-1].b + 1;
    last.b = last.a +1;
    last.contained= 0;
    I.push_back(last);
    int flag = I[0].b; //flag to help me figure if i should stop looking for a type C link
    for(int i=0;i<I.size();i++){
        link dummy;
        dummy.vertex=0;
        dummy.length=0;
        dummy.type=1;
        if(I[i].a < flag && I[i].contained == 0){
            dummy.vertex = i+1;
            cdtGraph.adjacents[0].push_back(dummy);
        }
        if(I[i].b < cierra_mas_temprano){
            flag = I[i].b;
        }
    }
    //Type C links for intervals from noProperlyContained
    for(int i=0;i < noProperlyContained.size();i++){
        link dummy;
        dummy.vertex=0;
        dummy.length=1;
        dummy.type=1;
        interval actual = noProperlyContained[i];
        int next;
        int toAdd = i;
        for(int k=0;k<noProperlyContained();k++){ // find the next that doesnt overlap with the i interval
            if(actual.b < noProperlyContained[k].a){
                next = k;
                break;
            }
            if(I[k].a > actual.a && !I[k].contained) toAdd++;
        }
        flag = I[next].b;
        for(int j=next;j<I.size();j++){
            if(!I[j].contained) toAdd++;
            if(I[j].a > noProperlyContained[i].b && I[j].a < flag && !I[j].contained){
                dummy.vertex = toAdd + 1;
                cdtGraph.adjacents[i+1].push_back(dummy);
            }
            if(I[j].b<flag){
                flag = I[j].b;
            }
        }
    }
*/
    //inout graph step
    graph cdtGraph_inout;

    cdtGraph_inout.adjacents.push_back(cdtGraph.adjacents[0]); // add vertex 0

    for(int i=1;i<cdtGraph.adjacents.size()-1;i++) { // add vertex from cdtGraph
        link dummy;
        dummy.vertex=0;
        dummy.length=0;
        dummy.type=2;

        cdtGraph_inout.adjacents.push_back(cdtGraph.adjacents[i]);
        cdtGraph_inout.adjacents.push_back(emptyList);
        dummy.vertex = 2*i;
        cdtGraph_inout.adjacents[(2 * i) - 1].push_back(dummy);
    }

    cdtGraph_inout.adjacents.push_back(cdtGraph.adjacents[cdtGraph.adjacents.size()-1]); // add vertex n+1




    for(link l : cdtGraph_inout.adjacents[0]){
        l.vertex = 2 * l.vertex;   // (0,i) -> (0,i_out)
    }
    for( int i=1; i < cdtGraph_inout.adjacents.size(); i+=2 ){
        vector<link> toDelete;
        for(link &l : cdtGraph_inout.adjacents[i]){
            if(l.type == TYPE_B){
                link newLink = l;
                newLink.vertex = ( 2* l.vertex ) - 1;
                cdtGraph_inout.adjacents[i+1].push_back((newLink));
                toDelete.push_back(l);
            }else if(l.type == TYPE_C){
                if(l.vertex != cdtGraph.adjacents.size() - 1 ) l.vertex = 2 * l.vertex;
                else l.vertex = (2 * l.vertex) - 1;

            }
        }

        for(link l: toDelete){
              cdtGraph_inout.adjacents[i].remove(l);
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

vector<interval> shortestPathCdt (graph cdtGraph,vector<interval> noProperlyContained){
    //notice interval vector is alrready sorted

    initShortestPathVertexes(cdtGraph);

    for (int i=0;i<cdtGraph.adjacents.size();i++){
        for(link l : cdtGraph.adjacents[i]){
            if(i==0){
                relajar(cdtGraph.vertexes[i],cdtGraph.vertexes[l.vertex],0);
            }else{
                relajar(cdtGraph.vertexes[i],cdtGraph.vertexes[l.vertex],1);
            }
        }
    }
    vector<interval> result;
    int i = cdtGraph.vertexes.size()-1;
    while(i>0){
        i = cdtGraph.vertexes[i].pi;
        result.push_back(noProperlyContained[i / 2]);
    }
    return result;
}



vector<interval> cdt (vector<interval> I){

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
        cout << i.realIndex << " ";
    }
    cout << endl;

    return 0;
}
