#include<stdio.h>
#include <stdlib.h>

typedef struct arvore{
	int valor;
	struct arvore *dir;
	struct arvore *esq;
}arvore;

void cria (arvore *inicio, int index){

	index++;
	inicio->valor = index;
	if(index <= 3){

		inicio->dir = (arvore*) malloc (sizeof(arvore));

		inicio->dir->valor = 0;
		inicio->dir->dir = NULL;
		inicio->dir->esq = NULL;

		cria(inicio->dir,index);

		inicio->esq = (arvore*) malloc (sizeof(arvore));

		inicio->esq->valor = 0;
		inicio->esq->dir = NULL;
		inicio->esq->esq = NULL;

		cria(inicio->esq,index);

	}

	printf("%d --> valor %d, dir %d,  esq %d \n",inicio,inicio->valor,inicio->dir,inicio->esq);
}


void liberta(arvore *inicio){
	
	if(inicio->dir != NULL){
		liberta(inicio->dir);
	}

	if(inicio->esq != NULL){
		liberta(inicio->esq);
	}

	free(inicio);

}

main()
{

arvore *inicio;

inicio = (arvore*) malloc (sizeof(arvore));
inicio->dir = NULL;
inicio->esq = NULL;

inicio->valor = 0;

cria(inicio,0);

liberta(inicio);


}
