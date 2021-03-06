#include<stdio.h>
#include<stdlib.h>

#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#endif

typedef struct Node{

    struct Node *next;
    struct Node *prev;
    int value;

}node;


typedef struct{

    node *headList;
    node *tailList;
    int size;

}lista;

lista makeList();

void addNode(lista *list, int value);

void printList(lista *list);

void freeList(lista *list);

void remove_position(lista *list, int val);

void remove_value(lista *list, int val);

void pause();

void addNodePosition(lista *list, int pos);

//=======================================================================

int main(void){

    int val;

    int op=0;

    lista list;

    list = makeList();

    while(op != 5){

        system(CLS);
        printf("----------- LISTA DINAMICA DUPLAMENTE ENCADEADA CIRCULAR -----------\n\n");
        printf("Lista:\n\n");
        printList(&list);
        printf("End. cabeca: %p\nEnd. cauda: %p\n\n",list.headList, list.tailList);
        printf("------------------------------------------------\n");
        printf("1 - Adicionar nodo.\n");
        printf("2 - Adicionar nodo por posicao.\n");
        printf("3 - Excluir nodo por valor.\n");
        printf("4 - Excluir nodo por posicao.\n");
        printf("5 - Sair.\n");

        scanf("%d", &op);

        switch(op){

            case 1:
                system(CLS);
                printf("Valor do nodo a ser adicionado: ");
                scanf("%d", &val);
                addNode(&list, val);
                pause();
                break;
            
            case 2:
                system(CLS);
                printf("Posicao do nodo a ser adicionado: ");
                scanf("%d", &val);
                addNodePosition(&list, val);
                pause();
                break;

            case 3:
                system(CLS);
                printf("Valor do nodo a ser excluido: ");
                scanf("%d", &val);
                remove_value(&list, val);
                pause();
                break;

            case 4:
                system(CLS);
                printf("Posicao do nodo a ser excluido: ");
                scanf("%d", &val);
                remove_position(&list, val);
                pause();
                break;
            
            case 5:
                system(CLS);
                printf("Limpando lista...\n");
                op = 5;
                freeList(&list); 
                pause();
                break;

            default: 
                system(CLS);
                printf("Opcao invalida!\n");
                pause();
                break;
        }
    }

    return 0;
}

//=======================================================================

lista makeList(){

    lista l;

    l.headList = NULL;
    l.tailList = NULL;
    l.size = 0;

    return l;
}

void addNode(lista *list, int value){

    node *tempNode=NULL;

    if(list->size == 0){

        tempNode = (node *) malloc(sizeof(node));

        if(tempNode != NULL){
            tempNode->next = tempNode;
            tempNode->prev = tempNode;
            tempNode->value = value;

            list->headList = list->tailList = tempNode;

            list->size++;

        }else{
            printf("Falha na alocacao de um nodo.\n");
            exit(1);
        }
        
    }else{

        tempNode = (node *) malloc(sizeof(node));

        if(tempNode != NULL){
            
            tempNode->prev = list->tailList;

            list->tailList->next = tempNode;

            list->tailList = tempNode;

            tempNode->next = list->headList;

            list->headList->prev = tempNode;

            tempNode->value = value;

            list->size++;

        }else{
            printf("Falha na alocacao de um nodo.\n");
            exit(1);
        } 

    }

    printf("Node %d adicionado.\n", list->size);

    return ;
}

void printList(lista *list){

    node *tempNode=NULL;

    int i;

    i = list->size;

    tempNode = list->headList;

    printf("%d elemento(s) na lista: <", list->size);
    while(i > 0){
        printf("%d, ", tempNode->value);

        tempNode = tempNode->next;

        i--;
    }
    printf(">\n\n");
    return ;
}

void freeList(lista *list){

    node *tempNode=NULL;

    
    while(list->size != 0){
        
        tempNode = list->headList;
        
        list->headList = list->headList->next;
        
        free(tempNode);

        list->size--;
    }


}

