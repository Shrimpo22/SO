#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void trata(int sinal){
	printf("Recebi o sinal nº %d\n", sinal);
	exit (0);
}

int main(){
	printf("%d\n", getpid());
	signal(SIGUSR1, trata);
	while(1){
		printf(">\n");
		sleep(5);
	}
}
