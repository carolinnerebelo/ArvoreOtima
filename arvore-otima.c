#include <stdio.h>
#include <stdlib.h>

int qtd_chaves, *chaves;

typedef struct {
    int raiz;
    int custo;
    int somatorio;
} Tabela;

typedef struct no{
    int valor;
    struct no *esquerda, *direita;
} NoArv;

NoArv *raiz = NULL;

Tabela **initTabela();
NoArv *inserir(NoArv *raiz, int num);
void inserirChave(Tabela **tabela, int i, int j);
void imprimirArvore(NoArv *raiz);
void calculaSomatorio(Tabela **tabela, int *frequencia, int *frequencia_externa);
int calculaCusto(Tabela **tabela, int i, int j);

int main () {
    int i, custo_min;
    int *frequencia, *frequencia_externa;
    Tabela **tabela;
    
    scanf("%d", &qtd_chaves);

    chaves = (int*) malloc ( (qtd_chaves+1) * sizeof(int));
    frequencia = (int*) malloc ( (qtd_chaves+1) * sizeof(int));
    frequencia_externa = (int*) malloc ( (qtd_chaves+1) * sizeof(int));

    for (i = 1; i < qtd_chaves+1; i++) {
        scanf("%d", &chaves[i]);
    }

    for (i = 1; i < qtd_chaves+1; i++) {
        scanf("%d", &frequencia[i]);
    }

    for (i = 0; i < qtd_chaves+1; i++) {
        scanf("%d", &frequencia_externa[i]);
    }

    tabela = initTabela();

    calculaSomatorio(tabela, frequencia, frequencia_externa);
    custo_min = calculaCusto(tabela, 0, qtd_chaves);

    printf("Custo ótimo = %d\n\n", custo_min);
    inserirChave(tabela, 0, qtd_chaves);
    imprimirArvore(raiz);

    free(tabela);
    free(raiz);
    return 0;
}

Tabela **initTabela() {
    int i, j;
    Tabela **tabela;

    tabela = (Tabela**) malloc((qtd_chaves+1) * sizeof(Tabela*));

    for (i = 0; i < qtd_chaves+1; i++) {
        tabela[i] = (Tabela*) malloc((qtd_chaves+1) * sizeof(Tabela));
    }

    for (i = 0; i < qtd_chaves+1; i++) {
        for (j = i; j < qtd_chaves+1; j++) {
            tabela[i][j].raiz = -1;
            tabela[i][j].custo = -1;
            tabela[i][j].somatorio = -1;
        }
    }

    return tabela;
}

NoArv *inserir(NoArv *raiz, int num) {

    if (raiz == NULL) {
        NoArv *aux = (NoArv*) malloc(sizeof(NoArv));
        if (aux) {
            aux->valor = num;
            aux->esquerda = NULL;
            aux->direita = NULL;
            return aux;
        }
        return NULL;
    }
    else {
        if (num < raiz->valor) {
            raiz->esquerda = inserir(raiz->esquerda, num);
        }
        else {
            raiz->direita = inserir(raiz->direita, num);
        }
        return raiz;
    }
}

void inserirChave(Tabela **tabela, int i, int j) {
    int k;

    if (i == j)
        return;

    k = tabela[i][j].raiz;

    if (j == i+1) {
        raiz = inserir(raiz, chaves[k]);
        return;
    }

    raiz = inserir(raiz, chaves[k]);
    inserirChave(tabela, i, k-1);
    inserirChave(tabela, k, j);
}

void imprimirArvore(NoArv *raiz) {
    if (raiz) {
        printf("%d ", raiz->valor);
        imprimirArvore(raiz->esquerda);
        imprimirArvore(raiz->direita);
    } 
}

void calculaSomatorio(Tabela **tabela, int *frequencia, int *frequencia_externa) {
    int i, j;

    for (i = 0; i < qtd_chaves+1; i++) {
        for (j = i; j < qtd_chaves+1; j++) {
            if (i == j) {
                tabela[i][j].somatorio = frequencia_externa[j];
            }
            else {
                tabela[i][j].somatorio = tabela[i][j-1].somatorio + frequencia[j] + frequencia_externa[j];
            }
        }
    }
}

int calculaCusto(Tabela **tabela, int i, int j) {
    int k, r, custo, custo_min;
    custo_min = -1;
    custo = -1;

    if (i == j) {
        tabela[i][j].custo = 0;
        return tabela[i][j].custo;
    }
    else if (j == i+1) {
        tabela[i][j].custo = tabela[i][j].somatorio;
        tabela[i][j].raiz = j;
        return tabela[i][j].custo;
    }
    else {
        custo_min = calculaCusto(tabela, i, i) + calculaCusto(tabela, i+1, j) + tabela[i][j].somatorio;
        k = i+1;
        for (r = i+2; r <= j; r++) {
            custo = calculaCusto(tabela, i, r-1) + calculaCusto(tabela, r, j) + tabela[i][j].somatorio;
            if (custo < custo_min) {
                custo_min = custo;
                k = r;
            }
        }
        tabela[i][j].custo = custo_min;
        tabela[i][j].raiz = k;
    }

    return tabela[i][j].custo;
}

