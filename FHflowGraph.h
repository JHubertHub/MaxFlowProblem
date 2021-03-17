// Name: Josh Hubert
// Class: CS 2C
// Date: 3/21/20
// Instructor: Eric Reed
// Assignment: 9 - The Maximum Flow Problem
// File Name: FHflowGraph.h

#ifndef FHFLOWGRAPH_H
#define FHFLOWGRAPH_H
#include <limits.h>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include <functional>
using namespace std;


// --------------------- FLOW VERTEX ---------------------
// FlowType is some numeric type that expresses Flow of edges
// Object is the non-graph data for a vertex
template <class Object, typename FlowType>
class FHflowVertex{
   // internal typedefs to simplify syntax
   typedef FHflowVertex<Object, FlowType>* VertPtr;
   typedef map<VertPtr, FlowType> EdgePairList;

public:
   static int nSortKey;
   static stack<int> keyStack;
   static enum { SORT_BY_DATA, SORT_BY_maxEdge } eSortType;
   static bool setNSortType( int whichType );
   static void pushSortKey() { keyStack.push(nSortKey); }
   static void popSortKey() { nSortKey = keyStack.top(); keyStack.pop(); }

   static int const INFINITY_FH = INT_MAX;  // defined in limits.h
   EdgePairList flowAdjList;
   EdgePairList resAdjList;
   Object data;
   VertPtr maxEdge; // attempted to get max path search functionality to work, but it was hard
   VertPtr nextInPath;  // used for client-specific info

   FHflowVertex( const Object & x = Object() );

   void addToFlowAdjList(VertPtr neighbor, FlowType Flow);
   void addToResAdjList(VertPtr neighbor, FlowType Flow);
   void showFlowAdjList();
   void showResAdjList();

   bool operator<( const FHflowVertex<Object, FlowType> & rhs) const;
   const FHflowVertex<Object, FlowType> & operator=
      ( const FHflowVertex<Object, FlowType> & rhs);
};


// static const initializations for Vertex --------------
template <class Object, typename FlowType>
int FHflowVertex<Object, FlowType>::nSortKey
            = FHflowVertex<Object, FlowType>::SORT_BY_DATA;
template <class Object, typename FlowType>
stack<int> FHflowVertex<Object, FlowType>::keyStack;


// vertex member functions
template <class Object, typename FlowType>
bool FHflowVertex<Object, FlowType>::setNSortType( int whichType ){
   switch (whichType){
   case SORT_BY_DATA:
   case SORT_BY_maxEdge:
      nSortKey = whichType;
      return true;
   default:
      return false;
   }
}


template <class Object, typename FlowType>
FHflowVertex<Object, FlowType>::FHflowVertex( const Object & x)
   : data(x), maxEdge(NULL),
   nextInPath(NULL) {}


template <class Object, typename FlowType>
void FHflowVertex<Object, FlowType>::addToFlowAdjList
   (FHflowVertex<Object, FlowType> *neighbor, FlowType Flow){
   flowAdjList[neighbor] = Flow;
}


template <class Object, typename FlowType>
void FHflowVertex<Object, FlowType>::addToResAdjList
(FHflowVertex<Object, FlowType> *neighbor, FlowType Flow){
   resAdjList[neighbor] = Flow;
}


template <class Object, typename FlowType>
bool FHflowVertex<Object, FlowType>::operator<(
   const FHflowVertex<Object, FlowType> & rhs) const{
   switch (nSortKey){
   case SORT_BY_maxEdge:
      return (maxEdge < rhs.maxEdge);
   case SORT_BY_DATA:
      return (data < rhs.data);
   default:
      return false;
   }
}


template <class Object, typename FlowType>
const FHflowVertex<Object, FlowType> & FHflowVertex<Object, FlowType>::operator=
                                 (const FHflowVertex<Object, FlowType> & rhs){
   flowAdjList = rhs.flowAdjList;
   resAdjList = rhs.resAdjList;
   data = rhs.data;
   maxEdge = rhs.maxEdge;
   nextInPath = rhs.nextInPath;
   return *this;
}


