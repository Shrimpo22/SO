#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>





char matrixdata[7][100];
int main(){
    FILE *ped;
    ped=fopen(FILE_PEDIDO_VACINA,"r");
    fseek(ped, 0, SEEK_END);
    int size=ftell(ped);
    fseek(ped, 0, SEEK_SET);
    char completedata[size];
    int i=0;
    my_fgets(completedata, size, ped);
	char modified[size];
	strcpy(modified, completedata);
    char * token = strtok(modified, ":");
    while( token != NULL ){
        strcpy(matrixdata[i],token);
        token = strtok(NULL, ":");
        i++;
    }
	debug("%s", completedata);
	debug("%s", matrixdata[0]);
	debug("%s", matrixdata[1]);
	debug("%s", matrixdata[2]);
	debug("%s", matrixdata[3]);
	debug("%s", matrixdata[4]);
	debug("%s", matrixdata[5]);
	debug("%s", matrixdata[6]);


}

