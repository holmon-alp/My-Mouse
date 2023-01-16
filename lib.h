#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
// * * * * * * * * *
#define START  '1' // initialize 
#define TARGET '2'
#define STEP   'o'
#define MAP_ERROR puts("MAP ERROR")
// * * * * * * * * * 
#define WIDTH int height(char* str) {\
        int len = 0;\
        for(int i=0; str[i] != '\0'; i++)\
            if(str[i] == '\n') len++;\
        return len+1;\
    }
#define HEIGHT int width(char* s) {\
        int i=0;\
        while(s[i] != '\n')\
            i++;\
        return i;\
    }
typedef struct s_loc {
    int x, y;
} Loc;
typedef struct s_queue {
    Loc* loc;
    int dis;
    struct s_queue* next;
}Queue;
Queue* queue = NULL;
