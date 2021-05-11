#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº:98654     Nome:Vasco do Nascimento Paiva Araújo 
## Nome do Módulo: menu.sh
## Descrição/Explicação do Módulo: Comentários espalhados pelo código para
## ajudar na leitura do mesmo! :)
##
###############################################################################

# Todos os 'printf' iguais a este apenas servem para mostrar o menu e pedir a escolha do utilizador.

printf "\e[1;93m\n    Hello World! :)\e[0m\n\e[1;97m===============================\n\n\n\e[0m\e[0;97m  1. Listar cidadãos\n  2. Adicionar enfermeiro\n  3. Stats\n  4. Agendar vacinação\n  0. Sair\n\n\e[0m\e[1;97m===============================\n\n\e[0m\033[97;5mType the number of the choice:\033[0m"

# Este ciclo while read irá ler cada input que o utilizador dá em resposta ao menu.

while read choice; do
		
		# O if imediatamente abaixo verifica se o valor introduzido pelo utilizador é um inteiro, de um digito, entre 0 e 4. Se n-ao for dará uma mensagem de erro, mostrando 
		# novamente o menu.

		if ! [[ ${choice} =~ ^[0-4]$ ]]; then
			printf "\e[1;91mNão foi inserido um número das escolhas disponíveis\e[0m\n"
			break
		fi
		
		# Os seguintes ifs executam os scripts consoante a escolha do utilizador pedindo ou não argumentos.

		if [ ${choice} -eq 1 ]; then
			./lista_cidadaos.sh
			printf "\n\e[1;97m===============================\n\n\n\e[0m\e[0;97m  1. Listar cidadãos\n  2. Adicionar enfermeiro\n  3. Stats\n  4. Agendar vacinação\n  0. Sair\n\n\e[0m\e[1;97m===============================\n\n\e[0m\033[97;5mType the number of the choice:\033[0m"

		fi
		
		if [ ${choice} -eq 2 ]; then
			printf "Preciso de argumentos\nPrimeiro e Último Nome:" 
			read nome
			printf "Nº de Cédula Profissional:"
			read EnfNum
			printf "Centro de Saúde Associado \"CSLocalidade\":"
			read CS
			printf "Disponibilidade(0-indiponível, 1-disponível):"
			read Disp
			./adiciona_enfermeiros.sh "${nome}" ${EnfNum} ${CS} ${Disp}
			printf "\n\e[1;97m===============================\n\n\n\e[0m\e[0;97m  1. Listar cidadãos\n  2. Adicionar enfermeiro\n  3. Stats\n  4. Agendar vacinação\n  0. Sair\n\n\e[0m\e[1;97m===============================\n\n\e[0m\033[97;5mType the number of the choice:\033[0m"
		fi
		
		if [ ${choice} -eq 3 ]; then
			printf "Preciso de argumentos:\n  1. cidadaos <localidade>\n  2. registados\n  3. enfermeiros\n  0. Cancelar\n"
			read arguments2
			if ! [[ ${arguments2} =~ ^[0-3]$ ]]; then
				printf "\e[0;31mNão foi introduzido uma opção válida\e[0m"      
			fi
			if [ ${arguments2} = 1 ]; then
				printf "\e[0;36mLocalidade:\e[0m"
				read localidade
				./stats.sh cidadaos ${localidade}
			fi
			if [ ${arguments2} = 2 ]; then
				./stats.sh registados
			fi
			if [ ${arguments2} = 3 ]; then
				./stats.sh enfermeiros
			fi

			printf "\n\e[1;97m===============================\n\n\n\e[0m\e[0;97m  1. Listar cidadãos\n  2. Adicionar enfermeiro\n  3. Stats\n  4. Agendar vacinação\n  0. Sair\n\n\e[0m\e[1;97m===============================\n\n\e[0m\033[97;5mType the number of the choice:\033[0m"

		fi
		
		if [ ${choice} -eq 4 ]; then
			./agendamento.sh
			printf "\n\e[1;97m===============================\n\n\n\e[0m\e[0;97m  1. Listar cidadãos\n  2. Adicionar enfermeiro\n  3. Stats\n  4. Agendar vacinação\n  0. Sair\n\n\e[0m\e[1;97m===============================\n\n\e[0m\033[97;5mType the number of the choice:\033[0m"

		fi
		
		# Quando o utilizador responde ao menu com '0' então este sai do ciclo e mostra uma mensagem de adeus

		if [ ${choice} -eq 0 ]; then
			break
		fi

	done

printf "\e[0;93mGoodbye! :)\n\e[0m"





