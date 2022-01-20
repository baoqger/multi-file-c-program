#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "main.h"
#include "mmencode.h"
#include "mmdecode.h"

extern char *optarg;
extern int optopt;
extern int opterr;
extern int opterr; // getopt-core.h

#define OPTARG "i:o:hV" // need ":" or not


#define MM_OP_INVALID 0
#define MM_OP_ENCODE 1
#define MM_OP_DECODE 2

int main(int argc, char *argv[]) {
    options_t options = OPTIONS_INT; // struct initialization
    int retval = -1;
    int codec_op = MM_OP_INVALID;
    int opt; 

    opterr = 0;
    
    if((codec_op = pick_operation(argv[0])) == MM_OP_INVALID) { // this is a good code style
        errno = EINVAL;
        perror("meow/unmeow argv[0] unrecognized!");
        exit(-1);
    } 

    while((opt = getopt(argc, argv, OPTARG)) != EOF) {
        switch (opt) {
            case 'i':
                /* explicitly as stdin */
                if (!(options.in_stream = fopen(optarg, "r"))) { // optarg current option value
                    perror("Error opening input stream");
                    exit(-1);
                }
                break;
            case 'o':
                /* explicityly as stdout */
                if (!(options.out_stream = fopen(optarg, "w"))) {
                    perror("Error opening output stream");
                    exit(-1);
                }
                break;
            case 'V':
                printf("%s version %s\n", basename(argv[0]), MEOWMEOW_VERSION);
                exit(0);
                break;
            case '?':
            case 'h':
            default:
                usage(basename(argv[0]), optopt); // optopt current option name
                break;
        }

        switch(codec_op) {
            case MM_OP_ENCODE:
                retval = mm_encode(options.in_stream, options.out_stream);
                break;
            case MM_OP_DECODE:
                retval = mm_decode(options.in_stream, options.out_stream);
                break;
            default:
                retval = -1;
                errno = EINVAL;
                fprintf(stderr, "impossible state: %s codec=%d\n", argv[0], codec_op);
                break;
        }

        if (retval < 0) {
            perror("codec failed");
        }

        return retval;

    }

}

int usage(char *argv0, int opt) {
    if (!argv0) {
        errno = EINVAL;
        perror("main:usage called with NULL argv[0]");
        exit(-1);
    }

    fprintf(stderr, "usage: %s [-i input] [-o output] [-V]\n", basename(argv0));

    if (opt != '?') 
        fprintf(stderr, "unknown option: \"%c\"\n", opt);
    exit(-1);
}


int pick_operation(char *argv0) {
    char *name;

    if (!argv0) {
        errno = EINVAL; // in errno.h
        return MM_OP_INVALID;
    }

    name = basename(argv0); // in libgen.h

    if (strncmp(name, CMD_MEOW, strlen(CMD_MEOW)) == 0) 
        return MM_OP_ENCODE; // encode command

    if (strncmp(name, CMD_UMMEOW, strlen(CMD_UMMEOW)) == 0) 
        return MM_OP_DECODE; // decode command 

    return MM_OP_INVALID; // invaid command
}