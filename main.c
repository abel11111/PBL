#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_EVENTOS 100
#define MAX_INSCRICOES 100
#define MAX_ESPERA 100
#define MAX_ORADORES 10

typedef struct
{

    int dia;
    int hora;
    int mes;
    int ano;
    int minuto;
} DataHora;

typedef struct
{
    char nome[100];
} Orador;

typedef struct
{
    char nome[100];
    DataHora dataHora;
    char local[100];
    int maxPessoas;
    int numInscritos;
    char filaEspera[MAX_ESPERA][100];
    int filaEsperaInicio;
    int filaEsperaFim;
    Orador oradores[MAX_ORADORES];
    int numOradores;
    float preco;
} Evento;

typedef struct
{
    char Telemovel[100];
    char nomeEvento[100];
    char telefone[20];
    char email[100];
    int pago; // 0 para gratuito, 1 para pago
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
int validarEmail(const char *email);
void exibirEstatisticas(Evento eventos[], int totalEventos, Inscricao inscricoes[], int totalInscricoes);
void editarEvento(Evento eventos[], int totalEventos);
void salvarEventos(Evento eventos[], int totalEventos);
void salvarInscricoes(Inscricao inscricoes[], int totalInscricoes);
void carregarEventos(Evento eventos[], int *totalEventos);
void carregarInscricoes(Inscricao inscricoes[], int *totalInscricoes);

int validarData(int dia, int mes, int ano) {
    // Verifica se o ano é válido (2024 ou superior)
    if (ano < 2024) {
        return 0;
    }

    // Verifica se o mês está entre 1 e 12
    if (mes < 1 || mes > 12) {
        return 0;
    }

    // Verifica se o dia está entre 1 e 31
    if (dia < 1 || dia > 31) {
        return 0;
    }

    // Verifica os meses com 30 dias
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return 0;
    }

    // Verifica fevereiro
    if (mes == 2) {
        // Verifica se o ano é bissexto
        int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
        if (dia > 29 || (dia == 29 && !bissexto)) {
            return 0;
        }
    }

    // Se todas as verificações passaram, a data é válida
    return 1;
}

int validarTelemovel(const char *Telemovel) {
   
    if (strlen(Telemovel) != 9) {
        return 0;
    }

    
    for (int i = 0; Telemovel[i] != '\0'; i++) {
        if (!isdigit(Telemovel[i])) {
            return 0;
        }
    }

    return 1;
}

