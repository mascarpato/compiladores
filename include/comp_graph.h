/*! 
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 * 
 * @file comp_graph.h
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_VERTICES 10;

#include <stdio.h>
#include <stdlib.h>
 
/*! \brief Vertice used in the graph.
 * 
 */
typedef struct AdjListNode
{
    int dest;
    void *data;
    struct AdjListNode* next;
}comp_graph_t;
 
/*! \brief Edges connected to a Vertice.
 * 
 */
typedef struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
}Edges;

/*! \brief The array of Vertices of a Graph
 * 
 * Array of adjacency lists.
 * Size of array will be V (number of vertices in graph)
 */
typedef struct Graph
{
    int V;
    struct AdjList* array;
}Graph_type;

/// functions

/*! \brief Creates a new graph
 * 
 * 	returns Graph pointer
 */
struct Graph* createGraph(int V);


/*! \brief Adds a new node to the graph.
 * 
 * 	returns Node pointer
 */
struct AdjListNode* newAdjListNode(int dest, void *data);



/*! \brief Adds an edge between two nodes.
 * 
 * 	no return
 */
void addEdge(struct Graph* graph, int src, int dest, void *data);

/*! \brief Prints the graph
 * 
 * 	no return
 */
void printGraph(struct Graph* graph);
