#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "all.h"

char *readFileVarLength(FILE *fp){
    printf("reading a file\n");
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
    for(i = 1; i < argc; i++){
        char * arg = argv[i];
        printf("(for debug only) %s\n",arg);
        if(arg[0] == '-'){
            if(arg[1] == '-'){
                if(strcmp("--help",arg) == 0){
                    flags |= 1<<FLAG_HELP;
                }else{
                    fprintf(stderr, "Unknown option detected %s\n",arg);
                    flags |= 1<<FLAG_HELP;
                }
            }else{
                int j = 1;
                int i_increment = 0;
                while(arg[j] != '\0'){
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
                            flags |= 1<<FLAG_VERSION;
                            break;
                        //if there were other single char flags, we can add them here
                        default:
                            fprintf(stderr, "Unknown option detected %s\n",arg);
                            flags |= 1<<FLAG_HELP;
                    }
                    j++;
                }
                i += i_increment;
            }
        }else{
            flags |= 1<<FLAG_FILE;
            flagargs[FLAG_FILE] = arg;
        }
    }
    
    char *code;
    printf("(for debug only) flag integer: %d\n",flags);
    
    if(flags & (1<<FLAG_HELP)){
        printf("displaying some help\n");
        exit(SUCCESS);
    }else if(flags & (1<<FLAG_VERSION)){
        printf("displaying some version info\n");
        exit(SUCCESS);
    }else if(flags & (1<<FLAG_E)){
        code = flagargs[FLAG_E];
    }else if(flags & (1<<FLAG_FILE)){
        FILE *fp;
        char *fname = flagargs[FLAG_FILE];
        if ((fp = fopen(fname, "r")) == NULL) {
            fprintf(stderr, "Error opening the file %s\n", fname);
            exit(FAILURE);
        }
        code = readFileVarLength(fp);
    }else{//get the contents from stdin
        printf("adsfasdfsa\n");
        code = readFileVarLength(stdin);
    }
    
    printf("(for debug only) got thecode: %s\n",code);
    exit(SUCCESS);
}