#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<sys/time.h>
#include<time.h>
#include<signal.h>
#include"snake.h"

struct position food[2];		/* food position */
struct position bomb[50];
Snake_Node *head1, *tail1;	/* double linked list's head1 and tail1 */
Snake_Node *head2, *tail2;	/* double linked list's head1 and tail1 */
int x1_dir = -1, y1_dir = 0;	/* init dirction of the snake moving */
int x2_dir = 1, y2_dir = 0;	/* init dirction of the snake moving */
int ttm1 = 5, ttg1 = 5;			/* two timers defined to control speed */
int ttm2 = 5, ttg2 = 5;	
static int length1 = 1;	
static int length2 = 1;
static int maxlength = 1;

void main(void)
{
	Init_Disp();			/* init and display the interface */
    
	Food_Disp();			/* display food */
    Bomb_Disp();
	DLL_Snake_Create();		/* create double linked list and display snake*/
	signal(SIGALRM, Snake_Move);
	set_ticker(DFL_SPEED);
	Key_Ctrl();				/* using keyboard to control snake */
	Wrap_Up();				/* turn off the curses */
}

/* Function: Init_Disp()
 * Usage: init and display the interface
 * Return: none
 */
void Init_Disp()
{
	char wall = ' ';
	int i, j;
	initscr();
	cbreak();				/* put termial to CBREAK mode */
	noecho();
	curs_set(0);			/* set cursor invisible */

	/* display some message about title and wall */
	attrset(A_NORMAL);		/* set NORMAL first */
	attron(A_REVERSE);		/* turn on REVERSE to display the wall */
	for(i = 0; i < LINES; i++)
	{
		mvaddch(i, LEFT_EDGE, wall);
		mvaddch(i, RIGHT_EDGE, wall);
	}
	for(j = 0; j < COLS; j++)
	{
		mvaddch(0, j, '=');
		mvaddch(TOP_ROW, j, wall);
		mvaddch(BOT_ROW, j, wall);
	}
	attroff(A_REVERSE);		/* turn off REVERSE */
	mvaddstr(1, 2, "Game: snake    version: 1.0    date: 2011/08/22");
    mvaddstr(2, 2, "Snake 1: Press 'f' to speed up, 's' to speed down,'q' to quit.");
    mvaddstr(2, 50, "Snake 2: Press '1' to speed up, '2' to speed down");
    mvaddstr(3, 2, "       Nagivation key controls snake moving.");
	refresh();
}

/* Function: Food_Disp()
 * Usage: display food position
 * Return: none
 */
void Bomb_Disp()
{
    //srand(time(-1));
    
    refresh();

}
void Food_Disp()
{
   srand(time(0)); 
    for(int i = 0;i<2;i++){
        
        food[i].x_pos = rand() % (COLS - 2) + 1;
        food[i].y_pos = rand() % (LINES - TOP_ROW - 2) + TOP_ROW + 1;
        mvaddch(food[i].y_pos, food[i].x_pos, FOOD_SYMBOL);/* display the food */
        
    }
    
   
    for(int i = 0;i<maxlength;i++){
        bomb[i].x_pos = rand() % (COLS - 2) + 1;
        bomb[i].y_pos = rand() % (LINES - TOP_ROW - 2) + TOP_ROW + 1;
        mvaddch(bomb[i].y_pos, bomb[i].x_pos, BOMB_SYMBOL);/* display the food */

    }
	refresh();
}

/* Function: DLL_Snake_Create()
 * Usage: create double linked list, and display the snake first node
 * Return: none
 */
void DLL_Snake_Create()
{
	Snake_Node *temp1 = (Snake_Node *)malloc(sizeof(Snake_Node));
        head1 = (Snake_Node *)malloc(sizeof(Snake_Node));
        tail1 = (Snake_Node *)malloc(sizeof(Snake_Node));
	if(temp1 == NULL || head1 == NULL || tail1 == NULL)
		perror("malloc");
	temp1->x_pos = COLS-5;
	temp1->y_pos = 10;
	head1->prev =NULL;
	tail1->next = NULL;
	head1->next = temp1;
	temp1->next = tail1;
	tail1->prev = temp1;
	temp1->prev = head1;
	mvaddch(temp1->y_pos, temp1->x_pos, SNAKE1_SYMBOL);

    Snake_Node *temp2 = (Snake_Node *)malloc(sizeof(Snake_Node));
    head2 = (Snake_Node *)malloc(sizeof(Snake_Node));
    tail2 = (Snake_Node *)malloc(sizeof(Snake_Node));
	if(temp2 == NULL || head2 == NULL || tail2 == NULL)
		perror("malloc");
	temp2->x_pos = 5;
	temp2->y_pos = 10;
	head2->prev =NULL;
	tail2->next = NULL;
	head2->next = temp2;
	temp2->next = tail2;
	tail2->prev = temp2;
	temp2->prev = head2;
	mvaddch(temp2->y_pos, temp2->x_pos, SNAKE2_SYMBOL);
	refresh();
}

