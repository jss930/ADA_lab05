#include <iostream>
#include <stdlib.h>
#include <cstdlib>
using namespace std;

#define vertex int

static int cnt;
int pre[1000];

/* La lista de adyacencia de un vértice v está formada por nodos de tipo nodo. Cada
nodo de la lista corresponde a un arco y contiene un vecino w de v y la dirección
del siguiente nodo de la lista. Un link es un puntero a un nodo. */
struct node {
    vertex w;
    struct node *next;
};

typedef struct node *link;

/* Ahora sí podemos definir graph con link ya declarado */
struct graph {
    int V;
    int A;
    link *adj;
};

/* Un Graph es un puntero para un graph. */
typedef struct graph *Graph;

/* La función NEWnode() recibe un vértice w y la dirección next de un nodo y
devuelve la dirección a de un nuevo nodo tal que a->w == w y a->next == next. */
static link NEWnode(vertex w, link next) {
    link a = (link)malloc(sizeof(struct node));
    a->w = w;
    a->next = next;
    return a;
}

/* REPRESENTACIÓN POR LISTAS DE ADYACENCIA: La función GRAPHinit() construye un grafo con
vértices 0 1 .. V-1 y ningún arco. */
Graph GRAPHinit(int V) {
    Graph G = (Graph)malloc(sizeof(struct graph));
    G->V = V;
    G->A = 0;
    G->adj = (link*)malloc(V * sizeof(link));
    for (vertex v = 0; v < V; ++v)
        G->adj[v] = NULL;
    return G;
}

/* REPRESENTACIÓN POR LISTAS DE ADYACENCIA: La función GRAPHinsertArc() inserta un arco v-w
en el grafo G. La función asume que v y w son distintos, positivos y menores que G->V. Si el
grafo ya tiene un arco v-w, la función no hace nada. */
void GRAPHinsertArc(Graph G, vertex v, vertex w) {
    for (link a = G->adj[v]; a != NULL; a = a->next)
        if (a->w == w) return;
    G->adj[v] = NEWnode(w, G->adj[v]);
    G->A++;
}

bool checkAdjacency(Graph G, vertex u, vertex v) {
    for (link a = G->adj[u]; a != NULL; a = a->next) {
        if (a->w == v)
            return true;
    }
    return false;
} 

int GRAPHoutdeg( Graph G, vertex v) {
   int outdeg = 0;
   for (link a = G->adj[v]; a != NULL; a = a->next) {
       outdeg++;
   }
   return outdeg;
}

bool GRAPHundir(Graph G) {
    for (vertex v = 0; v < G->V; v++) {
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            // Verificar si existe el arco inverso w->v
            bool found = false;
            for (link b = G->adj[w]; b != NULL; b = b->next) {
                if (b->w == v) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    // Si todos los arcos tienen su inverso, el grafo es no dirigido
    return true;
}

void GRAPHdestroy(Graph G) {
    // Liberar cada lista de adyacencia
    for (vertex v = 0; v < G->V; v++) {
        link a = G->adj[v];
        while (a != NULL) {
            link next = a->next;
            free(a);
            a = next;
        }
    }
    // Liberar el arreglo de listas de adyacencia
    free(G->adj);
    // Liberar la estructura del grafo
    free(G);
}

void UGRAPHremoveEdge( Graph G, vertex v, vertex w) {
   link *ap = &G->adj[v];
   while (*ap != NULL) {
       if ((*ap)->w == w) {
           link temp = *ap;
           *ap = (*ap)->next;
           free(temp);
           G->A--;
           break;
       }
       ap = &(*ap)->next;
   }
   if (v == w) {
       return;
   }
   ap = &G->adj[w];
   while (*ap != NULL) {
       if ((*ap)->w == v) {
           link temp = *ap;
           *ap = (*ap)->next;
           free(temp);
           G->A--;
           return;
       }
       ap = &(*ap)->next;
   }
}

/* La función dfsR() visita todos los vértices de G a los que se puede llegar desde el
vértice v sin pasar por vértices ya descubiertos. La función asigna cnt+k a pre[x] si x es el
k-ésimo vértice descubierto. (El código supone que G se representa mediante listas de
adyacencia). */
static void dfsR( Graph G, vertex v) {
    pre[v] = cnt++;
    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        if (pre[w] == -1)
            dfsR( G, w);
    }
}

/* La función GRAPHdfs() realiza una búsqueda en profundidad en el grafo G. Le asigna un
número de orden pre[x] a cada vértice x de modo que el k-ésimo vértice descubierto reciba el
número de orden k. (Código inspirado en el programa 18.3 de Sedgewick). */
void GRAPHdfs( Graph G) {
    cnt = 0;
    for (vertex v = 0; v < G->V; ++v)
        pre[v] = -1;
    for (vertex v = 0; v < G->V; ++v)
        if (pre[v] == -1)
            dfsR( G, v); // comienza nueva etapa
}

int main() {
    int V = 13;
    Graph G = GRAPHinit(V);
    /*
    0->1;
    0->4;
    1->2;
    1->5;
    2->3;
    3->7;
    4->8;
    5->4;
    6->5;
    6->10;
    6->2;
    7->11;
    7->6;
    8->9;
    9->5;
    9->8;
    10->9;
    11->10;
    */
    GRAPHinsertArc(G, 0, 1);
    GRAPHinsertArc(G, 0, 4);
    GRAPHinsertArc(G, 1, 2);
    GRAPHinsertArc(G, 1, 5);
    GRAPHinsertArc(G, 2, 3);
    GRAPHinsertArc(G, 3, 7);
    GRAPHinsertArc(G, 4, 8);
    GRAPHinsertArc(G, 5, 4);
    GRAPHinsertArc(G, 6, 5);
    GRAPHinsertArc(G, 6, 10);
    GRAPHinsertArc(G, 6, 2);
    GRAPHinsertArc(G, 7, 11);
    GRAPHinsertArc(G, 7, 6);
    GRAPHinsertArc(G, 8, 9);
    GRAPHinsertArc(G, 9, 5);
    GRAPHinsertArc(G, 9, 8);
    GRAPHinsertArc(G, 10, 9);
    GRAPHinsertArc(G, 11, 10);
    
    for (int v = 0; v < G->V; v++) {
        cout << "Adjacency list of vertex " << v << ": ";
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            cout << a->w << " ";
        }
        cout << endl;
    }

    cout << "Check adjacency between 0 and 5: " << (checkAdjacency(G, 0, 5) ? "Yes" : "No") << endl;
    cout << "Check adjacency between 3 and 1: " << (checkAdjacency(G, 3, 1) ? "Yes" : "No") << endl;

    cout << "Is the graph undirected? " << (GRAPHundir(G) ? "Yes" : "No") << endl;

    GRAPHdfs(G);

    cout << "DFS pre-order numbers (pre[v] = discovery time):" << endl;
    for (int v = 0; v < G->V; ++v) {
        cout << "vertex " << v << ": " << pre[v] << endl;
    }

    // Construir y mostrar orden de descubrimiento
    int *order = (int*)malloc(G->V * sizeof(int));
    for (int i = 0; i < G->V; ++i) order[i] = -1;
    for (int v = 0; v < G->V; ++v) {
        if (pre[v] >= 0 && pre[v] < G->V) order[pre[v]] = v;
    }

    cout << "Discovery order: ";
    for (int i = 0; i < G->V; ++i) {
        if (order[i] != -1) {
            if (i) cout << " -> ";
            cout << order[i];
        }
    }
    cout << endl;

    free(order);

    GRAPHdestroy(G);

    return 0;
}