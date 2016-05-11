#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int ** cars;
int laps = 0;

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

    return 0;

}


