/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº:98654    Nome:Vasco do Nascimento Paiva Araújo
 ** Nome do Módulo: servidor.c
 ** Descrição/Explicação do Módulo: Muito trabalho, dores de cabeça e de costas
 ** deu-me este módulo. Mas bem, comentários ao longo do código! Espero que sejam
 ** explicativos!:)
 ******************************************************************************/
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


Cidadao pessoa;
int vaga_index=0;			/* Guarda o índice da primeira vaga disponivel que o servidor encontra (ver mais à frente)*/
int numenf;					/* Número de enfermeiros guardados no ficheiro enfermeiros.dat */
int num_childs=0;			/* Número de processos filhos que foram criados (Independetemente de estarem vivos ou mortos)*/
Enfermeiro *distruct;		/* Inicialização da estrutura dinâmica para guardar os enfermeiros retirados do ficheiro enfermeiros.dat */
Vaga vagas[NUM_VAGAS];		/* Array de vagas */
int DedicatedServer[100];	/* Array que irá guardar todos os PID's dos servidores filhos */
int pedido;					/* Variável que indica se o servidor recebeu algum pedido do processo cidadão (0-Nenhum pedido, 1-Pedido Recebido)*/




/* A função a seguir é chamada quando o servidor recebe um pedido vindo do servidor cidadão, logo muda a variável pedido para 1 
   De seguida abre o ficheiro FILE_PEDIDO_VACINA se existir e verifica o seu tamanho, cria um array completedata com esse tamanho (array com o texto tal e qual como vem do 
   ficheiro). Depois cria uma cópia do array completedata chamada modified. De seguida a função strtok devolve desdo início até ao primeiro ':', mete na posição 0 da matriz 
   matrixdata[i], tem que ser chamada várias vezes para extrair todas as porções da string, e guarda cada pedaço na matriz. É criado um cidadão com os valores guardados na
   matrixdata. */
void vacina(){
	pedido=1;
	debug("\e[0;102m %d vacina\e[0m", pedido);
	char matrixdata[7][100];
	FILE *ped;
	if(access(FILE_PEDIDO_VACINA, F_OK)== -1){
		erro("S5.1) Não foi possível abrir o ficheiro FILE_PEDIDO_VACINA");
		exit(0);
	}
	if(access(FILE_PEDIDO_VACINA, R_OK)== -1){
		erro("S5.1) Não foi possível ler o ficheiro FILE_PEDIDO_VACINA");
		exit(0);
	}
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
		strcpy(matrixdata[i], token);
		token = strtok(NULL, ":");
		i++;
	}
	pessoa.num_utente=atoi(matrixdata[0]);	
	strcpy(pessoa.nome, matrixdata[1]);
	pessoa.idade=atoi(matrixdata[2]);
	strcpy(pessoa.localidade, matrixdata[3]);
	strcpy(pessoa.nr_telemovel, matrixdata[4]);
	pessoa.estado_vacinacao=atoi(matrixdata[5]);
	pessoa.PID_cidadao=atoi(matrixdata[6]);


	debug("modified string: %s", modified);
	debug("intact string: %s", completedata);
    debug("Número de utente: %s", matrixdata[0]);
    debug("Nome: %s", matrixdata[1]);
    debug("Idadade: %s", matrixdata[2]);
    debug("Localidade: %s", matrixdata[3]);
    debug("Nº de telemóvel: %s", matrixdata[4]);
    debug("Estado Vacinação: %s", matrixdata[5]);
    debug("PID: %s", matrixdata[6]);
	sucesso("S5.1) Dados Cidadão: %d; %s; %d; %s; %s; 0", pessoa.num_utente, pessoa.nome, pessoa.idade, pessoa.localidade, pessoa.nr_telemovel);
}

/* Handler para tratar o sinal SIGCHLD. No início faz um wait para conseguir obter o PID do filho que enviou o sinal SIGCHLD. Com este PID pesquisamos no array de vagas 
   até encontrarmos a que tem o mesmo PID. Depois limpa a vaga, mete a disponibilidade do enfermeiro que estava nessa vaga a 1, incrementa o numero de vacinas dadas por esse
   enfermeiro. Por fim escreve os atributos todos do enfermeiro da vaga para o ficheiro binário na posição correta */