template <class Object, typename FlowType>
void FHflowVertex<Object, FlowType>::showFlowAdjList(){
   typename EdgePairList::iterator iter;
   cout << "Flow Adj List for " << data << ": ";
   for (iter = flowAdjList.begin(); iter != flowAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}


template <class Object, typename FlowType>
void FHflowVertex<Object, FlowType>::showResAdjList(){
   typename EdgePairList::iterator iter;
   cout << "Resi Adj List for " << data << ": ";
   for (iter = resAdjList.begin(); iter != resAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}


//
//
//
//
// --------------------- FLOW GRAPH ---------------------
template <class Object, typename FlowType>
class FHflowGraph{
   // internal typedefs to simplify syntax
   typedef FHflowVertex<Object, FlowType> Vertex;
   typedef FHflowVertex<Object, FlowType>* VertPtr;
   typedef map<VertPtr, FlowType> EdgePairList;
   typedef set<VertPtr> VertPtrSet;
   typedef set<Vertex> VertexSet;

private:
   VertPtrSet vertPtrSet;
   VertexSet vertexSet;
   VertPtr startVertPtr;
   VertPtr endVertPtr;
   int numEdges;

public:
   FHflowGraph();
   void addEdge(const Object &source, const Object &dest, FlowType Flow);
   VertPtr addToVertexSet(const Object& object);
   bool setStartVert(const Object& x);
   bool setEndVert(const Object& x);
   void showFlowAdjTable();
   void showResAdjTable();
   VertPtrSet getVertPtrSet() const { return vertPtrSet; }
   int getNumVerts() const {return vertexSet.size();}
   int getNumEdges() const {return numEdges;}
   void clear();

   // algorithm
   FlowType findMaxFlow();

private:
   VertPtr getVertexWithThisData(const Object & x);
   // findMaxFlow helpers
   bool establishNextFlowPath();
   FlowType getLimitingFlowOnResPath();
   bool adjustPathByFlow(FlowType Flow);
   // getLimitingFlowOnResPath helper
   FlowType getFlowOfResEdge(VertPtr src, VertPtr dst);
   // adjustPathByFlow helpers
   bool addFlowToResEdge(VertPtr src, VertPtr dst, FlowType Flow);
   bool addFlowToFlowEdge(VertPtr src, VertPtr dst, FlowType Flow);
};


template <class Object, typename FlowType>
FHflowGraph<Object, FlowType>::FHflowGraph(){
   numEdges = 0;
   startVertPtr = NULL;
   endVertPtr = NULL;
}


// addEdge() - adds vertices as before, but adjacency lists are handled as follows:
// resAdjLists will receive two edges based on this one call: a forward edge, exactly  as before and a reverse edge with Flow 0
// flowAdjLists are built as before but with all Flows = 0
template <class Object, typename FlowType>
void FHflowGraph<Object, FlowType>::addEdge
(const Object& source, const Object& dest, FlowType Flow){
   VertPtr src, dst;

   // put both source and dest into vertex list(s) if not already there
   src = addToVertexSet(source);
   dst = addToVertexSet(dest);

   src->addToResAdjList(dst, 0); // add res forward edge
   dst->addToResAdjList(src, Flow); // add res reverse edge (for tracking remaining flow)
   src->addToFlowAdjList(dst, 0); // add flow forward edge
   numEdges++;
}


template <class Object, typename FlowType>
FHflowVertex<Object, FlowType>* FHflowGraph<Object, FlowType>::addToVertexSet(
   const Object& object){
   pair<typename VertexSet::iterator, bool> retVal;
   VertPtr vPtr;

   // save sort key for client
   Vertex::pushSortKey();
   Vertex::setNSortType(Vertex::SORT_BY_DATA);

   // build and insert vertex into master list
   retVal = vertexSet.insert( Vertex(object) );

   // get pointer to this vertex and put into vert pointer list
   vPtr = (VertPtr)&*retVal.first;
   vertPtrSet.insert(vPtr);

   Vertex::popSortKey();  // restore client sort key
   return vPtr;
}


template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::setStartVert(const Object& x){
   VertPtr temp;
   temp = getVertexWithThisData(x);

   // start and end verts not allowed to be the same
   if (temp != endVertPtr)
      startVertPtr = temp;
      return (startVertPtr != NULL);

   return false;
}


template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::setEndVert(const Object& x){
   VertPtr temp;
   temp = getVertexWithThisData(x);

   // start and end verts not allowed to be the same
   if (temp != startVertPtr)
      endVertPtr = temp;
      return (endVertPtr != NULL);

   return false;
}


template <class Object, typename FlowType>
void FHflowGraph<Object, FlowType>::clear(){
   vertexSet.clear();
   vertPtrSet.clear();
   startVertPtr = NULL;
   endVertPtr = NULL;
   numEdges = 0;
}


template <class Object, typename FlowType>
void FHflowGraph<Object, FlowType>::showFlowAdjTable(){
   typename VertPtrSet::iterator iter;
   cout << "------------ Flow Adj Table ------------ \n";
   for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
      (*iter)->showFlowAdjList();
   cout << endl;
}


template <class Object, typename FlowType>
void FHflowGraph<Object, FlowType>::showResAdjTable(){
   typename VertPtrSet::iterator iter;
   cout << "---------- Residual Adj Table ---------- \n";
   for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
      (*iter)->showResAdjList();
   cout << endl;
}


// ALGORITHMS

/*
the main public algorithm. (All the remaining algorithms are helpers and can be private.)  It returns the maximum flow found.  The method loops, calling establishNextFlowPath() followed by getLimitingFlowOnResPath() and follows those calls by adjusting the residual and flow graphs using adjustPathByFlow(). When establishNextFlowPath() returns false (or adjustPathByFlow() returns false or the limiting flow becomes 0, take your pick), the loop ends.  Finally, the flow graph is probed to find the total flow for the functional return.
 */
template <class Object, typename FlowType>
FlowType FHflowGraph<Object, FlowType>::findMaxFlow(){
   // check first
   if (startVertPtr == NULL || endVertPtr == NULL)
      return (FlowType)0;

   // minFlow is intermediate, maxFlow is final sum to return
   FlowType minFlow, maxFlow(0);
   int numLoops(0);
   typename EdgePairList::iterator edgePrIter;

   cout << "\n>> ALGORITHM START >>\n\n";

   while (establishNextFlowPath()){
      cout << "\nPath established: ";
      minFlow = getLimitingFlowOnResPath();

      // break cases
      if (minFlow == 0 || !adjustPathByFlow(minFlow))
         break;

      numLoops++;
   }

   // add up total flow (sum of flows leaving startVert)
   for (edgePrIter = startVertPtr->flowAdjList.begin();
        edgePrIter != startVertPtr->flowAdjList.end(); edgePrIter++)
      maxFlow += edgePrIter->second;

   cout << "Complete.\n\nLoops required: " << numLoops << "\n\n";

   return maxFlow;
}


// helper to findMaxFlow
template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::establishNextFlowPath(){
   // check first
   if (startVertPtr == NULL || endVertPtr == NULL)
      return false;

   typename VertPtrSet::iterator vIter;
   typename EdgePairList::iterator edgePrIter;
   VertPtr wPtr, vPtr;
   FlowType FlowVW;
   queue<VertPtr> partiallyProcessedVerts;

   cout << "Path search: ";
   // initialize the vertex list and place the starting vert in p_p_v queue
   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
      (*vIter)->nextInPath = NULL; // setting nextInPath ptrs to Null to start

   partiallyProcessedVerts.push( endVertPtr );

   // outer loop: pop a v off, and look for next w
   while ( !partiallyProcessedVerts.empty() ){
      vPtr = partiallyProcessedVerts.front();
      partiallyProcessedVerts.pop();

      // inner loop: for each vert w adj to v
      for (edgePrIter = vPtr->resAdjList.begin();
           edgePrIter != vPtr->resAdjList.end(); edgePrIter++){

         wPtr = edgePrIter->first;
         FlowVW = edgePrIter->second;

         // if FlowVW == 0, then either edge's flow has been used by another path
         // also make sure we're not back-tracking or overwriting path to end
         if (FlowVW != 0 && wPtr != endVertPtr && wPtr->nextInPath == NULL){
            cout << vPtr->data << " ~ " << wPtr->data;
            wPtr->nextInPath = vPtr; // paths must start from end and lead back to the source
            partiallyProcessedVerts.push(wPtr); // candidate path node

            if (wPtr == startVertPtr){
               cout << ".";
               return true;
            } else cout << ", ";
         }
      }
   }
   return false; // came to a dead end
}


// helper to findMaxFlow
template <class Object, typename FlowType>
FlowType FHflowGraph<Object, FlowType>::getLimitingFlowOnResPath(){
   // check first
   if (startVertPtr == NULL || endVertPtr == NULL)
      return NULL;

   VertPtr vp, start, stop;
   start = startVertPtr;
   stop = endVertPtr;
   FlowType minFlow, temp;
   minFlow = Vertex::INFINITY_FH; // make sure starting minFlow is bigger than everything

   // go until reach end vert
   for (vp = start; vp != stop; vp = vp->nextInPath){
      temp = getFlowOfResEdge(vp->nextInPath, vp); // checking reverse path flows (residual)
      if (temp < minFlow)
         minFlow = temp;
   }
   cout << "t. Limiting Flow " << minFlow << ". ";
   return minFlow;
}


// helper to findMaxFlow
template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::adjustPathByFlow(FlowType Flow){
   // check first
   if (startVertPtr == NULL || endVertPtr == NULL)
      return false;

   VertPtr vp, start, stop;
   start = startVertPtr;
   stop = endVertPtr;

   // go until reach start vert or until we come to a dead end (shouldn't happen)
   for (vp = start; vp != stop; vp = vp->nextInPath){
      addFlowToResEdge(vp->nextInPath, vp, -Flow); // res reverse edge (residual)
      addFlowToResEdge(vp, vp->nextInPath, Flow); // res forward edge (used)
      addFlowToFlowEdge(vp, vp->nextInPath, Flow); // flow edge
   }
   cout << "Flows Adjusted.\n\n";
   return true;
}


// helper to getLimitingFlowOnResPath
template <class Object, typename FlowType>
FlowType FHflowGraph<Object, FlowType>::getFlowOfResEdge(VertPtr src, VertPtr dst){
   typename EdgePairList::iterator foundIter;

   if (!(src == NULL || dst == NULL || src->resAdjList.empty())){
      foundIter = src->resAdjList.find(dst);

      if (foundIter != src->resAdjList.end())
         cout << dst->data << " -> ";
         return foundIter->second;
   }
   cout << "DIDN'T got flow. ";
   return (FlowType)0; // if pointers not valid. this shouldn't happen.
}


// helper to adjustPathByFlow
template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::addFlowToResEdge
(VertPtr src, VertPtr dst, FlowType Flow){
   if (src == NULL || dst == NULL)
      return false;

   typename EdgePairList::iterator foundIter;
   foundIter = src->resAdjList.find(dst);

   foundIter->second += Flow;
   return true;
}


// helper to adjustPathByFlow
template <class Object, typename FlowType>
bool FHflowGraph<Object, FlowType>::addFlowToFlowEdge
(VertPtr src, VertPtr dst, FlowType Flow){
   if (src == NULL || dst == NULL)
      return false;

   typename EdgePairList::iterator foundIter;
   foundIter = src->flowAdjList.find(dst);

   if (foundIter != src->flowAdjList.end()){
      foundIter->second += Flow;
   } else
      foundIter->second -= Flow;

   return true;
}


template <class Object, typename FlowType>
FHflowVertex<Object, FlowType>* FHflowGraph<Object, FlowType>::
getVertexWithThisData(const Object& x){
   typename VertexSet::iterator findResult;
   Vertex vert(x);

   Vertex::pushSortKey();  // save client sort key
   Vertex::setNSortType(Vertex::SORT_BY_DATA);
   findResult = vertexSet.find(vert);
   Vertex::popSortKey();  // restore client value

   if ( findResult == vertexSet.end() )
      return NULL;
   return  (VertPtr)&*findResult;     // the address of the vertex in the set of originals
}


#endif
