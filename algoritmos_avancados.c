#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASH 26
#define MAX_PISTAS 10
#define MAX_NOME 50

// Nível Novato: Árvore de Salas

typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esquerda = s->direita = NULL;
    return s;
}

void explorarSala(Sala *s) {
    if (!s) return;
    char op;
    while(1){
        printf("Você está na sala: %s\n", s->nome);
        printf("Escolha direção: (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &op);
        if(op=='s') break;
        else if(op=='e') s = s->esquerda;
        else if(op=='d') s = s->direita;
        else printf("Opção inválida!\n");
        if(!s) {
            printf("Não há sala nessa direção!\n");
            break;
        }
    }
}

// Nível Aventureiro: BST de Pistas

typedef struct Pista {
    char texto[MAX_NOME];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

Pista* criarPista(const char *texto) {
    Pista *p = (Pista*) malloc(sizeof(Pista));
    strcpy(p->texto, texto);
    p->esq = p->dir = NULL;
    return p;
}

Pista* inserirBST(Pista *raiz, Pista *novo) {
    if(!raiz) return novo;
    if(strcmp(novo->texto, raiz->texto)<0)
        raiz->esq = inserirBST(raiz->esq, novo);
    else
        raiz->dir = inserirBST(raiz->dir, novo);
    return raiz;
}

void emOrdem(Pista *raiz) {
    if(!raiz) return;
    emOrdem(raiz->esq);
    printf("- %s\n", raiz->texto);
    emOrdem(raiz->dir);
}

// Nível Mestre: Hash de Suspeitos

typedef struct PistaNode {
    char texto[MAX_NOME];
    struct PistaNode *prox;
} PistaNode;

typedef struct Suspeito {
    char nome[MAX_NOME];
    PistaNode *pistas;
    struct Suspeito *prox; // lista encadeada na hash
} Suspeito;

Suspeito* hash[MAX_HASH];

int hashFunction(const char *nome) {
    return (nome[0]-'A') % MAX_HASH;
}

void inicializarHash() {
    for(int i=0;i<MAX_HASH;i++) hash[i]=NULL;
}

Suspeito* buscarSuspeito(const char *nome) {
    int h = hashFunction(nome);
    Suspeito *s = hash[h];
    while(s){
        if(strcmp(s->nome, nome)==0) return s;
        s = s->prox;
    }
    return NULL;
}

void inserirHash(const char *pista, const char *suspeitoNome) {
    int h = hashFunction(suspeitoNome);
    Suspeito *s = buscarSuspeito(suspeitoNome);
    if(!s){
        s = (Suspeito*) malloc(sizeof(Suspeito));
        strcpy(s->nome, suspeitoNome);
        s->pistas = NULL;
        s->prox = hash[h];
        hash[h] = s;
    }
    PistaNode *pn = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(pn->texto, pista);
    pn->prox = s->pistas;
    s->pistas = pn;
}

void listarAssociacoes() {
    printf("=== Suspeitos e pistas ===\n");
    for(int i=0;i<MAX_HASH;i++){
        Suspeito *s = hash[i];
        while(s){
            printf("%s:\n", s->nome);
            PistaNode *p = s->pistas;
            while(p){
                printf("  - %s\n", p->texto);
                p = p->prox;
            }
            s = s->prox;
        }
    }
}

Suspeito* suspeitoMaisProvavel() {
    Suspeito *maisProvavel = NULL;
    int max = -1;
    for(int i=0;i<MAX_HASH;i++){
        Suspeito *s = hash[i];
        while(s){
            int count=0;
            PistaNode *p = s->pistas;
            while(p){ count++; p=p->prox;}
            if(count>max){ max=count; maisProvavel=s;}
            s=s->prox;
        }
    }
    return maisProvavel;
}

// MAIN

int main() {
    srand(time(NULL));
    
    // ---- Nível Novato ----
    Sala *hall = criarSala("Hall");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sotao");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;

    printf("=== Explorando a Mansão ===\n");
    explorarSala(hall);

    // ---- Nível Aventureiro ----
    Pista *raizPistas = NULL;
    raizPistas = inserirBST(raizPistas, criarPista("Pegada suspeita na cozinha"));
    raizPistas = inserirBST(raizPistas, criarPista("Livro fora do lugar na biblioteca"));
    raizPistas = inserirBST(raizPistas, criarPista("Janela aberta no sotao"));

    printf("\n=== Pistas Coletadas ===\n");
    emOrdem(raizPistas);

    // ---- Nível Mestre ----
    inicializarHash();
    inserirHash("Pegada suspeita na cozinha", "Sr. Preto");
    inserirHash("Janela aberta no sotao", "Sr. Preto");
    inserirHash("Livro fora do lugar na biblioteca", "Sra. Verde");

    listarAssociacoes();

    Suspeito *provavel = suspeitoMaisProvavel();
    if(provavel)
        printf("\nSuspeito mais provável: %s\n", provavel->nome);

    return 0;
}
