#include "lib.h"
WIDTH;
HEIGHT;

int col_num[4] = { 0, -1, 1, 0}; //{0,1,-1,0};
int row_num[4] = { -1, 0, 0, 1}; //{1,0,0,-1};
int wth = 0, hgt = 0;
int is_digit(char s) {
    return(s <= '9' && s >= '0');
}

void set_size(char*s) {
    char w[6] = {0}, h[6]= {0};
    int n = 0;
    for(int i=0; s[i] !='\0'; i++) {
        if (s[i] == 'x'){
            n = i + 1;
            break;
        }
        h[i] = s[i];
    }
    if(n > 0)
        s += n;
    else {
        MAP_ERROR;
        exit(1);
    }
    for(int i=0; s[i]!='\0';i++){
        if (!is_digit(s[i]))
            break;
        w[i]  = s[i];
    }
    wth = atoi(w);
    hgt = atoi(h);
    (wth==0 || hgt==0) ? exit(1) : 0;
}
int double_malloc(int**mt, int h, int w) {
    for(int i=0; i< h; i++) {
        mt[i] = (int*)malloc(sizeof(int) * w);
    }
    return mt != NULL;
}
void strcopy(char* des, char* src){
    int i = 0;
    do
        des[i] = src[i], i++;
    while (src[i] != '\0');
}

char* line(const int fd) {
    char temp[500];
    int i = 0, r;
    char c;
    while ((r = read(fd, &c, 1)) >= 0) {
        if (r == 0 && i == r) return NULL;
        if (c == '\n' || r == 0) break;
        temp[i++] = c;
    }
    temp[i] = '\0';
    if(strlen(temp) < 1 ){
        MAP_ERROR;
        exit(1);
    }
    char* ret = (char*)malloc(sizeof(char) * i);
    strcopy(ret, temp);
    return ret;
}
char* load_map(char*file) {
    int fd = open(file, O_RDONLY);
    struct stat fs;
    stat(file, &fs);
    char *lines = line(fd);
    char* map = malloc(fs.st_size+1 - strlen(lines));
    int r; off_t readable = fs.st_size - strlen(lines);
    if(( r = read(fd, map, readable)) > 0) {
        map[readable] = '\0';
    }
    return map;
}
int** parse_binary(char* my_map, int width, int height) {
    int **map = malloc(sizeof(int *) * height);
    if (!map)
        return NULL;
    double_malloc(map, height, width);
     int l = 0,j=0;
     for(int i=0; my_map[i]!='\0'; i++)
        if (my_map[i] == '\n') l++, j=0;
        else
            map[l][j++] = (my_map[i] == ' ' || my_map[i] == START || my_map[i] == TARGET); // fill array
    return map;
}

char** parce_matrix(char* file){
    const int fd = open(file, O_RDONLY);
    char* info = line(fd);
    printf("%s\n", info);
    set_size(info);
    char** matrix = (char**)malloc(sizeof(char*)*hgt);
    for(int i=0; i < hgt; i++)
        matrix[i] = (char*)malloc(wth);
    char* row;
    int i = 0;
    while ((row = line(fd)) != NULL) matrix[i++] = row;
    matrix[i] = NULL;
    return matrix;
}
// void print_binary(int**matrix, int w, int h) {
//     for (int i = 0; i < h; i++) {
//         for (int j = 0; j < w; j++)
//             printf("%d", matrix[i][j]);
//         puts("");
//     }
// }
void print_matrix(char** matrix){
    for (int i = 0; matrix[i]; i++){
        printf("%s\n", matrix[i]);
    }
}
void mmset(int**buff, int w, int h, int c) {
    for(int y=0;  y < h; y++)
        for(int x=0; x < w; x++)
            buff[y][x] = c;
}

