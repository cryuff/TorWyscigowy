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
int nrTeams;
char map[MAX_Y + 1][MAX_X + 6];
bool finish = false;
bool repaint = true;
int pitstoperzy[10];
int sila=0;
bool pitstop=false;
int raz = 1;
struct thread_data_t
{
  int id;
  int fuel;
  int speed;
  int laps;
} ;
pthread_mutex_t pitstop_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turn_left_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turn_left_second_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rest_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paint_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t * threads;
pthread_t pitstop_thread;
pthread_t paint_thread;
thread_data_t * threadsParameters;

void *car_thread_function(void *arg);
void *paint_thread_function(void *arg);
void initialize();
void clear_them_all();
void clear_part();
void results();
void print_lap();

/*void *pitstop_thread_function(void *arg)
{
	int power = 100;
	while(!finish)
	{
		if (pitstop)
		{
			if(power<30 && power>=0)
			{
				if (pthread_mutex_trylock(&rest_mutex)==0) 
				{	
					usleep(10);
					power = 100;
					pthread_mutex_unlock(&rest_mutex);
				}
			}
			if (power>=0)
			{
				power = power - 10;
				printf("%s %d %s","Siła:",power,"\n");
				sila = sila+power;
				usleep(10000);
			}
		}
}

	pthread_exit(NULL);
} 
*/
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
		cars[car_id][0]=x;
		cars[car_id][1]=y;
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
					pitstop = true;
					cars[car_id][0]=MAX_X + 2;
					cars[car_id][1]=0;
					usleep(rand() % 500 + 1000 );
					fuel_p = 100;
					sila = 0;
					pitstop = false;
					pthread_mutex_unlock(&pitstop_mutex);
				}
			}
		if (fuel_p < 0)
		{
			cars[car_id][0]=-1;
			cars[car_id][1]=-1;
			notcompleted[nc_place]=car_id;
			nc_place++;
			break;
		}
		if (laps_for_car == 0)
		{
			cars[car_id][0]=-1;
			cars[car_id][1]=-1;
			completed[c_place]=car_id;
			c_place++;
			break;
		}
	}
	pthread_exit(NULL);
}

void *paint_thread_function(void *arg)
{
	while(!finish)
	{
		print_lap();
		usleep(10000);
	}
	pthread_exit(NULL);
}

void print_lap()
{
    clear();
    strcpy(map[0], "-------------------  [ ]");
    for(int i=1; i<20;i++)
	{
        strcpy(map[i], "|                 |");
    }
    strcpy(map[MAX_Y], "-------------------");
	for(int i=0; i<nrTeams*2; i++)
	{
        if(cars[i][0] != -1 && cars[i][1] != -1) 
		{
            map[cars[i][1]][cars[i][0]] = (char)(49 + i);
        }
    }
    for(int i=0; i<MAX_Y+1 ; i++)
	{
        printw("%s\n",map[i]);
    }
    refresh();


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
	
	result = pthread_create(&paint_thread,NULL,paint_thread_function,NULL);
	/*for(int i=0; i<10; i++)
	{
        result = pthread_create(&pitstop_thread,NULL,pitstop_thread_function,NULL);
    } */

}

void clear_them_all()
{
	 pthread_mutex_destroy(&pitstop_mutex);
	 pthread_mutex_destroy(&turn_left_mutex);
	 pthread_mutex_destroy(&turn_left_second_mutex);
	 pthread_mutex_destroy(&rest_mutex);
	 pthread_mutex_destroy(&paint_mutex);
	 pthread_join(pitstop_thread, NULL); 
	 pthread_join(paint_thread, NULL);
}

void clear_part()
{
	 for (int i=0; i<nrTeams*2; i++) 
	 {
        pthread_join(threads[i], NULL);
	 }   
	 finish = true;
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
int main(int argc, char **argv)
{
    if(argc < 2) 
	{
        printf("Podaj liczbę zespołów!\n");
        return 0;
    }
    nrTeams = atoi(argv[1]);
    printf("Podaj ilość okrążeń:\n");
    scanf("%d", &laps);
	threads = new pthread_t[nrTeams*2];
	threadsParameters = new thread_data_t[nrTeams*2];
    cars = new int *[nrTeams*2];
    for (int i=0;i<nrTeams*2;i++)
    {
		cars[i] = new int [2];
    }
	for(int j=0; j<nrTeams*2;j++)
	{
        threadsParameters[j].id = j;
		threadsParameters[j].fuel = rand() % 50 + 50; 
        threadsParameters[j].speed = 400 - threadsParameters[j].fuel;
		threadsParameters[j].laps = laps;
    }
    initscr();
	initialize();
	clear_part();
	clear_them_all();
	results();
    return 0;
}


