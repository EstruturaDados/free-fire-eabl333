#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// =========================================
// STRUCTS
// =========================================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// =========================================
// VARIÁVEIS GLOBAIS
// =========================================

int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// =========================================
// FUNÇÕES PARA VETOR
// =========================================

void inserirItemVetor(Item vetor[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia (vetor)!\n");
        return;
    }

    Item novo;
    printf("\nNome: ");
    fgets(novo.nome, 30, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo.tipo, 20, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    vetor[*qtd] = novo;
    (*qtd)++;

    printf("Item adicionado ao vetor!\n");
}

void listarItensVetor(Item vetor[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia (vetor)\n");
        return;
    }
    printf("\n--- Itens no vetor ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

void removerItemVetor(Item vetor[], int *qtd) {
    char nome[30];
    printf("\nDigite o nome do item a remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < *qtd; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            for (int j = i; j < *qtd - 1; j++) {
                vetor[j] = vetor[j + 1];
            }
            (*qtd)--;
            printf("Item removido do vetor!\n");
            return;
        }
    }
    printf("Item nao encontrado no vetor!\n");
}

void ordenarVetor(Item vetor[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Item temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
    printf("Itens ordenados (Bubble Sort)\n");
}

void buscarSequencialVetor(Item vetor[], int qtd) {
    comparacoesSequencial = 0;
    char nome[30];
    printf("\nDigite o nome do item para busca sequencial: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < qtd; i++) {
        comparacoesSequencial++;
        if (strcmp(vetor[i].nome, nome) == 0) {
            printf("Item encontrado (vetor): %s | Tipo: %s | Quantidade: %d\n",
                   vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoesSequencial);
            return;
        }
    }

    printf("Item nao encontrado.\nComparacoes realizadas: %d\n", comparacoesSequencial);
}

void buscarBinariaVetor(Item vetor[], int qtd) {
    comparacoesBinaria = 0;
    char nome[30];
    printf("\nDigite o nome do item para busca binaria: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int inicio = 0, fim = qtd - 1;
    while (inicio <= fim) {
        comparacoesBinaria++;
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(nome, vetor[meio].nome);

        if (cmp == 0) {
            printf("Item encontrado (binaria): %s | Tipo: %s | Quantidade: %d\n",
                   vetor[meio].nome, vetor[meio].tipo, vetor[meio].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoesBinaria);
            return;
        } else if (cmp > 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    printf("Item nao encontrado.\nComparacoes realizadas: %d\n", comparacoesBinaria);
}

// =========================================
// FUNÇÕES PARA LISTA ENCADEADA
// =========================================

No* inserirItemLista(No* inicio) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        printf("Erro de memoria!\n");
        return inicio;
    }

    printf("\nNome: ");
    fgets(novo->dados.nome, 30, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo->dados.tipo, 20, stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();

    novo->proximo = inicio;
    printf("Item adicionado na lista encadeada!\n");
    return novo;
}

No* removerItemLista(No* inicio) {
    if (!inicio) {
        printf("Lista vazia!\n");
        return NULL;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = inicio, *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL)
                inicio = atual->proximo;
            else
                anterior->proximo = atual->proximo;

            free(atual);
            printf("Item removido da lista encadeada!\n");
            return inicio;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item nao encontrado na lista encadeada.\n");
    return inicio;
}

void listarItensLista(No* inicio) {
    if (!inicio) {
        printf("Lista encadeada vazia.\n");
        return;
    }

    printf("\n--- Itens na lista encadeada ---\n");
    No* atual = inicio;
    while (atual != NULL) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

void buscarSequencialLista(No* inicio) {
    comparacoesSequencial = 0;
    char nome[30];
    printf("\nDigite o nome do item para busca (lista): ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No* atual = inicio;
    while (atual != NULL) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            printf("Item encontrado (lista): %s | Tipo: %s | Quantidade: %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoesSequencial);
            return;
        }
        atual = atual->proximo;
    }

    printf("Item nao encontrado.\nComparacoes realizadas: %d\n", comparacoesSequencial);
}

// =========================================
// FUNÇÃO PRINCIPAL
// =========================================

int main() {
    Item mochilaVetor[MAX_ITENS];
    int qtd = 0;
    No* mochilaLista = NULL;
    int opcaoEstrutura, opcao;

    do {
        printf("\n=== ESCOLHA A ESTRUTURA ===\n");
        printf("1. Mochila com vetor\n");
        printf("2. Mochila com lista encadeada\n");
        printf("0. Sair\n> ");
        scanf("%d", &opcaoEstrutura);
        getchar();

        if (opcaoEstrutura == 1) {
            do {
                printf("\n--- Mochila com Vetor ---\n");
                printf("1. Inserir\n2. Remover\n3. Listar\n4. Ordenar\n5. Busca Sequencial\n6. Busca Binaria\n0. Voltar\n> ");
                scanf("%d", &opcao);
                getchar();

                switch (opcao) {
                    case 1: inserirItemVetor(mochilaVetor, &qtd); break;
                    case 2: removerItemVetor(mochilaVetor, &qtd); break;
                    case 3: listarItensVetor(mochilaVetor, qtd); break;
                    case 4: ordenarVetor(mochilaVetor, qtd); break;
                    case 5: buscarSequencialVetor(mochilaVetor, qtd); break;
                    case 6: buscarBinariaVetor(mochilaVetor, qtd); break;
                }
            } while (opcao != 0);

        } else if (opcaoEstrutura == 2) {
            do {
                printf("\n--- Mochila com Lista Encadeada ---\n");
                printf("1. Inserir\n2. Remover\n3. Listar\n4. Buscar\n0. Voltar\n> ");
                scanf("%d", &opcao);
                getchar();

                switch (opcao) {
                    case 1: mochilaLista = inserirItemLista(mochilaLista); break;
                    case 2: mochilaLista = removerItemLista(mochilaLista); break;
                    case 3: listarItensLista(mochilaLista); break;
                    case 4: buscarSequencialLista(mochilaLista); break;
                }
            } while (opcao != 0);
        }

    } while (opcaoEstrutura != 0);

    printf("\nSaindo do sistema...\n");
    return 0;
}
