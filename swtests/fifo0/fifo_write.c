#include <or1k-support.h>
#include <spr-defs.h>


int* fifo0_write = (int*) 0xc0000000; //write byte

unsigned int data = 0x00000000;	

int main(void)
{
  int i=0;

  do{
  *(fifo0_write) = data++;
  i++;
  }while (i<4);


  return 0;
}