/* Function: Snake_Move()
 * Usage: use Navigation key to control snake moving, and judge
 * if the snake touch the food.
 * Return:
 */
void Snake_Move()
{
	//static int length = 1;		/* length of snake */
	int Length_Flag1 = 0;		/* default snake's length no change */
    int Length_Flag2 = 0;
	int moved1 = 0;
    int moved2 = 0;
	signal(SIGALRM, SIG_IGN);
	/* judge if the snake crash the wall */
	if((head1->next->x_pos == RIGHT_EDGE-1 && x1_dir == 1)
		|| (head1->next->x_pos == LEFT_EDGE+1 && x1_dir == -1)
		|| (head1->next->y_pos == TOP_ROW+1 && y1_dir == -1)
		|| (head1->next->y_pos == BOT_ROW-1 && y1_dir == 1))
	{
		gameover(2,1);
	}

    if((head2->next->x_pos == RIGHT_EDGE-1 && x2_dir == 1)
		|| (head2->next->x_pos == LEFT_EDGE+1 && x2_dir == -1)
		|| (head2->next->y_pos == TOP_ROW+1 && y2_dir == -1)
		|| (head2->next->y_pos == BOT_ROW-1 && y2_dir == 1))
	{
		gameover(1,1);
	}
	/* judge if the snake crash itself */
    if(mvinch(head1->next->y_pos + y1_dir, head1->next->x_pos + x1_dir) == 'o')
		gameover(2,2);
	if(mvinch(head2->next->y_pos + y2_dir, head2->next->x_pos + x2_dir) == 'x')
		gameover(1,2);

	if(ttm1 > 0 && ttg1-- == 1)
	{
        DLL_Snake_Insert1(head1->next->x_pos + x1_dir, head1->next->y_pos + y1_dir);
		ttg1 = ttm1;		/* reset */
        moved1 = 1;
	}

    if(ttm2 > 0 && ttg2-- == 1)
	{
		/* snake moves */
        DLL_Snake_Insert2(head2->next->x_pos + x2_dir, head2->next->y_pos + y2_dir);
		ttg2 = ttm2;		/* reset */
        moved2 = 1;
	}

	if(moved1)
	{
		/* snake eat the food */
        for(int i = 0;i<2;i++){
            if(head1->next->x_pos == food[i].x_pos && head1->next->y_pos == food[i].y_pos)
            {
                Length_Flag1 = 1;
                length1++;
                if(length1>length2) maxlength = length1;
                /* Mission Complete */
                /* reset display the food randomly */
                
                for(int j = 0;j<2;j++){
                    mvaddch(food[j].y_pos, food[j].x_pos, ' ');
                }
                for(int j = 0;j<maxlength;j++){
                    mvaddch(bomb[j].y_pos, bomb[j].x_pos, ' ');
                }
                Food_Disp();
                break;
            }
        }

        for(int i = 0;i<maxlength;i++){
            if(head1->next->x_pos == bomb[i].x_pos && head1->next->y_pos == bomb[i].y_pos)
            {
                gameover(2,3);
                break;
            }
        }
        
        if(Length_Flag1 == 0)
        {
            /* delete the tail1->prev node */
            mvaddch(tail1->prev->y_pos, tail1->prev->x_pos, ' ');
            DLL_Snake_Delete_Node1();
            
        }    
        if(mvinch(head1->next->y_pos + y1_dir, head1->next->x_pos + x1_dir) == 'x')
		    gameover(2,4);
		mvaddch(head1->next->y_pos, head1->next->x_pos, SNAKE1_SYMBOL);
		refresh();
	}
    if(moved2)
	{
		/* snake eat the food */
        for(int i = 0;i<2;i++){
            if(head2->next->x_pos == food[i].x_pos && head2->next->y_pos == food[i].y_pos)
            {
                Length_Flag2 = 1;
                length2++;
                if(length2>length1) maxlength = length2;
                /* Mission Complete */
                /* reset display the food randomly */
                
                for(int j = 0;j<2;j++){
                    mvaddch(food[j].y_pos, food[j].x_pos, ' ');
                }
                for(int j = 0;j<maxlength;j++){
                    mvaddch(bomb[j].y_pos, bomb[j].x_pos, ' ');
                }
                Food_Disp();
                break;
            }
        }

        for(int i = 0;i<maxlength;i++){
            if(head2->next->x_pos == bomb[i].x_pos && head2->next->y_pos == bomb[i].y_pos)
            {
                gameover(1,3);
                break;
            }
        }
        
        if(Length_Flag2 == 0)
        {
            /* delete the tail1->prev node */
            mvaddch(tail2->prev->y_pos, tail2->prev->x_pos, ' ');
            DLL_Snake_Delete_Node2();
            
        }   
        Snake_Node *temp = (Snake_Node *)malloc(sizeof(Snake_Node));
        temp = tail1; 
        if(mvinch(head2->next->y_pos + y2_dir, head2->next->x_pos + x2_dir) == 'o')
		    gameover(1,4);
		mvaddch(head2->next->y_pos, head2->next->x_pos, SNAKE2_SYMBOL);
		refresh();
	}
	signal(SIGALRM, Snake_Move);
}

