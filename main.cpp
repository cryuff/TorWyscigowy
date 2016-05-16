#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_X 20
#define MAX_Y 20

int result;
int notcompleted[50];
int nc_place=0;
int completed[50];
int c_place=0;
int ** cars;
int laps = 0;
char map[MAX_X][MAX_Y];


pthread_mutex_t pitstop_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turn_left_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turn_left_second_mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_data_t
{
  int id;
  int fuel;
  int speed;
  int laps;
} ;

bool repaint = true;
int nrTeams;

pthread_t * threads;
pthread_t paint_thread;
thread_data_t * threadsParameters;

void *car_thread_function(void *arg);
void initialize();
void clear_them_all();
void results();

void *car_thread_function(void *arg) 
{

    thread_data_t *data = (thread_data_t *)arg;
	int fuel_p = data->fuel;
	int loop = 0;
	int x = 0;
	int y = 0;
	int car_id = data->id;
	int laps_for_car = data->laps;
	float speed_p = data->speed;
	float new_speed = 0;
	while (laps_for_car>0)
	{
		usleep(500-int(speed_p));
		if(x > 0 && y == 0) { x--; }
		else if(x == 0 && y < MAX_Y) { y++; }
		else if(x < MAX_X && y == MAX_Y) { x++; }
		else if(x == MAX_X && y > 0 ) { y--; }
		fuel_p=fuel_p - 1;
		speed_p = 400 - fuel_p;
		if(x == MAX_X && y ==0) 
		{
			laps_for_car--;
			new_speed = speed_p + 1000/fuel_p;
		}
		if (x == 0 && y == MAX_Y)
		{
			pthread_mutex_lock(&turn_left_mutex);
			usleep(100);
			pthread_mutex_unlock(&turn_left_mutex);
		}
		if (x == 0 && y == 0)
		{
			pthread_mutex_lock(&turn_left_second_mutex);
			usleep(50);
			pthread_mutex_unlock(&turn_left_second_mutex);
		}
		if (fuel_p < 20 && fuel_p > 0)
			{		
				if (pthread_mutex_trylock(&pitstop_mutex)==0) 
				{
					printf("%s %d %s","W pitstopie stoi:",car_id,"\n");
					usleep(rand() % 500 + 1000 );
					fuel_p = 100;
					pthread_mutex_unlock(&pitstop_mutex);
				}
			}
		if (fuel_p < 0)
		{
			notcompleted[nc_place]=car_id;
			nc_place++;
			break;
		}
		if (laps_for_car == 0)
		{
			completed[c_place]=car_id;
			c_place++;
			break;
		}
	}
	pthread_exit(NULL);
}

void initialize ()
{
	for(int i=0; i<nrTeams*2; i++)
	{
        if((result = pthread_create(&threads[i], NULL, car_thread_function, &threadsParameters[i])))
		{
            printw("Thread %d init error \n", i) ;
        }
    }   
}

void clear_them_all()
{
	 for (int i=0; i<nrTeams*2; i++) 
	 {
        pthread_join(threads[i], NULL);
	 }   
	 pthread_mutex_destroy(&pitstop_mutex);
	 pthread_mutex_destroy(&turn_left_mutex);
	 pthread_mutex_destroy(&turn_left_second_mutex);
}

void results()
{
	for (int i=0; i<c_place;i++)
	{
		printf("%s %d %s %d %s","Miejsce",i+1,"zajął pojazd nr.",completed[i],"\n");
	}
	for (int i=0; i<nc_place;i++)
	{
		printf("%s %d %s","Do mety nie dojechał pojazd ",notcompleted[i],"\n");
	}
}
int main(int argc, char **argv){
    
    if(argc < 2) {
        printf("Podaj liczbę zespołów!\n");
        return 0;
    }
    nrTeams = atoi(argv[1]);
    printf("Podaj ilość okrążeń:\n");
    scanf("%d", &laps);
	threads = new pthread_t[nrTeams*2];
	threadsParameters = new thread_data_t[nrTeams*2];
    cars = new int *[nrTeams*2];
    for (int i=0;i<nrTeams;i++)
    {
	cars[i] = new int [2];
    }
	for(int j=0; j<nrTeams*2;j++){
        threadsParameters[j].id = j;
		threadsParameters[j].fuel = rand() % 50 + 50; 
        threadsParameters[j].speed = 400 - threadsParameters[j].fuel;
		threadsParameters[j].laps = laps;
    }
    
    initscr();
	initialize();
	clear_them_all();
	results();
    return 0;

}


