#include <stdio.h>

#define MAXNODES 500
typedef struct nodetype {
    int info;
    int point;
    int next;
    
    // para bfs
    char cor;
    int d;
    int pai;
} tipoNodo;

typedef tipoNodo listaDeNodos[MAXNODES];

// FILA
typedef struct {
    int indMemoria;
    int indInf;
} DADOS;

typedef struct nodo {
    DADOS inf;
    struct nodo *next;
} NODO;

typedef struct {
    NODO *INICIO;
    NODO *FIM;
} DESCRITOR;

typedef DESCRITOR * FILA_ENC;

// Função para criar uma fila
void cria_fila(FILA_ENC *f) {
    *f = (FILA_ENC)malloc(sizeof(DESCRITOR));
    if (*f == NULL) {
        perror("Erro ao criar a fila");
        exit(1);
    }
    (*f)->INICIO = NULL;
    (*f)->FIM = NULL;
}

// Verifica se a fila está vazia
int eh_vazia(FILA_ENC f) {
    return f->INICIO == NULL;
}

// Insere um elemento na fila
void ins(FILA_ENC f, DADOS d) {
    NODO *novo = (NODO *)malloc(sizeof(NODO));
    if (novo == NULL) {
        perror("Erro ao inserir na fila");
        exit(1);
    }
    novo->inf = d;
    novo->next = NULL;
    
    if (eh_vazia(f)) {
        f->INICIO = novo;
    } else {
        f->FIM->next = novo;
    }
    f->FIM = novo;
}

// Consulta o elemento na frente da fila (sem remover)
DADOS cons(FILA_ENC f) {
    if (eh_vazia(f)) {
        fprintf(stderr, "Erro: tentativa de consultar uma fila vazia.\n");
        exit(1);
    }
    return f->INICIO->inf;
}

// Remove o elemento na frente da fila
void ret(FILA_ENC f) {
    if (eh_vazia(f)) {
        fprintf(stderr, "Erro: tentativa de remover de uma fila vazia.\n");
        exit(1);
    }
    NODO *temp = f->INICIO;
    f->INICIO = f->INICIO->next;
    if (f->INICIO == NULL) {
        f->FIM = NULL;
    }
    free(temp);
}

// Consulta e remove o elemento na frente da fila
DADOS cons_ret(FILA_ENC f) {
    if (eh_vazia(f)) {
        fprintf(stderr, "Erro: tentativa de consultar e remover de uma fila vazia.\n");
        exit(1);
    }
    DADOS d = f->INICIO->inf;
    ret(f);
    return d;
}

// Função para liberar a memória da fila
void libera_fila(FILA_ENC f) {
    while (!eh_vazia(f)) {
        ret(f);
    }
    free(f);
}

void inicializarGrafo (int *graph, int *listaDeNodosVazios, listaDeNodos node);
void criaListaDeNodosVazios (int *listaDeNodosVazios, listaDeNodos node);
int getnode (int *listaDeNodosVazios, listaDeNodos node);
void freenode (int *listaDeNodosVazios, listaDeNodos node, int r);
void joinwt (listaDeNodos node,int *listaDeNodosVazios, int p, int q, int wt);
void join (listaDeNodos node, int *listaDeNodosVazios, int p, int q);
void remv (listaDeNodos node, int *listaDeNodosVazios, int p, int q);
void remvwt (listaDeNodos node, int *listaDeNodosVazios, int p, int q, int x);
char adjacent (listaDeNodos node, int p, int q);
int findnode (listaDeNodos node, int graph, int x);
int addnode (listaDeNodos node, int *listaDeNodosVazios, int * pgraph, int x);
int remvnode(int *listaDeNodosVazios, listaDeNodos node, int * graph, int p);

void inicializarGrafo(int *graph, int *listaDeNodosVazios, listaDeNodos node) {
    *graph = -1; // Representa que o grafo está vazio
    criaListaDeNodosVazios(listaDeNodosVazios, node); // Inicializa a lista de nós vazios
}

void criaListaDeNodosVazios(int *listaDeNodosVazios, listaDeNodos node) {
    int i; // Declara a variável de controle do laço.
    
    // Laço para encadear os nós sequencialmente no vetor `node`.
    for (i = 1; i < MAXNODES; i++) {
        node[i-1].next = i; // O nó atual aponta para o próximo nó no índice `i`.
    }

    // Configura o último nó da lista para apontar para -1, indicando o fim da lista.
    node[i-1].next = -1;

    // Inicializa a cabeça da lista de nós vazios apontando para o primeiro índice (0).
    *listaDeNodosVazios = 0;
}