int validarNome(const char *nome)
{
    for (int i = 0; nome[i] != '\0'; i++)
    {
        if (!isalpha(nome[i]) && nome[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

void menuUtilizador(char *Telemovel, char *nomeUtilizador, Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes)
{
    while (!validarNome(nomeUtilizador))
    {
        printf("Nome inválido! O nome deve conter apenas letras e espaços.\n");
        printf("Por favor, insira novamente o nome: ");
        getchar();
        fgets(nomeUtilizador, 100, stdin);
        nomeUtilizador[strcspn(nomeUtilizador, "\n")] = '\0';
    }

    int opcao;

    do
    {
        printf("\n==================================");
        printf("\n===     Menu do Utilizador     ===");
        printf("\n===                            ===");
        printf("\n=== 1. Ver eventos             ===");
        printf("\n===                            ===");
        printf("\n=== 2. Inscrever-se em evento  ===");
        printf("\n===                            ===");
        printf("\n=== 3. Remover inscrição       ===");
        printf("\n===                            ===");
        printf("\n=== 4. Gerar bilhete eletrônico===");
        printf("\n===                            ===");
        printf("\n=== 5. Voltar ao menu principal===");
        printf("\n==================================\n");
        printf("\n==>Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            listarEventos(eventos, *totalEventos);
            break;
        case 2:
            inscreverEvento(Telemovel, nomeUtilizador, eventos, *totalEventos, inscricoes, totalInscricoes);
            break;
        case 3:
            removerInscricao(Telemovel, eventos, *totalEventos, inscricoes, totalInscricoes);
            break;
        case 4:
        {
            int encontrado = 0;
            for (int i = 0; i < *totalInscricoes; i++)
            {
                if (strcmp(inscricoes[i].Telemovel, Telemovel) == 0)
                {
                    for (int j = 0; j < *totalEventos; j++)
                    {
                        if (strcmp(inscricoes[i].nomeEvento, eventos[j].nome) == 0)
                        {
                            gerarBilheteEletronico(nomeUtilizador, Telemovel, inscricoes[i], eventos[j]);
                            encontrado = 1;
                            break;
                        }
                    }
                }
            }
            if (!encontrado)
            {
                printf("Você não está inscrito em nenhum evento.\n");
            }
            break;
        }
        case 5:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 5);
}


void gerarQRCodeSimulado(const char *dados)
{
    printf("\nCódigo QR (simulado):\n");
    for (size_t i = 0; i < strlen(dados); i++)
    {
        if (i % 5 == 0)
        {
            printf("\n");
        }
        printf("%c ", dados[i] == ' ' ? '#' : '*');
    }
    printf("\n");
}

void gerarBilheteEletronico(char *nomeUtilizador, char *Telemovel, Inscricao inscricao, Evento evento)
{
    char dadosQR[300];
    snprintf(dadosQR, sizeof(dadosQR), "Nome: %s\nTelemovel: %s\nEvento: %s\nData e Hora:%d %d %d %d %d\nLocal: %s",
             nomeUtilizador, Telemovel, evento.nome, evento.dataHora.dia, evento.dataHora.mes, evento.dataHora.ano, evento.dataHora.hora, evento.dataHora.minuto, evento.local);

    printf("\n==================================");
    printf("\n===     Bilhete Eletrônico     ===");
    printf("\n==================================\n");
    printf("Nome: %s\n", nomeUtilizador);
    printf("Telemovel: %s\n", Telemovel);
    printf("Nome do Evento: %s\n", evento.nome);
    printf("Data e Hora: %d %d %d  %d %d\n", evento.dataHora.dia, evento.dataHora.mes, evento.dataHora.ano, evento.dataHora.hora, evento.dataHora.minuto);
    printf("Local: %s\n", evento.local);
    gerarQRCodeSimulado(dadosQR);
    printf("==================================\n");
}

// Funções restantes...

void removerInscricao(char *utilizador, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes)
{
    if (*totalInscricoes == 0)
    {
        printf("Nenhuma inscrição encontrada para remover.\n");
        return;
    }

    printf("\n=== Remover Inscrição ===\n");
    int indiceEvento;
    listarEventos(eventos, totalEventos);
    printf("Digite o número do evento do qual deseja remover sua inscrição: ");
    scanf("%d", &indiceEvento);

    if (indiceEvento < 1 || indiceEvento > totalEventos)
    {
        printf("Número de evento inválido. Operação cancelada.\n");
        return;
    }

    Evento *eventoSelecionado = &eventos[indiceEvento - 1];

    int encontrado = 0;
    for (int i = 0; i < *totalInscricoes; i++)
    {
        if (strcmp(inscricoes[i].Telemovel, utilizador) == 0 && strcmp(inscricoes[i].nomeEvento, eventoSelecionado->nome) == 0)
        {
            encontrado = 1;
            for (int j = i; j < *totalInscricoes - 1; j++)
            {
                inscricoes[j] = inscricoes[j + 1];
            }
            (*totalInscricoes)--;
            eventoSelecionado->numInscritos--;

            printf("Inscrição removida com sucesso!\n");

            verificarFilaDeEspera(indiceEvento - 1, eventos, inscricoes, totalInscricoes);
            break;
        }
    }

    if (!encontrado)
    {
        printf("Você não está inscrito neste evento.\n");
    }
}

void criarEvento(Evento eventos[], int *totalEventos) {
    if (*totalEventos >= MAX_EVENTOS) {
        printf("Limite de eventos atingido. Não é possível criar mais eventos.\n");
        return;
    }

    Evento novoEvento;

    printf("Digite o nome do evento: ");
    getchar();
    fgets(novoEvento.nome, sizeof(novoEvento.nome), stdin);
    novoEvento.nome[strcspn(novoEvento.nome, "\n")] = '\0';

    printf("Digite a data do evento (DD/MM/AAAA): ");
    while (scanf("%d/%d/%d", &novoEvento.dataHora.dia, &novoEvento.dataHora.mes, &novoEvento.dataHora.ano) != 3 ||
           !validarData(novoEvento.dataHora.dia, novoEvento.dataHora.mes, novoEvento.dataHora.ano)) {
        printf("Data inválida! Digite novamente (DD/MM/AAAA): ");
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }

    printf("Digite a hora do evento (HH:MM): ");
    while (scanf("%d:%d", &novoEvento.dataHora.hora, &novoEvento.dataHora.minuto) != 2 ||
           novoEvento.dataHora.hora < 0 || novoEvento.dataHora.hora > 23 ||
           novoEvento.dataHora.minuto < 0 || novoEvento.dataHora.minuto > 59) {
        printf("Hora inválida! Digite novamente (HH:MM): ");
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }

    getchar(); // Limpa o buffer de entrada
    printf("Digite o local do evento: ");
    fgets(novoEvento.local, sizeof(novoEvento.local), stdin);
    novoEvento.local[strcspn(novoEvento.local, "\n")] = '\0';

    printf("Digite o número de oradores para este evento: ");
    while (scanf("%d", &novoEvento.numOradores) != 1 || novoEvento.numOradores < 1 || novoEvento.numOradores > MAX_ORADORES) {
        printf("Número inválido! Digite um valor entre 1 e %d: ", MAX_ORADORES);
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < novoEvento.numOradores; i++) {
        printf("Digite o nome do orador %d: ", i + 1);
        fgets(novoEvento.oradores[i].nome, sizeof(novoEvento.oradores[i].nome), stdin);
        novoEvento.oradores[i].nome[strcspn(novoEvento.oradores[i].nome, "\n")] = '\0';
    }

    printf("Digite o número máximo de pessoas para este evento: ");
    while (scanf("%d", &novoEvento.maxPessoas) != 1 || novoEvento.maxPessoas <= 0) {
        printf("Número inválido! Digite um valor positivo: ");
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }

    printf("Digite o preço do evento (0 para gratuito): ");
    while (scanf("%f", &novoEvento.preco) != 1 || novoEvento.preco < 0) {
        printf("Preço inválido! Digite um valor positivo: ");
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
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
        printf("\n=== Data e Hora:%d %d %d  %d %d        ", eventos[i].dataHora.dia, eventos[i].dataHora.mes, eventos[i].dataHora.ano, eventos[i].dataHora.hora, eventos[i].dataHora.minuto);
        printf("\n===                            ===");
        printf("\n=== Local: %s                  ", eventos[i].local);
        printf("\n===                            ===");
        printf("\n=== Oradores: ");
        for (int j = 0; j < eventos[i].numOradores; j++) {
            printf("%s", eventos[i].oradores[j].nome);
            if (j < eventos[i].numOradores - 1) {
                printf(", ");
            }
        }
        printf("\n");
        printf("\n===                            ===");
        printf("\n=== Máximo de Pessoas: %d      ", eventos[i].maxPessoas);
        printf("\n===                            ===");
        printf("\n=== Inscritos: %d              ", eventos[i].numInscritos);
        printf("\n===                            ===");
        printf("\n=== Preço: %.2f                ", eventos[i].preco);
        printf("\n===                            ===");
        printf("\n=== Fila de Espera: %d pessoas ", eventos[i].filaEsperaFim - eventos[i].filaEsperaInicio);
        printf("\n==================================\n");
    }
}

void apagarEvento(Evento eventos[], int *totalEventos)
{
    if (*totalEventos == 0)
    {
        printf("Nenhum evento para apagar.\n");
        return;
    }

    int indice;
    listarEventos(eventos, *totalEventos);
    printf("Digite o número do evento que deseja apagar (ou 0 para voltar): ");
    scanf("%d", &indice);

    if (indice == 0)
    {
        printf("Operação cancelada. Voltando ao menu do administrador...\n");
        return;
    }

    if (indice < 1 || indice > *totalEventos)
    {
        printf("Número inválido. Operação cancelada.\n");
        return;
    }

    for (int i = indice - 1; i < *totalEventos - 1; i++)
    {
        eventos[i] = eventos[i + 1];
    }
    (*totalEventos)--;

    printf("Evento apagado com sucesso!\n");
}

void inscreverEvento(char *utilizador, char *telefone, Evento eventos[], int totalEventos, Inscricao inscricoes[], int *totalInscricoes) {
    if (totalEventos == 0) {
        printf("Nenhum evento disponível para inscrição.\n");
        return;
    }

    int indice;
    listarEventos(eventos, totalEventos);
    printf("Digite o número do evento que deseja inscrever-se: ");
    while (scanf("%d", &indice) != 1 || indice < 1 || indice > totalEventos) {
        printf("Número inválido! Digite um valor entre 1 e %d: ", totalEventos);
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }

    Evento *eventoSelecionado = &eventos[indice - 1];

    // Verifica se o usuário já está inscrito
    for (int i = 0; i < *totalInscricoes; i++) {
        if (strcmp(inscricoes[i].Telemovel, utilizador) == 0 && strcmp(inscricoes[i].nomeEvento, eventoSelecionado->nome) == 0) {
            printf("Você já está inscrito neste evento.\n");
            return;
        }
    }

    Inscricao novaInscricao;
    strcpy(novaInscricao.Telemovel, utilizador);
    strcpy(novaInscricao.telefone, telefone);
    strcpy(novaInscricao.nomeEvento, eventoSelecionado->nome);

    // Define se o evento é pago ou gratuito
    novaInscricao.pago = (eventoSelecionado->preco > 0) ? 1 : 0;

    if (eventoSelecionado->numInscritos < eventoSelecionado->maxPessoas) {
        inscricoes[*totalInscricoes] = novaInscricao;
        (*totalInscricoes)++;
        eventoSelecionado->numInscritos++;
        printf("Inscrição realizada com sucesso!\n");
    } else {
        if (eventoSelecionado->filaEsperaFim < MAX_ESPERA) {
            strcpy(eventoSelecionado->filaEspera[eventoSelecionado->filaEsperaFim], utilizador);
            eventoSelecionado->filaEsperaFim++;
            printf("Evento cheio! Você foi adicionado à fila de espera.\n");
        } else {
            printf("Fila de espera cheia! Não é possível adicionar à fila.\n");
        }
    }

    verificarFilaDeEspera(indice - 1, eventos, inscricoes, totalInscricoes);
}

void verificarFilaDeEspera(int eventoIndex, Evento eventos[], Inscricao inscricoes[], int *totalInscricoes)
{
    Evento *evento = &eventos[eventoIndex];

    if (evento->numInscritos < evento->maxPessoas && evento->filaEsperaInicio < evento->filaEsperaFim)
    {

        Inscricao novaInscricao;
        strcpy(novaInscricao.Telemovel, evento->filaEspera[evento->filaEsperaInicio]);
        strcpy(novaInscricao.nomeEvento, evento->nome);
        printf("Digite o telefone do próximo da fila (%s): ", novaInscricao.Telemovel);
        scanf("%s", novaInscricao.telefone);

        inscricoes[*totalInscricoes] = novaInscricao;
        (*totalInscricoes)++;
        evento->numInscritos++;
        evento->filaEsperaInicio++;

        printf("Próximo da fila (%s) foi inscrito automaticamente no evento \"%s\".\n", novaInscricao.Telemovel, evento->nome);
    }
}

void listarInscritosPorEvento(Evento eventos[], int totalEventos, Inscricao inscricoes[], int totalInscricoes)
{
    if (totalInscricoes == 0)
    {
        printf("Nenhuma inscrição cadastrada.\n");
        return;
    }

    for (int i = 0; i < totalEventos; i++)
    {
        printf("\nInscritos no evento: %s\n", eventos[i].nome);
        Inscricao inscritosDoEvento[MAX_INSCRICOES];
        int contador = 0;

        for (int j = 0; j < totalInscricoes; j++)
        {
            if (strcmp(inscricoes[j].nomeEvento, eventos[i].nome) == 0)
            {
                inscritosDoEvento[contador++] = inscricoes[j];
            }
        }

        if (contador == 0)
        {
            printf("Nenhum inscrito neste evento.\n");
            continue;
        }

        for (int a = 0; a < contador - 1; a++)
        {
            for (int b = a + 1; b < contador; b++)
            {
                if (strcmp(inscritosDoEvento[a].Telemovel, inscritosDoEvento[b].Telemovel) > 0)
                {
                    Inscricao temp = inscritosDoEvento[a];
                    inscritosDoEvento[a] = inscritosDoEvento[b];
                    inscritosDoEvento[b] = temp;
                }
            }
        }

        for (int k = 0; k < contador; k++)
        {
            printf("Nome: %s, Telemovel: %s, Email: %s, Telefone: %s\n", inscritosDoEvento[k].Telemovel, inscritosDoEvento[k].Telemovel, inscritosDoEvento[k].email, inscritosDoEvento[k].telefone);
        }
    }
}
void menuAdministrador(Evento eventos[], int *totalEventos, Inscricao inscricoes[], int *totalInscricoes)
{
    char senha[10];

    printf("Insira a senha para acessar o menu do administrador: ");
    scanf("%s", senha);

    if (strcmp(senha, "1234") != 0)
    {
        printf("Senha incorreta! Voltando ao menu principal...\n");
        return;
    }

    int opcao;
    do
    {
        printf("\n==================================");
        printf("\n===   Menu do Administrador    ===");
        printf("\n==================================");
        printf("\n=== 1. Criar evento            ===");
        printf("\n=== 2. Listar eventos          ===");
        printf("\n=== 3. Apagar evento           ===");
        printf("\n=== 4. Editar evento           ===");
        printf("\n=== 5. Listar inscritos        ===");
        printf("\n=== 6. Exibir estatísticas     ===");
        printf("\n=== 7. Voltar ao menu principal===");
        printf("\n==================================\n");
        printf("\n==> Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
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
            editarEvento(eventos, *totalEventos);
            break;
        case 5:
            listarInscritosPorEvento(eventos, *totalEventos, inscricoes, *totalInscricoes);
            break;
        case 6:
            exibirEstatisticas(eventos, *totalEventos, inscricoes, *totalInscricoes);
            break;
        case 7:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 7);
}

void exibirEstatisticas(Evento eventos[], int totalEventos, Inscricao inscricoes[], int totalInscricoes) {
    if (totalEventos == 0) {
        printf("Nenhum evento cadastrado.\n");
        return;
    }

    int totalInscritos = 0;
    int eventosCheios = 0;
    float capacidadeMedia = 0;
    float inscritosMedia = 0;
    float totalPrecos = 0;
    int eventosPagos = 0;
    int eventosGratuitos = 0;

    for (int i = 0; i < totalEventos; i++) {
        totalInscritos += eventos[i].numInscritos;
        capacidadeMedia += eventos[i].maxPessoas;
        totalPrecos += eventos[i].preco;

        if (eventos[i].numInscritos >= eventos[i].maxPessoas) {
            eventosCheios++;
        }

        if (eventos[i].preco > 0) {
            eventosPagos++;
        } else {
            eventosGratuitos++;
        }
    }

    capacidadeMedia /= totalEventos;
    inscritosMedia = (float)totalInscritos / totalEventos;
    float precoMedio = totalPrecos / totalEventos;

    printf("\n=== Estatísticas ===\n");
    printf("Número total de eventos: %d\n", totalEventos);
    printf("Capacidade média dos eventos: %.2f\n", capacidadeMedia);
    printf("Número médio de inscritos por evento: %.2f\n", inscritosMedia);
    printf("Eventos que atingiram a capacidade máxima: %d\n", eventosCheios);
    printf("Preço médio dos eventos: %.2f\n", precoMedio);
    printf("Eventos pagos: %d\n", eventosPagos);
    printf("Eventos gratuitos: %d\n", eventosGratuitos);
}

void editarEvento(Evento eventos[], int totalEventos) {
    if (totalEventos == 0) {
        printf("Nenhum evento cadastrado.\n");
        return;
    }

    int indice;
    listarEventos(eventos, totalEventos);
    printf("Digite o número do evento que deseja editar: ");
    scanf("%d", &indice);

    if (indice < 1 || indice > totalEventos) {
        printf("Número inválido.\n");
        return;
    }

    Evento *evento = &eventos[indice - 1];

    printf("Digite o novo nome do evento (atual: %s): ", evento->nome);
    getchar();
    fgets(evento->nome, sizeof(evento->nome), stdin);
    evento->nome[strcspn(evento->nome, "\n")] = '\0';

    printf("Digite a nova data do evento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &evento->dataHora.dia, &evento->dataHora.mes, &evento->dataHora.ano);

    printf("Digite a nova hora do evento (HH:MM): ");
    scanf("%d:%d", &evento->dataHora.hora, &evento->dataHora.minuto);

    getchar();
    printf("Digite o novo local do evento (atual: %s): ", evento->local);
    fgets(evento->local, sizeof(evento->local), stdin);
    evento->local[strcspn(evento->local, "\n")] = '\0';

    printf("Digite o novo número máximo de pessoas: ");
    scanf("%d", &evento->maxPessoas);

    printf("Digite o novo preço do evento (atual: %.2f): ", evento->preco);
    scanf("%f", &evento->preco);

    // Editar oradores
    printf("Digite o número de oradores para este evento: ");
    while (scanf("%d", &evento->numOradores) != 1 || evento->numOradores < 1 || evento->numOradores > MAX_ORADORES) {
        printf("Número inválido! Digite um valor entre 1 e %d: ", MAX_ORADORES);
        while (getchar() != '\n')
            ; // Limpa o buffer de entrada
    }
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < evento->numOradores; i++) {
        printf("Digite o nome do orador %d: ", i + 1);
        fgets(evento->oradores[i].nome, sizeof(evento->oradores[i].nome), stdin);
        evento->oradores[i].nome[strcspn(evento->oradores[i].nome, "\n")] = '\0';
    }

    printf("Evento atualizado com sucesso!\n");
}

int validarEmail(const char *email)
{
    int temArroba = 0;
    int temPonto = 0;
    int posicaoArroba = -1;

    for (int i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            temArroba = 1;
            posicaoArroba = i;
        }
        if (temArroba && email[i] == '.' && i > posicaoArroba)
        {
            temPonto = 1;
        }
    }

    return temArroba && temPonto;
}
int main() {
    Evento eventos[MAX_EVENTOS];
    Inscricao inscricoes[MAX_INSCRICOES];
    int totalEventos = 0, totalInscricoes = 0;
    int opcao;
    char Telemovel[100];
    char email[100];

    // Carregar dados dos arquivos ao iniciar o programa
    carregarEventos(eventos, &totalEventos);
    carregarInscricoes(inscricoes, &totalInscricoes);

    do {
        printf("\n====================================");
        printf("\n=== Sistema de Gestão de Eventos ===\n");
        printf("===                              ===");
        printf("\n=== 1. Entrar como Utilizador    ===");
        printf("\n===                              ===");
        printf("\n=== 2. Entrar como Administrador ===");
        printf("\n===                              ===");
        printf("\n=== 3. Sair                      ===");
        printf("\n===                              ===");
        printf("\n====================================\n");
        printf("\n==> Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                do {
                    printf("\nDigite o seu Telemovel (somente números): ");
                    scanf("%s", Telemovel);
                    if (!validarTelemovel(Telemovel)) {
                        printf("Erro: O Telemovel deve conter apenas números.\n");
                    }
                } while (!validarTelemovel(Telemovel));

                char nomeUtilizador[100];
                printf("\nDigite seu nome: ");
                getchar();
                fgets(nomeUtilizador, sizeof(nomeUtilizador), stdin);
                nomeUtilizador[strcspn(nomeUtilizador, "\n")] = '\0';

                do {
                    printf("\nDigite seu email: ");
                    scanf("%s", email);
                    if (!validarEmail(email)) {
                        printf("Erro: Email inválido. Deve conter '@' e '.'.\n");
                    }
                } while (!validarEmail(email));

                menuUtilizador(Telemovel, nomeUtilizador, eventos, &totalEventos, inscricoes, &totalInscricoes);
                break;
            case 2:
                menuAdministrador(eventos, &totalEventos, inscricoes, &totalInscricoes);
                break;
            case 3:
                // Salvar dados nos arquivos antes de sair
                salvarEventos(eventos, totalEventos);
                salvarInscricoes(inscricoes, totalInscricoes);
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}

void salvarEventos(Evento eventos[], int totalEventos) {
    FILE *arquivo = fopen("eventos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar eventos.\n");
        return;
    }

    fwrite(&totalEventos, sizeof(int), 1, arquivo);
    fwrite(eventos, sizeof(Evento), totalEventos, arquivo);

    fclose(arquivo);
    printf("Eventos salvos com sucesso!\n");
}

void salvarInscricoes(Inscricao inscricoes[], int totalInscricoes) {
    FILE *arquivo = fopen("inscricoes.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar inscrições.\n");
        return;
    }

    fwrite(&totalInscricoes, sizeof(int), 1, arquivo);
    fwrite(inscricoes, sizeof(Inscricao), totalInscricoes, arquivo);

    fclose(arquivo);
    printf("Inscrições salvas com sucesso!\n");
}

void carregarEventos(Evento eventos[], int *totalEventos) {
    FILE *arquivo = fopen("eventos.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de eventos não encontrado. Criando novo arquivo.\n");
        return;
    }

    fread(totalEventos, sizeof(int), 1, arquivo);
    fread(eventos, sizeof(Evento), *totalEventos, arquivo);

    fclose(arquivo);
    printf("Eventos carregados com sucesso!\n");
}

void carregarInscricoes(Inscricao inscricoes[], int *totalInscricoes) {
    FILE *arquivo = fopen("inscricoes.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo de inscrições não encontrado. Criando novo arquivo.\n");
        return;
    }

    fread(totalInscricoes, sizeof(int), 1, arquivo);
    fread(inscricoes, sizeof(Inscricao), *totalInscricoes, arquivo);

    fclose(arquivo);
    printf("Inscrições carregadas com sucesso!\n");
}