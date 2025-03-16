#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define SLU_RL_BUFFSIZE 1024

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
    if(c == EOF || c == '\n') {
      buffer[position] = '\0';
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

#define SLU_TOK_BUFFSIZE 64
#define SLU_TOK_DELIM " \t\r\n\a"

char **slu_split_line(char *line) {
  // declare bufsize
  int bufsize = SLU_TOK_BUFFSIZE;
  int position = 0;
  // allocate mem for **tokens [], [], []
  char **tokens = malloc(bufsize * sizeof(char*));
  // pointer variable to hold token
  char *token;
  
  // display error mem allocation
  if (!tokens) {
    fprintf(stderr, "shellyu: Mem allocation error \n");
    exit(EXIT_FAILURE);
  }
  // tokenize words by spaces base on the delim and return pointer to the next token found in the string
  token = strtok(line, SLU_TOK_DELIM);
  while (token != NULL) {
    // assign each token pointer to tokens [token], [token], [token]
    tokens[position] = token;
    position++;
  

    // realloc mem if not enough
    if (position >= bufsize) {
      bufsize += SLU_TOK_BUFFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      // display error mem alloc
      if (!tokens) {
        fprintf(stderr, "shellyu: Mem allocation error \n");
        exit(EXIT_FAILURE);
      }
    }
    // to rerun strtok and remember it state
    token = strtok(NULL, SLU_TOK_DELIM);
  }
  // and put NULL on the rest of the mem space
  tokens[position] = NULL;
  // finally return tokens
  return tokens;
}

int slu_launch(char **args) {
  // variable for process id
  pid_t pid, wpid;
  // variable that holds status
  int status;
  
  // copy the process of the parents memory to child
  pid = fork();
  if (pid == 0) {
    // execute program in the child process
    if (execvp(args[0], args) == -1) {
      // if exec fail error
      perror("slu");
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    // if fork fails return error
    perror("slu");
  } else {
    // wait for change of state and status if success
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
      // loop until change of status either exit or other status codes
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

// built in cmd functions
int slu_cd(char **args);
int slu_help(char **args);
int slu_exit(char **args);

// built in cmd str
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

// assign each function into the corresponding builtin cmd
int (*builtin_func[]) (char **) = {
  &slu_cd,
  &slu_help,
  &slu_exit
};

// get the number of built in cmd
int slu_num_builtins() {
  return sizeof(builtin_str) / sizeof(char*);
}

// cd cmd function
int slu_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if(chdir(args[1]) != 0) {
      perror("slu");
    }
  }
  return 1;
}

// help cmd function
int slu_help(char **args) {
  int i;

  printf("Jhun Chester Lalongisip's SLU \n");
  printf("A custom shell for you \n");
  printf("Type program names and arguments, and hit enter \n");
  printf("The following are built in: \n");

  for (i = 0; i < slu_num_builtins(); i++) {
      printf(" %s\n", builtin_str[i]);
  }
  printf("Use the man command for information on other programs. \n");
  return 1;
}

// exit cmd function
int slu_exit(char **args) {
  return 0;
}

int slu_execute(char **args) {
  int i;
  if (args[0] == NULL) {
    return 1;
  }
  // looop through each cmd and comparing if it exists in the args pos 0
  for (i = 0; i < slu_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  // else launch the program
  return slu_launch(args);
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
