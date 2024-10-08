/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos
 **
 ** Aluno: Nº:98654       Nome: Vasco do Nascimento Paiva Araujo
 ** Nome do Módulo: cidadao.c v3
 ** Descrição/Explicação do Módulo: 
 **
 **
 ******************************************************************************/
#include "common.h"
#include "utils.h"
#include <signal.h>
#include <unistd.h>
#include <sys/msg.h>
#include "colors.h"
/* Variáveis globais */
int msg_id;             // ID da Fila de Mensagens usada
MsgCliente mensagem;    // Variável que tem a mensagem enviada do Cidadao para o Servidor
MsgServidor resposta;   // Variável que tem a mensagem de resposta enviadas do Servidor para o Cidadao

/* Protótipos de funções */
void init_ipc();                    // Função a ser implementada pelos alunos
void cria_mensagem();               // Função a ser implementada pelos alunos
void envia_mensagem_servidor();     // Função a ser implementada pelos alunos
void espera_resposta_servidor();    // Função a ser implementada pelos alunos
void trata_resposta_servidor();     // Função a ser implementada pelos alunos
void pedido();                      // Função a ser implementada pelos alunos
void vacina();                      // Função a ser implementada pelos alunos
void cancela_pedido(int);           // Função a ser implementada pelos alunos
void print_info(Cidadao cidadao);

int main() {    // Não é suposto que os alunos alterem nada na função main()
    // C1) Chama a função init_ipc(), que tenta abrir uma fila de mensagens IPC que tem a KEY IPC_KEY definida em common.h (alterar esta KEY para ter o valor do nº do aluno, como indicado nas aulas). Deve assumir que a fila de mensagens já foi criada. Se tal não aconteceu, dá erro e termina com exit status 1. Esta função, em caso de sucesso, preenche a variável global msg_id;
    init_ipc();
    // C2) Chama a função cria_mensagem()
    cria_mensagem();
    // C7) Arma e trata o sinal SIGINT para que, quando o utilizador interromper o processo Cidadão com <CTRL+C>, chame a função cancela_pedido()
    signal(SIGINT, cancela_pedido);
    // Faz o pedido ao servidor e aguarda a resposta do mesmo
    pedido();
    // C6) Inicia o processo de vacinação
    vacina();
}

/**
 * C1) tenta abrir uma fila de mensagens que tem a KEY IPC_KEY
 * definida em common.h (alterar esta KEY para ter o valor do nº do aluno, como indicado nas aulas).
 * Deve assumir que a fila de mensagens já foi criada.
 * Se tal não aconteceu, dá erro e termina com exit status 1.
 * Esta função, em caso de sucesso, preenche a variável global msg_id;
 */
void init_ipc() {
    debug("<");
		// Outputs esperados (os itens entre <> deverão ser substituídos pelos valores correspondentes):
		msg_id = msgget(IPC_KEY, 0);
		exit_on_error(msg_id, "C1) Fila de Mensagens com a Key definida não existe ou não pode ser aberta");
		sucesso("C1) Fila de Mensagens com a Key %x aberta com o ID %d", IPC_KEY, msg_id);
    debug(">");
}

/**
 * C2.1) Pede ao Cidadão (utilizador) os seus dados, nomeadamente o número de utente e nome, obrigatoriamente nessa ordem, preenchendo os dados na variável global mensagem;
 * C2.2) Preenche os campos PID_cidadao da variável global mensagem com o PID deste processo Cidadão, tipo da mensagem com o tipo 1, e pedido = PEDIDO;
 */
