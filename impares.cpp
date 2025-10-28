#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/* Definiciones básicas */
#define MAXV 1000
typedef int vertex;

/* Estructura de nodo para lista de adyacencia */
typedef struct node *link;
struct node {
    vertex w;
    link next;
};

/* Estructura del grafo */
typedef struct graph *Graph;
struct graph {
    int V;
    int A;
    link *adj;
};

/* Variables globales */
static int cnt;
int pre[MAXV];

/* Función para crear un grafo con V vértices */
Graph GRAPHinit(int V) {
    Graph G = (Graph) malloc(sizeof *G);
    G->V = V;
    G->A = 0;
    G->adj = (link *) malloc(V * sizeof(link));
    for (int v = 0; v < V; ++v)
        G->adj[v] = NULL;
    return G;
}


/* Inserta un arco v-w en el grafo dirigido */
void GRAPHinsertArc(Graph G, vertex v, vertex w) {
    link a;
    for (a = G->adj[v]; a != NULL; a = a->next)
        if (a->w == w) return;
    a = (link) malloc(sizeof *a);
    a->w = w;
    a->next = G->adj[v];
    G->adj[v] = a;
    G->A++;
}

/* Función recursiva que realiza la búsqueda en profundidad */
static void dfsR_anterior(Graph G, vertex v) {
    pre[v] = cnt++;
    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        if (pre[w] == -1)
            dfsR_anterior(G, w);
    }
}

/* Función principal que recorre todos los vértices del grafo */
void GRAPHdfs_anterior(Graph G) {
    cnt = 0;
    for (vertex v = 0; v < G->V; ++v)
        pre[v] = -1;
    for (vertex v = 0; v < G->V; ++v)
        if (pre[v] == -1)
            dfsR_anterior(G, v);
}


// Función recursiva modificada
static int sangria = 0;
static void dfsR(Graph G, vertex v) {
    // Imprime la entrada con sangría
    for (int i = 0; i < sangria; i++) printf("  ");
    printf("ENTRADA v=%d\n", v);

    pre[v] = cnt++;
    for (int i = 0; i < sangria; i++) printf("  ");
    printf("  pre[%d] = %d\n", v, pre[v]);

    sangria++; // aumenta sangría al entrar en la recursión

    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        for (int i = 0; i < sangria; i++) printf("  ");
        if (pre[w] == -1) {
            printf("  explorando arco %d -> %d (no visitado)\n", v, w);
            dfsR(G, w);
        } else {
            printf("  omitiendo arco %d -> %d (visitado, pre=%d)\n", v, w, pre[w]);
        }
    }

    sangria--; // disminuye sangría al volver
    for (int i = 0; i < sangria; i++) printf("  ");
    printf("SALIDA v=%d (pre=%d)\n", v, pre[v]);
}

// Función principal de DFS
void GRAPHdfs(Graph G) {
    cnt = 0;
    for (vertex v = 0; v < G->V; v++)
        pre[v] = -1;
    for (vertex v = 0; v < G->V; v++)
        if (pre[v] == -1)
            dfsR(G, v);
}





void dfsIterativa(Graph G, vertex s) {
    vector<vertex> st;
    st.reserve(G->V);
    st.push_back(s);
    pre[s] = cnt++;

    while (!st.empty()) {
        vertex v = st.back();
        st.pop_back();
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            if (pre[w] == -1) {
                pre[w] = cnt++;
                st.push_back(w);
            }
        }
    }
}

Graph GRAPHreadFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", filename);
        exit(1);
    }

    int V;
    fscanf(fp, "%d", &V);          // Leer número de vértices
    Graph G = GRAPHinit(V);        // Crear grafo vacío

    int v, w;
    while (fscanf(fp, "%d %d", &v, &w) == 2) {
        GRAPHinsertArc(G, v, w);   // Insertar arco v → w
        GRAPHinsertArc(G, w, v);   // Insertar arco w → v (para hacerlo no dirigido)
    }

    fclose(fp);
    return G;
}


/* Programa principal de demostración */
int main() {
/*     Graph G = GRAPHinit(6);

    // Ejemplo A: arcos 0-1 1-2 1-3 2-4 2-5
    GRAPHinsertArc(G, 0, 1);
    GRAPHinsertArc(G, 1, 2);
    GRAPHinsertArc(G, 1, 3);
    GRAPHinsertArc(G, 2, 4);
    GRAPHinsertArc(G, 2, 5);

    GRAPHdfs(G); */
    Graph G = GRAPHreadFromFile("grafo.txt");
    GRAPHdfs(G);


    cout << "Vector pre[] final:\n";
    for (int v = 0; v < G->V; v++)
        cout << "v " << v << " -> pre[" << v << "] = " << pre[v] << endl;

    return 0;
}
