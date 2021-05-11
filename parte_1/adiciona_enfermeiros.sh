#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº:98654   Nome:Vasco do Nascimento Paiva Araújo 
## Nome do Módulo: adiciona_enfermeiros.sh
## Descrição/Explicação do Módulo: Foram colocados ao longo do código
## comentários de modo a facilitar a compreensão do mesmo.
##
###############################################################################


# Este ciclo if irá verificar se foram dados exatamente 4 argumentos, se isto
# não ocorrer devolverá uma mensagem de erro e encerrará o script.
if [ $# -eq 4 ]; then
	
# Este ciclo if, e os próximos irão verificar se o argumento passado é uma 
# string ou um número inteiro, consoante o pedido no enunciado. Se algum 
# dos argumentos passados não estiver como pedido irá lançar um erro 
# apropriado e encerrar o script. Se estiver tudo certo associará os argumentos
# a variáveis.

	if ! [[ $1 =~ ^[0-9]+$ ]]; then
		name=$1
		if [[ $2 =~ ^[0-9]{5}$ ]]; then
			num=$2
			if  [[ ! ($3 =~ ^[0-9]+$) ]] && [[ $3 =~ ^CS[a-zA-Z]+$ ]]; then
				cs=$3
				if [[ $4 =~ ^[0-1]$ ]]; then
					disp=$4
				else
					printf "\e[0;31mErro: "$4"não é um inteiro ou não está no intervalo 0-1\n\e"
					exit 1
				fi
			else
				printf "\e[0;31mErro: "$3" não é uma  string ou input invalid "CS*"\n\e[0m"
				exit 1
			fi
		else
			printf "\e[0;31mErro: "$2" não é um inteiro ou é menor ou maior que 5 algarismos\n\e[0m"
			exit 1
		fi
	else
		printf "\e[0;31mErro: "$1" não é uma string\n\e[0m"
		exit 1
	fi
else
	printf "\e[0;31mErro: Síntaxe: $0 <número cédula profissional>:<nome>:<centro saúde associado>:<nº de vacinações efetuadas>:<disponibilidade>\n\e[0m"
	exit 1
fi

# Se tudo acima tiver corrido bem iniciará esta parte do script.
# Apenas se já existir um fichiero enfermeiros.txt é que o script verifica se existe
# algum problema com o registo do enfemeiro.
if [ -f ./enfermeiros.txt ]; then
	if	cat enfermeiros.txt | grep $cs -q; then
		printf "\e[0;31mErro: O Centro de Saúde já tem um enfermeiro registado\n\e[0m"
		exit 1
	else
		if cat enfermeiros.txt | grep $num -q; then
			printf "\e[0;31mErro: O enfermeiro já está registado\n\e[0m"
			exit 1
		else
			echo $num:$name:$cs:0:$disp >> enfermeiros.txt
			printf "\e[1;93m\nConteudo enfermeiros.txt\n\e[0m\e[0;36mCerNum:Name:AssoCS:VacStatus:Avail\n\e[0m\e[1;32m=================================\n\e[0m$(cat enfermeiros.txt)\n\n"
			exit 1
		fi
	fi
fi

echo $num:$name:$cs:0:$disp >> enfermeiros.txt
            printf "\e[1;93m\nConteudo enfermeiros.txt\n\e[0m\e[0;36mCerNum:Name:AssoCS:VacStatus:Avail\n\e[0m\e[1;32m=================================\n\e[0m$(cat enfermeiros.txt)\n\n"




