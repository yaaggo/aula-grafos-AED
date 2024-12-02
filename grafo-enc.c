#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
    Nesse daqui, a lógica de todas as funções é praticamente a mesma, então mantive apenas os comentarios de linder do slide
    Porém, na busca em largura (bfs) e busca em profundidade (dfs), eu coloquei comentários ja que são funções que não tem no slide
    sendo feitas por mim
*/

typedef struct _nodetype {
    int info;
    struct _nodetype *point;
    struct _nodetype *next;

    // para bfs
    char cor;
    int d; // distancia
    int f;
    struct _nodetype *pai;

} nodetype;

typedef nodetype *TADgraph;

void inicializaGrafo(TADgraph *);
void joinwt (nodetype *, nodetype *, int);
void join (nodetype *, nodetype *);
void remvwt (nodetype *, nodetype *, int);
void remv (nodetype *, nodetype *);
char adjacent (nodetype *, nodetype *);
nodetype *findnode (TADgraph, int);
nodetype *addnode (TADgraph*, int);
int remvnode(TADgraph *, nodetype *);


void inicializaGrafo (TADgraph *grafo) {
    *grafo = NULL;
}

void joinwt (nodetype *p, nodetype *q, int wt) {
    nodetype *r, *r2;
    r2 = NULL;
    r = p->point;
    while (r && r->point != q) {
        r2 = r;
        r = r->next;
    }
    if (r) {
        r->info = wt;
        return;
    }
    if (!(r = (nodetype *) malloc(sizeof(nodetype)))) exit(1);
    r->point = q;
    r->next = NULL;
    r->info = wt;
    (!r2) ? (p->point = r) : (r2->next = r);
}

void join (nodetype *p, nodetype *q) {
    nodetype *r, *r2;
    r2 = NULL;
    r = p->point;
    while (r && r->point != q) {
        r2 = r;
        r = r->next;
    }
    if (r) {
        return;
    }
    if (!(r = (nodetype *) malloc(sizeof(nodetype)))) exit(1);
    r->point = q;
    r->next = NULL;
    (!r2) ? (p->point = r) : (r2->next = r);
}

void remv (nodetype *p, nodetype *q) {
    nodetype *r, *r2;
    r2 = NULL;
    r = p->point;
    while (r && r->point != q) {
        r2 = r;
        r = r->next;
    }
    if(r) {
        if(!r2)
            p->point = r->next;
        else
            r2->next = r->next;
        free(r);
        return;
    }
}

void remv (nodetype *p, nodetype *q, int x) {
    nodetype *r, *r2;
    r2 = NULL;
    r = p->point;
    while (r && r->point != q) {
        r2 = r;
        r = r->next;
    }
    if(r) {
        if(!r2)
            p->point = r->next;
        else
            r2->next = r->next;
        r->info = x;
        free(r);
        return;
    }
}

char adjacent (nodetype *p, nodetype *q) {
    nodetype *r;
    r = q->point;
    while(r)
        if(r->point == q)
            return 1;
        else
            r = r->next;
    return 0;
}

nodetype *findnode (TADgraph graph, int x) {
    nodetype * p;
    p = graph;
    while(p)
        if (p->info == x) return p;
        else p = p->next;
    return NULL;
}

nodetype *addnode (TADgraph* pgraph, int x) {
    TADgraph p;
    if (!(p = (TADgraph)malloc(sizeof(nodetype)))) exit(1);
    p->info = x;
    p->point = NULL;
    p->next = *pgraph;
    *pgraph = p;
    return p;
}

