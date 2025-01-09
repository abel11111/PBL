#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Gestão de Eventos ---\n");
        printf("1. Lista de Eventos\n");
        printf("2. Criar Evento");
        printf("3. Gerir Evento\n");
        printf("4. Gestão de Inscrições\n");
        printf("5. Registar Utilizador\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
            if(opcao==1) 
                {
                printf("Lista de Eventos");
                }
            else
                if(opcao==2)
                {
                printf("Criar Evento");
                }
            else
                if(opcao==3)
                {
                printf("Gerir Evento");
                }
            else
                if(opcao==4){
                printf("Gerir inscricoes");
                }
            else
                if(opcao==6)
                {
                printf("Registar Utilizador");
                }
            else
                if(opcao==0)
                {
                printf("Saindo...\n");
                }
            else(opcao==7){
                printf("Opção inválida!\n");
                }
            
    } while (opcao != 0);
}


    int main() {
        menu();
        return 0;
    }