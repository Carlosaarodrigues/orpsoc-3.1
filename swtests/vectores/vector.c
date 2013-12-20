#include<stdio.h>
#include <string.h>

main()
{

char nome[8] = "Carlos"; 

char outro[10] = "Antonio";
    printf("%s \n", nome);

strcpy(nome,outro);

    printf("%s \n", nome);

}