void wakeUp(){
	int PID_Filho=wait(NULL);
	debug("PID_FILHO: %d", PID_Filho);
	FILE *enfdat;
	int i=0;
	while(vagas[i].PID_filho != PID_Filho){
		debug("\e[0;104mNºVaga no while do WakeUp\e[0m", i);
		i++;
	}

	debug("Vaga_index para eliminar: %d", i);
	int j=vagas[i].index_enfermeiro;
	debug("Vaga para fazer reset: %d", i);
	debug("NUM_VAC_DADAS enf %s: %d", distruct[j].nome, distruct[j].num_vac_dadas);
	debug("Index enfermeiro WakeUp: %d", vagas[i].index_enfermeiro);
	vagas[i].index_enfermeiro=-1;
	debug("Index enfermeiro WakeUp after: %d", vagas[i].index_enfermeiro);
	sucesso("S5.5.3.1) Vaga %d que era do servidor dedicado %d libertada", i, PID_Filho);
	debug("Enf Disponibilidade WakeUp: %d", distruct[j].disponibilidade);
	distruct[j].disponibilidade=1;
	debug("Enf Disponibilidade WakeUp after: %d", distruct[j].disponibilidade);
	sucesso("S5.5.3.2) Enfermeiro %d atualizado para disponível", j);
	debug("Num Vac dadas WakeUp: %d", distruct[j].num_vac_dadas);
	distruct[j].num_vac_dadas++;
	sucesso("S5.5.3.3) Enfermeiro %d atualizado para %d vacinas dadas", j, distruct[j].num_vac_dadas);
	enfdat=fopen(FILE_ENFERMEIROS, "rb+");
	fseek(enfdat,sizeof(Enfermeiro)*j,SEEK_SET);
	fwrite(&distruct[j], sizeof(Enfermeiro), 1, enfdat);
	fseek(enfdat, 0, SEEK_SET);
	fclose(enfdat);
	sucesso("S5.5.3.4) Ficheiro FILE_ENFERMEIROS %d atualizado para %d vacinas dadas", j, distruct[j].num_vac_dadas);
	sucesso("S5.5.3.5) Retorna");

}

/* Handler para quando os processos filhos receberem o sinal SIGTERM*/

void terminate(){
	sucesso("S5.6.1) SIGTERM recibido, servidor dedicado termina Cidadão");
	kill(vagas[vaga_index].cidadao.PID_cidadao, SIGTERM);
	kill(getpid(), SIGKILL);
}

/* O GRANDE E ÚNICO TERMINATOR!! Percorre todo o array onde estão guardados os PID's dos filhos e manda um sinal de SIGTERM a todos. */

void THETERMINATOR(){
	debug("NUM_CHILDS: %d, estou no PID: %d", num_childs, getpid());
	for(int i=0; i<num_childs; i++){
		debug("Process Finished: %d", DedicatedServer[i]);
		kill(DedicatedServer[i], SIGTERM);
	}	
	remove(FILE_PID_SERVIDOR);
	kill(getpid(), SIGKILL);
}

