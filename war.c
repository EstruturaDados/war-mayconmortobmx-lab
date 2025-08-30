#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// ============================================================================
//                        PROJETO WAR ESTRUTURADO
// ============================================================================

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10
#define NUM_MISSOES 2

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Setup e gerenciamento de memória
Territorio* alocarMapa(int n);
void inicializarTerritorios(Territorio* mapa, int n);
void liberarMemoria(Territorio* mapa);

// Interface com o usuário
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int n);
void exibirMissao(int missaoID);

// Lógica do jogo
void faseDeAtaque(Territorio* mapa, int n);
void simularAtaque(Territorio* origem, Territorio* destino);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int n, int missaoID);

// Função utilitária
void limparBufferEntrada();

// ============================================================================
//                             FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // 1. Configuração inicial
    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    int missaoID = sortearMissao();
    int opcao;
    do {
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(missaoID);
        exibirMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, missaoID))
                    printf("Parabéns! Missão cumprida!\n");
                else
                    printf("Missão ainda não cumprida.\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        printf("\nPressione Enter para continuar...");
        getchar();
    } while(opcao != 0 && !verificarVitoria(mapa, NUM_TERRITORIOS, missaoID));

    liberarMemoria(mapa);
    return 0;
}

// ============================================================================
//                          IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

// Aloca dinamicamente o mapa
Territorio* alocarMapa(int n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

// Inicializa territórios com valores padrão
void inicializarTerritorios(Territorio* mapa, int n) {
    char nomes[NUM_TERRITORIOS][MAX_NOME] = {"Territorio1", "Territorio2", "Territorio3", "Territorio4", "Territorio5"};
    char cores[NUM_TERRITORIOS][MAX_COR] = {"Vermelho", "Azul", "Verde", "Amarelo", "Preto"};
    int tropas_iniciais[NUM_TERRITORIOS] = {5, 5, 5, 5, 5};

    for(int i = 0; i < n; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = tropas_iniciais[i];
    }
}

// Libera memória do mapa
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Exibe menu principal
void exibirMenuPrincipal() {
    printf("\n=== MENU ===\n");
    printf("1 - Ataque\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

// Exibe mapa
void exibirMapa(const Territorio* mapa, int n) {
    printf("\n=== MAPA ===\n");
    printf("%-12s %-10s %-6s\n", "Territorio", "Cor", "Tropas");
    printf("---------------------------\n");
    for(int i = 0; i < n; i++) {
        printf("%-12s %-10s %-6d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe missão
void exibirMissao(int missaoID) {
    printf("\n=== MISSÃO ===\n");
    switch(missaoID) {
        case 0:
            printf("Conquistar 3 territórios.\n");
            break;
        case 1:
            printf("Destruir o exército Azul.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

// Fase de ataque
void faseDeAtaque(Territorio* mapa, int n) {
    int origem, destino;
    printf("\nDigite o número do território de origem (1-%d): ", n);
    scanf("%d", &origem);
    printf("Digite o número do território de destino (1-%d): ", n);
    scanf("%d", &destino);
    limparBufferEntrada();

    if(origem < 1 || origem > n || destino < 1 || destino > n || origem == destino) {
        printf("Territórios inválidos!\n");
        return;
    }

    simularAtaque(&mapa[origem-1], &mapa[destino-1]);
}

// Simula ataque entre territórios
void simularAtaque(Territorio* origem, Territorio* destino) {
    if(origem->tropas <= 1) {
        printf("Não há tropas suficientes para atacar!\n");
        return;
    }

    int ataque = rand() % origem->tropas + 1;
    int defesa = rand() % destino->tropas + 1;

    printf("%s ataca %s!\n", origem->nome, destino->nome);
    printf("Tropas de ataque: %d, Tropas de defesa: %d\n", ataque, defesa);

    if(ataque > defesa) {
        destino->tropas -= defesa;
        origem->tropas -= defesa/2;
        if(destino->tropas <= 0) {
            printf("%s foi conquistado!\n", destino->nome);
            strcpy(destino->cor, origem->cor);
            destino->tropas = 1;
            origem->tropas--;
        }
    } else {
        origem->tropas -= ataque;
        destino->tropas -= ataque/2;
        printf("Ataque falhou!\n");
    }
}

// Sorteia missão aleatória
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// Verifica vitória
int verificarVitoria(const Territorio* mapa, int n, int missaoID) {
    switch(missaoID) {
        case 0: { // Conquistar 3 territórios
            int count = 0;
            for(int i = 0; i < n; i++) {
                if(strcmp(mapa[i].cor, "Vermelho") == 0) count++; // Supondo jogador Vermelho
            }
            return count >= 3;
        }
        case 1: { // Destruir exército Azul
            for(int i = 0; i < n; i++) {
                if(strcmp(mapa[i].cor, "Azul") == 0 && mapa[i].tropas > 0) return 0;
            }
            return 1;
        }
        default:
            return 0;
    }
}

// Limpa buffer de entrada
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

