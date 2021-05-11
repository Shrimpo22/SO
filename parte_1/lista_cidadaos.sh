#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº:98654		 Nome:Vasco do Nascimento Paiva Araújo 
## Nome do Módulo: lista_cidadaos.sh
## Descrição/Explicação do Módulo: Ao longo do código existem comentários,
## ajudando à coerência do mesmo.
##
###############################################################################


cl=1 #<--current line number

	if ! [ -f ./listagem.txt ]; then
		printf "\e[0;31mErro: Não existe o ficheiro listagem.txt!\n\e[0m"
		exit 1
	fi

# Sempre que o ficheiro cidadaos.txt existir, elimina-o.

	if [ -f ./cidadaos.txt ]; then
		rm cidadaos.txt 
	fi

# Este ciclo while irá ler cada linha do ficheiro listagem.txt individualmente.

    while read  line; do

# Cada variável a seguir definida serve para depois juntar tudo mais facilmente.
# Todas elas, exceto o 'usernum' (que apenas pega na variável 'current line number' e soma 10000),
# funcionam cortando, consoante o necessário, o conteúdo do comando 'echo "$line"' 
# que é justamente a linha que se quer no momento.

        name=$(echo "$line" | cut -f1,2 -d ' ' | cut -f2 -d ':')

        usernum=$(( ${cl}+10000 ))

        age=$(( 2021-$(echo "$line" | cut -f6 -d ' ' | cut -f3 -d '-') ))

        city=$(echo "$line" | cut -f8 -d ' ' | cut -f2 -d ':')

        telefonenum=$(echo "$line" | cut -f10 -d ' ' | cut -f2 -d ':')

# O comando abaixo monta as variáveis acima e reencaminha o resultado para o ficheiro 'cidadaos.txt'

        echo $usernum:$name:$age:$city:$telefonenum:0 >> cidadaos.txt

# No final de todas as variáveis estarem montadas o cl incrementa um, de modo a
# dizer o número da próxima linha que será usada pelo ciclo while

        cl=$((${cl}+1))

    done < listagem.txt

printf "\n\n\e[1;93mConteudo cidadaos.txt\e[0m\n\e[0;36mUserN:Name:Age:City:TelefoneNum:VacStatus\e[0m\n\e[1;32m===========================================\e[0m\n$(cat cidadaos.txt)\n\n"

