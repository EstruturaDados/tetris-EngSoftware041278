#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ------------------------------------------------------------
// Estrutura de uma peça
// ------------------------------------------------------------
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// ------------------------------------------------------------
// Estrutura da Fila Circular (peças futuras)
// ------------------------------------------------------------
typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ------------------------------------------------------------
// Estrutura da Pilha (peças reservadas)
// ------------------------------------------------------------
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// ------------------------------------------------------------
// Funções auxiliares da fila
// ------------------------------------------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void exibirFila(Fila *f);

// ------------------------------------------------------------
// Funções auxiliares da pilha
// ------------------------------------------------------------
void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
void exibirPilha(Pilha *p);

// ------------------------------------------------------------
// Outras funções
// ------------------------------------------------------------
Peca gerarPeca();
void exibirEstado(Fila *f, Pilha *p);

// ------------------------------------------------------------
// Função principal
// ------------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças automáticas
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    do {
        printf("\n=== TETRIS STACK - NIVEL AVENTUREIRO ===\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n>> Jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca()); // mantém fila cheia
                } else {
                    printf("\nFila vazia! Nenhuma peça para jogar.\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Nao ha peca para reservar.\n");
                    break;
                }
                if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Nao e possivel reservar mais pecas.\n");
                    break;
                }
                Peca reservada = desenfileirar(&fila);
                empilhar(&pilha, reservada);
                printf("\n>> Reservou a peca [%c %d]\n", reservada.nome, reservada.id);
                enfileirar(&fila, gerarPeca()); // gera nova peça
                break;
            }
            case 3: { // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n>> Usou a peca reservada [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha vazia! Nenhuma peca reservada para usar.\n");
                }
                break;
            }
            case 0:
                printf("\nEncerrando o jogo... Ate a proxima rodada!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ------------------------------------------------------------
// Funções da fila
// ------------------------------------------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
    srand(time(NULL));
}

int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

int filaCheia(Fila *f) {
    return (f->quantidade == MAX_FILA);
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->pecas[f->tras] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'?', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->quantidade--;
    return removida;
}

void exibirFila(Fila *f) {
    printf("Fila de pecas:\t");
    if (filaVazia(f)) {
        printf("[vazia]");
        return;
    }
    for (int i = 0; i < f->quantidade; i++) {
        int index = (f->frente + i) % MAX_FILA;
        printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
    }
}

// ------------------------------------------------------------
// Funções da pilha
// ------------------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == MAX_PILHA - 1);
}

void empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->pecas[++(p->topo)] = peca;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->pecas[p->topo--];
    return removida;
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
}

// ------------------------------------------------------------
// Exibir estado geral do jogo
// ------------------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    exibirFila(f);
    printf("\n");
    exibirPilha(p);
    printf("\n");
}

// ------------------------------------------------------------
// Gerar nova peça automaticamente
// ------------------------------------------------------------
Peca gerarPeca() {
    static int contador = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contador++;
    return nova;
}
