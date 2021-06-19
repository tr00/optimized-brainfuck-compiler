#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "all.h"

char *readFileVarLength(FILE *fp)
{
    printf("reading a file\n");
    char *line = NULL;
    size_t linebufflen = 0;
    int linelen;
    int ln = 0;

    int bufflen = 512;
    int currentPosition = 0;
    char *buff = safe_malloc(bufflen);

    while ((linelen = getline(&line, &linebufflen, fp)) != EOF)
    {
        //printf("Retrieved line of length %zu:\n", linelen);
        //printf("allocated buffer of length %zu:\n", linebufflen);
        //printf("got the line: (%s)\n",line);

        if (currentPosition + linelen + 2 > bufflen)
        {
            bufflen *= 2;
            buff = safe_realloc(buff, bufflen);
        }

        strncpy(&buff[currentPosition], line, linelen);
        currentPosition += linelen;
        //printf("current position: %d\n",currentPosition);
    }

    // free(line);
    return buff;
}

int parseArgs(int argc, char **argv, char **flagargs)
{
    int flags = 0;
    int i;
    for (i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        printf("(for debug only) %s\n", arg);
        if (arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                if (strcmp("--help", arg) == 0)
                {
                    flags |= 1 << FLAG_HELP;
                }
                else
                {
                    fprintf(stderr, "Unknown option detected %s\n", arg);
                    flags |= 1 << FLAG_HELP;
                }
            }
            else
            {
                int j = 1;
                int i_increment = 0;
                while (arg[j] != '\0')
                {
                    switch (arg[j])
                    {
                    case 'e': //directly execute
                        i_increment = 1;
                        //because the next arg is determined to be the code. don't want it to parse as a cmd argument
                        if (i + 1 >= argc)
                        {
                            fprintf(stderr, "Expected an inline code after -e option\n");
                            exit(FAILURE);
                        }
                        flags |= 1 << FLAG_EXEC;
                        flagargs[FLAG_EXEC] = argv[i + 1];
                        break;
                    case 'v': //display version
                        flags |= 1 << FLAG_VERSION;
                        break;
                    //if there were other single char flags, we can add them here
                    default:
                        fprintf(stderr, "Unknown option detected %s\n", arg);
                        flags |= 1 << FLAG_HELP;
                    }
                    j++;
                }
                i += i_increment;
            }
        }
        else
        {
            flags |= 1 << FLAG_FILE;
            flagargs[FLAG_FILE] = arg;
        }
    }
    return flags;
}

int main(int argc, char **argv)
{
    printf("test%s",TEST);
    int i;
    char *flagargs[32];
    int flags;

    // had to add this bc otherwise Seg Fault
    for(i = 0; i < 32; i++)
        flagargs[i] = NULL;
    
    flags = parseArgs(argc, argv, flagargs);

    // for debug only, looking inside the flagargs.
    printf("(for debug only) flag integer: %d\n", flags);
    printf("(for debug only) printing the contents of flagargs\n");
    for (i = 0; i < 5; i++)
    {
        printf("(for debug only) %d: ", i);
        if (flagargs[i] != NULL)
        {
            printf("%s", flagargs[i]);
        }
        else
        {
            printf("NULL");
        }
        printf("\n");
    }

    char *code;
    if (flags & (1 << FLAG_HELP))
    {
        printf("displaying some help\n");
        exit(SUCCESS);
    }
    else if (flags & (1 << FLAG_VERSION))
    {
        printf("displaying some version info\n");
        exit(SUCCESS);
    }
    else if (flags & (1 << FLAG_EXEC))
    {
        code = flagargs[FLAG_EXEC];
    }
    else if (flags & (1 << FLAG_FILE))
    {
        FILE *fp;
        char *fname = flagargs[FLAG_FILE];
        if ((fp = fopen(fname, "r")) == NULL)
        {
            fprintf(stderr, "Error opening the file %s\n", fname);
            exit(FAILURE);
        }
        code = readFileVarLength(fp);
    }
    else
    { //get the contents from stdin
        code = readFileVarLength(stdin);
    }

    printf("(for debug only) got the code: %s\n", code);

    size_t length = strlen(code);
    BasicBlock *block = scan(code, length);
    normalize(block);

    printBB(stdout, block);

    exit(SUCCESS);
}
