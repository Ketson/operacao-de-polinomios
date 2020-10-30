#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

typedef struct no{
    int coef;   //coeficiente
    int exp;    //expoente
    struct no *prox;    //ponteiro para o proximo nó da lista
}no;

//criar monomio
no *criaNo(int coef, int exp){

    no *novo; //ponteiro nó
    novo = (no*)malloc(sizeof(no)); //aloca

    //se a alocação falhar
    if(novo == NULL){
        printf("ERRO: problemas com a alocação de memoria.\n");
        exit(1);
    }
    novo -> coef = coef;    //insere o valor no coeficiente
    novo -> exp = exp;  //insere o valor no expoente
    novo -> prox = NULL; //o ponteiro prox aponta para NULL
    return novo;
}

//função para inserir e ordenar os nós
no *insere(no *L, int coef, int exp){
    int soma = 0;
    no *aux = L;    //variavel auxiliar aponta para o primeiro elemento da lista, também pode ser vázio
    no *ant = NULL; //variavel anterior ao auxiliar
    no *novo = criaNo(coef, exp); //será criado um nó com os coeficientes e os expoentes

    //se a lista estiver vazia, os elementos ficara na primeira posição
    if(L == NULL){
        L = novo;
        return L;
    }
    //se o expoente ser maior que o primeiro valor da lista
    if(novo -> exp > aux -> exp){
        novo -> prox = L;   //o novo aponta para o primeiro da lista
        L = novo;   //a lista aponta para o novo, que se tornou o novo primeiro
        return L;
    }

    //ira procurar o menor expoente ou igual, e inserir antes
    while (aux -> prox != NULL && aux -> exp > novo -> exp){
        ant = aux; //a variavel ant sempre é o anterior a aux
        aux = aux -> prox;  //posição do menor expoente após o novo expoente
    }

    if(aux -> prox == NULL && aux -> exp > novo -> exp){
        aux -> prox = novo;
    }else{ //o novo expoente na posição da lista ordenada
        if(aux -> exp == novo -> exp){  //// se o novo expoente ja existir na lista, vai realizar a soma dos expoentes
            soma = aux -> coef + novo -> coef;
            aux -> coef = soma;
        }else{ //se o novo expoente não exista na lista, o novo aponta para o proximo nó do aux,e depois o ant aponta para o novo
            novo -> prox = aux;
            ant -> prox = novo;
        }
    }
    return L;
}

//liberar a lista
no *liberaLista(no *L){
    if(L != NULL){
        no *aux = L; //percorrer a lista

    // atualizar L e liberar a memória de aux
    while(aux != NULL){
        L = L->prox;
        free(aux);
        aux = L;
    }
  }
  return L;
}

//função para somar a lista
no *somar(no *L1, no *L2){
    no *L3 = NULL, //variavel para resultado
    //auxiliares para percorrer a lista
    *aux1 = L1,
    *aux2 = L2;

    if(L1 == NULL){ //se estiver vazio
        L3 = L2;
        return L3;
    }
    if(L2 == NULL){ //se estiver vazio
        L3 = L1;
        return L3;
    }
    //a função de inserir um a um polinômio trata o se caso em que dois expoentes sejam iguais.logo, basta apenas adicionar todos os monômios dos dois polinômios que o resultado será a soma dos dois
    while(aux1 != NULL){ //adicionando L1 no final
        L3 = insere(L3,aux1 -> coef,aux1 -> exp);
        aux1 = aux1 -> prox;
    }
    while(aux2 != NULL){ //adicionando L2 no final
         L3 = insere(L3,aux2 -> coef,aux2 -> exp);
        aux2 = aux2 -> prox;
    }
    return L3;
}

//função para subtrair a lista
no  *sub(no *L1, no *L2){
    no *L3 = NULL, //variavel para resultado
    //auxiliares para percorrer a lista
    *aux1 = L1,
    *aux2 = L2;

    if(L2 == NULL){ //se estiver vazio
        return L3 = L1;
    }
    if(L1 == NULL){//se estiver vazio
        while(aux2 != NULL){ //adiciona L2 na lista final
            L3 = insere(L3,aux2 -> coef*(-1),aux2 -> exp); //mudar de sinal
            aux2 = aux2 -> prox;
        }
        return L3;
    }

    /*a função de inserir um a um polinômio trata o se caso em que dois expoentes sejam iguais.logo,
    basta apenas adicionar todos os monômios dos dois polinômios que o resultado será a soma dos dois
    só que o segundo polinomio tera que ter os sinais trocados
    */
    while(aux1 != NULL){ //adicionando L1 na lista no final
        L3 = insere(L3,aux1 -> coef,aux1 -> exp);
        aux1 = aux1 -> prox;
    }
    while(aux2 != NULL){ // adicionando L2 na lista final
        L3 = insere(L3,aux2 -> coef*(-1),aux2 -> exp);
        aux2 = aux2 -> prox;
    }
    return L3;
}

