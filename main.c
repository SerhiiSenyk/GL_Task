#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <file_searcher.h>
#define OPT_INDEX 3
#define USAGE "Usage: %s -i filename\n"

int main(int argc, char *argv[])
{
    int opt;
    char filename[FILENAME_MAX];

    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
	    switch (opt)
	    {
	    case 'i':
	        strncpy(filename, optarg, FILENAME_MAX);
	        break;
	    default:
	        fprintf(stderr, USAGE, argv[0]);
	        return -1;
	    }
    }
    if (optind != OPT_INDEX)
    {
	    printf("Missing opt argument\n");
	    fprintf(stderr, USAGE, argv[0]);
	    return -1;
    }
    search_file(filename);
    printf("\nExit search file app\n");
    return 0;
}
