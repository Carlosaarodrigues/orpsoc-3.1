#include <or1k-support.h>
#include <spr-defs.h>
#include <stdio.h>



int* fifo0_write = (int*) 0xc0000000; //write byte

unsigned int data = 0xffffffff;	

int main(void)
{
  int i=0;

  do{
  *(fifo0_write) = data++;
  i++;
  }while (i<4);


//read
  i=0;

  do{
  data = *(fifo0_write);
  printf("%08X \n",data);
  i++;
  }while (i<4);


  return 0;
}