void remove_position(lista *list, int val){

    node *tempNode=NULL;

    char removed = 0;

    tempNode = list->headList;

    if(list->size > 0){ //executa caso a lista possuir n??s

        if(val == 1){

            if(list->headList->next != list->headList){ //verifica se o pr??ximo elemento ?? a cabe??a da lista, caso n??o for:

                list->headList = list->headList->next;

                list->tailList->next = list->headList;

                list->headList->prev = list->tailList;

                list->size--;

                val = tempNode->value;

                free(tempNode); //libera o elemento

                removed = 1;
                
            }
            else{ //caso n??o haja ele ir?? remover a cabe??a que tamb??m era a cauda e setar a lista.

                list->headList = NULL;
                list->tailList = NULL;
                list->size=0;

                val = tempNode->value;

                free(tempNode); //libera o elemento

                removed = 1;

            }

        }else if(val <= list->size && val > 1){

            while(tempNode->next != list->headList){
                
                tempNode = tempNode->next;

                val--;

                if(val == 1){

                    if(tempNode->next == list->headList){ //verifica se o elemento possui outro na frente ou se ?? o ??ltimo

                        tempNode->prev->next = list->headList; //seta o endere??o do elemento anterior a temp node para o proximo

                        list->headList->prev = tempNode->prev;

                        list->tailList = tempNode->prev;

                    }else{
                        tempNode->prev->next = tempNode->next;

                        tempNode->next->prev = tempNode->prev;
                    }

                    list->size--;

                    val = tempNode->value;

                    free(tempNode); //libera o elemento

                    removed = 1; //sinaliza que o valor foi removido

                    break;
                    
                }

            }

        }
    }

    if(removed == 1) printf("Valor %d removido da lista.\n\n", val);
    else printf("Posicao informada e invalida.\n\n");

    return ;
}

void remove_value(lista *list, int val){

    node *tempNode=NULL; //nodeAnterior servir?? para armazenar o endere??o do n?? anterior ao que estiver sendo avaliado para o caso dele ser removido.
    
    char removed = 0;

    tempNode = list->headList; //endere??o tempor??rio recebe o endere??o da cabe??a da lista

    do{
        
        if(tempNode->value == val){ 
            
            if(tempNode == list->headList){ //verifica se o elemento que ser?? removido ?? a cabe??a da lista

                if(list->headList->next != list->headList){ //verifica se o pr??ximo elemento ?? a cabe??a da lista

                    list->headList = list->headList->next;

                    list->tailList->next = list->headList;

                    list->headList->prev = list->tailList;

                    list->size--;

                    val = tempNode->value;

                    free(tempNode); //libera o elemento

                    removed = 1;

                    tempNode = list->headList; //re-seta a tempor??ria para continuar procurando na lista por mais elementos repetidos

                    continue;
                
                }else{ //caso n??o haja ele ir?? remover a cabe??a que tamb??m era a cauda e setar a lista.

                    list->headList = NULL;
                    list->tailList = NULL;
                    list->size=0;

                    val = tempNode->value;

                    free(tempNode); //libera o elemento

                    removed = 1;

                    break;
                }

            }else{ //caso o elemento n??o seja a cabe??a
                
                if(tempNode->next == list->headList){ //verifica se o elemento possui outro na frente ou se ?? o ??ltimo

                    tempNode->prev->next = list->headList; //seta o endere??o do elemento anterior a temp node para o proximo

                    list->headList->prev = tempNode->prev;

                    list->tailList = tempNode->prev;

                }else{
                    tempNode->prev->next = tempNode->next;

                    tempNode->next->prev = tempNode->prev;
                } 

                removed = 1;
            
                list->size--;

                free(tempNode); //libera o elemento

                tempNode = list->headList; //re-seta a tempor??ria para continuar procurando na lista por mais elementos repetidos

                continue;

            }
        }

        if(tempNode->next == list->headList) break;

        tempNode = tempNode->next;

    }while(1);

    if(removed == 1) printf("Valor %d removido da lista.\n\n", val);
    else printf("Valor nao encontrado na lista.\n\n");

    return;
}

void pause(){

    setbuf(stdin, NULL);
    printf("\nPressione ENTER para continuar..."); //n??o pode ter "lixo" no stdin para ela funcionar
    getchar();

    return;
}

