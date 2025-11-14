#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20

/* Struct que representa um componente da torre */
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 (baixo) a 10 (alto)
} Componente;

/* Prototipos */
void lerLinha(char *buffer, int tamanho);
void cadastrarComponente(Componente v[], int *n);
void mostrarComponentes(Componente v[], int n);
void bubbleSortPorNome(Componente v[], int n, long *comparacoes, double *tempoSeg);
void insertionSortPorTipo(Componente v[], int n, long *comparacoes, double *tempoSeg);
void selectionSortPorPrioridade(Componente v[], int n, long *comparacoes, double *tempoSeg);
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long *comparacoes);
void limparTela();

/* Le uma linha com fgets e remove o '\n' */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

/* Cadastra um novo componente (até MAX_COMPONENTES) */
void cadastrarComponente(Componente v[], int *n) {
    if (*n >= MAX_COMPONENTES) {
        printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
        return;
    }

    Componente c;
    printf("Nome do componente: ");
    lerLinha(c.nome, sizeof(c.nome));
    if (strlen(c.nome) == 0) {
        printf("Nome vazio - cadastro cancelado.\n");
        return;
    }

    printf("Tipo do componente (ex: controle, suporte, propulsao): ");
    lerLinha(c.tipo, sizeof(c.tipo));

    char buffer[20];
    int prioridade = 0;
    do {
        printf("Prioridade (1 a 10): ");
        lerLinha(buffer, sizeof(buffer));
        prioridade = atoi(buffer);
        if (prioridade < 1 || prioridade > 10) {
            printf("Valor invalido. Insira entre 1 e 10.\n");
        }
    } while (prioridade < 1 || prioridade > 10);
    c.prioridade = prioridade;

    v[*n] = c;
    (*n)++;
    printf("Componente cadastrado com sucesso! (total: %d)\n", *n);
}

/* Exibe a lista de componentes formatada */
void mostrarComponentes(Componente v[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\n---- Componentes (%d) ----\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-25s | Tipo: %-12s | Prioridade: %d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
    printf("---------------------------\n");
}

/* Bubble sort por nome (strings). Conta comparações e mede tempo. */
void bubbleSortPorNome(Componente v[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // otimização simples
    }

    clock_t fim = clock();
    *tempoSeg = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* Insertion sort por tipo (strings). Conta comparações e mede tempo. */
void insertionSortPorTipo(Componente v[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        // comparar tipos lexicograficamente
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }

    clock_t fim = clock();
    *tempoSeg = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* Selection sort por prioridade (int). Conta comparações e mede tempo. */
void selectionSortPorPrioridade(Componente v[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int idxMin = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (v[j].prioridade < v[idxMin].prioridade) {
                idxMin = j;
            }
        }
        if (idxMin != i) {
            Componente tmp = v[i];
            v[i] = v[idxMin];
            v[idxMin] = tmp;
        }
    }

    clock_t fim = clock();
    *tempoSeg = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* Busca binária por nome (assume vetor previamente ordenado por nome).
   Retorna índice ou -1 se não encontrado. Conta comparações. */
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, v[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp > 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/* Limpa a tela (simples, portable-ish) */
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* Programa principal: menu e orquestração */
int main() {
    Componente inventario[MAX_COMPONENTES];
    int qtd = 0;
    int opcaoPrincipal = -1;

    while (opcaoPrincipal != 0) {
        printf("\n=== MONTAGEM: TORRE DE RESGATE ===\n");
        printf("1) Cadastrar componente\n");
        printf("2) Listar componentes\n");
        printf("3) Ordenar (escolher algoritmo)\n");
        printf("4) Busca binaria por nome (apenas se ordenado por nome)\n");
        printf("0) Sair\n");
        printf("Escolha: ");
        char escolhaStr[10];
        lerLinha(escolhaStr, sizeof(escolhaStr));
        opcaoPrincipal = atoi(escolhaStr);

        if (opcaoPrincipal == 1) {
            cadastrarComponente(inventario, &qtd);
        } else if (opcaoPrincipal == 2) {
            mostrarComponentes(inventario, qtd);
        } else if (opcaoPrincipal == 3) {
            if (qtd == 0) {
                printf("Nenhum componente para ordenar. Cadastre primeiro.\n");
                continue;
            }
            printf("\nEscolha o algoritmo de ordenacao:\n");
            printf("1) Bubble Sort por NOME (string)\n");
            printf("2) Insertion Sort por TIPO (string)\n");
            printf("3) Selection Sort por PRIORIDADE (int)\n");
            printf("Escolha: ");
            lerLinha(escolhaStr, sizeof(escolhaStr));
            int opAlg = atoi(escolhaStr);

            long comparacoes = 0;
            double tempo = 0.0;

            if (opAlg == 1) {
                printf("Executando Bubble Sort por NOME...\n");
                bubbleSortPorNome(inventario, qtd, &comparacoes, &tempo);
                printf("Ordenacao concluida (por NOME).\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(inventario, qtd);
            } else if (opAlg == 2) {
                printf("Executando Insertion Sort por TIPO...\n");
                insertionSortPorTipo(inventario, qtd, &comparacoes, &tempo);
                printf("Ordenacao concluida (por TIPO).\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(inventario, qtd);
            } else if (opAlg == 3) {
                printf("Executando Selection Sort por PRIORIDADE...\n");
                selectionSortPorPrioridade(inventario, qtd, &comparacoes, &tempo);
                printf("Ordenacao concluida (por PRIORIDADE).\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(inventario, qtd);
            } else {
                printf("Opcao invalida.\n");
            }

        } else if (opcaoPrincipal == 4) {
            if (qtd == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            // Aviso: busca binaria exige ordenação por nome
            printf("A busca binaria exige que os componentes estejam ordenados por NOME.\n");
            printf("Se voce ja ordenou por NOME, digite o nome do componente-chave:\n");
            char chave[30];
            printf("Nome a buscar: ");
            lerLinha(chave, sizeof(chave));
            if (strlen(chave) == 0) {
                printf("Nome vazio - busca cancelada.\n");
                continue;
            }
            long comparacoesBusca = 0;
            int idx = buscaBinariaPorNome(inventario, qtd, chave, &comparacoesBusca);
            if (idx >= 0) {
                printf("Componente encontrado na posicao %d:\n", idx + 1);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       inventario[idx].nome, inventario[idx].tipo, inventario[idx].prioridade);
                printf("Comparacoes na busca binaria: %ld\n", comparacoesBusca);
            } else {
                printf("Componente nao encontrado. Comparacoes: %ld\n", comparacoesBusca);
            }
        } else if (opcaoPrincipal == 0) {
            printf("Encerrando modulo de montagem. Boa sorte na fuga!\n");
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