/* Function: set_ticker(number_of_milliseconds)
 * Usage: arrange for interval timer to issue SIGALRM's at regular intervals
 * Return: -1 on error, 0 for ok
 * arg in milliseconds, converted into whole seconds and microseconds
 * note: set_ticker(0) turns off ticker
 */
int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;					/* int second part */
	n_usecs = (n_msecs % 1000) * 1000L;		/* microsecond part */

	new_timeset.it_interval.tv_sec = n_sec;	/* set reload */
	new_timeset.it_interval.tv_usec = n_usecs;

	new_timeset.it_value.tv_sec = n_sec;	/* set new ticker value */
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

/* Function: Wrap_Up()
 * Usage: turn off the curses
 * Return: none
 */
void Wrap_Up()
{
    
	set_ticker(0);		/* turn off the timer */
	while(getchar() != 'q'){
        
    }
    endwin();
    exit(0);
    

    
	
}

/* Function: Key_Ctrl()
 * Usage: using keyboard to control snake action; 'f' means speed up,
 * 's' means speed down, 'q' means quit, navigation key control direction.
 * Return: none
 */
void Key_Ctrl()
{
	int c;
	keypad(stdscr, true);		/* use little keyboard Navigation Key */
    
	while(c = getch(), c != 'q')
	{
        mvprintw(5,2,"snake1 point:%d",length1);
        mvprintw(5,22,"snake1 speed:%d",ttm1);
        mvprintw(5,50,"snake2 point:%d",length2);
        mvprintw(5,72,"snake1 speed:%d",ttm2);
		if(c == '1')
		{
			if(ttm1 == 1)
				continue;
			ttm1--;
		}
		else if(c == '2')
		{
			if(ttm1 == 8)
				continue;
			ttm1++;
		}
		if(c == KEY_LEFT)
		{
			if(tail1->prev->prev->prev != NULL && x1_dir == 1 && y1_dir == 0)
				continue; /* it can't turn reverse when snake have length */
			x1_dir = -1;
			y1_dir = 0;
		}
		else if(c == KEY_RIGHT)
		{
			if(tail1->prev->prev->prev != NULL && x1_dir == -1 && y1_dir == 0)
				continue;
			x1_dir = 1;
			y1_dir = 0;
		}
		else if(c == KEY_UP)
		{
			if(tail1->prev->prev->prev != NULL && x1_dir == 0 && y1_dir == 1)
				continue;
			x1_dir = 0;
			y1_dir = -1;
		}
		else if(c == KEY_DOWN)
		{
			if(tail1->prev->prev->prev != NULL && x1_dir == 0 && y1_dir == -1)
				continue;
			x1_dir = 0;
			y1_dir = 1;
		}else if(c == 'f')
		{
			if(ttm2 == 1)
				continue;
			ttm2--;
		}
		else if(c == 'g')
		{
			if(ttm2 == 8)
				continue;
			ttm2++;
		}
		if(c == 'a')
		{
			if(tail2->prev->prev->prev != NULL && x2_dir == 1 && y2_dir == 0)
				continue; /* it can't turn reverse when snake have length */
			x2_dir = -1;
			y2_dir = 0;
		}
		else if(c == 'd')
		{
			if(tail2->prev->prev->prev != NULL && x2_dir == -1 && y2_dir == 0)
				continue;
			x2_dir = 1;
			y2_dir = 0;
		}
		else if(c == 'w')
		{
			if(tail2->prev->prev->prev != NULL && x2_dir == 0 && y2_dir == 1)
				continue;
			x2_dir = 0;
			y2_dir = -1;
		}
		else if(c == 's')
		{
			if(tail2->prev->prev->prev != NULL && x2_dir == 0 && y2_dir == -1)
				continue;
			x2_dir = 0;
			y2_dir = 1;
		}
	}
}

