# Min-Cost-Network-Flow-Lemon
A Simple C++ Implementation of the Lemon Optimization Library to Solve a Minimum Cost Flow problem in a given Graph Network with Supply/Demand Values of Nodes and Capacity, Unit Cost of Flow for each Edge.  


## Problem Definition
A minimum cost flow problem is where a real-world network of physical locations is represented by a Graph in the following manner : 
* Nodes - represent each physical location associated with a certain amount of Supply (+ve value) or Demand (-ve value)
* Edges -  Connects nodes as per per available routes for real-world resource transfer. Each edge is associated with a certain max capacity of transfer and a cost of transfer per unit of resource

**For the purpose of demonstration we have used a simple problem illustrated on the Google OR Tools webpage found [here](https://developers.google.com/optimization/flow/mincostflow) :**



The problem data has been recorded as per requirement in .csv format to be read by the 