//função para multiplicar a lista
no  *mult(no *L1, no *L2){
    no *L3 = NULL, //variavel resultado
    //auxiliareas para percorrer as listas
    *aux1 = L1,
    *aux2 = L2;

    //se um dos conjuntos não tiver conteúdo, ou seja, NULL
    // o resultado do produto será ZERO, ou NULL
    if(L1 == NULL || L2 == NULL){
        return L3 = NULL;
    }
    //se os polinomios tiver conteudo
    while(aux1 != NULL){
        while(aux2 != NULL){
        /*na multiplicação, vai ser feito distributiva cada termo do primeiro polinômio será multiplicado por todos os termos do segundo
        o resultado final será a soma de todas essas multiplicações
        passando essas informações como parâmetro da função insere, ela fará o trabalho de ordenar e somar os termos com expoentes iguais*/
            L3 = insere(L3, aux1 -> coef * aux2 -> coef, aux1 -> exp + aux2 -> exp);
            aux2 = aux2 -> prox;
        }
        aux1 = aux1 -> prox;
        aux2 = L2;
    }

    return L3;
}

//função imprimir lista
void imprime(no *L){
    no *aux = L; // aux aponta para o primeiro elemento da lista L

    while(aux != NULL){

        if(aux -> exp > 1)
            printf(" %dx^%d", aux -> coef, aux -> exp);

    //se o expoente é igual a 1,vai escreve lo
        if(aux -> exp == 1)
            printf(" %dx", aux -> coef);

    //quando o expoente é igual a zero, o resultado é 1 e nós multiplicamos pelo coeficiente, sobrando apenas o coeficiente
        if(aux -> exp == 0)
            printf(" %d",aux -> coef);

            aux = aux -> prox; // percorrer toda a lista
    }
  printf("\n");
}

//função preencher lista
no *preencher(no *L){
    int coef, exp;
    printf("\nDigite 0 no coeficiente para parar o programa");
    do{
        printf("\nCoeficiente: ");
        scanf("%d",&coef);

        if(coef != 0){
            printf("\nExpoente: ");
            scanf("%d", &exp);
            L = insere(L, coef, exp);
        }
    }while(coef != 0);

    return L;
}

//função principal
int main() {

    setlocale(LC_ALL,"portuguese");

    no *Lista1 = NULL, *Lista2 = NULL, *Lista3 = NULL;
    int op;

    do{
        printf("==============================================================================\n");
      printf("                            CALCULADORA DE POLINOMIOS                        \n");
      printf("==============================================================================\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("                            DIGITE A OPCAO:\n");
      printf("\n");
      printf("                          [1] Adição                      \n");
      printf("                          [2] Subtração                     \n");
      printf("                          [3] Multiplicação                          \n");
      printf("                          [ ] Sair do sistema                            \n");
      printf("\n");

      printf("Digite a operação desejada: ");
      scanf("%d", &op);

        switch(op){
            case 1:
                printf("\nPrimeiro polinomio: ");
                Lista1 = preencher(Lista1);

                printf("\nSegundo polinomio: ");
                Lista2 = preencher(Lista2);


                printf("\nPolinomio 1: ");
                imprime(Lista1);

                printf("\nPolinomio 2: ");
                imprime(Lista2);

                printf("\nResultado da soma:");
                Lista3 = somar(Lista1,Lista2);

                imprime(Lista3);


                Lista1 = liberaLista(Lista1);
                Lista2 = liberaLista(Lista2);
                Lista3 = liberaLista(Lista3);


                break;

            case 2:
                printf("\nPrimeiro polinomio: ");
                Lista1 = preencher(Lista1);

                printf("\nSegundo polinomio: ");
                Lista2 = preencher(Lista2);

                printf("\nPolinomio 1: ");
                imprime(Lista1);

                printf("\nPolinomio 2: ");
                imprime(Lista2);

                printf("\nResultado da subtração:");
                Lista3 = sub(Lista1,Lista2);
                imprime(Lista3);


                Lista1 = liberaLista(Lista1);
                Lista2 = liberaLista(Lista2);
                Lista3 = liberaLista(Lista3);

                break;

            case 3:
                printf("\nPrimeiro polinomio: ");
                Lista1 = preencher(Lista1);

                printf("\nSegundo polinomio: ");
                Lista2 = preencher(Lista2);

                printf("\nPolinomio 1: ");
                imprime(Lista1);

                printf("\nPolinomio 2: ");
                imprime(Lista2);

                printf("\nResultado da subtração:");
                Lista3 = mult(Lista1,Lista2);
                imprime(Lista3);

                Lista1 = liberaLista(Lista1);
                Lista2 = liberaLista(Lista2);
                Lista3 = liberaLista(Lista3);

                break;
            default:
                exit(1);
      }
    }while(op);

    return 0;
}
