#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ------------------------------------------------------------
// Estrutura que representa uma peça
// ------------------------------------------------------------
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// ------------------------------------------------------------
// Estrutura da fila circular
// ------------------------------------------------------------
typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ------------------------------------------------------------
// Estrutura da pilha de reserva
// ------------------------------------------------------------
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// ------------------------------------------------------------
// Protótipos das funções
// ------------------------------------------------------------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
Peca gerarPeca();
void exibirEstado(Fila *f, Pilha *p);
void trocarFrenteTopo(Fila *f, Pilha *p);
void trocarMultiplas(Fila *f, Pilha *p);

// ------------------------------------------------------------
// Função principal
// ------------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < MAX_FILA; i++)
        enfileirar(&fila, gerarPeca());

    int opcao;

    do {
        printf("\n=== TETRIS STACK - NIVEL AVANCADO ===\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n>> Jogou a peça [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nFila vazia! Nenhuma peça disponível.\n");
                }
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Não é possível reservar mais peças.\n");
                    break;
                }
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Nenhuma peça para reservar.\n");
                    break;
                }
                Peca reservada = desenfileirar(&fila);
                empilhar(&pilha, reservada);
                printf("\n>> Reservou a peça [%c %d]\n", reservada.nome, reservada.id);
                enfileirar(&fila, gerarPeca());
                break;
            }

            case 3: { // Usar peça da pilha
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n>> Usou a peça reservada [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha vazia! Nenhuma peça reservada para usar.\n");
                }
                break;
            }

            case 4: { // Troca direta
                trocarFrenteTopo(&fila, &pilha);
                break;
            }

            case 5: { // Troca múltipla
                trocarMultiplas(&fila, &pilha);
                break;
            }

            case 0:
                printf("\nEncerrando o jogo... até a próxima partida!\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ------------------------------------------------------------
// Funções auxiliares da fila
// ------------------------------------------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == MAX_FILA;
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

// ------------------------------------------------------------
// Funções auxiliares da pilha
// ------------------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
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

// ------------------------------------------------------------
// Gera uma nova peça automaticamente
// ------------------------------------------------------------
Peca gerarPeca() {
    static int contador = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contador++;
    return nova;
}

// ------------------------------------------------------------
// Exibir estado atual do jogo
// ------------------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");

    // Fila
    printf("Fila de peças\t");
    if (filaVazia(f))
        printf("[vazia]");
    else {
        for (int i = 0; i < f->quantidade; i++) {
            int index = (f->frente + i) % MAX_FILA;
            printf("[%c %d] ", f->pecas[index].nome, f->pecas[index].id);
        }
    }

    // Pilha
    printf("\nPilha de reserva\t(Topo -> base): ");
    if (pilhaVazia(p))
        printf("[vazia]");
    else {
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }

    printf("\n");
}

// ------------------------------------------------------------
// Trocar a peça da frente da fila com o topo da pilha
// ------------------------------------------------------------
void trocarFrenteTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNão é possível realizar a troca (fila ou pilha vazia).\n");
        return;
    }

    int idx = f->frente;
    Peca temp = f->pecas[idx];
    f->pecas[idx] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;

    printf("\n>> Troca realizada entre a peça da frente da fila e o topo da pilha.\n");
}

// ------------------------------------------------------------
// Trocar as 3 primeiras da fila com as 3 da pilha
// ------------------------------------------------------------
void trocarMultiplas(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\nNão é possível realizar a troca múltipla (faltam peças).\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % MAX_FILA;
        int idxPilha = p->topo - i;
        Peca temp = f->pecas[idxFila];
        f->pecas[idxFila] = p->pecas[idxPilha];
        p->pecas[idxPilha] = temp;
    }

    printf("\n>> Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
}
