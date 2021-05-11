/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº:98654   Nome:Vasco do Nascimento Paiva Araújo 
 ** Nome do Módulo: cidadao.c
 ** Descrição/Explicação do Módulo: Comentários ao longo do código :) 
 **
 **
 ******************************************************************************/
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* Variável que indica se o ficheiro FILE_PEDIDO_VACINA existe ou não */
int exists;

/* A Seguinte função serve para pedir ao cidadão os seus dados, criando uma entrada nova para a estructura 'Cidadao' com as respostas. 
   No final retorna à função que a chamou, esse mesmo cidadão.																		*/
Cidadao setCitizen(){
	Cidadao P;
	printf("Número de utente:");
	scanf("%d", &P.num_utente);
	printf("Nome:");
	my_gets(P.nome, 100);
	printf("Idade:");
	scanf("%d", &P.idade);
	printf("Localidade:");
	my_gets(P.localidade, 100);
	printf("Nº de telemóvel:");
	my_gets(P.nr_telemovel, 10);
	P.estado_vacinacao = 0;
	P.PID_cidadao = getpid();
	sucesso("C1) Dados Cidadão: %d; %s; %d; %s; %s; %d", P.num_utente, P.nome, P.idade, P.localidade, P.nr_telemovel, P.estado_vacinacao);
	sucesso("C2) PID Cidadão: %d", P.PID_cidadao);
	return(P);
	
}

/* A seguinte função é um handler para o sinal SIGALRM. Apenas serve para verificar se o ficheiro FILE_PEDIDO_VACINA já foi apagado ou não.
   Se tiver sido já apagado então muda a variável exists para -1 indicando que o ficheiro já não existe */
void waitFile(){
	debug("File still exists...");
	if(access(FILE_PEDIDO_VACINA, F_OK) == -1){
		debug("File does not exist!!!");
		exists = -1;
	}
}

/* Handler para o sinal SIGTERM */
void notPossible(){
	sucesso("C9) Não é possível vacinar o cidadão no pedido nº %d", getpid());
	remove(FILE_PEDIDO_VACINA);
	kill(getpid(),SIGKILL);
}
/* Handler para o sinal SIGUSR2 */
void finished(){
	sucesso("C8) Vacinação do cidadão com o pedido nº %d concluída", getpid());
	kill(getpid(),SIGKILL);
}
/*Handler para o sinal SIGUSR1*/
void process(){
	sucesso("C7) Vacinação do cidadão com o pedido nº %d em curso", getpid());
	remove(FILE_PEDIDO_VACINA);
}
/*Handler para o sinal SIGINT */
void cancel(){
	sucesso("C5) O cidadão cancelou a vacinação, o pedido nº%d foi cancelado", getpid());
	remove(FILE_PEDIDO_VACINA);
	kill(getpid(),SIGKILL);
}

int main(){
	FILE *fpv;				/* Inicialização de variáveis que serão usadas posteriormente */
	int ServerPID;
	char PIDString[100]; 
	Cidadao Pessoa;			/* Inicializa uma entrada na estructura Cidadao e depois preenche-a chamando a função setCitizen() */
	Pessoa = setCitizen();

	
	/* A porção de código seguinte apenas verifica se já existe um ficheiro FILE_PEDIDO_VACINA, se existir então irá armar o sinal SIGALRM e a cada 5 segundos irá verificar de novo se o ficheiro existe ou não. Ficará neste loop até deixar de existir o ficheiro */
	if( !access( FILE_PEDIDO_VACINA, F_OK)){
		exists=0;
		erro("C3) Não é possível iniciar o processo de vacinação neste momento");
		signal(SIGALRM,waitFile);
		while(exists == 0){
			debug("Still in cycle...");
			alarm(5);
			pause();
		}
	}

	/* Se o código chega aqui então abre o ficheiro FILE_PEDIDO_VACINA e escreve lá dentro os dados do Cidadão criado no início */
	sucesso("C3) Ficheiro FILE_PEDIDO_VACINA pode ser criado");
	fpv=fopen(FILE_PEDIDO_VACINA, "w");
	fprintf(fpv, "%d:%s:%d:%s:%s:%d:%d\n",Pessoa.num_utente, Pessoa.nome, Pessoa.idade, Pessoa.localidade, Pessoa.nr_telemovel, Pessoa.estado_vacinacao, Pessoa.PID_cidadao );
	fclose(fpv);
	if( !access( FILE_PEDIDO_VACINA, F_OK)){
		sucesso("C4) Ficheiro FILE_PEDIDO_VACINA criado e preenchido");
	}else{
		erro("C4) Não é possível criar o ficheiro FILE_PEDIDO_VACINA");
}
		
	signal(SIGINT, cancel);
	
	/* Aqui o código irá ler do ficheiro FILE_PID_SERVIDOR o PID do servidor a quem vai mandar o sinal SIGUSR1 */
	if(!access(FILE_PID_SERVIDOR, F_OK)){
		fpv=fopen(FILE_PID_SERVIDOR, "r");
		my_fgets(PIDString,100, fpv);
		ServerPID = atoi(PIDString);
		fclose(fpv);
		debug("ServerPID to signal: %d", ServerPID);
		kill(ServerPID, SIGUSR1);
		sucesso("C6) Sinal enviado ao Servidor: %d", ServerPID);
	}else{
		erro("C6) Não existe ficheiro FILE_PID_SERVIDOR!");
	}
	
	/* Arma os sinais a seguir */
	signal(SIGUSR1, process);
	signal(SIGUSR2, finished);
	signal(SIGTERM, notPossible);
	
	/* No final (apesar de não ser pedido) ponho o processo cidadao on hold, porque senão não daria para receber sinais do processo servidor! */
	while(1){
		sucesso("A processar...");
		pause();
	}
}







