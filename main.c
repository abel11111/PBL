#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Gestão de Eventos ---\n");
        printf("1. Apresentação de Eventos\n");
        printf("2. Emitir Bilhete Eletrônico\n");
        printf("3. Exibir Número de Inscritos por Evento\n");
        printf("4. Gestão de Inscrições em Eventos com Vagas Limitadas\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("apresentacaoEventos");
                break;
            case 2:
                printf("emitirBilheteEletronico");
                break;
            case 3:
                printf("NumeroInscritos");
                break;
            case 4:
                printf("gestaoInscricoes");
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