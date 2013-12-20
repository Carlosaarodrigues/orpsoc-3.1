#include <or1k-support.h>
#include <spr-defs.h>
#include <stdio.h>


char* spi_base = (char*)  0xb0000000; // config Control Register
char* spi_base2 = (char*)  0xb0000003; // config Extension Register
char* spi_write = (char*) 0xb0000002; //write byte
char* spi_read = (char*) 0xb0000002; //read byte
char* spi_slave = (char*)  0xb0000004; //select slave

unsigned char read = 0x03;
unsigned char enable_write = 0x06;
unsigned char disable_write = 0x04;
unsigned char page_programming = 0x02;
unsigned char addr1 = 0xA0;
unsigned char addr2 = 0xB0;
unsigned char addr3 = 0x00;
unsigned char data = 0x00;	
unsigned char data1;
unsigned char data2;
unsigned char data3;

int main(void)
{
  int i=0;

//write
  *(spi_base) = 0xFF;
  *(spi_base2) = 0xFF; 

  *(spi_slave) = 0x01; //select slave

  *(spi_write) = enable_write;

  *(spi_slave) = 0x00; 

  *(spi_slave) = 0x01; //select slave

  *(spi_write) = page_programming; // write 256 bytes 

  *(spi_write) = addr1;
  *(spi_write) = addr2;
  *(spi_write) = addr3;
  
  do{
  *(spi_write) = data++;
  i++;
  }while (i<256);

  *(spi_write) = disable_write;

  *(spi_slave) = 0x00;

 

//read
  i=0;
  *(spi_slave) = 0x01; 

  *(spi_write) = read;
  *(spi_write) = addr1;
  *(spi_write) = addr2;
  *(spi_write) = addr3;
  
  do{
  data=*(spi_read);
 // printf(" ,%#02X, \n",data);
  i++;
  }while (i<256);
  *(spi_slave) = 0x00; 


  return 0;
}
