#define SNAKE1_SYMBOL    'o'     /* snake body and food symbol */  
#define SNAKE2_SYMBOL    'x'     /* snake body and food symbol */  
#define FOOD_SYMBOL     '*' 
#define BOMB_SYMBOL     'b'   
#define MAX_NODE        30      /* maximum snake nodes */  
#define DFL_SPEED       50      /* snake default speed */  
#define TOP_ROW     6           /* top_row */  
#define BOT_ROW     LINES - 1  
#define LEFT_EDGE   0  
#define RIGHT_EDGE  COLS - 1  

typedef struct node         /* Snake_node structure */  
{  
    int x_pos;  
    int y_pos;  
    struct node *prev;  
    struct node *next;  
} Snake_Node;  

struct position             /* food position structure */  
{  
    int x_pos;  
    int y_pos;  
} ;  
void Init_Disp();           /* init and display the interface */  
void Food_Disp();           /* display the food position */  
void Bomb_Disp();
void Wrap_Up();             /* turn off the curses */  
void Key_Ctrl();            /* using keyboard to control snake */  
int set_ticker(int n_msecs);/* ticker */  

void DLL_Snake_Create();    /* create double linked list*/  
void DLL_Snake_Insert1(int x, int y);    /* insert node */ 
void DLL_Snake_Insert2(int x, int y);  
void DLL_Snake_Delete_Node1();   /* delete a node */ 
void DLL_Snake_Delete_Node2();   /* delete a node */  
void DLL_Snake_Delete1();        /* delete all the linked list */  
void DLL_Snake_Delete2();        /* delete all the linked list */  
void Snake_Move();          /* control the snake move and judge */  
void gameover(int sanke,int n);       /* different n means different state */