void cria_mensagem() {
    debug("<");

    // C2.1) Pede ao Cidadão (utilizador) os seus dados, nomeadamente o número de utente e nome, obrigatoriamente nessa ordem, preenchendo os dados na variável global mensagem;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
    printf("\tNúmero de Utente: ");
	scanf("%d", &mensagem.dados.num_utente);
	printf("\n\tNome: ");
	my_gets(mensagem.dados.nome, 100);
	sucesso("C2.1) Dados Cidadão: %d, %s", mensagem.dados.num_utente, mensagem.dados.nome);

    // C2.2) Preenche os campos PID_cidadao da variável global mensagem com o PID deste processo Cidadão, tipo da mensagem com o tipo 1, e pedido = PEDIDO;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
  	mensagem.dados.PID_cidadao = getpid();
	mensagem.tipo = 1;
	mensagem.dados.pedido = PEDIDO;
	sucesso("C2.2) PID Cidadão: %d", getpid());
    debug(">");
}

/**
 * Envia a mensagem para a fila de mensagens; em caso de erro no envio, termina com erro e exit status 1.
 */
void envia_mensagem_servidor() {
    debug("<");

    // Envia a mensagem que está na variável global mensagem para a fila de mensagens; em caso de erro, termina com erro e exit status 1.
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
	int	status = msgsnd(msg_id, &mensagem, sizeof(mensagem.dados), IPC_NOWAIT);
	exit_on_error(status, "Não é possível enviar mensagem para o servidor");
	sucesso("Mensagem para o servidor enviada");
    debug(">");
}

/**
 * Espera a resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta; 
 */
void espera_resposta_servidor() {
    debug("<");

    // Espera a resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta; 
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
    int status = msgrcv(msg_id, &resposta, sizeof(resposta.dados), getpid(), 0);
	exit_on_error(status, "Não é possível ler a resposta do servidor");
    sucesso("Servidor enviou resposta");
    debug(">");
}

/**
 * Envia o pedido ao servidor e aguarda a sua resposta
 */
