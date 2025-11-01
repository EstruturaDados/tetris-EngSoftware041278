#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5  // Capacidade máxima da fila

// ------------------------------------------------------------
// Estrutura que representa uma peça de Tetris
// ------------------------------------------------------------
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// ------------------------------------------------------------
// Estrutura da Fila de peças
// ------------------------------------------------------------
typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ------------------------------------------------------------
// Funções auxiliares
// ------------------------------------------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void exibirFila(Fila *f);
Peca gerarPeca();

// ------------------------------------------------------------
// Função principal
// ------------------------------------------------------------
int main() {
    Fila fila;
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças iniciais
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n=== TETRIS STACK - FILA DE PECAS FUTURAS ===\n");
        exibirFila(&fila);

        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nVoce jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                } else {
                    printf("\nA fila esta vazia! Nao ha peca para jogar.\n");
                }
                break;
            case 2:
                if (!filaCheia(&fila)) {
                    enfileirar(&fila, gerarPeca());
                    printf("\nNova peca adicionada ao final da fila.\n");
                } else {
                    printf("\nA fila esta cheia! Espere liberar espaco.\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo... ate a proxima partida!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// ------------------------------------------------------------
// Inicializa a fila vazia
// ------------------------------------------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
    srand(time(NULL)); // semente para geração aleatória
}

// ------------------------------------------------------------
// Verifica se a fila está vazia
// ------------------------------------------------------------
int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

// ------------------------------------------------------------
// Verifica se a fila está cheia
// ------------------------------------------------------------
int filaCheia(Fila *f) {
    return (f->quantidade == MAX_FILA);
}

// ------------------------------------------------------------
// Insere uma nova peça no final da fila (enqueue)
// ------------------------------------------------------------
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Erro: fila cheia!\n");
        return;
    }
    f->tras = (f->tras + 1) % MAX_FILA;
    f->pecas[f->tras] = p;
    f->quantidade++;
}

// ------------------------------------------------------------
// Remove a peça da frente da fila (dequeue)
// ------------------------------------------------------------
Peca desenfileirar(Fila *f) {
    Peca pecaRemovida = {'?', -1};

    if (filaVazia(f)) {
        printf("Erro: fila vazia!\n");
        return pecaRemovida;
    }

    pecaRemovida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->quantidade--;

    return pecaRemovida;
}

// ------------------------------------------------------------
// Exibe todas as peças na fila
// ------------------------------------------------------------
void exibirFila(Fila *f) {
    printf("\nFila de Pecas Futuras:\n");
    if (filaVazia(f)) {
        printf("[Fila vazia]\n");
        return;
    }

    int i, index;
    for (i = 0; i < f->quantidade; i++) {
        index = (f->frente + i) % MAX_FILA;
        printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
    }
    printf("\n");
}

// ------------------------------------------------------------
// Gera automaticamente uma nova peça
// ------------------------------------------------------------
Peca gerarPeca() {
    static int contadorID = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}
