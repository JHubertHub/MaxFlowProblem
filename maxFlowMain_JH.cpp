// Name: Josh Hubert
// Class: CS 2C
// Date: 3/21/20
// Instructor: Eric Reed
// Assignment: 9 - The Maximum Flow Problem
// File Name: maxFlowMain_JH.cpp

#include <iostream>
#include <string>
#include <time.h>
#include "FHflowGraph.h"

using namespace std;


int main(){
   int finalFlow;
   clock_t startTime, stopTime;
   double elapsed;

   cout << "<========================< NETWORK MAXIMUM FLOW SOLVER >========================>\n\n";
   cout << "Note: Path search proceeds from end to start, in order to traverse a found path\n" << "from start to end. This also required making the residual graph reverse paths\n"
      << "hold the residual values, while their forward paths mirror the flow graph\n"
      << "forward paths.\n\n";

   FHflowGraph<string, int> myG;

   // ----------------------- GRAPH 1 -----------------------
   myG.addEdge("s","a", 3);    myG.addEdge("s","b", 2);
   myG.addEdge("a","b", 1);    myG.addEdge("a","c", 3); myG.addEdge("a","d", 4);
   myG.addEdge("b","d", 2);
   myG.addEdge("c","t", 2);
   myG.addEdge("d","t", 3);

   cout << "\n<----------------------------< GRAPH 1 >---------------------------->\n\n";
   cout << myG.getNumVerts() << " vertices, " << myG.getNumEdges() << " edges.\n\n";

   // show the original flow graph
   cout << "Adjacency tables before:\n\n";
   myG.showResAdjTable();
   myG.showFlowAdjTable();

   myG.setStartVert("s");
   myG.setEndVert("t");

   // Timing it
   startTime = clock();
   finalFlow = myG.findMaxFlow();
   stopTime = clock();

   cout << "Maximum flow: " << finalFlow << endl << endl;
   cout << "\nAdjacency tables after:\n\n";
   myG.showResAdjTable();
   myG.showFlowAdjTable();

   elapsed = (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
   cout << "Elapsed time: " << elapsed << " seconds" << endl << endl;


   // ----------------------- GRAPH 2 -----------------------
   myG.clear();
   myG.addEdge("s","A", 1); myG.addEdge("s","D", 4); myG.addEdge("s","G", 6);
   myG.addEdge("A","B", 2); myG.addEdge("A","E", 2);
   myG.addEdge("B","C", 2);
   myG.addEdge("C","t", 4);
   myG.addEdge("D","E", 3); myG.addEdge("D","A", 3);
   myG.addEdge("E","C", 2); myG.addEdge("E","F", 3); myG.addEdge("E","I", 3);
   myG.addEdge("F","C", 1); myG.addEdge("F","t", 3);
   myG.addEdge("G","D", 2); myG.addEdge("G","E", 1); myG.addEdge("G","H", 6);
   myG.addEdge("H","E", 2); myG.addEdge("H","I", 6);
   myG.addEdge("I","F", 1); myG.addEdge("I","t", 4);

   cout << "\n<----------------------------< GRAPH 2 >---------------------------->\n\n";
   cout << myG.getNumVerts() << " vertices, " << myG.getNumEdges() << " edges.\n\n";

   // show the original flow graph
   cout << "Adjacency tables before:\n\n";
   myG.showResAdjTable();
   myG.showFlowAdjTable();

   myG.setStartVert("s");
   myG.setEndVert("t");

   // Timing it
   startTime = clock();
   finalFlow = myG.findMaxFlow();
   stopTime = clock();

   cout << "Maximum flow: " << finalFlow << endl << endl;
   cout << "\nAdjacency tables after:\n\n";
   myG.showResAdjTable();
   myG.showFlowAdjTable();

   elapsed = (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
   cout << "Elapsed time: " << elapsed << " seconds." << endl << endl;

   cout << endl;

   return 0;
}



/* --------------------------------- OUTPUT ------------------------------------
<========================< NETWORK MAXIMUM FLOW SOLVER >========================>

Note: Path search proceeds from end to start, in order to traverse a found path
from start to end. This also required making the residual graph reverse paths
hold the residual values, while their forward paths mirror the flow graph
forward paths.


<----------------------------< GRAPH 1 >---------------------------->

6 vertices, 8 edges.

Adjacency tables before:

---------- Residual Adj Table ----------
Resi Adj List for s: a(0) b(0)
Resi Adj List for a: s(3) b(0) c(0) d(0)
Resi Adj List for b: s(2) a(1) d(0)
Resi Adj List for c: a(3) t(0)
Resi Adj List for d: a(4) b(2) t(0)
Resi Adj List for t: c(2) d(3)

------------ Flow Adj Table ------------
Flow Adj List for s: a(0) b(0)
Flow Adj List for a: b(0) c(0) d(0)
Flow Adj List for b: d(0)
Flow Adj List for c: t(0)
Flow Adj List for d: t(0)
Flow Adj List for t:


>> ALGORITHM START >>

Path search: t ~ c, t ~ d, c ~ a, d ~ b, a ~ s.
Path established: s -> a -> c -> t. Limiting Flow 2. Flows Adjusted.

Path search: t ~ d, d ~ a, d ~ b, a ~ s.
Path established: s -> a -> d -> t. Limiting Flow 1. Flows Adjusted.

Path search: t ~ d, d ~ a, d ~ b, a ~ c, b ~ s.
Path established: s -> b -> d -> t. Limiting Flow 2. Flows Adjusted.

Path search: Complete.

Loops required: 3

Maximum flow: 5


Adjacency tables after:

---------- Residual Adj Table ----------
Resi Adj List for s: a(3) b(2)
Resi Adj List for a: s(0) b(0) c(2) d(1)
Resi Adj List for b: s(0) a(1) d(2)
Resi Adj List for c: a(1) t(2)
Resi Adj List for d: a(3) b(0) t(3)
Resi Adj List for t: c(0) d(0)

------------ Flow Adj Table ------------
Flow Adj List for s: a(3) b(2)
Flow Adj List for a: b(0) c(2) d(1)
Flow Adj List for b: d(2)
Flow Adj List for c: t(2)
Flow Adj List for d: t(3)
Flow Adj List for t:

Elapsed time: 0.000538 seconds


<----------------------------< GRAPH 2 >---------------------------->

11 vertices, 21 edges.

Adjacency tables before:

---------- Residual Adj Table ----------
Resi Adj List for A: s(1) D(3) B(0) E(0)
Resi Adj List for s: A(0) D(0) G(0)
Resi Adj List for D: A(0) s(4) G(2) E(0)
Resi Adj List for G: s(6) D(0) E(0) H(0)
Resi Adj List for B: A(2) C(0)
Resi Adj List for E: A(2) D(3) G(1) C(0) F(0) I(0) H(2)
Resi Adj List for C: B(2) E(2) t(0) F(1)
Resi Adj List for t: C(4) F(3) I(4)
Resi Adj List for F: E(3) C(0) t(0) I(1)
Resi Adj List for I: E(3) t(0) F(0) H(6)
Resi Adj List for H: G(6) E(0) I(0)

------------ Flow Adj Table ------------
Flow Adj List for A: B(0) E(0)
Flow Adj List for s: A(0) D(0) G(0)
Flow Adj List for D: A(0) E(0)
Flow Adj List for G: D(0) E(0) H(0)
Flow Adj List for B: C(0)
Flow Adj List for E: C(0) F(0) I(0)
Flow Adj List for C: t(0)
Flow Adj List for t:
Flow Adj List for F: C(0) t(0)
Flow Adj List for I: t(0) F(0)
Flow Adj List for H: E(0) I(0)


>> ALGORITHM START >>

Path search: t ~ C, t ~ F, t ~ I, C ~ B, C ~ E, I ~ H, B ~ A, E ~ D, E ~ G, A ~ s.
Path established: s -> A -> B -> C -> t. Limiting Flow 1. Flows Adjusted.

Path search: t ~ C, t ~ F, t ~ I, C ~ B, C ~ E, I ~ H, B ~ A, E ~ D, E ~ G, D ~ s.
Path established: s -> D -> E -> C -> t. Limiting Flow 2. Flows Adjusted.

Path search: t ~ C, t ~ F, t ~ I, C ~ B, F ~ E, I ~ H, B ~ A, E ~ D, E ~ G, D ~ s.
Path established: s -> D -> E -> F -> t. Limiting Flow 1. Flows Adjusted.

Path search: t ~ C, t ~ F, t ~ I, C ~ B, F ~ E, I ~ H, B ~ A, E ~ G, A ~ D, G ~ s.
Path established: s -> G -> E -> F -> t. Limiting Flow 1. Flows Adjusted.

Path search: t ~ C, t ~ F, t ~ I, C ~ B, F ~ E, I ~ H, B ~ A, H ~ G, A ~ D, G ~ s.
Path established: s -> G -> H -> I -> t. Limiting Flow 4. Flows Adjusted.

Path search: t ~ C, t ~ F, C ~ B, F ~ E, F ~ I, B ~ A, E ~ H, A ~ D, H ~ G, D ~ s.
Path established: s -> D -> A -> B -> C -> t. Limiting Flow 1. Flows Adjusted.

Path search: t ~ F, F ~ E, F ~ I, E ~ A, E ~ C, E ~ H, A ~ D, A ~ B, H ~ G, G ~ s.
Path established: s -> G -> H -> E -> F -> t. Limiting Flow 1. Flows Adjusted.

Path search: Complete.

Loops required: 7

Maximum flow: 11


Adjacency tables after:

---------- Residual Adj Table ----------
Resi Adj List for A: s(0) D(2) B(2) E(0)
Resi Adj List for s: A(1) D(4) G(6)
Resi Adj List for D: A(1) s(0) G(2) E(3)
Resi Adj List for G: s(0) D(0) E(1) H(5)
Resi Adj List for B: A(0) C(2)
Resi Adj List for E: A(2) D(0) G(0) C(2) F(3) I(0) H(1)
Resi Adj List for C: B(0) E(0) t(4) F(1)
Resi Adj List for t: C(0) F(0) I(0)
Resi Adj List for F: E(0) C(0) t(3) I(1)
Resi Adj List for I: E(3) t(4) F(0) H(2)
Resi Adj List for H: G(1) E(1) I(4)

------------ Flow Adj Table ------------
Flow Adj List for A: B(2) E(0)
Flow Adj List for s: A(1) D(4) G(6)
Flow Adj List for D: A(1) E(3)
Flow Adj List for G: D(0) E(1) H(5)
Flow Adj List for B: C(2)
Flow Adj List for E: C(2) F(3) I(0)
Flow Adj List for C: t(4)
Flow Adj List for t:
Flow Adj List for F: C(0) t(3)
Flow Adj List for I: t(4) F(0)
Flow Adj List for H: E(1) I(4)

Elapsed time: 0.001342 seconds.


Program ended with exit code: 0

----------------------------------------------------------------------------- */