int getnode(int *listaDeNodosVazios, listaDeNodos node) {
    // Captura o índice do primeiro nó vazio da lista.
    int i = *listaDeNodosVazios;

    // Verifica se há nós disponíveis na lista.
    if (i != -1) {
        // Atualiza a lista de nós vazios para apontar para o próximo nó disponível.
        *listaDeNodosVazios = node[*listaDeNodosVazios].next;

        // Retorna o índice do nó alocado.
        return i;
    } else {
        // Caso não haja nós disponíveis, exibe mensagem de erro e encerra o programa.
        printf("Memoria insuficiente\n");
        exit(1);
    }
}

void freenode (int *listaDeNodosVazios, listaDeNodos node, int r) {
    // Configura o campo `next` do nó `r` para apontar para o início da lista de nós vazios.
    node[r].next = *listaDeNodosVazios;

    // Atualiza a cabeça da lista de nós vazios para o índice `r`.
    *listaDeNodosVazios = r;
}

void joinwt(listaDeNodos node, int *listaDeNodosVazios, int p, int q, int wt) {
    int r, r2;
    r2 = -1;             // Inicializa `r2` como -1, já que ainda não há nó anterior.
    r = node[p].point;   // Começa pelo primeiro nó na lista de adjacência de `p`.

    // Percorre a lista de adjacência de `p` para encontrar uma conexão com `q`.
    while (r >= 0 && node[r].point != q) {
        r2 = r;          // Atualiza o índice do nó anterior.
        r = node[r].next; // Avança para o próximo nó.
    }

    // Se encontrou uma conexão para `q`, atualiza o peso e retorna.
    if (r >= 0) {
        node[r].info = wt;
        return;
    }

    // Caso contrário, cria uma nova conexão.
    r = getnode(listaDeNodosVazios, node); // Aloca um novo nó.
    node[r].point = p;                     // Define o nó de origem `p`.
    node[r].next = -1;                     // Inicializa como o último nó da lista.
    node[r].info = wt;                     // Define o peso da nova conexão.

    // Insere o novo nó na lista encadeada de `p`.
    (r2 == -1) ? (node[p].point = r) : (node[r2].next = r);
}

void join (listaDeNodos node, int *listaDeNodosVazios, int p, int q) {
    int r, r2;
    r2 = -1;
    r = node[p].point;
    while (r >= 0 && node[r].point != q) {
        r2 = r;
        r = node[r].next;
    }
    if (r >= 0) {
        return;
    }
    r = getnode(listaDeNodosVazios, node);
    node[r].point = q;
    node[r].next = -1;
    (r2 > 0) ? (node[p].point = r) : (node[r2].next = r);
}

void remvwt(listaDeNodos node, int *listaDeNodosVazios, int p, int q, int x) {
    int r, r2;
    r2 = -1;             // Inicializa `r2` como -1 (nenhum nó anterior).
    r = node[p].point;   // Começa pelo primeiro nó na lista de adjacência de `p`.

    // Percorre a lista de adjacência de `p` procurando a conexão com `q`.
    while (r >= 0 && node[r].point != q) {
        r2 = r;          // Atualiza `r2` para o nó atual.
        r = node[r].next; // Avança para o próximo nó na lista.
    }

    // Se a conexão com `q` foi encontrada.
    if (r >= 0) {
        // Atualiza os ponteiros para remover o nó `r` da lista de adjacência.
        if (r2 < 0) 
            node[p].point = node[r].next; // Caso `r` seja o primeiro nó na lista.
        else 
            node[r2].next = node[r].next; // Caso `r` esteja no meio ou no final da lista.

        // Atualiza o valor do nó removido antes de liberá-lo.
        node[r].info = x;

        // Devolve o nó à lista de nós disponíveis.
        freenode(listaDeNodosVazios, node, r);
    }
}

void remv (listaDeNodos node, int *listaDeNodosVazios, int p, int q) {
    int r, r2;
    r2 = -1;
    r = node[p].point;
    while (r >= 0 && node[r].point != q) {
        r2 = r;
        r = node[r].next;
    }
    if(r >= 0) {
        if(r2 < 0)
            node[p].point = node[r].next;
        else
            node[r2].next = node[r].next;
        freenode(listaDeNodosVazios, node, r);
    }
}


