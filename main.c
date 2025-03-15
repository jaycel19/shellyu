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

const SLU_TOK_BUFFSIZE = 64;
const SLU_TOK_DELIM = "\t\r\n\a";

char **slu_split_line(char *line) {
  // declare bufsize
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  // allocate mem for **tokens [], [], []
  char **tokens = malloc(bufsize * sizeof(char*));
  // pointer variable to hold token
  char *token;
  
  // display error mem allocation
  if (!tokens) {
    fprintf(stderr, "shellyu: Mem allocation error \n);
    exit(EXIT_FAILURE);
  }
  // tokenize words by spaces base on the delim and return pointer to the next token found in the string
  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    // assign each token pointer to tokens [token], [token], [token]
    tokens[position] = token;
    position++;
  

    // realloc mem if not enough
    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      // display error mem alloc
      if (!tokens) {
        fprintf(stderr, "shellyu: Mem allocation error \n);
        exit(EXIT_FAILURE);
      }
    }
    // to rerun strtok and remember it state
    token = strtok(NULL, LSH_TOK_DELIM);
  }
  // and put NULL on the rest of the mem space
  tokens[position] = NULL;
  // finally return tokens
  return tokens;
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
