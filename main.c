#include <stdio.h>


const SLU_RL_BUFFSIZE = 1024;

char *slu_read_line() {
  // initialize buffer size
  int bufsize = SLU_RL_BUFFSIZE;
  // start at position zero
  int position = 0;
  // allocate mem
  char *buffer = malloc(sizeof(char) * bufsize);
  // variable that would hold chars
  int c;
  
  // check if there is a problem with allocating mem
  if (!buffer) {
    fprintf(stderr,  "shellyu: Mem allocation error \n");
    exit(EXIT_FAILURE);
  }
  
  // read char
  while (1) {
    c = getchar();
    
    // check if end of file if it is return null else return the char
    if(c == EOF || c == "\n") {
      buffer[position] = "\0";
      return buffer;
    }else {
      buffer[position] = c;
    }
    position++;

    // reallocate mem if the char size exceeds the constant SLU_RL_BUFFSIZE
    if(position >= bufsize) {
      bufsize += SLU_RL_BUFFSIZE;
      buffer = realloc(buffer, bufsize);
      // check again if there is allocation error
      if (!buffer) {
        fprintf(stderr,  "shellyu: Mem allocation error \n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void slu_loop(){
  char *line;
  char **args;
  int status;
  
  do{
    printf("shellyu> ");
    line = slu_read_line();
    args = slu_split_line(line);
    status = slu_execute(args);

    free(line);
    free(args);
  } while (status);

}


int main (int argc, char **argv) {

  slu_loop();
  
  return EXIT_SUCCESS;
}
