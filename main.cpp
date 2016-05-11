#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_X 20
#define MAX_Y 20

int c;
int ** cars;
int laps = 0;
char map[MAX_X][MAX_Y];

struct thread_data_t{
  int id;
  int fuel;
  int speed;
} ;

pthread_t car1;
thread_data_t car1Params;

void *car_thread_function(void *arg);
void initialize();
void clear_them_all();

void initialize ()
{
	c = pthread_create(&car1,NULL,car_thread_function,&car1Params);
}

void clear_them_all()
{
	 pthread_join(car1, NULL);
}
void *car_thread_function(void *arg) 
{

    thread_data_t *data = (thread_data_t *)arg;
	int fuel_p = data->fuel;
	int loop = 0;
	int x = 0;
	int y = 0;
	int car_id = data->id;
	
	for (int i=0; i<1000;i++)
	{
		if(x > 0 && y == 0) { x--; }
		else if(x == 0 && y < MAX_Y) { y++; }
		else if(x < MAX_X && y == MAX_Y) { x++; }
		else if(x == MAX_X && y > 0 ) { y--; }
		printf("%d %s",x,"\n");
		printf("%d %s",y,"\n");
		if(x == MAX_X && y ==0) 
		{
                loop++;
		}
	}
	printf("Zrobiłem %d okrążeń,", loop);
	pthread_exit(NULL);
}

int main(int argc, char **argv){
    
    if(argc < 2) {
        printf("Podaj liczbę zespołów!\n");
        return 0;
    }
    int nrTeams = atoi(argv[1]);
    printf("Podaj ilość okrążeń:\n");
    scanf("%d", &laps);
    cars = new int *[nrTeams];
    for (int i=0;i<nrTeams;i++)
    {
	cars[i] = new int [2];
    }
    
    initscr();
	initialize();
	clear_them_all();

    return 0;

}


