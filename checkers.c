#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
//#include <pthread.h>
#include <semaphore.h>

void desk_init(char* mas){ //fill the desk
    int i;
    for(i=0;i<24;i++){
        if(((i/8 + 1) + (i+1)%8) %2) mas[i] = '+';
        else mas[i] = 'O';
    }
    for(i=24;i<40;i++){
        mas[i] = 'O';
    }
    for(i=40;i<64;i++){
        if(((i/8 + 1) + (i+1)%8) %2) mas[i] = '*';
        else mas[i] = 'O';
    }
}

void desk_print(char* mas){ //print the desk
    int i,j;
    printf("\n  1 2 3 4 5 6 7 8\n");
    for(i=0;i<8;i++){
        if(i<8) printf("%d", i+1);
        for(j=0;j<8;j++){
            printf(" %c", mas[i*8 + j]);
        }
        printf("\n");
    }
}

int up_right(int x, int y, char *mas){
    if(x == 1 || y == 8) return 1;  //border
    char c;
    int adr = (x-1)*8 + y - 1;
    c = mas[adr];
    if(c == 'O') return 1; 	//pustoe mesto
    if(c == mas[adr - 7]) return 1;  // ne bey svoikh
    if((mas[adr - 7] != 'O') && (mas[adr - 7 - 7] != c)) { //yeda
        if(x == 2 || y == 7) return 1; //border
        mas[adr] = 'O';
        mas[adr - 7] = 'O';
        mas[adr - 7 - 7] = c;
        return 2;  //syeli
    }
    mas[adr] = 'O';
    mas[adr - 7] = c;
    return 0; 	//end of turn
}

int up_left(int x, int y, char *mas){
    if(x == 1 || y == 1) return 1;  //border
    char c;
    int adr = (x-1)*8 + y - 1;
    c = mas[adr];
    if(c == 'O') return 1; 	//pustoe mesto
    if(c == mas[adr - 9]) return 1;  // ne bey svoikh
    if((mas[adr - 9] != 'O') && (mas[adr - 9 - 9] != c)) { //yeda
        if(x == 2 || y == 2) return 1; //border
        mas[adr] = 'O';
        mas[adr - 9] = 'O';
        mas[adr - 9 - 9] = c;
        return 2; //syeli
    }
    mas[adr] = 'O';
    mas[adr - 9] = c;
    return 0; 	//end of turn
}

int down_left(int x, int y, char *mas){
    if(x == 8 || y == 1) return 1;  //border
    char c;
    int adr = (x-1)*8 + y - 1;
    c = mas[adr];
    if(c == 'O') return 1; 	//pustoe mesto
    if(c == mas[adr + 7]) return 1;  // ne bey svoikh
    if((mas[adr + 7] != 'O') && (mas[adr + 7 + 7] != c)) { //yeda
        if(x == 8 || y == 2) return 1; //border
        mas[adr] = 'O';
        mas[adr + 7] = 'O';
        mas[adr + 7 + 7] = c;
        return 2;  //syeli
    }
    mas[adr] = 'O';
    mas[adr + 7] = c;
    return 0; 	//end of turn
}

int down_right(int x, int y, char *mas){
    if(x == 8 || y == 8) return 1; 	//border
    char c;
    int adr = (x-1)*8 + y - 1;
    c = mas[adr];
    if(c == 'O') return 1; 	//pustoe mesto
    if(c == mas[adr + 9]) return 1;  // ne bey svoikh
    if((mas[adr + 9] != 'O') && (mas[adr + 9 + 9] != c) ) { //yeda
        if(x == 7 || y == 7) return 1; //border
        mas[adr] = 'O';
        mas[adr + 9] = 'O';
        mas[adr + 9 + 9] = c;
        return 2; //syeli
    }
    mas[adr] = 'O';
    mas[adr + 9] = c;
    return 0; 	//end of turn
}

int main()
{   
    const char s[] = "DIRECTIONS: up_right, down_right, down_left, up_left\n\
represented by - 1,2,3,4 respectively.\n\
End of turn is 0 (for eating multiple checkers).\n\
TURN INPUT is: d, x, y  - where d is direction, x - line, y - column";
    printf("\n%s\n",s);
    int shm;
    char* addr;
    
    shm = shm_open("/desk", O_RDWR | O_CREAT, 0777); //shared memory
    perror("shm");
    ftruncate(shm, 65);
    addr = mmap(0, 65, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    perror("addr");
    
    desk_init(addr); //fill
    desk_print(addr); //print
    
    sem_t* sem; //svetofors
    sem = sem_open("/sema", O_CREAT, 0666, 1);
    perror("sem_open");
    sem_init(sem, 2, 1);
    perror("sem_init");
    
    pid_t p1; //main part
    int x,y, step;
    int counter = 12; //checkers on each side
    
    int turn(int step, int x, int y, int *counter)
    { //turns realization
        int result;
        switch(step) {
            case 0: return 0;
    		case 1: result = up_right(x,y,addr);
    			break;
    		case 2: result = down_right(x,y,addr);
    			break;
    		case 3: result = down_left(x,y,addr);
    			break;
    		case 4: result = up_left(x,y,addr);
    			break;
    		}
    		if(result == 1) { //wrong turn
    			printf("\ninvalid turn, try again:\n");
    			return 1;
    		}
    		if(result == 2) { //yeda
    			desk_print(addr);
    			printf("\nminus one, next turn:\n");
    			(*counter)--;
    			return 2;
    		}
    		printf("%d of enemy left \n", *counter);
    		return 0; //end of proper turn
    }
    
    if((p1 = fork()) == 0){
        sleep(5); //white's first turn
        while(counter){
        	sem_wait(sem); //zhdet
        	perror("sem_wait son");
        	printf("2nd player turn:\n");
            scanf("%d %d %d", &step, &x, &y);  //first input
            
            while(turn(step, x, y, &counter)){ //continue, if wrong input
            	scanf("%d %d %d", &step, &x, &y);  //or eating has happened
            }
    		desk_print(addr); //new desk
            sem_post(sem); //up the semafore
            sleep(2);
        }
    
    } else {
        while(counter){
            sem_wait(sem);
            perror("sem_wait father");
            printf("\n1st player turn:\n");
            scanf("%d %d %d", &step, &x, &y); //first input
            
            while(turn(step, x, y, &counter)){ //continue, if wrong input
          		scanf("%d %d %d", &step, &x, &y); //or eating has happened
          	}
    		desk_print(addr); //new desk
            sem_post(sem);  //up the semafore
            sleep(2);
        }
    }
    sem_destroy(sem); //finish him
    munmap(addr, 65);
    close(shm);
    return 0;
}