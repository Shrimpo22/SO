#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº:98654    Nome:Vasco do Nascimento Paiva Araújo
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: Comentários ao longo do código de forma a 
## uma melhor compreensão do código.
##
###############################################################################


# Este primeiro if serve para testar se não foi passado nenhum argumento.

	if [ $# -eq 0 ]; then
		printf "\e[0;91mErro: Nenhum Argumento Passado\n\e[0m"
		exit 1
	fi

# O if a seguir testa se foi passado no primeiro argumento a palavra 'cidadaos'.
# Se a condição acima retornar verdadeira então um novo if iniciar-se-á para testar
# se o segundo argumento é composto por inteiros ou se está vazio. Se retornar verdade
# para alguma destas condições retornará erro, saindo do código. Se retornar falso
# então irá cortar o conteudo do ficheiro cidadaos.txt na coluna da localidade e contar 
# todas as linhas que correpondam ao segundo argumento passado. 

	if [ $1 = cidadaos ]; then

# Só para verificar se o ficheiros cidadaos.txt existe		
		if ! [ -f ./cidadaos.txt ]; then
			printf "\e[0;31mErro: Não existe o ficheiro Cidadaos.txt!\n\e[0m"
		    exit 1
	    fi

		if [[ $2 =~ ^[0-9]+$ ]] || [ -z $2 ]; then
			printf "\e[0;91mErro: '$2' não é uma string\n\e[0m"
			exit 1
		fi
		num=$(cat cidadaos.txt | cut -f4 -d':' | grep -w ${2^} | wc -l)
		printf "\n\e[0;36mO número de cidadãos registados em "${2^}" é "$num"\n\n\e[0m"
		exit 1
	fi

# O seguinte if inicializa se o primeiro argumento passado for 'registados' e irá ler o ficheiro
# cidadaos.txt linha a linha, e guarda numa variavel 'line', dentro do if corta a linha de modo a
# apenas mostrar a idade e compara se é maior que 60, se for guarda essa linha. Depois do ciclo
# while read, irá dar como output o ficheiro temporário cortado de forma a mostrar apenas o pedido.

	if [ $1 = registados ]; then

# Só para verificar se o ficheiros cidadaos.txt existe		
		if ! [ -f ./cidadaos.txt ]; then
			printf "\e[0;31mErro: Não existe o ficheiro Cidadaos.txt!\n\e[0m"
		    exit 1
	    fi

		while read line; do
			if [ $(echo "$line" | cut -f3 -d':') -gt 60 ]; then
				echo "${line}" >> cidtemp.txt
			fi
		done < cidadaos.txt
		printf "\n\e[1;93mCidadãos Registados com mais de 60 anos do mais velho para o mais novo\n\e[0m\e[0;92mUserNum:NameFirstLast\n\e[0m\e[0;92m======================\n\e[0m$(cat cidtemp.txt | sort -t':' -k3 -nr | cut -f1,2 -d':' )\n\n"
	rm cidtemp.txt
	exit 1
fi

# No último if, o script testa se foi passado como argumento a palavra 'enfermeiros'.
# Se tiver sido passada, iniciará um ciclo while read, lendo todas as linhas individualmente
# do ficheiro enfermeiros.txt, verificando se o enfermeiro dessa linha está marcado como 
# disponível, se tiver então guardará a linha toda num ficheiro temporário e no final fará os cortes
# necessários para mostrar apenas os nomes dos enfermeiros disoníveis. Por fim apaga o ficheiro temporário

if [ $1 = enfermeiros ]; then

# Só para verificar se o ficheiro enfermeiros.txt existe
	 if ! [ -f ./enfermeiros.txt ]; then
        printf "\e[0;31mErro: Não existe o ficheiro enfermeiros.txt!\n\e[0m"
        exit 1
    fi

	while read line; do
		if [ $(echo "$line" | cut -f5 -d':') -eq 1 ]; then
			echo "$line" >> enftemp.txt
		fi
	done < enfermeiros.txt
	printf "\n\e[1;93mEnfermeiros registados disponíveis\n\e[0m\e[0;92m==============================\n\e[0m$(cat enftemp.txt | cut -f2 -d':')\n\n"
	rm enftemp.txt
	exit 1
fi

# Se o código não entrar em nenhum ciclo if, então retornará uma mensagem de erro

	printf "\e[0;91mErro Argumento Inválido\n\e[0m"
	exit 1

