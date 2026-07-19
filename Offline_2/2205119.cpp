#include<bits/stdc++.h>
#include<random>
using namespace std;

struct Edge{
    int u;
    int v;
    int weight;
};

class Graph{
    public:
    int vertices,edges;
    vector<vector<pair<int,int>>> adj;
    Graph(int v,int e){
        vertices=v;
        edges=e;
        adj.resize(vertices+1);
    }
    void addEdge(int u,int v,int w){
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

};
int calculateCutWeight(const Graph &graph,const vector<int> &side){
    int cutWeight=0;
    for(int u=1;u<=graph.vertices;u++){
        for(const auto &edge : graph.adj[u]){
            int v=edge.first;
            int w=edge.second;
            if(u<v && side[u]!=side[v]){
                cutWeight+=w;
            }
        }
    }
    return cutWeight;
}

int randomizedMaxCut(const Graph &graph, mt19937 &rng){
    vector<int> side(graph.vertices+1);
    uniform_int_distribution<int> dist(0,1);
    for(int v=1;v<=graph.vertices;v++){
        side[v]=dist(rng);
    }
    return calculateCutWeight(graph,side);
}

double randomizedAverageCut(const Graph &graph,int trials,int seed){
    mt19937 rng(seed);
    double total = 0;
    for(int i=0;i<trials;i++){
        total += randomizedMaxCut(graph,rng);
    }
    return total/trials;
}

Edge findMaximumEdge(const Graph &graph){
    int maxWeight = -1;
    int maxU = -1;
    int maxV = -1;
    for(int u=1;u<=graph.vertices;u++){
        for(const auto &edge : graph.adj[u]){
            int v = edge.first;
            int w = edge.second;
            if(u<v && w>maxWeight){
                maxWeight = w;
                maxU = u;
                maxV = v;
            }
        }
    }
    return {maxU,maxV,maxWeight};
}
int greedyMaxCut(const Graph &graph)
{
    if (graph.vertices < 2)return 0;
    Edge seed = findMaximumEdge(graph);
    vector<int> side(graph.vertices + 1, -1);
    side[seed.u] = 0;
    side[seed.v] = 1;
    int assigned = 2;
    vector<int> sigmaX(graph.vertices + 1, 0);
    vector<int> sigmaY(graph.vertices + 1, 0);
    for (const auto &edge : graph.adj[seed.u]){
        sigmaX[edge.first] += edge.second;
    }
    for (const auto &edge : graph.adj[seed.v]){
        sigmaY[edge.first] += edge.second;
    }
    while (assigned < graph.vertices){
        int bestVertex = -1;
        int bestValue = -1;
        for (int v = 1; v <= graph.vertices; v++){
            if (side[v] != -1){
                continue;
            }
            int value = max(sigmaX[v], sigmaY[v]);
            if (value > bestValue){
                bestValue = value;
                bestVertex = v;
            }
        }
        if (bestVertex == -1){
            break;
        }
        if (sigmaY[bestVertex] > sigmaX[bestVertex]){
            side[bestVertex] = 0;
            for (const auto &edge : graph.adj[bestVertex]){
                sigmaX[edge.first] += edge.second;
            }
        }
        else{
            side[bestVertex] = 1;
            for (const auto &edge : graph.adj[bestVertex]){
                sigmaY[edge.first] += edge.second;
            }
        }
        assigned++;
    }
    return calculateCutWeight(graph, side);
}

int semiGreedyMaxCut(const Graph &graph, double alpha, mt19937 &rng, vector<int> &side){
    if(graph.vertices < 2)return 0;
    Edge seed = findMaximumEdge(graph);
    side.assign(graph.vertices+1,-1);
    side[seed.u] = 0;
    side[seed.v] = 1;
    int assigned = 2;
    vector<int> sigmaX(graph.vertices + 1, 0);
    vector<int> sigmaY(graph.vertices + 1, 0);
    for (const auto &edge : graph.adj[seed.u]){
        sigmaX[edge.first] += edge.second;
    }
    for (const auto &edge : graph.adj[seed.v]){
        sigmaY[edge.first] += edge.second;
    }
    while(assigned < graph.vertices){
        int maxValue=INT_MIN;
        int minValue=INT_MAX;
        for(int v=1;v<=graph.vertices;v++){
            if(side[v] != -1){
                continue;
            }
            int value = max(sigmaX[v], sigmaY[v]);
            maxValue=max(maxValue,value);
            minValue=min(minValue,value);
        }
        double mu = minValue + alpha * (maxValue-minValue);
        vector<int> rcl;
        for(int v=1;v<=graph.vertices;v++){
            if(side[v]!=-1){
                continue;
            }
            int value=max(sigmaX[v],sigmaY[v]);
            if(value >=mu){
                rcl.push_back(v);
            }
        }
        if(rcl.empty()){
            break;
        }
        uniform_int_distribution<int> dist(0, rcl.size()-1);
        int chosenVertex = rcl[dist(rng)];
        if (sigmaY[chosenVertex] > sigmaX[chosenVertex]){
            side[chosenVertex] = 0;
            for (const auto &edge : graph.adj[chosenVertex]){
                sigmaX[edge.first] += edge.second;
            }
        }
        else{
            side[chosenVertex] = 1;
            for (const auto &edge : graph.adj[chosenVertex]){
                sigmaY[edge.first] += edge.second;
            }
        }
        assigned++;
    }
    return calculateCutWeight(graph,side);
}

int localSearchMaxCut(const Graph &graph, vector<int> &side){
    vector<int> sigmaX(graph.vertices+1,0);
    vector<int> sigmaY(graph.vertices+1,0);
    for(int u=1;u<=graph.vertices;u++){
        for(const auto &edge : graph.adj[u]){
            int v=edge.first;
            int w=edge.second;
            if(u<v){
                if(side[u]==side[v]){
                    if(side[u]==0){
                        sigmaX[u]+=w;
                        sigmaX[v]+=w;
                    }
                    else{
                        sigmaY[u]+=w;
                        sigmaY[v]+=w;
                    }
                }
                else{
                    if (side[u] == 0){
                        sigmaY[u] += w;
                        sigmaX[v] += w;
                    }
                    else{
                        sigmaX[u] += w;
                        sigmaY[v] += w;
                    }
                }
            }
        }
    }
    while(true){
        int bestVertex = -1;
        int bestDelta = INT_MIN;
        for(int v = 1;v <= graph.vertices; v++){
            int delta;
            if(side[v]==0){
                delta = sigmaX[v]-sigmaY[v];
            }
            else{
                delta = sigmaY[v]-sigmaX[v];
            }
            if(delta > bestDelta){
                bestDelta = delta;
                bestVertex = v;
            }
        }
        if(bestDelta <= 0){
            break;
        }
        int oldSide = side[bestVertex];
        side[bestVertex] = 1-oldSide;
        for(const auto &edge : graph.adj[bestVertex]){
            int neighbour = edge.first;
            int weight = edge.second;
            /*if(side[neighbour] == oldSide){
                sigmaX[neighbour]-=weight;
                sigmaY[neighbour]+=weight;
            }
            else{
                sigmaX[neighbour]+=weight;
                sigmaY[neighbour]-=weight;
            }*/
            if(oldSide == 0){
                sigmaX[neighbour] -= weight;
                sigmaY[neighbour] += weight;
            }
            else{
                sigmaY[neighbour] -= weight;
                sigmaX[neighbour] += weight;
            }
        }
        //swap(sigmaX[bestVertex] , sigmaY[bestVertex]);
    }
    return calculateCutWeight(graph,side);
}

int graspMaxCut(const Graph &graph,double alpha, int iterations, int seed){
    mt19937 rng(seed);
    int bestCut = -1;
    vector<int> bestSide;
    for(int i=0;i<iterations;i++){
        vector<int> side;
        semiGreedyMaxCut(graph,alpha,rng,side);
        int currentCut = localSearchMaxCut(graph , side);
        if(currentCut > bestCut){
            bestCut=currentCut;
            bestSide=side;
        }
    }
    return bestCut;
}

Graph readGraph(const string &filename){
    ifstream file(filename);
    int vertices,edges;
    file >> vertices >> edges;
    Graph graph(vertices,edges);
    for(int i = 0; i < edges; i++){
        int u,v,w;
        file >> u >> v >> w;
        graph.addEdge(u,v,w);
    }
    file.close();
    return graph;
}




static const map<string,int> knownBestMap = {
    {"G1",12078}, {"G2",12084}, {"G3",12077},
    {"G11",627},  {"G12",621},  {"G13",645},
    {"G14",3187}, {"G15",3169}, {"G16",3172},
    {"G22",14123},{"G23",14129},{"G24",14131},
    {"G32",1560}, {"G33",1537}, {"G34",1541},
    {"G35",8000}, {"G36",7996}, {"G37",8009},
    {"G43",7027}, {"G44",7022}, {"G45",7020},
    {"G48",6000}, {"G49",6000}, {"G50",5988}
};

void benchmark(int numGraphs = 54,
                const string &folder = "graph_GRASP/set1/",
                const string &outFile = "output.txt",
                double alpha = 0.5,
                int randTrials = 30,
                int sgReps = 30,
                int lsReps = 30,
                int graspIters = 100,
                int seed = 42)
{
    ofstream out(outFile);
    if(!out){ cerr << "Error: cannot open " << outFile << " for writing.\n"; return; }

    out << "Name,V,E,Randomized_avg,Greedy,"
        << "SemiGreedy_avg,SemiGreedy_best,SemiGreedy_alpha,"
        << "LocalSearch_avg,LocalSearch_best,LocalSearch_reps,"
        << "GRASP_best,GRASP_iterations,KnownBestOrUpperBound\n";

    for(int i = 1; i <= numGraphs; i++){
        string name = "G" + to_string(i);
        string path = folder + "g" + to_string(i) + ".rud";   // <-- matches your g1.rud naming

        ifstream test(path);
        if(!test){ cerr << "Warning: could not open " << path << " -- skipping.\n"; continue; }
        test.close();

        auto t0 = chrono::steady_clock::now();
        Graph graph = readGraph(path);
        cout << "Processing " << name << " (n=" << graph.vertices << ", m=" << graph.edges << ") ... " << flush;

        double randAvg = randomizedAverageCut(graph, randTrials, seed);
        int greedyVal = greedyMaxCut(graph);

        mt19937 sgRng(seed);
        long long sgSum = 0; int sgBest = -1;
        for(int t = 0; t < sgReps; t++){
            vector<int> side;
            int val = semiGreedyMaxCut(graph, alpha, sgRng, side);
            sgSum += val; sgBest = max(sgBest, val);
        }
        double sgAvg = (double)sgSum / sgReps;

        mt19937 lsRng(seed);
        long long lsSum = 0; int lsBest = -1;
        for(int t = 0; t < lsReps; t++){
            vector<int> side;
            semiGreedyMaxCut(graph, alpha, lsRng, side);
            int val = localSearchMaxCut(graph, side);
            lsSum += val; lsBest = max(lsBest, val);
        }
        double lsAvg = (double)lsSum / lsReps;

        int graspVal = graspMaxCut(graph, alpha, graspIters, seed);

        string knownBestStr = knownBestMap.count(name) ? to_string(knownBestMap.at(name)) : "";

        out << name << "," << graph.vertices << "," << graph.edges << ","
            << randAvg << "," << greedyVal << ","
            << sgAvg << "," << sgBest << "," << alpha << ","
            << lsAvg << "," << lsBest << "," << lsReps << ","
            << graspVal << "," << graspIters << ","
            << knownBestStr << "\n";

        auto t1 = chrono::steady_clock::now();
        cout << "done in " << chrono::duration<double>(t1 - t0).count() << "s\n";
    }
    out.close();
    cout << "\nBenchmark complete. Results written to " << outFile << "\n";
}



int main(){
    benchmark(54, "../graph_GRASP/set1/", "output.txt");  // expects g1.rud ... g54.rud in graphs/
    return 0;
}




/*int main(){
    string filename;
    cin >> filename;
    Graph graph = readGraph(filename);
    int choice;
    cin >> choice;
    if(choice == 1){
        int trials , seed;
        cin >> trials >> seed;

        cout << randomizedAverageCut(graph , trials , seed);
    }
    else if(choice ==  2){
        cout << greedyMaxCut(graph) << endl;
    }
    else if(choice == 3){
        double alpha;
        int seed;
        cin >> alpha  >> seed;
        mt19937 rng(seed);
        vector<int> side;
        cout << semiGreedyMaxCut(graph , alpha, rng, side);
    }
    else if(choice == 4 ){
        double alpha;
        int iterations;
        int seed;
        cin >> alpha >> iterations >> seed;
        cout << graspMaxCut(graph , alpha , iterations, seed) << endl;
    }

}*/
/*int main()
{
    Graph graph(8, 14);

graph.addEdge(1,2,8);
graph.addEdge(1,3,2);
graph.addEdge(1,4,1);
graph.addEdge(2,3,7);
graph.addEdge(2,5,6);
graph.addEdge(3,4,9);
graph.addEdge(3,6,5);
graph.addEdge(4,5,4);
graph.addEdge(4,7,8);
graph.addEdge(5,6,3);
graph.addEdge(5,8,7);
graph.addEdge(6,7,6);
graph.addEdge(6,8,2);
graph.addEdge(7,8,9);

    mt19937 rng(42);

    cout << "Randomized Average Cut : "
        << randomizedAverageCut(graph, 1000, 42) << endl;

    cout << "Greedy Cut            : "
        << greedyMaxCut(graph) << endl;

    vector<int> side;

    cout << "Semi-Greedy Cut       : "
        << semiGreedyMaxCut(graph, 0.5, rng, side) << endl;

    cout << "After Local Search    : "
        << localSearchMaxCut(graph, side) << endl;

    cout << "GRASP Cut             : "
        << graspMaxCut(graph, 0.5, 100, 42) << endl;

    return 0;
}*/