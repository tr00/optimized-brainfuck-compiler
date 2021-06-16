#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1

enum CMDFLAGS
{
    FLAG_E,
    FLAG_V,
    FLAG_HELP
}

char *readFileVarLength(FILE *fp){
    printf("reading a file");
    char *line = NULL;
    size_t linebufflen = 0;
    ssize_t linelen;
    int ln = 0;
    
    int bufflen = 512;
    int currentPosition = 0; 
    char *buff = malloc(bufflen);
    
    while ((linelen = getline(&line, &linebufflen, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", linelen);
        //printf("allocated buffer of length %zu:\n", linebufflen);
        //printf("got the line: (%s)\n",line);
        
        if(currentPosition+linelen+2 > bufflen){
            bufflen *= 2;
            buff = realloc(buff,bufflen);
        }
        
        strncpy(&buff[currentPosition],line,linelen);
        currentPosition += linelen;
        //printf("current position: %d\n",currentPosition);
    }

    free(line);
    return buff;
}


int main(int argc, char **argv){
    int i;
    int flags;
    char * flagargs[32];
    char * options[10];
    for(i = 0; i < argc; i++){
        char * arg = argv[i];
        printf("%s\n",arg);
        if(arg[0] == '-'){
            if(arg[1] == '-'){
                if(strcmp("--help",arg) == 0){
                    flags |= 1<<FLAG_HELP;
                }
            }else{
                int j = 1;
                int i_increment = 0;
                while(arg[j] != NULL){
                    switch(arg[j]){
                        case 'e'://directly execute
                            i_increment = 1;
                            //because the next arg is determined to be the code. don't want it to parse as a cmd argument
                            if(i+1 >= argc){
                                fprintf(stderr, "Expected an inline code after -e option\n");
                                exit(FAILURE);
                            }
                            flags |= 1<<FLAG_E;
                            flagargs[FLAG_E] = argv[i+1];
                            break;
                        case 'v'://display version
                            flags |= 1<<FLAG_V;
                            break;
                        //if there were other single char flags, we can add them here
                    }
                    j++;
                }
                i += i_increment;
            }
        }else{
            
        }
    }
    
    
    //getting the file pointer
    FILE *fp;
    char *fname = "README.md";
    char *content;
    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "Error opening the file %s\n", fname);
        exit(FAILURE);
    }
    content = readFileVarLength(fp);
    printf("%s",content);
}