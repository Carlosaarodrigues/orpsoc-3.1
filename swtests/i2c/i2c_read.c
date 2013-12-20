#include <or1k-support.h>
#include <spr-defs.h>


char* i2c_base = (char*)  0xa0000000; //start + write byte
char* i2c_write = (char*) 0xa0000001; //write byte
char* i2c_read = (char*) 0xa0000003; //read byte
char* i2c_read_stop = (char*)  0xa0000004; //read byte + stop

unsigned char addr = 0xA0;
unsigned char dat = 0xAA;

int main(void)
{
  int i=0;


  *(i2c_base) = addr; 

  *(i2c_write) = 0XAA;

  *(i2c_base) = addr+1; 
do{
  *(i2c_read) = 0xFF;
  dat =  *(i2c_read);
	i++;
}while(i<=10);
  *(i2c_read_stop) = 0x00;
  dat =  *(i2c_read_stop);

  return 0;
}