/* Function: DLL_Snake_Insert1(int x, int y)
 * Usage: Insert node in the snake.
 * Return: none
 */
void DLL_Snake_Insert1(int x, int y)
{
	Snake_Node *temp1 = (Snake_Node *)malloc(sizeof(Snake_Node));
	if(temp1 == NULL)
		perror("malloc");
	temp1->x_pos = x;
	temp1->y_pos = y;
	temp1->prev = head1->next->prev;
	head1->next->prev = temp1;
	temp1->next = head1->next;
	head1->next = temp1;
}

void DLL_Snake_Insert2(int x, int y)
{
	Snake_Node *temp2 = (Snake_Node *)malloc(sizeof(Snake_Node));
	if(temp2 == NULL)
		perror("malloc");
	temp2->x_pos = x;
	temp2->y_pos = y;
	temp2->prev = head2->next->prev;
	head2->next->prev = temp2;
	temp2->next = head2->next;
	head2->next = temp2;
}

/* Function: gameover(int n)
 * Usage: gameover(0) means Mission Completes; gameover(1) means crashing
 * the wall; gameover(2) means crash itself.
 * Return: none
 */
void gameover(int snake,int n)
{
    int s1 = 0;
    int s2 = 0;
    if(snake == 1){
        s1 = 1;
        s2 = 2;
    }else{
        s1 = 2;
        s2 = 1;
    }
	switch(n)
	{
		case 1:
            mvprintw(LINES/2, COLS/3 - 4,"snake%d WIN!!,snake%d crash the wall,press any key to exit\n",s1,s2);
			break;
		case 2:
            mvprintw(LINES/2, COLS/3 - 4,"snake%d WIN!!,snake%d crash yourself,press any key to exit\n",s1,s2);
			break;
        case 3:
            mvprintw(LINES/2, COLS/3 - 4,"snake%d WIN!!,snake%d crash the bmob,press any key to exit\n",s1,s2);
			break;
        case 4:
            mvprintw(LINES/2, COLS/3 - 4,"snake%d WIN!!,snake%d crash the snake%d's body,press any key to exit\n",s1,s2,s1);
		default:
			break;
	}
	refresh();
	/* delete the whole double linked list */
	DLL_Snake_Delete1();
    DLL_Snake_Delete2();
	Wrap_Up();
}

/* Function: DLL_Snake_Delete_Node1()
 * Usage: delete a tail1 node, not the whole linked list
 * Return: none
 */
void DLL_Snake_Delete_Node1()
{
	Snake_Node *temp1;
	temp1 = tail1->prev;
	tail1->prev = tail1->prev->prev;
	temp1->prev->next = tail1;
	free(temp1);
}

void DLL_Snake_Delete_Node2()
{
	Snake_Node *temp2;
	temp2 = tail2->prev;
	tail2->prev = tail2->prev->prev;
	temp2->prev->next = tail2;
	free(temp2);
}

/* Function: DLL_Snake_Delete()
 * Usage: delete the whole double linked list
 * Return: none
 */
void DLL_Snake_Delete1()
{
	while(head1->next != tail1)
		DLL_Snake_Delete_Node1();
	head1->next = tail1->prev = NULL;
	free(head1);
	free(tail1);
}

void DLL_Snake_Delete2()
{
	while(head2->next != tail2)
		DLL_Snake_Delete_Node2();
	head2->next = tail2->prev = NULL;
	free(head2);
	free(tail2);
}