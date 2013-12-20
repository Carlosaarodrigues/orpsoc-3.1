#include<stdio.h>

int chama (int *idade){

	int E=23;

	*idade=E;
return(E);
}


main(){
	int idade;
	int idade2;

	idade2=chama(&idade);

	printf("%d, %d \n", idade,idade2);


}
