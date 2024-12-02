#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500
#define MAXS 50

typedef struct {
    int ref;
    char s[MAXS];
} pers;

typedef struct node {
    pers p;
    struct node *left, *right;
} node;

int n, m, now, idx = 0;
int g[MAX][MAX] = {}, vi[MAX] = {};

node * ins(node *t, char *n) {
    if(!t) {
        t = (node *) malloc(sizeof(node));
        strcpy(t->p.s, n);
        t->p.ref = now = idx++;
        t->left = t->right = NULL;
    } else {
        if(strcmp(t->p.s, n) > 0) 
            t->left = ins(t->left, n);
        else if(strcmp(t->p.s, n) < 0)
            t->right = ins(t->right, n);
        else
            now = t->p.ref;
    }
    return t;
}

void dfs (int x) {
    vi[x] = 1;

    for (int i = 0; i<n; i++){
        if(g[x][i] && !vi[i]) {
            dfs(i);
        }
    }
}

int main() {
    int i, ans = 0;
    node *t = NULL;

    scanf("%d %d", &n, &m);

    for(int i = 0; i<m; i++){
        int a, b;
        char n1[MAXS], n2[MAXS];

        scanf(" %s %*s %s", n1, n2);

        t = ins(t, n1); a = now;
        t = ins(t, n2); b = now;

        g[a][b] = 1;
        g[b][a] = 1;
    }

    for(i = 0; i<n; i++) {
        if(!vi[i]) {
            ans++;
            dfs(i); // busca em profundidade
        }
    }

    printf("%d\n", ans);
}