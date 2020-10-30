#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

typedef struct no{
    int coef;   //coeficiente
    int exp;    //expoente
    struct no *prox;    //ponteiro para o proximo n� da lista
}no;

//criar monomio
no *criaNo(int coef, int exp){

    no *novo; //ponteiro n�
    novo = (no*)malloc(sizeof(no)); //aloca

    //se a aloca��o falhar
    if(novo == NULL){
        printf("ERRO: problemas com a aloca��o de memoria.\n");
        exit(1);
    }
    novo -> coef = coef;    //insere o valor no coeficiente
    novo -> exp = exp;  //insere o valor no expoente
    novo -> prox = NULL; //o ponteiro prox aponta para NULL
    return novo;
}

//fun��o para inserir e ordenar os n�s
no *insere(no *L, int coef, int exp){
    int soma = 0;
    no *aux = L;    //variavel auxiliar aponta para o primeiro elemento da lista, tamb�m pode ser v�zio
    no *ant = NULL; //variavel anterior ao auxiliar
    no *novo = criaNo(coef, exp); //ser� criado um n� com os coeficientes e os expoentes

    //se a lista estiver vazia, os elementos ficara na primeira posi��o
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
        ant = aux; //a variavel ant sempre � o anterior a aux
        aux = aux -> prox;  //posi��o do menor expoente ap�s o novo expoente
    }

    if(aux -> prox == NULL && aux -> exp > novo -> exp){
        aux -> prox = novo;
    }else{ //o novo expoente na posi��o da lista ordenada
        if(aux -> exp == novo -> exp){  //// se o novo expoente ja existir na lista, vai realizar a soma dos expoentes
            soma = aux -> coef + novo -> coef;
            aux -> coef = soma;
        }else{ //se o novo expoente n�o exista na lista, o novo aponta para o proximo n� do aux,e depois o ant aponta para o novo
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

    // atualizar L e liberar a mem�ria de aux
    while(aux != NULL){
        L = L->prox;
        free(aux);
        aux = L;
    }
  }
  return L;
}

//fun��o para somar a lista
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
    //a fun��o de inserir um a um polin�mio trata o se caso em que dois expoentes sejam iguais.logo, basta apenas adicionar todos os mon�mios dos dois polin�mios que o resultado ser� a soma dos dois
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

//fun��o para subtrair a lista
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

    /*a fun��o de inserir um a um polin�mio trata o se caso em que dois expoentes sejam iguais.logo,
    basta apenas adicionar todos os mon�mios dos dois polin�mios que o resultado ser� a soma dos dois
    s� que o segundo polinomio tera que ter os sinais trocados
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

//fun��o para multiplicar a lista
no  *mult(no *L1, no *L2){
    no *L3 = NULL, //variavel resultado
    //auxiliareas para percorrer as listas
    *aux1 = L1,
    *aux2 = L2;

    //se um dos conjuntos n�o tiver conte�do, ou seja, NULL
    // o resultado do produto ser� ZERO, ou NULL
    if(L1 == NULL || L2 == NULL){
        return L3 = NULL;
    }
    //se os polinomios tiver conteudo
    while(aux1 != NULL){
        while(aux2 != NULL){
        /*na multiplica��o, vai ser feito distributiva cada termo do primeiro polin�mio ser� multiplicado por todos os termos do segundo
        o resultado final ser� a soma de todas essas multiplica��es
        passando essas informa��es como par�metro da fun��o insere, ela far� o trabalho de ordenar e somar os termos com expoentes iguais*/
            L3 = insere(L3, aux1 -> coef * aux2 -> coef, aux1 -> exp + aux2 -> exp);
            aux2 = aux2 -> prox;
        }
        aux1 = aux1 -> prox;
        aux2 = L2;
    }

    return L3;
}

//fun��o imprimir lista
void imprime(no *L){
    no *aux = L; // aux aponta para o primeiro elemento da lista L

    while(aux != NULL){

        if(aux -> exp > 1)
            printf(" %dx^%d", aux -> coef, aux -> exp);

    //se o expoente � igual a 1,vai escreve lo
        if(aux -> exp == 1)
            printf(" %dx", aux -> coef);

    //quando o expoente � igual a zero, o resultado � 1 e n�s multiplicamos pelo coeficiente, sobrando apenas o coeficiente
        if(aux -> exp == 0)
            printf(" %d",aux -> coef);

            aux = aux -> prox; // percorrer toda a lista
    }
  printf("\n");
}

//fun��o preencher lista
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

//fun��o principal
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
      printf("                          [1] Adi��o                      \n");
      printf("                          [2] Subtra��o                     \n");
      printf("                          [3] Multiplica��o                          \n");
      printf("                          [ ] Sair do sistema                            \n");
      printf("\n");

      printf("Digite a opera��o desejada: ");
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

                printf("\nResultado da subtra��o:");
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

                printf("\nResultado da subtra��o:");
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
