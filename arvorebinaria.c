#include <stdio.h>
#include <stdlib.h>

typedef struct arv {
    char info;
    struct arv* esq;
    struct arv* dir;
}Arv;

Arv* arv_criavazia(void);
Arv* arv_cria(char c, Arv* e, Arv* d);
Arv* rotacao_ll(Arv* a);
Arv* rotacao_rr(Arv* a);
Arv* balancear(Arv* a);
Arv* insere_recursivo(Arv* a, char c);
Arv* insere_nao_recursivo(Arv* a, char c); //sempre ordenado
Arv* remove_folha(Arv* a, char c);
int arv_vazia(Arv* a);
Arv* arv_libera (Arv* a);
Arv* arv_busca(Arv* a, char c);
void arv_imprime_prefix(Arv* a);
void arv_imprime_infix(Arv* a);
void arv_imprime_posfix(Arv* a);
int conta_nos (Arv* a);
int conta_folhas(Arv* a);
int altura (Arv* a);

int main(void) {
    //Main apenas para testes das funçoes
    #define insere_avl insere_recursivo
    #define remove_avl remove_folha

    // 1. Cria uma árvore vazia
    Arv* arvore = arv_criavazia();

    printf("--- Teste de Insercao e Balanceamento ---\n");

    printf("\nInserindo 'C'...\n");
    arvore = insere_avl(arvore, 'C');
    printf("Em ordem: "); arv_imprime_infix(arvore); printf("\n");

    printf("\nInserindo 'B'...\n");
    arvore = insere_avl(arvore, 'B');
    printf("Em ordem: "); arv_imprime_infix(arvore); printf("\n");

    printf("\nInserindo 'A'... (Deve causar desbalanceamento)\n");
    arvore = insere_avl(arvore, 'A'); // Força uma rotação 'll' no nó 'C'
    printf("Em ordem: "); arv_imprime_infix(arvore); printf("\n");
    printf("Pre-ordem (para ver a estrutura): "); arv_imprime_prefix(arvore); printf("\n");

    printf("\nInserindo 'D'...\n");
    arvore = insere_avl(arvore, 'D');
    printf("\nInserindo 'E'... (Deve causar desbalanceamento)\n");
    arvore = insere_avl(arvore, 'E'); // Força uma rotação 'rr' no nó 'D'
    printf("Em ordem: "); arv_imprime_infix(arvore); printf("\n");
    printf("Pre-ordem (para ver a estrutura): "); arv_imprime_prefix(arvore); printf("\n");

    printf("\n--- Teste das Funcoes de Consulta ---\n");
    printf("Altura da arvore: %d\n", altura(arvore));
    printf("Numero de nos: %d\n", conta_nos(arvore));
    printf("Numero de folhas: %d\n", conta_folhas(arvore));

    char busca_char = 'D';
    if (arv_busca(arvore, busca_char)) {
        printf("Caractere '%c' encontrado na arvore.\n", busca_char);
    } else {
        printf("Caractere '%c' NAO encontrado na arvore.\n", busca_char);
    }

    printf("\n--- Teste de Remocao e Balanceamento ---\n");
    printf("Removendo a folha 'E'...\n");
    arvore = remove_avl(arvore, 'E');
    printf("Em ordem: "); arv_imprime_infix(arvore); printf("\n");
    printf("Pre-ordem (para ver a estrutura): "); arv_imprime_prefix(arvore); printf("\n");

    printf("\n--- Liberando a memoria ---\n");
    arvore = arv_libera(arvore);
    if (arv_vazia(arvore)) {
        printf("Arvore liberada com sucesso!\n");
    }

    return 0;
}
Arv* arv_criavazia(void) {
    return NULL;
}
Arv* arv_cria(char c, Arv* e, Arv* d) {
    Arv* novo = (Arv*)malloc(sizeof(Arv));
    novo->info = c;
    novo->esq = e;
    novo->dir = d;
    return novo;
}
Arv* rotacao_ll(Arv* a) { // quando fator positivo
    printf("Aplicando rotacao ll em %c\n", a->info); // Ajuda a ver o que acontece
    Arv* no = a->esq;
    a->esq = no->dir;
    no->dir = a;
    return no; // 'no' é a nova raiz da sub-árvore
}
Arv* rotacao_rr(Arv* a) { //quando fator negativo
    printf("Aplicando rotacao rr em %c\n", a->info); // Ajuda a ver o que acontece
    Arv* no = a->dir;
    a->dir = no->esq;
    no->esq = a;
    return no; // 'no' é a nova raiz da sub-árvore
}
Arv* balancear(Arv* a) {
    // Se o nó for nulo, não há o que balancear.
    if (a == NULL) {
        return NULL;
    }

    // Calcula o Fator de Balanço
    int fb = altura(a->esq) - altura(a->dir);

    // ---- DESBALANCEADO PARA A ESQUERDA (Fator POSITIVO) ----
    if (fb > 1) {
        int fb_filho = altura(a->esq->esq) - altura(a->esq->dir);
        if (fb_filho < 0) { // Caso Esquerda-Direita
            a->esq = rotacao_rr(a->esq);
        }
        return rotacao_ll(a); // Retorna a nova raiz da sub-árvore
    }

    // ---- DESBALANCEADO PARA A DIREITA (Fator NEGATIVO) ----
    if (fb < -1) {
        int fb_filho = altura(a->dir->esq) - altura(a->dir->dir);
        if (fb_filho > 0) { // Caso Direita-Esquerda
            a->dir = rotacao_ll(a->dir);
        }
        return rotacao_rr(a); // Retorna a nova raiz da sub-árvore
    }

    // Se não precisou rotacionar, apenas retorna o ponteiro do nó original.
    return a;
}
Arv* insere_recursivo(Arv* a, char c) {
        // PARTE 1: INSERÇÃO
        if (a == NULL) {
            return arv_cria(c, NULL, NULL);
        }

        if (c < a->info) {
            a->esq = insere_recursivo(a->esq, c);
        }
        else if (c > a->info) {
            a->dir = insere_recursivo(a->dir, c);
        }
        else return a;

        return balancear(a);
}


