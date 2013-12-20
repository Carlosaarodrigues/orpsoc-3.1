#include <or1k-support.h>
#include <spr-defs.h>


char* i2c_base = (char*)  0xa0000000;  //(start + write byte)
char* i2c_write = (char*) 0xa0000001;  //(write byte)
char* i2c_write_stop = (char*)  0xa0000002; //(write byte + stop)

unsigned char addr = 0xA0;
unsigned char dat = 0xA1;

int main(void)
{
  int i=0;

  *(i2c_base) = addr	; 
do{
  *(i2c_write) = dat++;
   i++;
}while(i<=10);
  *(i2c_write_stop) = dat++;

  return 0;
}