char adjacent(listaDeNodos node, int p, int q) {
    int r = node[p].point; // Inicializa `r` com o índice do primeiro nó na lista de adjacência de `p`.

    // Percorre a lista de adjacência de `p`.
    while (r >= 0) {
        // Verifica se o nó atual conecta `p` a `q`.
        if (node[r].point == q)
            return 1; // Conexão encontrada, retorna verdadeiro.

        // Avança para o próximo nó na lista.
        r = node[r].next;
    }

    // Se a conexão não foi encontrada, retorna falso.
    return 0;
}

int findnode(listaDeNodos node, int graph, int x) {
    int p = graph; // Inicializa `p` com o índice do primeiro nó na lista encadeada.

    // Percorre a lista de nós começando em `graph`.
    while (p >= 0) {
        // Verifica se o campo `info` do nó atual contém o valor procurado.
        if (node[p].info == x)
            return p; // Valor encontrado, retorna o índice do nó.

        // Avança para o próximo nó na lista.
        p = node[p].next;
    }

    // Valor não encontrado na lista, retorna -1.
    return -1;
}

int addnode(listaDeNodos node, int *listaDeNodosVazios, int *pgraph, int x) {
    // Obtém um nó disponível da lista de nós vazios.
    int p = getnode(listaDeNodosVazios, node);

    // Inicializa o novo nó.
    node[p].info = x;      // Define o valor do campo `info` com o valor fornecido `x`.
    node[p].point = -1;    // Define `point` como -1 (sem conexões ou arestas).
    
    // Insere o nó no início da lista encadeada.
    node[p].next = *pgraph; // Aponta o `next` para o antigo primeiro nó da lista.
    *pgraph = p;            // Atualiza a cabeça da lista para o índice do novo nó.

    // Retorna o índice do novo nó.
    return p;
}

int remvnode(int *listaDeNodosVazios, listaDeNodos node, int *graph, int p) {
    int nodoAtual, nodoAnterior, retorno = 0;
    nodoAtual = nodoAnterior = *graph;

    // Percorre a lista de nós do grafo
    while (nodoAtual >= 0) {
        // Se encontrar o nó a ser removido
        if (nodoAtual == p) {
            int nodoAux, nodoAux2;

            // Remove o nó da lista de adjacência
            if (nodoAtual == *graph)  // Se for o primeiro nó da lista
                *graph = node[*graph].next;
            else  // Caso contrário, ajusta o próximo nó do nó anterior
                node[nodoAnterior].next = node[nodoAtual].next;

            // Libera todas as arestas (conexões) do nó removido
            nodoAux = node[nodoAtual].point;
            while (nodoAux >= 0) {
                nodoAux2 = nodoAux;
                nodoAux = node[nodoAux].next;
                freenode(listaDeNodosVazios, node, nodoAux2);
            }

            // Libera o próprio nó
            freenode(listaDeNodosVazios, node, p);

            // Marca a remoção como bem-sucedida
            retorno = 1;
        } else {
            int auxAnterior, auxAtual, nodeAux;

            // Percorre a lista de adjacência do nó atual
            auxAnterior = auxAtual = node[nodoAtual].point;
            while (auxAtual >= 0) {
                if (node[auxAtual].point == p) {  // Se encontrar o nó a ser removido na lista de adjacência
                    if (auxAtual == auxAnterior)  // Se for o primeiro nó da lista de adjacência
                        node[nodoAtual].point = node[auxAtual].next;
                    else  // Caso contrário, ajusta o próximo nó do nó anterior
                        node[auxAnterior].next = node[auxAtual].next;

                    // Libera o nó removido da lista de adjacência
                    nodeAux = auxAtual;
                    auxAnterior = auxAtual;
                    auxAtual = node[auxAtual].next;
                    freenode(listaDeNodosVazios, node, nodeAux);
                } else {
                    auxAnterior = auxAtual;
                    auxAtual = node[auxAtual].next;
                }
            }
        }

        // Avança para o próximo nó
        nodoAnterior = nodoAtual;
        nodoAtual = node[nodoAtual].next;
    }

    return retorno;  // Retorna 1 se o nó foi removido, ou 0 se não foi encontrado
}