Loc* get_start(char**map, int h, int w) {
    Loc* start = (Loc*)malloc(sizeof(Loc)*1);
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
            if(map[y][x] == START) {
                start->x = x;
                start->y = y;
                return start;
            }
    MAP_ERROR;
    puts("Start point not found");
    exit(-1);
}
Loc* get_target(char**map, int h, int w) {
    Loc* target = (Loc*)malloc(sizeof(Loc));
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
            if(map[y][x] == TARGET) {
                target->x = x;
                target->y = y;
                return target;
            }
    MAP_ERROR;
    puts("Target point not found");
    exit(-1);
}

void push(Queue* new){
    if (queue == NULL){
        queue = new;
        return;
    }
    Queue* last = queue;
    while (last->next != NULL)
        last = last->next;
    last->next = new;
}

void pop() {
    Queue *toDelete;
    if(queue != NULL) {
        toDelete = queue;
        queue = queue->next;
        free(toDelete);
    }
}
int check_valid(int y, int x){
	return ((y >= 0) && (y < hgt) && (x >= 0) && (x < wth));
}
Queue* new(Loc* loc, int n) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
        q->next = NULL,
        q->loc = loc,
        q->dis = n;
    return q;
}

Loc* new_loc(int x, int y) {
    Loc* l = (Loc*)malloc(sizeof(Loc));
    l->x = x;
    l->y = y;
    return l;
}
int **Solver(int **mat, Loc* src, Loc* dest) {
	if (!mat[src->y][src->x] || !mat[dest->y][dest->x]) 
		return 0;

	int **visited = (int**)malloc(sizeof(int*)*hgt);
    double_malloc(visited, hgt, wth);
	mmset(visited, wth, hgt, 0);
	visited[src->y][src->x] = 1;
    Queue *s = malloc(sizeof(Queue));
    s->dis = 0;
    s->loc = src;
    s->next = NULL;
	push(s);
	while (queue){
		Queue *curr = queue;
		Loc *pt = curr->loc;
		if (pt->x == dest->x && pt->y == dest->y) return visited;
		for (int i = 0; i < 4; i++){   
			int row = pt->y + row_num[i];
			int col = pt->x + col_num[i];
			if (check_valid(row, col) && mat[row][col] && visited[row][col]==0){
				visited[row][col] = curr->dis + 1;
                Loc* l = new_loc(col, row);
                Queue *cell = new(l, curr->dis+1);
				push(cell);
			}
		}
        pop();
	}
    return(0);
}
Loc* get_next(int**road, Loc*now, int next_step) {
    for(int i=0; i < 4; i++) {
        int y = now->y + row_num[i];
        int x = now->x + col_num[i];
        if(!check_valid(y, x))   continue;
        if(road[y][x] == next_step) {
            now->y = y;
            now->x = x;
            return now;
        }
    }
    exit(1);
}

void replace(int**road, char**map, Loc*t) {
    int i=road[t->y][t->x];
    for(; i >= 2; i--) {
        t = get_next(road, t, road[t->y][t->x]-1);
        map[t->y][t->x] = STEP;
    }
}

int exist(char* file) {
    return (access(file, F_OK) == 0); 
}
int main(int ac, char** av) {
        if (ac < 2) {
        puts("Less argument");
        exit(1);
    } else if (!exist(av[1])) {
        MAP_ERROR;
        exit(1);
    }
    char** map = parce_matrix(av[1]);
    // print_matrix(map);
    char* m = load_map(av[1]);
    wth = width(m);
    hgt = height(m);
    int **matrix = parse_binary(m, wth, hgt);
    Loc* s = get_start(map,  hgt, wth);
    Loc* t = get_target(map, hgt, wth);
    int **solved = Solver(matrix, s, t);
    if(!solved) {
        MAP_ERROR;
        puts("Path not found");
        exit(1);
    }
    int step = solved[t->y][t->x]-1;
    replace(solved, map, t);
    print_matrix(map);
    printf("%d STEPS!\n", step);
    return 0;
}