Arv* insere_nao_recursivo(Arv* a,char c) {
    Arv* novo = (Arv*)malloc(sizeof(Arv));
    if (!novo) {
        exit(1);
    }
    novo->info = c;
    novo->esq = NULL;
    novo->dir = NULL;

    if (!a) {
        return novo;
    }
    Arv* atual = a;
    Arv* pai = NULL;

    while (atual != NULL) {
        pai = atual;
        if (c < atual->info) {
            atual = atual->esq;
        }
        else if (c > atual->info) {
            atual = atual->dir;
        }
        else {
            free(novo);
            return a;
        }
    }
    if (c < pai->info) {
        pai->esq = novo;
    } else pai->dir = novo;

    return a;
}

Arv* remove_folha(Arv* a, char c) {
        // Caso base: se a árvore é vazia, não há o que remover.
        if (a == NULL) {
            return NULL;
        }

        // Passo 1: Busca pelo nó, de forma recursiva.
        if (c < a->info) {
            a->esq = remove_folha(a->esq, c);
        }
        else if (c > a->info) {
            a->dir = remove_folha(a->dir, c);
        }
        // Encontramos o nó que talvez seja removido (c == a->info).
        else {
            // Passo 2: Verifica se é uma folha.
            if (a->esq == NULL && a->dir == NULL) {
                free(a);     // Libera a memória do nó.
                return NULL; // Retorna NULL para o nó pai, efetivamente desconectando a folha.
            }
        }
    return balancear(a);
}

int arv_vazia(Arv* a) {
    return a == NULL;
}
Arv* arv_libera (Arv* a) { //implementaçao pos fix
    if (!arv_vazia(a)) {
        arv_libera(a->esq);
        arv_libera(a->dir);
        free(a);
    }
    return NULL;
}
Arv* arv_busca(Arv* a, char c) {
    // Caso Base 1: A árvore é vazia ou chegamos a uma folha e não encontramos.
    if (!a) {
        return NULL;
    }

    // Se o valor é menor, procuramos recursivamente na esquerda.
    if (c < a->info) {
        return arv_busca(a->esq, c);
    }
    // Se o valor é maior, procuramos recursivamente na direita.
    else if (c > a->info) {
        return arv_busca(a->dir, c);
    }
    // Se não é nem menor, nem maior, então é igual!
    else {
        // Caso Base 2: Encontramos o nó! Retornamos o ponteiro para ele.
        return a;
    }
}
void arv_imprime_prefix(Arv* a) {
    if (a != NULL) {
        //primeiro raiz
        printf("%c ", a->info);
        //dps esquerda
        arv_imprime_prefix(a->esq);
        //dps direita
        arv_imprime_prefix(a->dir);
    }
}
void arv_imprime_infix(Arv* a) {
    if (a != NULL) {
        //esq
        arv_imprime_infix(a->esq);
        //raiz
        printf("%c ", a->info);
        //dir
        arv_imprime_infix(a->dir);
    }
}

void arv_imprime_posfix(Arv* a) {
    if (a != NULL) {
        //esq
        arv_imprime_posfix(a->esq);
        //dir
        arv_imprime_posfix(a->dir);
        //raiz
        printf("%c ", a->info);
    }
}
int conta_nos (Arv* a) {
    if (!a) {
        return 0;
    }
    int x, y;
    x = conta_nos(a->esq);
    y = conta_nos(a->dir);
    return (x + y + 1);
}

int conta_folhas(Arv* a) {
        // Caso Base 1: Uma árvore (ou sub-árvore) vazia não tem folhas.
        if (!a) {
            return 0;
        }

        // Caso Base 2: Se os dois filhos são nulos, este nó é uma folha.
        if (a->esq == NULL && a->dir == NULL) {
            return 1; // Contamos este nó como uma folha.
        }

        // Passo Recursivo: Se não é um dos casos acima, é um nó interno.
        // O total de folhas é a soma das folhas de cada lado.
        // Note que não somamos +1 aqui, pois o nó interno não é uma folha.
        return conta_folhas(a->esq) + conta_folhas(a->dir);
}

int altura (Arv* a) {
    if (!a) {
        return 0;
    }
    int x,y;
    x = altura (a->esq);
    y = altura (a->dir);
    if (x > y) {
        return x+1;
    } else return y+1;
}