int main () {
	FILE *spid;
	int sz;
    spid=fopen(FILE_PID_SERVIDOR, "w+");										  /*													*/
	fprintf(spid, "%d\n", getpid());											  /*													*/
	fclose(spid);															   	  /*													*/
	if (!access(FILE_PID_SERVIDOR, F_OK))										  /*   Escreve no Ficheiro FILE_PID_SERVIDOR o seu PID	*/
		sucesso("S1) Escrevi no ficheiro FILE_PID_SERVIDOR o PID: %d", getpid()); /*													*/
	else{																		  /*													*/
		erro("S1) Não consegui registar o servidor!");							  /*													*/
		exit(1);																  /*													*/
	}																			  /*													*/
	
	if(access(FILE_ENFERMEIROS, F_OK) == -1){
		erro("S2) Não consegui ler o ficheiro FILE_ENFERMEIROS!");
		exit(1);
	}

	/* Aqui o código vai ao ficheiro FILE_ENFERMEIROS e retira cada enfermeiro pela ordem que aparece no ficheiro, guardando os na  estrutura dinâmica */
	
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
	if(spid==NULL){
		debug("No file");
		exit(1);
	}
	fread(distruct, sizeof(Enfermeiro), numenf, spid);

	for(int i=0; i<numenf;i++){
		debug("\e[0;106mCed_profissional: %d\e[0m", distruct[i].ced_profissional);
		debug("\e[0;106mNome: %s\e[0m", distruct[i].nome);
		debug("\e[0;106mCS_enfermeiro: %s\e[0m", distruct[i].CS_enfermeiro);
		debug("\e[0;106mnum_vac_dadas: %d\e[0m", distruct[i].num_vac_dadas);
		debug("\e[0;106mdisponibilidade: %d\e[0m", distruct[i].disponibilidade);
	}
	
	/* Limpa as entradas de todas as vagas */

	for(int i=0; i<NUM_VAGAS; i++)			
		vagas[i].index_enfermeiro=-1;
		sucesso("S3)Iniciei a lista de %d vagas", NUM_VAGAS);
		signal(SIGUSR1, vacina);
		debug("\e[0;102m %d bf while\e[0m", pedido);

	/* Parte do código que é realizada sempre que o servidor aguarda um pedido, e apenas sai do while quando a variável pedido for diferente de 0 ou seja quando receber um pedido
	   como visto anteriormente. Logo a seguir muda esta variável para zero. */
espera:
		while( pedido == 0){
			debug("\e[0;102m %d in while\e[0m", pedido);
			sucesso("S4) Servidor espera pedidos");
			pause();
		}
		debug("\e[0;102m %d after while\e[0m", pedido);
		debug("\e[0;102mEntrei aqui\e[0m");
		pedido=0;

		
		char CS[]= "CS";
		debug("Localidade sem CS: %s", pessoa.localidade);
		strcat(CS,pessoa.localidade);
		debug("Localidade cidadão: %s", CS);
		debug("PID cidadao: %d", pessoa.PID_cidadao);
	
		/* Procura na estrutura dinâmica distruct se existe algum enfermeiro atribuido ao centro de saúde do cidadão. Caso exista verifica a sua disponibilidade */
		for(int i=0; i<numenf; i++){
			debug("Número de enfermeiro: %d", i);
			debug("CSEnfermeiro: %s", distruct[i].CS_enfermeiro);
			if(!strcmp(distruct[i].CS_enfermeiro, CS)){
				debug("CS enfermeiro: %s", distruct[i].CS_enfermeiro);
				debug("Disponibilidade: %d", distruct[i].disponibilidade);
				debug("Nome: %s", distruct[i].nome);
				debug("Número de enfermeiro: %d", i);
				debug("\e[0;102mNUM_VAC_DADAS por %s: %d\e[0m", distruct[i].nome, distruct[i].num_vac_dadas);	
				if(distruct[i].disponibilidade==1){
					debug("Entrei no if disp=1!");
					debug("Número de enfermeiro: %d", i);
					sucesso("S5.2.1) Enfermeiro %d disponível para o pedido %d", i, pessoa.PID_cidadao);

					/* Caso exista enfermeiro para o CS e esta esteja disponível então procura no array de vagas a primeira que está vaga e ocupa-a, indo para a etiqueta sucesso */
					for(vaga_index ; vaga_index<NUM_VAGAS; vaga_index++){
						debug("Número da vaga: %d", vaga_index);
						debug("Vaga ocupada ?: %d", vagas[vaga_index].index_enfermeiro);

						if(vagas[vaga_index].index_enfermeiro==-1){
							sucesso("S5.2.2) Há vaga para vacinação para o pedido %d", pessoa.PID_cidadao);
							vagas[vaga_index].index_enfermeiro=i;
							debug("Vaga ocupada ?: %d", vagas[vaga_index].index_enfermeiro);
							vagas[vaga_index].cidadao=pessoa;
							distruct[i].disponibilidade=0;
							debug("Disponibilidade enf depois: %d", distruct[i].disponibilidade);
							debug("Vagas enf index: %d", vagas[vaga_index].index_enfermeiro);
							sucesso("S5.3) Vaga nº %d preenchida para o pedido %d", vaga_index, pessoa.PID_cidadao);
							goto sucesso;
						}
					}

					/* Se não houver vagas corre estas linhas de código e volta para a etiqueta espera, ou seja para o while à espera de pedidos */
					erro("S5.2.2) Não há vaga para vacinação para o pedido %d", pessoa.PID_cidadao);
					kill(pessoa.PID_cidadao, SIGTERM);
					goto espera;

				}else{
			
					/* Se o enfermeiro não estiver disponível manda um SIGTERM ao processo cidadao */
					erro("S5.2.1) Enfermeiro %d indisponível para o pedido %s para o Centro de Saúde %s", i, pessoa.PID_cidadao, distruct[i].CS_enfermeiro);	
					kill(pessoa.PID_cidadao, SIGTERM);
					goto espera;
				}
			}
		}		/* Não sei se haveria de adicionar mas adicionei, esta parte é caso não exista nenhum enfermeiro para a localidade selecionada */
				erro("Não existe enfermeiro para %s", pessoa.localidade);
				kill(pessoa.PID_cidadao, SIGUSR2);
				goto espera;
			
sucesso:debug("Só entro aqui quando tiver preenchido uma vaga!"); 

		/* Cada fork() que seja corrido irá guardar o seu retorno no array DedicatedServer, ficando lá um registo de todos os PID's dos filhos */
		DedicatedServer[num_childs] = fork();
		if(DedicatedServer[num_childs]<0){
			erro("S5.4) Não foi possível criar o servidor dedicado");
		}else{
			if(DedicatedServer[num_childs]>0){
			
				/* Código corrido pelo servidor Pai */

				debug("\e[0;101mEstou no PID PAI: %d\e[0m", getpid());
				signal(SIGINT, THETERMINATOR);

				num_childs++; /* Incrementa um ao número de filhos, ou seja, DedicatedServer[num_childs] aponta para a primeira posição vazia e DedicatedServer[num_childs--] aponta
								 para a última posição preenchida*/

				sucesso("S5.4) Servidor dedicado %d criado para o pedido %d", DedicatedServer[num_childs-1], pessoa.PID_cidadao);
				vagas[vaga_index].PID_filho=DedicatedServer[num_childs-1];
				sucesso("S5.5.1) Servidor dedicado %d na vaga %d", DedicatedServer[num_childs-1], vaga_index);
				signal(SIGCHLD, wakeUp);
				sucesso("S5.5.2) Servidor aguarda fim do servidor dedicado %d", DedicatedServer[num_childs-1]);
				goto espera;
			}else{
				
				/* Código corrido pelo filho */
	
				debug("\e[0;101mEstou no PID FILHO: %d\e[0m", getpid());
				debug("\e[0;102mVaga nº%d para PID:%d\e[0m", vaga_index, getpid());
				signal(SIGTERM, terminate);	
				kill(vagas[vaga_index].cidadao.PID_cidadao, SIGUSR1);
				sucesso("S5.6.2) Servidor dedicado inicia consulta de vacinação");
				sleep(TEMPO_CONSULTA);
				sucesso("S5.6.3) Vacinação terminada para o cidadão com o pedido nº %d", vagas[vaga_index].cidadao.PID_cidadao);
				kill(vagas[vaga_index].cidadao.PID_cidadao, SIGUSR2);
				sucesso("S5.6.4) Servidor dedicado termina consulta de vacinação");
				debug("A matar: %d", getpid());
				debug("Vaga_Index em FILHO: %d é: %d", getpid(), vaga_index);	
				kill(getpid(), SIGKILL);
			}
		}
}		/*|*/
		/*|*/
		/*|*/	
		/*|*/
		/*!*/
		/* Peço desculpa se o código estiver confuso, mas espero que os meus comentários tenham-no ajudado! */
