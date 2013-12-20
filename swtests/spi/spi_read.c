#include <or1k-support.h>
#include <spr-defs.h>


char* spi_base = (char*)  0xb0000000; // config Control Register
char* spi_base2 = (char*)  0xb0000003; // config Extension Register
char* spi_write = (char*) 0xb0000002; //write byte
char* spi_read = (char*) 0xb0000002; //read byte
char* spi_slave = (char*)  0xb0000004; //select slave

unsigned char read = 0x03;
unsigned char addr1 = 0xA0;
unsigned char addr2 = 0xA0;
unsigned char addr3 = 0xA0;

int main(void)
{
  int i=0;
  char data;


  *(spi_base) = 0xFF;
  *(spi_base2) = 0xFF; 
  *(spi_slave) = 0x01; 

  *(spi_write) = read;
  *(spi_write) = addr1;
  *(spi_write) = addr2;
  *(spi_write) = addr3;
  
  data=*(spi_read);
  data=*(spi_read);
  data=*(spi_read);


  *(spi_slave) = 0x00; 

  return 0;
}
