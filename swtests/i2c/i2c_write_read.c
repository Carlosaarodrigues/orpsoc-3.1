#include <or1k-support.h>
#include <spr-defs.h>
#include <stdio.h>


char* i2c_base = (char*)  0xa0000000;  //(start + write byte)
char* i2c_write = (char*) 0xa0000001;  //(write byte)
char* i2c_write_stop = (char*)  0xa0000002; //(write byte + stop)
char* i2c_read = (char*) 0xa0000003; //read byte
char* i2c_read_stop = (char*)  0xa0000004; //read byte + stop

unsigned char addr_slave = 0xA0;
unsigned char addr_mem = 0x00;
unsigned char data = 0xAA;

int main(void)
{
  int i=0;


//write
  *(i2c_base) = addr_slave	;
  *(i2c_write) = addr_mem; 
do{
  *(i2c_write) = data++;
   i++;
}while(i<=10);
  *(i2c_write_stop) = data++;

//read
i=0;
  *(i2c_base) = addr_slave; 

  *(i2c_write) = addr_mem;

  *(i2c_base) = addr_slave+1; 
do{
  *(i2c_read) = 0xFF;
  data =  *(i2c_read);
	i++;
}while(i<=10);

  *(i2c_read_stop) = 0x00;
  data =  *(i2c_read_stop);
  printf(" ,%#08X, \n",data);

  return 0;
}
