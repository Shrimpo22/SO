#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>



Enfermeiro *distruct;

int main(){
	FILE *spid;
	int sz;
	int numenf;
	spid=fopen(FILE_ENFERMEIROS, "r");
    fseek(spid, 0,SEEK_END);
    sz = ftell(spid);
    fseek(spid, 0,SEEK_SET);
    debug("sizeof(FILE_ENFERMEIROS): %d", sz);
    debug("sizeof(Enfermeiro): %d", sizeof(Enfermeiro));
    numenf=(sz/sizeof(Enfermeiro));
    debug("%d", numenf);
    sucesso("S2) Ficheiro FILE_ENFERMEIROS TEM %d bytes, ou seja, %d enfermeiros", sz, numenf);

    distruct = (Enfermeiro *) malloc(numenf*sizeof(Enfermeiro));
    if(NULL == distruct){
        erro("NULL Pointer");
        exit(1);
    }

    spid=fopen(FILE_ENFERMEIROS, "r");
    fread(distruct, sizeof(Enfermeiro), numenf, spid);
	debug("num_vac_dadas por %s: %d", distruct[0].nome, distruct[0].num_vac_dadas);
}
