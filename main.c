#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Gestão de Eventos ---\n");
        printf("1. Lista de Eventos\n");
        printf("2. \n");
        printf("3. Exibir Número de Inscritos por Evento\n");
        printf("4. Gestão de Inscrições em Eventos com Vagas Limitadas\n");
        printf("5. Registar Utilizador\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Lista de Eventos");
                break;
            case 2:
                printf("Criar Evento");
                break;
            case 3:
                printf("Gerir Evento");
                break;
            case 4:
                printf("Gerir inscricoes");
                break;
            case 5:
                printf("Registar Utilizador");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}


    int main() {
        menu();
        return 0;
    }