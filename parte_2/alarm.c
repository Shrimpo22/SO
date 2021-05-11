#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
int exists = -1;
void waitFile(){
	debug("Inside Handler: %d", exists);
    if(!fopen(FILE_PEDIDO_VACINA, "r")){
		printf("Made it? %d", exists);
        exists = -1;
	}
}


int main(){

if(fopen(FILE_PEDIDO_VACINA,"r")){
    exists=0;
	signal(SIGALRM,waitFile);
	debug("1: %d", exists);
	while(exists == 0){
		debug("2 LOOP: %d", exists);
		alarm(5);
		pause();
	}
		debug("3: %d", exists);
}
debug("4: Se cheguei aqui pode ter dado bem ou mal :(  %d", exists);
}