void pedido() {
    debug("<");
	
	do{
        // C3) Envia um pedido de consulta de vacinação para o processo Servidor, chamando a função envia_mensagem_servidor(), que envia uma mensagem para a fila de mensagens com tipo 1, com pedido = PEDIDO e os dados do cidadão; em caso de erro, termina com erro e exit status 1.
        envia_mensagem_servidor();
		debug(GRNHB"Tipo Mensagem: %d" reset, mensagem.tipo);
        // C4) Chama a função espera_resposta_servidor(), que espera a resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta; em caso de erro, termina com erro e exit status 1.
        espera_resposta_servidor();

        // C5) O comportamento do processo Cidadão agora irá depender da resposta enviada pelo processo Servidor no campo status:
		if(resposta.dados.status == DESCONHECIDO){
        // C5.1) Se o status for DESCONHECIDO, imprime uma mensagem de erro, e termina com exit status 1;
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			erro("C5.1) Não existe registo do utente %d, %s", mensagem.dados.num_utente, mensagem.dados.nome);
			exit(1);
		}

		if(resposta.dados.status == NAOHAENFERMEIRO){
        // C5.1) Se o status for NAOHAENFERMEIRO, imprime uma mensagem de erro, e termina com exit status 1;
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			erro("C5.1) Não existe enfermeiro na localidade do utente %d, %s", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
			exit(1);
		}

		if(resposta.dados.status == VACINADO){
        // C5.2) Se o status for VACINADO, imprime uma mensagem de sucesso, e termina com exit status 0;
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			sucesso("C5.2) O utente %d, %s já foi vacinado", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
			exit(0);
		}
	
		if(resposta.dados.status == EMCURSO){
        // C5.2) Se o status for EMCURSO, imprime uma mensagem de sucesso, e termina com exit status 0;
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			sucesso("C5.3) A vacinação do utente %d, %s já está em curso", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
			exit(0);
		}
		
		if(resposta.dados.status == AGUARDAR){
        // C5.3) Se o status for AGUARDAR, imprime uma mensagem de sucesso, aguarda (sem espera ativa!) um tempo correspondente a TEMPO_ESPERA segundos, e depois retorna ao ponto C3;
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			sucesso("C5.4) Utente %d, %s, por favor aguarde...", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
			sleep(TEMPO_ESPERA);
		}

		if(resposta.dados.status == OK){
        // C5.4) Se o status for OK, imprime uma mensagem de sucesso, e depois vai para o ponto C6.
        // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
			sucesso("C5.5) Utente %d, %s, vai agora ser vacinado", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
		}
	}while (OK != resposta.dados.status);
    debug(">");
}

/**
 * Imprime informação sobre o utente
 * @param cidadao Cidadao a reportar
 **/
void print_info(Cidadao cidadao) {
    debug("<");
    printf("Número de utente: %d\n", cidadao.num_utente);
    printf("Nome            : %s\n", cidadao.nome);
    printf("Idade           : %d ano%s\n", cidadao.idade, cidadao.idade > 1 ? "s" : "");
    printf("Localidade      : %s\n", cidadao.localidade);
    printf("N.º Telemóvel   : %s\n", cidadao.nr_telemovel);
    printf("Vacina          : %dª dose\n", cidadao.estado_vacinacao + 1);
    debug(">");
}

/**
 * Espera a resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta; 
 */
void vacina() {
    debug("<");

    // C6.1) Chama a função print_info(cidadao) com a informação recebida na resposta do processo Servidor, que irá imprimir a informação completa sobre o cidadão que vai ser vacinado;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
    sucesso("C6.1) Dados completos sobre o cidadão a ser vacinado");
    print_info(resposta.dados.cidadao);

    // C6.2) Chama novamente a função espera_resposta_servidor(), que espera uma nova resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta;
	espera_resposta_servidor();
    // C6.3) O comportamento do processo Cidadão agora irá depender da resposta enviada pelo processo Servidor no campo status:
    if(resposta.dados.status == TERMINADA){
	// C6.3.1) Se o status for TERMINADA, imprime uma mensagem de sucesso, e termina com exit status 0;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
		sucesso("C6.3.1) Utente %d, %s vacinado com sucesso", resposta.dados.cidadao.num_utente, resposta.dados.cidadao.nome);
		exit(0);
	}
	
	if(resposta.dados.status == CANCELADA){
    // C6.3.2) Se o status for CANCELADA, imprime uma mensagem de erro, e termina com exit status 1;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
		erro("C6.3.2) O servidor cancelou a vacinação em curso");
		exit(1);
	}
    debug(">");
}

/**
 * C7) Quando o utilizador interromper o processo Cidadão com <CTRL+C>
 */
void cancela_pedido(int sinal) {
    debug("<");

    // C7.1) Escreve no ecrã uma mensagem;
    // Outputs esperados (os itens entre <> deverão ser substituídos pelos valores correspondentes):
    sucesso("C7.1) O cidadão cancelou a vacinação no processo %d", getpid());

    // C7.2) Altera a variável global mensagem, tornando pedido = CANCELAMENTO. Chama a função envia_mensagem_servidor(), que envia a mensagem para a fila de mensagens; em caso de erro no envio, afixa uma mensagem de erro e termina com exit status 1;
    mensagem.dados.pedido = CANCELAMENTO;
	envia_mensagem_servidor();

    // C7.3) Chama novamente a função espera_resposta_servidor(), que espera uma nova resposta do processo Servidor (na fila de mensagens com o tipo = PID_Cidadao) e preenche a mensagem enviada pelo processo Servidor na variável global resposta;
    espera_resposta_servidor();

    // C7.4) O comportamento do processo Cidadão agora irá depender da resposta enviada pelo processo Servidor no campo status:
	if(resposta.dados.status == CANCELADA){
    // C7.4.1) Se o status for CANCELADA, imprime uma mensagem de sucesso, e termina com exit status 0;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
		sucesso("C7.4.1) Servidor confirmou cancelamento");
		exit(0);
	}

	if(resposta.dados.status == TERMINADA){
    // C7.4.2) Se o status for TERMINADA, imprime mensagem sucesso, termina com exit status 0;
    // Outputs esperados (itens entre <> substituídos pelos valores correspondentes):
		sucesso("C7.4.2) A vacinação já tinha sido concluída");
		exit(0);
	}
    debug(">");
}
