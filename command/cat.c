/*****************************************************************************
 *****************************************************************************
 * @file   cat.c
 * @brief  print files to stdout
 * @author TF 141
 *****************************************************************************
 *****************************************************************************/


#include "stdio.h"
#include "sys/const.h"
#include "type.h"

#define BUFSIZ 1024
char stdbuf[BUFSIZ];

int main(int argc, char ** argv) {
    int fflg = 0;           // Flag set to 1 if reading from stdin
    int fi;                 // File handle
    int dev = -1, ino = -1;      // Keep track of file inodes
    struct stat statb;      // File status struct
    int i;
    for (i = 1; i < argc; i++) {
        char* filename = argv[i];
        if (stat(filename, &statb)) {  // Get file status
            printf("cat: %s not found\n", filename);
            continue;
        }
        // If file is not 'character special' or 'block special'
        if (statb.st_mode != I_CHAR_SPECIAL && statb.st_mode != I_BLOCK_SPECIAL) {
            dev = statb.st_dev;
            ino = statb.st_ino;
        }
        if ((fi = open(filename, O_RDWR)) == -1) {
            printf("cat: cannot open %s\n", filename);
            continue;
        }
        int size = statb.st_size;
        while (size > 0) {
            int readbytes = read(fi, stdbuf, BUFSIZ);
            size -= readbytes;
            if (readbytes > 0)
                printf("%s", stdbuf);
        }
        printf("\n");
    }
}
