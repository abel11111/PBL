#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


#define MAX_EVENTOS 100
#define MAX_INSCRICOES 100
#define MAX_ESPERA 100  


typedef struct {
    char nome[100];
    char dataHora[50];
    char local[100];
    int maxPessoas;   
    int numInscritos; 
    char filaEspera[MAX_ESPERA][100];  
    int filaEsperaInicio;  
    int filaEsperaFim;  
} Evento;

typedef struct {
    char Telemovel[100];
    char nomeEvento[100];
    char telefone[20];
} Inscricao;


void menuUtilizador(char *Telemovel, char *nomeUtilizador, Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes);
void menuAdministrador(Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes);
void criarEvento(Evento eventos[], int *totalEventos);
void listarEventos(Evento eventos[], int totalEventos);
void apagarEvento(Evento eventos[], int *totalEventos);
void inscreverEvento(char *utilizador, char *telefone, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes);
void verificarFilaDeEspera(int eventoIndex, Evento eventos[], Inscricao inscricoes[], int *totalInscricoes);
void removerInscricao(char *utilizador, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes);
void listarInscritosPorEvento(Evento eventos[], int totalEventos, Inscricao inscricoes[], int totalInscricoes);
void gerarBilheteEletronico(char *nomeUtilizador, char *Telemovel, Inscricao inscricao, Evento evento);

int validarTelemovel(const char *Telemovel) {
    for (int i = 0; Telemovel[i] != '\0'; i++) {
        if (!isdigit(Telemovel[i])) {
            return 0;  
        }
    }
    return 1;  
}

int validarNome(const char *nome) {
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') {
            return 0;  
        }
    }
    return 1;  
}