/*
    u: A variável usada para percorrer os nós.
    d: Vetor que armazena as distâncias de cada vértice em relação ao vértice de origem s.
    pai: Vetor que armazena o pai (vértice anterior) de cada vértice durante a busca.
    vertice: Vetor que armazena os valores dos vértices no grafo.
    numVertices: Conta o número total de vértices no grafo.
    v: Variável usada para iterar sobre os vértices adjacentes de um nó.
    ind: Índice usado para iterar no vetor de vértices.
    cor: Vetor que armazena a cor de cada vértice (usado para indicar o status de visita: 'B' para não visitado, 'C' para visitado, 'P' para processado).
    Q: Fila usada para implementar a BFS (estruturalmente, uma fila de prioridade).
    aux: Estrutura auxiliar usada para manipulação de elementos na fila.
*/

void buscaEmLargura(listaDeNodos node, int G, int s) { // inclusive isso é uma péssima forma de fazer essa porra
    int u, *d = NULL, *pai = NULL, *vertice = NULL, numVertices = 0, v, ind;
    char *cor = NULL;
    FILA_ENC Q;
    DADOS aux;
    u = G;

    // Inicializa os vetores que armazenam as informações dos vértices
    while (u >= 0) {
        ++numVertices;  // Incrementa o contador de vértices
        // Realoca os vetores para incluir o novo vértice
        d = (int *) realloc(d, numVertices * sizeof(int));
        pai = (int *) realloc(pai, numVertices * sizeof(int));
        cor = (char *) realloc(cor, numVertices * sizeof(char));
        vertice = (int *) realloc(vertice, numVertices * sizeof(int));

        // Inicializa as informações do vértice u
        if (u != s) {
            // Para todos os vértices, exceto o de origem
            cor[numVertices - 1] = 'B';  // Cor inicial: Branco (não visitado)
            d[numVertices - 1] = -1;  // Distância inicial: -1
            pai[numVertices - 1] = -1;  // Pai inicial: -1
            vertice[numVertices - 1] = node[u].info;  // Atribui o valor do vértice
        } else {
            // Para o vértice de origem
            ind = numVertices - 1;
            cor[numVertices - 1] = 'C';  // Cor inicial: Cinza (em processo)
            d[numVertices - 1] = 0;  // Distância inicial: 0 para o vértice de origem
            pai[numVertices - 1] = -1;  // O vértice de origem não tem pai
            vertice[numVertices - 1] = node[u].info;  // Atribui o valor do vértice de origem
        }
        u = node[u].next;  // Avança para o próximo nó na lista de adjacência
    }

    // Cria a fila para armazenar os vértices a serem explorados
    criar_fila(&Q);
    aux.indInf = s;  // Índice do vértice de origem
    aux.indMemoria = ind;  // Índice do vértice no vetor
    ins(Q, aux);  // Insere o vértice de origem na fila

    // Inicia o processamento da fila
    while (!eh_vazia(Q)) {
        aux = cons_ret(Q);  // Retira o próximo elemento da fila
        u = aux.indInf;  // Vértice atual
        v = G;  // Vértice a ser explorado
        ind = -1;

        // Explora todos os vizinhos do vértice u
        while (v >= 0) {
            ind++;  // Incrementa o índice para acessar os vizinhos
            if (adjacent(node, u, v)) {  // Se v é adjacente a u
                if (cor[ind] == 'B') {  // Se v ainda não foi visitado
                    DADOS aux2;
                    cor[ind] = 'C';  // Marca v como visitado
                    d[ind] = d[aux.indMemoria] + 1;  // Atualiza a distância de v
                    pai[ind] = u;  // Define u como o pai de v
                    aux2.indInf = v;  // Define o índice de v
                    aux2.indMemoria = ind;  // Define o índice de memória de v
                    ins(Q, aux2);  // Adiciona v à fila para explorar seus vizinhos
                }
            }
            v = node[v].next;  // Passa para o próximo vizinho de v
        }

        cor[aux.indMemoria] = 'P';  // Marca o vértice u como processado
    }
}

/*
    Aqui eu vou usar a mesma fila que usei para anterior, nela esta definido para o tipo DADOS
    Mas como dessa segunda forma só precisa de um unico inteiro armazenado na fila, so usarei o primeiro campo da struct
    
    struct DADOS {
        int indMemoria;
        int indInf
    };

    Sendo usado apenas o indMemoria, porem, para fazer essa busca, provavelmente voce vai ter uma fila que armazena inteiros apenas
    os momentos em que essa abstração é feita estão comentados, mas na prova não será necessário fazer isso, provavelmente voce terá a fila do tipo adequado
*/

