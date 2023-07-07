#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <math.h>
#include "auxillary.h"

#define MAX_MEM 100000000

int main(int argc, char *argv[]) {

  //Declare Variables
  int fd_read, fd_write, chunks, sub_chunks;
  int i = 0, l = 0, j = 0;
  unsigned int x, y, z;
  ssize_t bytes_read, file_size;
  unsigned char *buffer, *data;

  //INPUT ERROR HANDLING HERE
 
  fd_read = open(argv[2], O_RDONLY);
  fd_write = open(argv[3],  O_WRONLY | O_CREAT);

  //Check file size to to calculate chunks to read into allocated data

  file_size = lseek(fd_read, 0, SEEK_END);
  lseek(fd_read, -1*file_size, SEEK_END);

  chunks = num_chunks(file_size-6, MAX_MEM);

  buffer = (unsigned char *)malloc(MAX_MEM);
  data = (unsigned char *)malloc(MAX_MEM);
  
  //read the dimensions + other metadata and write it into the new file
  
  z = read_twobytes(fd_read);
  y = read_twobytes(fd_read);
  x = read_twobytes(fd_read);
  
  printf("%u x %u x %u\n", x, y, z);
  printf("data size: %d\n", file_size);
  printf("%d chunks needed.\n", chunks);

  //Iterate through all data and compress it while writing

  write_twobytes(fd_write, z);
  write_twobytes(fd_write, y);
  write_twobytes(fd_write, x);
  
  
    
//Compress data
  if(strcmp(argv[1], "-c") == 0) {

    while((bytes_read = read(fd_read, buffer, MAX_MEM)) > 0) {

      sub_chunks = num_chunks(bytes_read, 8.0);
      
      data = realloc(data, sub_chunks);
      memset(data, 0, sub_chunks);
      
      i = 0;
      l = 0;
      
      while(i <= (int) (bytes_read)) {
	
	for(int j = 7; j >= 0; j--) {
	  if(buffer[i] == 1) {
	    setbit(&data[l], j);
	  }
	  i++;
	}
	l++;
      }
      write(fd_write, data, sub_chunks);  
    }
  }
      
//Decompress data
  
    if(strcmp(argv[1], "-d") == 0) {

      unsigned char byte, bit;
      
      while(bytes_read = read(fd_read, buffer, MAX_MEM)) {

	unsigned char* new_data = realloc(data, bytes_read * 8);
	data = new_data;
      
	for (int i = 0; i < (int) bytes_read; i++) {
    
	  byte = buffer[i];

	  //iterate through bits in the read byte
	  //and write them into data memory
    
	  for (int j = 7; j >= 0; j--) {
	    bit = (byte >> j) & 1;
	    //printf("%d\n", bit);
	    data[l] = bit;
	    l++;
	  }
	}
	printf("writing %d bytes\n", bytes_read * 8);
	write(fd_write, data, bytes_read * 8);
	memset(buffer, 0, MAX_MEM);
      }
    }
    
    //printf("\n");
    close(fd_read);
    close(fd_write);

    //free(buffer);
    //free(data);
}