void menuUtilizador(char *Telemovel, char *nomeUtilizador, Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes) {
    while (!validarNome(nomeUtilizador)) {
        printf("Nome invÃ¡lido! O nome deve conter apenas letras e espaÃ§os.\n");
        printf("Por favor, insira novamente o nome: ");
        getchar(); 
        fgets(nomeUtilizador, 100, stdin);
        nomeUtilizador[strcspn(nomeUtilizador, "\n")] = '\0';
    }

    int opcao;

    do {
        printf("\n==================================");
        printf("\n===     Menu do Utilizador     ===");
        printf("\n===                            ===");
        printf("\n=== 1. Ver eventos             ===");
        printf("\n===                            ===");
        printf("\n=== 2. Inscrever-se em evento  ===");
        printf("\n===                            ===");
        printf("\n=== 3. Remover inscricao       ===");
        printf("\n===                            ===");
        printf("\n=== 4. Gerar bilhete eletranico===");
        printf("\n===                            ===");
        printf("\n=== 5. Voltar ao menu principal===");
        printf("\n==================================\n");
        printf("\n==>Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listarEventos(eventos, *totalEventos);
                break;
            case 2:
                inscreverEvento(Telemovel, nomeUtilizador, eventos, *totalEventos, inscricoes, totalInscricoes);
                break;
            case 3:
                removerInscricao(Telemovel, eventos, *totalEventos, inscricoes, totalInscricoes);
                break;
            case 4: {
                int encontrado = 0;
                for (int i = 0; i < *totalInscricoes; i++) {
                    if (strcmp(inscricoes[i].Telemovel, Telemovel) == 0) {
                        for (int j = 0; j < *totalEventos; j++) {
                            if (strcmp(inscricoes[i].nomeEvento, eventos[j].nome) == 0) {
                                gerarBilheteEletronico(nomeUtilizador, Telemovel, inscricoes[i], eventos[j]);
                                encontrado = 1;
                                break;
                            }
                        }
                    }
                }
                if (!encontrado) {
                    printf("Voce nao esta inscrito em nenhum evento.\n");
                }
                break;
            }
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void gerarQRCodeSimulado(const char *dados) {
    printf("\nCodigo QR (simulado):\n");
    for (size_t i = 0; i < strlen(dados); i++) {
        if (i % 5 == 0) {
            printf("\n");
        }
        printf("%c ", dados[i] == ' ' ? '#' : '*');  
    }
    printf("\n");
}

void gerarBilheteEletronico(char *nomeUtilizador, char *Telemovel, Inscricao inscricao, Evento evento) {
    char dadosQR[300];
    snprintf(dadosQR, sizeof(dadosQR), "Nome: %s\nTelemovel: %s\nEvento: %s\nData: %s\nLocal: %s",
             nomeUtilizador, Telemovel, evento.nome, evento.dataHora, evento.local);

    printf("\n==================================");
    printf("\n===     Bilhete Eletranico     ===");
    printf("\n==================================\n");
    printf("Nome: %s\n", nomeUtilizador);
    printf("Telemovel: %s\n", Telemovel);
    printf("Nome do Evento: %s\n", evento.nome);
    printf("Data e Hora: %s\n", evento.dataHora);
    printf("Local: %s\n", evento.local);
    gerarQRCodeSimulado(dadosQR);  
    printf("==================================\n");
}


// FunÃ§Ãµes restantes...

void removerInscricao(char *utilizador, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes) {
    if (*totalInscricoes == 0) {
        printf("Nenhuma inscricao encontrada para remover.\n");
        return;
    }

    printf("\n=== Remover Inscricao ===\n");
    int indiceEvento;
    listarEventos(eventos, totalEventos);
    printf("Digite o nÃºmero do evento do qual deseja remover sua inscricao: ");
    scanf("%d", &indiceEvento);

    if (indiceEvento < 1 || indiceEvento > totalEventos) {
        printf("Numero de evento invalido. Operacao cancelada.\n");
        return;
    }

    Evento *eventoSelecionado = &eventos[indiceEvento - 1];

    
    int encontrado = 0;
    for (int i = 0; i < *totalInscricoes; i++) {
        if (strcmp(inscricoes[i].Telemovel, utilizador) == 0 && strcmp(inscricoes[i].nomeEvento, eventoSelecionado->nome) == 0) {
            
            encontrado = 1;
            for (int j = i; j < *totalInscricoes - 1; j++) {
                inscricoes[j] = inscricoes[j + 1];
            }
            (*totalInscricoes)--;
            eventoSelecionado->numInscritos--;

            printf("Inscricao removida com sucesso!\n");

            
            verificarFilaDeEspera(indiceEvento - 1, eventos, inscricoes, totalInscricoes);
            break;
        }
    }

    if (!encontrado) {
        printf("Voce nao esta inscrito neste evento.\n");
    }
}

void criarEvento(Evento eventos[], int *totalEventos) {
    if (*totalEventos >= MAX_EVENTOS) {
        printf("Limite de eventos atingido. Nao e possi­vel criar mais eventos.\n");
        return;
    }

    Evento novoEvento;

    printf("Digite o nome do evento: ");
    getchar(); 
    fgets(novoEvento.nome, sizeof(novoEvento.nome), stdin);
    novoEvento.nome[strcspn(novoEvento.nome, "\n")] = '\0';

    printf("Digite a data e hora do evento (Ex: 10/01/2025 15:00): ");
    fgets(novoEvento.dataHora, sizeof(novoEvento.dataHora), stdin);
    novoEvento.dataHora[strcspn(novoEvento.dataHora, "\n")] = '\0';

    printf("Digite o local do evento: ");
    fgets(novoEvento.local, sizeof(novoEvento.local), stdin);
    novoEvento.local[strcspn(novoEvento.local, "\n")] = '\0';

    int valido = 0;
    while (!valido) {
        printf("Digite o numero maximo de pessoas para este evento: ");
        if (scanf("%d", &novoEvento.maxPessoas) != 1 || novoEvento.maxPessoas <= 0) {
            printf("--------------------------------- \n");
            printf("Entrada invalida! \nPor favor, insira um numero inteiro positivo.\n");
            printf("--------------------------------- \n");
            while (getchar() != '\n'); 
        } else {
            valido = 1;
        }
    }

    novoEvento.numInscritos = 0;
    novoEvento.filaEsperaInicio = 0;
    novoEvento.filaEsperaFim = 0;

    eventos[*totalEventos] = novoEvento;
    (*totalEventos)++;

    printf("Evento criado com sucesso!\n");
}


void listarEventos(Evento eventos[], int totalEventos) {
    if (totalEventos == 0) {
        printf("Nenhum evento cadastrado no momento.\n");
        return;
    }
        printf("\n==================================");
        printf("\n===      Lista de Eventos      ===");
    for (int i = 0; i < totalEventos; i++) {
        printf("\n===                            ===");
        printf("\n=== Evento: %d                 ", i + 1);
        printf("\n===                            ===");
        printf("\n=== Nome: %s                   ", eventos[i].nome);
        printf("\n===                            ===");
        printf("\n=== Data e Hora: %s            ", eventos[i].dataHora);
        printf("\n===                            ===");
        printf("\n=== Local: %s                  ", eventos[i].local);
        printf("\n===                            ===");
        printf("\n=== Maximo de Pessoas: %d      ", eventos[i].maxPessoas);
        printf("\n===                            ===");
        printf("\n=== Inscritos: %d              ", eventos[i].numInscritos);
        printf("\n===                            ===");
        printf("\n=== Fila de Espera: %d pessoas ", eventos[i].filaEsperaFim - eventos[i].filaEsperaInicio);
        printf("\n==================================\n");
    }
}

void apagarEvento(Evento eventos[], int *totalEventos) {
    if (*totalEventos == 0) {
        printf("Nenhum evento para apagar.\n");
        return;
    }

    int indice;
    listarEventos(eventos, *totalEventos);
    printf("Digite o namero do evento que deseja apagar (ou 0 para voltar): ");
    scanf("%d", &indice);

    if (indice == 0) {
        printf("Operacao cancelada. Voltando ao menu do administrador...\n");
        return; 
    }

    if (indice < 1 || indice > *totalEventos) {
        printf("Numero invalido. Operacao cancelada.\n");
        return;
    }

    for (int i = indice - 1; i < *totalEventos - 1; i++) {
        eventos[i] = eventos[i + 1];
    }
    (*totalEventos)--;

    printf("Evento apagado com sucesso!\n");
}


void inscreverEvento(char *utilizador, char *telefone, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes) {
    if (totalEventos == 0) {
        printf("Nenhum evento disponi­vel para inscricao.\n");
        return;
    }

    int indice;
    listarEventos(eventos, totalEventos);
    printf("Digite o numero do evento que deseja inscrever-se: ");
    scanf("%d", &indice);

    if (indice < 1 || indice > totalEventos) {
        printf("Numero invalido. Operacao cancelada.\n");
        return;
    }

    Evento *eventoSelecionado = &eventos[indice - 1];

    if (!validarTelemovel(utilizador)) {
        printf("Telemovel invalido! O Telemovel deve conter apenas numeros.\n");
        return;
    }

    if (!validarNome(telefone)) {
        printf("Nome invalido! O nome deve conter apenas letras e espacos.\n");
        return;
    }

    if (eventoSelecionado->numInscritos < eventoSelecionado->maxPessoas) {
        
        Inscricao novaInscricao;
        strcpy(novaInscricao.Telemovel, utilizador);
        strcpy(novaInscricao.telefone, telefone);
        strcpy(novaInscricao.nomeEvento, eventoSelecionado->nome);

        inscricoes[*totalInscricoes] = novaInscricao;
        (*totalInscricoes)++;
        eventoSelecionado->numInscritos++;

        printf("Inscricao realizada com sucesso!\n");
    } else {
        
        if (eventoSelecionado->filaEsperaFim < MAX_ESPERA) {
            strcpy(eventoSelecionado->filaEspera[eventoSelecionado->filaEsperaFim], utilizador);
            eventoSelecionado->filaEsperaFim++;
            printf("Evento cheio! Voce foi adicionado a fila de espera.\n");
        } else {
            printf("Fila de espera cheia! NÃ£o Ã© possi­vel adicionar a fila.\n");
        }
    }

    verificarFilaDeEspera(indice - 1, eventos, inscricoes, totalInscricoes);
}

void verificarFilaDeEspera(int eventoIndex, Evento eventos[], Inscricao inscricoes[], int *totalInscricoes) {
    Evento *evento = &eventos[eventoIndex];

    
    if (evento->numInscritos < evento->maxPessoas && evento->filaEsperaInicio < evento->filaEsperaFim) {
        

                Inscricao novaInscricao;
        strcpy(novaInscricao.Telemovel, evento->filaEspera[evento->filaEsperaInicio]);
        strcpy(novaInscricao.nomeEvento, evento->nome);
        printf("Digite o telefone do proximo da fila (%s): ", novaInscricao.Telemovel);
        scanf("%s", novaInscricao.telefone);

        inscricoes[*totalInscricoes] = novaInscricao;
        (*totalInscricoes)++;
        evento->numInscritos++;
        evento->filaEsperaInicio++;

        printf("Proximo da fila (%s) foi inscrito automaticamente no evento \"%s\".\n", novaInscricao.Telemovel, evento->nome);
    }
}

void listarInscritosPorEvento(Evento eventos[], int totalEventos, Inscricao inscricoes[], int totalInscricoes) {
    if (totalInscricoes == 0) {
        printf("Nenhuma inscricao cadastrada.\n");
        return;
    }

    for (int i = 0; i < totalEventos; i++) {
        printf("\nInscritos no evento: %s\n", eventos[i].nome);
        Inscricao inscritosDoEvento[MAX_INSCRICOES];
        int contador = 0;

        for (int j = 0; j < totalInscricoes; j++) {
            if (strcmp(inscricoes[j].nomeEvento, eventos[i].nome) == 0) {
                inscritosDoEvento[contador++] = inscricoes[j];
            }
        }

        if (contador == 0) {
            printf("Nenhum inscrito neste evento.\n");
            continue;
        }

        
        for (int a = 0; a < contador - 1; a++) {
            for (int b = a + 1; b < contador; b++) {
                if (strcmp(inscritosDoEvento[a].Telemovel, inscritosDoEvento[b].Telemovel) > 0) {
                    Inscricao temp = inscritosDoEvento[a];
                    inscritosDoEvento[a] = inscritosDoEvento[b];
                    inscritosDoEvento[b] = temp;
                }
            }
        }

        for (int k = 0; k < contador; k++) {
            printf("Nome: %s, Telemovel: %s, Telefone: %s\n", inscritosDoEvento[k].Telemovel, inscritosDoEvento[k].Telemovel, inscritosDoEvento[k].telefone);
        }
    }
}
void menuAdministrador(Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes) {
    char senha[10];

    printf("Insira a senha para acessar o menu do administrador: ");
    scanf("%s", senha);

    if (strcmp(senha, "1234") != 0) {
        printf("Senha incorreta! Voltando ao menu principal...\n");
        return;
    }

    int opcao;
    do {
        printf("\n==================================");
        printf("\n===   Menu do Administrador    ===");
        printf("\n==================================");
        printf("\n=== 1. Criar evento            ===");
        printf("\n=== 2. Listar eventos          ===");
        printf("\n=== 3. Apagar evento           ===");
        printf("\n=== 4. Listar inscritos        ===");
        printf("\n=== 5. Voltar ao menu principal===");
        printf("\n==================================\n");
        printf("\n==> Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                criarEvento(eventos, totalEventos);
                break;
            case 2:
                listarEventos(eventos, *totalEventos);
                break;
            case 3:
                apagarEvento(eventos, totalEventos);
                break;
            case 4:
                listarInscritosPorEvento(eventos, *totalEventos, inscricoes, *totalInscricoes);
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);
}

int main() {
    Evento eventos[MAX_EVENTOS];
    Inscricao inscricoes[MAX_INSCRICOES];
    int totalEventos = 0, totalInscricoes = 0;
    int opcao;
    char Telemovel[100];

    do {
        printf("\n====================================");
        printf("\n=== Sistema de GestÃo de Eventos ===\n");
        printf("===                              ===");
        printf("\n=== 1. Entrar como Utilizador    ===");
        printf("\n===                              ===");
        printf("\n=== 2. Entrar como Administrador ===");
        printf("\n===                              ===");
        printf("\n=== 3. Sair                      ===");
        printf("\n===                              ===");
        printf("\n====================================\n");
        printf("\n==> Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                do {
                    printf("\nDigite o seu Telemovel (somente numeros): ");
                    scanf("%s", Telemovel);
                    if (!validarTelemovel(Telemovel)) {
                        printf("Erro: O Telemovel deve conter apenas numeros.\n");
                    }
                } while (!validarTelemovel(Telemovel));

                char nomeUtilizador[100];
                printf("\nDigite seu nome: ");
                getchar(); 
                fgets(nomeUtilizador, sizeof(nomeUtilizador), stdin);
                nomeUtilizador[strcspn(nomeUtilizador, "\n")] = '\0';

                menuUtilizador(Telemovel, nomeUtilizador, eventos, &totalEventos, inscricoes, &totalInscricoes);
                break;
            case 2:
                menuAdministrador(eventos, &totalEventos, inscricoes, &totalInscricoes);
                break;
            case 3:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}