void buscaEmLargura(listaDeNodos node, int G, int s) {
    int u, v, ind;
    FILA_ENC Q;  // Fila usada para a busca em largura

    u = G;  // Começa no nó de índice G

    // Inicializa as propriedades dos nós do grafo
    while (u >= 0) {
        if (u != s) {  // Para todos os nós, exceto o nó de origem
            node[u].cor = 'B';  // Cor do nó: Branco (não visitado)
            node[u].d = -1;  // Distância do nó: -1 (não determinado)
            node[u].pai = -1;  // Pai do nó: -1 (não existe)
        } else {
            // Para o nó de origem
            node[u].cor = 'C';  // Cor do nó: Cinza (em processamento)
            node[u].d = 0;  // Distância do nó de origem: 0
            node[u].pai = -1;  // O nó de origem não tem pai
        }
        u = node[u].next;  // Avança para o próximo nó na lista de adjacência
    }

    // Cria a fila e insere o nó de origem nela
    cria_fila(&Q);  // Cria a fila
    ins(Q, (DADOS){s, 0});  // Insere o nó de origem na fila com índice s (se fosse uma fila de inteiros seria ins(Q, s); apenas)

    // Inicia o processo de busca em largura
    while (!eh_vazia(Q)) {
        // Retira um nó da fila para ser explorado
        u = cons_ret(Q).indMemoria;  // u = cons_ret(Q); para a fila de inteiros
        
        // Explora todos os vizinhos de u
        v = node[u].point;  // Primeiro vizinho de u
        while (v >= 0) {
            // Verifica se o vizinho v não foi visitado
            if (node[node[v].point].cor == 'B') {  // Se v é Branco (não visitado)
                node[node[v].point].cor = 'C';  // Marca v como Cinza (em processamento)
                node[node[v].point].d = node[u].d + 1;  // Distância de v é a de u + 1
                node[node[v].point].pai = u;  // Define o pai de v como u
                ins(Q, (DADOS){node[v].point, 0});  // Insere v na fila (para a fila de inteiros ins(Q,node[v].point);)
            }
            v = node[v].next;  // Passa para o próximo vizinho de v
        }
        // Marca o nó u como processado
        node[u].cor = 'P';  // Cor do nó: Preto (processado)
    }
}


void imprimirCaminho(listaDeNodos node, int s, int v) {
    // Caso base: se o destino v é o mesmo que a origem s, imprime o nó de origem
    if (v == s) {
        printf("%c ", node[s].info);  // Imprime o valor do nó de origem
    } else {
        // Se o pai do nó v for -1, significa que não há caminho de s a v
        if (node[v].pai == -1) {
            printf("\nNenhum caminho de \"%c\" para \"%c\" existente.\n",
                   node[s].info, node[v].info);  // Informa que não existe caminho
        } else {
            // Caso contrário, imprime o caminho recursivamente, indo até o pai de v
            imprimirCaminho(node, s, node[v].pai);  // Chamada recursiva para imprimir o caminho até o pai
            printf("%c ", node[v].info);  // Imprime o nó v
        }
    }
}

void buscaEmProfundidade(listaDeNodos node, int G) {
    int u = G;
    while (u >= 0) {
        node[u].cor = 'B';  // Inicializa todos os nós como não visitados (Branco)
        node[u].pai = -1;   // Nenhum pai inicialmente
        u = node[u].next;   // Vai para o próximo nó na lista
    }
    
    //tempo = 0;  // Tempo inicial para a DFS, é uma variável global
    u = G;
    
    // Começa a busca a partir do nó G
    while (u >= 0) {
        if (node[u].cor == 'B')  // Se o nó não foi visitado, realiza a DFS nele
            visitaBuscaEmProfundidade(node, u);
        u = node[u].next;   // Vai para o próximo nó na lista
    }
}

void visitaBuscaEmProfundidade(listaDeNodos node, int u) {
    int v;
    
    node[u].cor = 'C';  // Marca o nó como em processamento (Cinza)
    //tempo++;            // Incrementa o tempo de descoberta
    //node[u].d = tempo;  // Marca o tempo de descoberta

    v = node[u].point;  // Pega o primeiro vizinho do nó u
    while (v >= 0) {
        // Se o vizinho v ainda não foi visitado (Branco), visita recursivamente
        if (node[node[v].point].cor == 'B') {
            node[node[v].point].pai = u;  // Define o pai de v
            visitaBuscaEmProfundidade(node, node[v].point);  // Chamada recursiva
        }
        v = node[v].next;  // Vai para o próximo vizinho de u
    }

    node[u].cor = 'P';   // Marca o nó como completamente processado (Preto)
    // node[u].f = ++tempo; // Marca o tempo de término
}


int main() {

}