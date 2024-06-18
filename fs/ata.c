#include "ata.h"
#include "heap.h"

//
// LEITURA DE DISCO
//

// Na primeira parte de read_disk_chs, enviam-se os valores necessários
// para as portas apropriadas.
// Na porta BASE_PORT + 6 definimos que a unidade é 0 e a cabeça é 0,
// também define que o modo de endereçamento a ser usado é o CHS. 
// Na porta BASE_PORT + 2 está a definição de leitura de disco,
// a função read_disk_chs lê 512 bytes do disco a cada chamada.
// Na porta BASE_PORT + 3, é definido  o número do setor a ser lido, que
// no caso é o parâmetro passado na função.
// Em BASE_PORT + 5 e BASE_PORT + 4 é passado qual cilindo a ser lido.
//
// Finalmente é feita uma solicitação de leitura emitindo 0x20 para
// BASE_PORT + 7
void *read_disk_chs(int sector)
{
  // Part 1
  dev_write(BASE_PORT + 6, 0x0a0);
  dev_write(BASE_PORT + 2, 1);
  dev_write(BASE_PORT + 4, 0);
  dev_write(BASE_PORT + 5, 0);
  dev_write(BASE_PORT + 7, 0x20);

  // ...
  
  // Part 2
  
  wait_drive_until_ready();

  // ...
  
  // Part 3

  short *buffer = kalloc(SECTOR_SIZE);

  for(int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++) {
    buffer[currByte] = dev_read(BASE_PORT);
  }

  return buffer;
}

// Garante que o código não seja executado até que o dispositivo
// ATA termine seu trabalho.
// dev_read(BASE_PORT + 7) lê o valor da porta BASE_PORT + 7 que
// contém o status do dispositivo.
void wait_drive_until_ready()
{
  int status = 0;

  do {
    status = dev_read(BASE_PORT + 7);
  } while ((status ^ 0x80) == 128);
}

void *read_disk(int address)
{
  dev_write(BASE_PORT + 6, (0x0e0 | ((address & 0x0F000000 ) > 24) ));
  dev_write(BASE_PORT + 2, 1);
  dev_write(BASE_PORT + 3, address & 0x000000FF);
  dev_write(BASE_PORT + 4, (address & 0x0000FF00) >> 8);
  dev_write(BASE_PORT + 5, (address & 0x00FF0000) >> 16);
  dev_write(BASE_PORT + 7, 0x20);

  short *buffer = kalloc(SECTOR_SIZE);
  for(int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++) {
  	buffer[currByte] = dev_read(BASE_PORT);
  }

}


// GRAVAÇÃO EM DISCO
void write_disk_chs(int sector, short *buffer)
{
  dev_write(BASE_PORT + 6, 0x0a0);
  dev_write(BASE_PORT + 2, 1);
  dev_write(BASE_PORT + 3, sector);
  dev_write(BASE_PORT + 4, 0);
  dev_write(BASE_PORT + 5, 0);
  dev_write(BASE_PORT + 7, 0x30);

  wait_drive_until_ready();

  for(int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++) {
    dev_write_word(BASE_PORT, buffer[currByte]);
  }
  wait_drive_until_ready();
}

void write_disk(int address, short *buffer)
{
  dev_write(BASE_PORT + 6, (0x0e0 | ((address & 0x0F000000) >> 24)));
  dev_write(BASE_PORT + 2, 1);
  dev_write(BASE_PORT + 3, address & 0x000000FF);
  dev_write(BASE_PORT + 4, (address & 0x0000FF00) >> 8);
  dev_write(BASE_PORT + 5, (address & 0x00FF0000) >> 16);

  wait_drive_until_ready();

  for(int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++)
  {
    dev_write_word(BASE_PORT, buffer[currByte]);
  }
}
