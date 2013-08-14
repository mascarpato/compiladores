#include <stdio.h>
#include <stdlib.h>
#include "../include/comp_graph.h"


/** Tests the graph function defined in \file comp_graph.h
 */


int main()
{
    // create the graph
    int V = 5;
    struct Graph* graph = createGraph(V);
	addEdge(graph, 0, 1, 0);
    addEdge(graph, 0, 4, 0);
    addEdge(graph, 1, 2, 0);
    addEdge(graph, 1, 3, 0);
    addEdge(graph, 1, 4, 0);
    addEdge(graph, 2, 3, 0);
    addEdge(graph, 3, 4, 0);
 
    // print the adjacency list representation of the above graph
    printGraph(graph);
 
    return 0;
}