void addNodePosition(lista *list, int pos){

    node *tempNode=NULL, *tempNodeWhile=NULL;

    int i; //contadores

    int value;

    i=pos;

    system(CLS);

    if(list->size == 0 && pos == 1){ //verifica se a lista est?? vazia e a posi????o desejada ?? a primeira. Neste caso o nodo ser?? adicionado como primeiro elemento

        printf("Qual o valor que deseja adicionar na posicao %d: ", pos);

        scanf("%d", &value);

        tempNode = (node *) malloc(sizeof(node));

        if(tempNode != NULL){

            tempNode->next = tempNode;
            tempNode->prev = tempNode;
            tempNode->value = value;

            list->headList = list->tailList = tempNode;

        }else{
            printf("Falha na alocacao de um nodo.\n");
            exit(1);
        }

        list->size++;


    }else if(pos == list->size && list->size > 1){ //verifica se a posi????o desejada ?? a ??ltima. Neste caso o nodo movera o ??ltimo nodo para frente

        printf("Qual o valor que deseja adicionar na posicao %d: ", pos);

        scanf("%d", &value);

        tempNode = (node *) malloc(sizeof(node));

        if(tempNode != NULL){
            
            tempNode->value = value;

            list->tailList->prev->next = tempNode;
            tempNode->prev = list->tailList->prev;
            tempNode->next = list->tailList;
            list->tailList->prev = tempNode;

        }else{
            printf("Falha na alocacao de um nodo.\n");
            exit(1);
        }

        list->size++;


    }else if(pos == list->size+1){ //verifica a posi????o desejada ?? a seguinte da ??ltima. Neste caso o nodo ser?? adicionado como ??ltimo elemento

        printf("Qual o valor que deseja adicionar na posicao %d: ", pos);

        scanf("%d", &value);

        tempNode = (node *) malloc(sizeof(node));

        if(tempNode != NULL){

            tempNode->value = value;

            list->tailList->next = tempNode;

            tempNode->prev = list->tailList;

            tempNode->next = list->headList;

            list->headList->prev = tempNode;

            list->tailList = tempNode;

        }else{
            printf("Falha na alocacao de um nodo.\n");
            exit(1);
        }

        list->size++;

    }else if(pos <= list->size && pos > 0){ //caso nenhuma condi????o acima for verdadeira ser?? feito a busca at?? a posi????o do elemento e ser?? inclu??do um no seu lugar, movendo o 
    // elemento em quest??o para frente. Ex: se a posi????o selecionada for 4, e existir um elemento nesta posi????o, um novo elemento ser?? colocado no lugar do elemento anterior que ser?? movido para frente.

        printf("Qual o valor que deseja adicionar na posicao %d: ", pos);

        scanf("%d", &value);

        tempNodeWhile = list->headList;

        do{

            if(i==1){

                tempNode = (node *) malloc(sizeof(node));
                
                if(tempNode != NULL){
                    
                    tempNode->value = value;

                    if(tempNodeWhile == list->headList){ //se a posi????o for o 1 elemento da lista

                        list->headList = tempNode; //cabe??a da lista ?? re-setada

                        tempNode->next = tempNodeWhile; //novo valor tem seu "next" setado para o antigo n?? da cabe??a

                        tempNodeWhile->prev = tempNode; //a antiga cabe??a tem seu "prev" setado para o novo valor

                        tempNode->prev = list->tailList; // como ?? uma lista circular o tempNode->prev recebe o endere??o da cauda

                        list->tailList->next = tempNode; // e a cauda recebe seu endere??o

                    }else{

                        tempNodeWhile->prev->next = tempNode;

                        tempNode->next = tempNodeWhile;

                    }

                    list->size++;

                    break;

                }else{
                    printf("Falha na alocacao de um nodo.\n");
                    exit(1);
                }
            }

            i--;
            
            tempNodeWhile = tempNodeWhile->next;

        }while(i!=0);

    }else printf("Posicao informada invalida!\n\n");

    return;
}
