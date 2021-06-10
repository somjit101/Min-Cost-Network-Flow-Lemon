#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <map>

#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>
#include <lemon/cost_scaling.h>

#include <chrono>

using namespace lemon;
using namespace std;
using namespace std::chrono;

using Weight = long;
using Capacity = long;
using Resource = long;

using Graph = SmartDigraph;

using Node = Graph::Node;
using Arc = Graph::Arc;

template<typename ValueType>
using ArcMap = SmartDigraph::ArcMap<ValueType>;

template<typename ValueType>
using NodeMap = SmartDigraph::NodeMap<ValueType>;


using NS = NetworkSimplex<SmartDigraph, Capacity, Weight>;
using CS = CostScaling<SmartDigraph, Capacity, Weight>;

int nodeNum = 5;
int arcNum = 9;

void readNodeTable(int nodeID[5], long nodeSD[5]);
void readArcTable(int arcFrom[9], int arcTo[9], int arcMin[9], long arcMax[9], long arcWeight[9]);


//template<size_t N>


int main()
{

//------------------------------ Reading Data ---------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------- Read Node Table -----------------

    int nodeID[nodeNum];
    long nodeSD[nodeNum];


    readNodeTable(nodeID, nodeSD);

    //---------- Check Ingested Node Table --------
        // for(int i = 0; i < nodeNum; i++)
        // {
        //     cout << nodeID[i] << "   " << nodeSD[i] << endl;
        // }
    //----------------------------------------------
        
//---------------------------------------------


//----------- Read Arc Table -----------------

    int arcFrom[arcNum];
    int arcTo[arcNum];
    int arcMin[arcNum];
    long arcMax[arcNum];
    long arcWeight[arcNum];

    readArcTable(arcFrom, arcTo, arcMin, arcMax, arcWeight);

    // //---------- Check Ingested Arc Table --------
        // for(int i = 0; i < arcNum; i++)
        // {
        //     cout << arcFrom[i] << "   " << arcTo[i] << "   " << arcMin[i] << "   "  << arcMax[i] << "   "  << arcWeight[i] << endl;
        // }
    // //----------------------------------------------
        
//---------------------------------------------

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------



//------------------------------ Consstructing Network Graph --------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Graph g;

//------------- Constructing Nodes -------------

    Node nodes[nodeNum];
    NodeMap<Resource> supplyDemands(g);
    map<int, Node> nodeIndexMap;

    for(int i = 0 ; i < nodeNum ; i++)
    {
        nodes[i] = g.addNode();
        nodeIndexMap[nodeID[i]] = nodes[i];
        supplyDemands[nodes[i]] = nodeSD[i];

    }
//------------------------------------------

//------------- Constructing Arcs/Edges -------------

    Arc arcs[arcNum];
    ArcMap<Weight> weights(g);
    ArcMap<Resource> lowerBounds(g);
    ArcMap<Capacity> capacities(g);

    for(int i = 0 ; i < arcNum ; i++)
    {
        arcs[i] = g.addArc(nodeIndexMap[arcFrom[i]] , nodeIndexMap[arcTo[i]]);
        weights[arcs[i]] = arcWeight[i];
        lowerBounds[arcs[i]] = arcMin[i];
        capacities[arcs[i]] = arcMax[i];

    }
//------------------------------------------

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------


//-------------------------- Running Optimization Solver ----------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

NS solver(g);

solver.lowerMap(lowerBounds).upperMap(capacities).costMap(weights).supplyMap(supplyDemands);

ArcMap<Capacity> flows(g);

NS::ProblemType status = solver.run();

switch (status) 
{
    case NS::INFEASIBLE:
        cerr << "insufficient flow" << endl;
        break;

    case NS::OPTIMAL:
        solver.flowMap(flows);

        cout << "Successful Execution !" << endl;

        cout << "\n------- Suggested Flows ---------" << endl;
        cout << "\n";

        for(int i = 0 ; i < arcNum ; i++)
        {
            if(flows[arcs[i]] != 0)
            {
                cout << arcFrom[i] << " ----> "<< arcTo[i] << "  : "  << flows[arcs[i]] << "  ; Cost : " << flows[arcs[i]] * weights[arcs[i]] << endl;
            }
        }

        cout << "Total Cost = " << solver.totalCost() << endl; 
        break;

    case NS::UNBOUNDED:
        cerr << "infinite flow" << endl;
        break;

    default:
        break;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------

return 0;

}




void readNodeTable(int nodeID[100], long nodeSD[100])
{

    fstream nodeColumn1;
    fstream nodeColumn2;

    string temp;
    string nodeIDVals, nodeSDVals;

    nodeColumn1.open("Demo_Data/nodeID_demo.csv", ios::in);
    nodeColumn2.open("Demo_Data/nodeSD_demo.csv", ios::in);

    int rowIndex = 0;

//---------- Serially Reading Data in Node Table -------------
    while(nodeColumn1 >> temp && nodeColumn2 >> temp)
    {

        getline(nodeColumn1, nodeIDVals, ',');
        nodeID[rowIndex] = stoi(nodeIDVals);

        getline(nodeColumn2, nodeSDVals, ',');
        nodeSD[rowIndex] = stol(nodeSDVals);

        rowIndex++;

    }

    // nodeID[0] = 1;
    // nodeID[1] = 2;
//-------------------------------------------------------------   

}


void readArcTable(int arcFrom[158], int arcTo[158], int arcMin[158], long arcMax[158], long arcWeight[158])
{

    fstream arcColumn1;
    fstream arcColumn2;
    fstream arcColumn3;
    fstream arcColumn4;
    fstream arcColumn5;

    string temp;
    string fromIDVals, toIDVals, arcMinVals, arcMaxVals, arcWeightVals;

    arcColumn1.open("Demo_Data/arcFrom_demo.csv", ios::in);
    arcColumn2.open("Demo_Data/arcTo_demo.csv", ios::in);
    arcColumn3.open("Demo_Data/arcMin_demo.csv", ios::in);
    arcColumn4.open("Demo_Data/arcMax_demo.csv", ios::in);
    arcColumn5.open("Demo_Data/arcWeight_demo.csv", ios::in);

    

    int rowIndex = 0;

//---------- Serially Reading Data from Arc Table -------------
    while(arcColumn1 >> temp && arcColumn2 >> temp && arcColumn3 >> temp && arcColumn4 >> temp && arcColumn5 >> temp)
    {

        getline(arcColumn1, fromIDVals, ',');
        arcFrom[rowIndex] = stoi(fromIDVals);

        getline(arcColumn2, toIDVals, ',');
        arcTo[rowIndex] = stoi(toIDVals);

        getline(arcColumn3, arcMinVals, ',');
        arcMin[rowIndex] = stoi(arcMinVals);

        getline(arcColumn4, arcMaxVals, ',');
        arcMax[rowIndex] = stol(arcMaxVals);

        getline(arcColumn5, arcWeightVals, ',');
        arcWeight[rowIndex] = stol(arcWeightVals);

        rowIndex++;

    }
//--------------------------------------------------------------    

    // arcMin[0] = 0;
    // arcMax[0] = 1000000L;

}


