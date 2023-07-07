#ifndef AUXILLARY_H
#define AUXILLARY_H

int read_twobytes(int fd) {
  char  bytes[2];
  ssize_t dump = read(fd, &bytes, 2);
  
  unsigned int value = (unsigned char)bytes[0] << 8 | (unsigned char)bytes[1];

  return  value;
}

void write_twobytes(int fd, int input) {
  unsigned int bytes = input;
  
  unsigned char byte1 = (bytes >> 8) & 0xFF;
  unsigned char byte2 = bytes & 0xFF;

  ssize_t dump = write(fd, &byte1, 1);
  dump = write(fd, &byte2, 1);
}


int readbinary(unsigned char* buffer, unsigned char* data, size_t size, int l) {

  //iterate through bytes read
  for (int i = 0; i < (int) size; i++) {
    
    unsigned char byte = buffer[i];

    //iterate through bits in the read byte
    //and write them into data memory
    
    for (int j = 7; j >= 0; j--) {
      unsigned char bit = (byte >> j) & 1;
      data[l] = bit;
      l++;
    }
  }
  return l;
}

int num_chunks(int file_size, double chunk_size) {
  double num_chunks_float = (double) file_size / chunk_size;
  int num_chunks = (int)ceil(num_chunks_float);

  if (num_chunks_float - (double)num_chunks > 0) {
    num_chunks++;
  }
  return num_chunks;
}

void setbit(unsigned char* byte, int position) {
  *byte |= (1 << position);
}

void clearbit(unsigned char* byte, int position) {
  *byte &= (1 << position);
}

void printdata(char* buffer, ssize_t bytesread) {
 printf("\n");
 for (int i = 0; i < bytesread; i++) {
   printf("%3x ", buffer[i]);
   if ((i + 1) % 10 == 0) {
     printf("\n");
   }
 }
}

void compress(unsigned char* buffer, size_t bytes_read, int fd_write, unsigned char* data) {
  
    int i = 0;
    int l = 0;

    int sub_chunks = num_chunks(bytes_read, 8.0);
    
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

#endif /* AUXILLARY_H */