int remvnode(TADgraph *graph, nodetype *p) {
    int retorno = 0;
    nodetype *nodoAtual, *nodoAnterior;
    nodoAtual = nodoAnterior = *graph;
    while (nodoAtual) {
        if (nodoAtual == p) {
            nodetype *nodoAux, *nodoAux2;
            if (nodoAtual == *graph) /*eliminado vértice do conjunto de vértices*/
                *graph = (*graph)->next;
            else
                nodoAnterior->next = nodoAtual->next;
            nodoAux = nodoAtual->point;
            while (nodoAux) {
                nodoAux2 = nodoAux;
                nodoAux = nodoAux->next;
                free(nodoAux2);/*eliminado arestas*/
            }
            nodoAnterior = nodoAtual;
            nodoAtual = nodoAtual->next;
            free(p); /*eliminando vértice*/
            retorno = 1;
        } else {
            nodetype *auxAnterior, *auxAtual, *nodeAux;
            auxAnterior = auxAtual = nodoAtual->point;
            while (auxAtual) {
                if (auxAtual->point == p) {
                    if (auxAtual == auxAnterior)
                        nodoAtual->point = auxAtual->next;
                    else
                        auxAnterior->next = auxAtual->next;
                nodeAux = auxAtual;
                auxAnterior = auxAtual;
                auxAtual = auxAtual->next;
                free(nodeAux);
                } else {
                    auxAnterior = auxAtual;
                    auxAtual = auxAtual->next;
                }
            }
            nodoAnterior = nodoAtual;
            nodoAtual = nodoAtual->next;
        }
    }
    return (retorno);
}

// Estrutura da fila
typedef struct _queue {
    nodetype *node;
    struct _queue *next;
} queue;

// Funções auxiliares para a fila
queue *create_queue() {
    return NULL;
}

int is_empty(queue *q) {
    return (q == NULL);
}

void enqueue(queue **q, nodetype *node) {
    queue *new_node = (queue *)malloc(sizeof(queue));
    new_node->node = node;
    new_node->next = NULL;
    if (*q == NULL) {
        *q = new_node;
    } else {
        queue *temp = *q;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

nodetype *dequeue(queue **q) {
    if (is_empty(*q)) {
        return NULL;
    }
    queue *temp = *q;
    nodetype *node = temp->node;
    *q = temp->next;
    free(temp);
    return node;
}

void bfs(nodetype *grafo, nodetype *origem) {
    // Inicializa todos os nós do grafo
    nodetype *temp = grafo;
    while (temp != NULL) {
        temp->cor = 'B';  // Branco (não visitado)
        temp->d = INT_MAX; // Distância infinita
        temp->pai = NULL;  // Sem pai
        temp = temp->next;
    }

    // Configurações iniciais do nó de origem
    origem->cor = 'C'; // Cinza (visitando)
    origem->d = 0;
    origem->pai = NULL;

    // Inicializa a fila com o nó de origem
    queue *q = create_queue();
    enqueue(&q, origem);

    // Enquanto a fila não estiver vazia
    while (!is_empty(q)) {
        nodetype *u = dequeue(&q);

        // Percorre os vizinhos do nó atual
        nodetype *v = u->point; // Ponteiro para lista de adjacência
        while (v != NULL) {
            if (v->cor == 'B') { // Vizinho não visitado
                v->cor = 'C'; // Torna cinza (em processo)
                v->d = u->d + 1; // Atualiza a distância
                v->pai = u;      // Define o pai
                enqueue(&q, v);  // Adiciona na fila
            }
            v = v->next; // Próximo vizinho
        }

        // Finaliza o processamento do nó atual
        u->cor = 'P'; // Preto (processado)
    }
}

int tempo;

void dfs(nodetype *grafo) {
    // Inicializa todos os nós do grafo
    nodetype *temp = grafo;
    while (temp != NULL) {
        temp->cor = 'B';  // Branco (não visitado)
        temp->pai = NULL; // Sem pai
        temp->d = 0;
        temp->f = 0;
        temp = temp->next;
    }

    // Executa a DFS em todos os nós ainda não visitados
    temp = grafo;
    while (temp != NULL) {
        if (temp->cor == 'B') {
            dfs_visit(temp);
        }
        temp = temp->next;
    }
}

// Função para realizar a DFS em um único nó
void dfs_visit(nodetype *u) {
    tempo++;
    u->d = tempo; // Tempo de descoberta
    u->cor = 'C'; // Marca como cinza (em processamento)

    // Percorre os vizinhos
    nodetype *v = u->point;
    while (v != NULL) {
        if (v->cor == 'B') { // Vizinho não visitado
            v->pai = u; // Define o pai
            dfs_visit(v); // Chama recursivamente
        }
        v = v->next;
    }

    u->cor = 'P'; // Marca como preto (finalizado)
    tempo++;
    u->f = tempo; // Tempo de finalização
}