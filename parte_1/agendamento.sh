#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº:98654     Nome:Vasco do Nascimento Paiva Araújo 
## Nome do Módulo: agendamento.sh
## Descrição/Explicação do Módulo: Comentários ao longo do código :) 
##
##
###############################################################################

# Se existir um ficheiro agenda.txt então apaga-o
	if [ -f ./agenda.txt ]; then
		rm agenda.txt
	fi

# De seguida, existem dois ciclos while read, o script entra no primeiro que lê as linhas
# individualmente do ficheiro enfermeiros.txt e verifica, através do if, se nessa linha
# o enfermeiro em questão está disponível (1). Se tiver então cria variàveis para cada valor
# necessário (nº de cédula, nome do enfermeiro e CS associado).
# No segundo ciclo while read, dentro do primeiro, serve para ler as linhas do ficheiro cidadaos.txt
# que tenham o CS associado ao enfermeiro selecionado no ciclo anterior. Depois 
# guarda as variáveis que são necessárias (Nome do cidadao, Número de Utente). Por fim, monta as variáveis
# todas e mete o resultado dessa linha no ficheiro agenda.txt 

	if ! [ -f ./cidadaos.txt ]; then
		printf "\e[0;31mErro: Não existe o ficheiro Cidadaos.txt!\n\e[0m"
		exit 1
	fi
	if ! [ -f ./enfermeiros.txt ]; then
		printf "\e[0;31mErro: Não existe o ficheiro enfermeiros.txt!\n\e[0m"
		exit 1
    fi

	while read line; do
		if [ $(echo "$line" | cut -f5 -d':') -eq 1 ]; then
			CerNum=$(echo "$line" | cut -f1 -d':')
			EnfName=$(echo "$line" | cut -f2 -d':')
			CS=$(echo "$line" | cut -f3 -d':')
cat cidadaos.txt | grep -w ${CS:2} | while read line; do

# O if a seguir serve para garantir que o argumento ${CS:2} encontra-se 
# exclusivamente na coluna da localidade. 
				if [ $(echo "${line}" | cut -f4 -d':') = ${CS:2} ]; then
					UserName=$(echo "$line" | cut -f2 -d':')
					UserNum=$(echo "$line" | cut -f1 -d':')
					printf "${EnfName}:${CerNum}:${UserName}:${UserNum}:${CS}:$(date --rfc-3339 date)\n" >> agenda.txt
				fi
			done
		fi
	done < enfermeiros.txt 
	
# Pergunta ao utilizador se pretende ver o ficheiro criado se tiver sido criado um ficheiro agenda.txt.
 
	if [ -f ./agenda.txt ]; then
		printf "\e[0;36mSucesso! Deseja ver o conteúdo do ficheiro agenda.txt criado? (y/n)\e[0m"
		read answer
		if [ ${answer} = "y" ]; then
			printf "\n\n\e[1;93mConteudo agenda.txt\e[0m\n\e[1;32m===========================================\e[0m\n$(cat agenda.txt)\n\n"
		fi
	fi
echo ${worked}
