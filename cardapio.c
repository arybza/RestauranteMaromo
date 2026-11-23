#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cardapio.h"

// Categorias aceitas pelo sistema 
#define QTD_CATEGORIAS 4
const char *CATEGORIAS[QTD_CATEGORIAS] = {"Prato", "Bebida", "Lanche", "Sobremesa"};

// MEMORIA 

// Todos os ponteiros comecam apontando explicitamente para NULL
void inicializarVetor(ItemCardapio *vetor[]) {
    for (int i = 0; i < MAX; i++) {
        vetor[i] = NULL;
    }
}

// Libera cada posicao nao nula do vetor
void liberarMemoria(ItemCardapio *vetor[]) {
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            free(vetor[i]);
            vetor[i] = NULL;
        }
    }
}

// ARQUIVO 

// Le o arquivo linha a linha. Retorna quantos itens carregou.
int carregarArquivo(ItemCardapio *vetor[]) {
    FILE *p_arq = fopen(ARQUIVO, "r");
    if (p_arq == NULL) {
        // arquivo ainda nao existe: sistema comeca vazio, sem erro
        return 0;
    }

    char linha[TAM_LINHA];
    int qtd = 0;
    ItemCardapio temp;

    while (fgets(linha, TAM_LINHA, p_arq) != NULL) {
        // formato: codigo;nome;categoria;preco;tempo
        if (sscanf(linha, "%d;%50[^;];%20[^;];%lf;%d",
                   &temp.codigo, temp.nome, temp.categoria,
                   &temp.preco, &temp.tempoPreparo) != 5) {
            continue; // linha invalida ou em branco: ignora
        }

        int pos = posicaoLivre(vetor);
        if (pos == -1) {
            printf("Aviso: cardapio cheio, nem todos os itens foram carregados.\n");
            break;
        }

        vetor[pos] = (ItemCardapio *) malloc(sizeof(ItemCardapio));
        if (vetor[pos] == NULL) {
            printf("Erro: memoria insuficiente.\n");
            break;
        }
        *vetor[pos] = temp; // copia o registro lido para a memoria alocada
        qtd++;
    }

    fclose(p_arq);
    return qtd;
}

// Grava todos os ponteiros nao nulos no arquivo. Retorna 1 se deu certo.
int salvarArquivo(ItemCardapio *vetor[]) {
    FILE *p_arq = fopen(ARQUIVO, "w");
    if (p_arq == NULL) {
        printf("Erro ao abrir o arquivo para gravacao!\n");
        return 0;
    }

    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            fprintf(p_arq, "%d;%s;%s;%.2f;%d\n",
                    vetor[i]->codigo, vetor[i]->nome, vetor[i]->categoria,
                    vetor[i]->preco, vetor[i]->tempoPreparo);
        }
    }

    fclose(p_arq);
    return 1;
}

// OPERACOES DO MENU 

void cadastrarItem(ItemCardapio *vetor[]) {
    int pos = posicaoLivre(vetor);
    if (pos == -1) {
        printf("Cardapio cheio! Limite de %d itens.\n", MAX);
        return;
    }

    printf("\n--- Cadastro de item ---\n");
    printf("Codigo: ");
    int codigo = lerInteiro();
    if (codigo <= 0) {
        printf("Codigo deve ser maior que zero.\n");
        return;
    }
    if (buscarPosicao(vetor, codigo) != -1) {
        printf("Ja existe um item com o codigo %d.\n", codigo);
        return;
    }

    // so aloca depois de validar o codigo
    ItemCardapio *novo = (ItemCardapio *) malloc(sizeof(ItemCardapio));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }

    novo->codigo = codigo;

    printf("Nome do prato/bebida: ");
    lerTexto(novo->nome, TAM_NOME);
    while (strlen(novo->nome) == 0 && !feof(stdin)) {
        printf("Nome nao pode ficar vazio. Digite novamente: ");
        lerTexto(novo->nome, TAM_NOME);
    }

    lerCategoria(novo->categoria);

    printf("Preco (R$): ");
    novo->preco = lerReal();
    while (novo->preco <= 0 && !feof(stdin)) {
        printf("Preco deve ser maior que zero. Digite novamente: ");
        novo->preco = lerReal();
    }

    printf("Tempo estimado de preparo (min): ");
    novo->tempoPreparo = lerInteiro();
    while (novo->tempoPreparo < 0 && !feof(stdin)) {
        printf("Tempo nao pode ser negativo. Digite novamente: ");
        novo->tempoPreparo = lerInteiro();
    }

    vetor[pos] = novo;
    printf("Item cadastrado com sucesso!\n");
}

void listarItens(ItemCardapio *vetor[]) {
    if (contarItens(vetor) == 0) {
        printf("\nNenhum item cadastrado.\n");
        return;
    }

    printf("\n%-6s | %-30s | %-10s | %10s | %s\n",
           "Codigo", "Nome", "Categoria", "Preco", "Preparo");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            printf("%-6d | %-30s | %-10s | R$ %7.2f | %d min\n",
                   vetor[i]->codigo, vetor[i]->nome, vetor[i]->categoria,
                   vetor[i]->preco, vetor[i]->tempoPreparo);
        }
    }
    printf("Total: %d item(ns)\n", contarItens(vetor));
}

void buscarItem(ItemCardapio *vetor[]) {
    printf("\nCodigo do item: ");
    int codigo = lerInteiro();
    int pos = buscarPosicao(vetor, codigo);
    if (pos == -1) {
        printf("Item com codigo %d nao encontrado.\n", codigo);
    } else {
        exibirItem(vetor[pos]);
    }
}

// Relatorio: media de preco por categoria + item mais rapido de preparar
void relatorio(ItemCardapio *vetor[]) {
    if (contarItens(vetor) == 0) {
        printf("\nNenhum item cadastrado para gerar relatorio.\n");
        return;
    }

    printf("\n===== RELATORIO DO CARDAPIO =====\n");
    printf("\nMedia de preco por categoria:\n");

    for (int c = 0; c < QTD_CATEGORIAS; c++) {
        double soma = 0;
        int qtd = 0;
        for (int i = 0; i < MAX; i++) {
            if (vetor[i] != NULL && strcmp(vetor[i]->categoria, CATEGORIAS[c]) == 0) {
                soma += vetor[i]->preco;
                qtd++;
            }
        }
        if (qtd > 0) {
            printf("  %-10s: R$ %7.2f  (%d item(ns))\n", CATEGORIAS[c], soma / qtd, qtd);
        } else {
            printf("  %-10s: sem itens\n", CATEGORIAS[c]);
        }
    }

    // ponteiro para o item mais rapido (comeca em NULL)
    ItemCardapio *maisRapido = NULL;
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            if (maisRapido == NULL || vetor[i]->tempoPreparo < maisRapido->tempoPreparo) {
                maisRapido = vetor[i];
            }
        }
    }

    printf("\nItem mais rapido de preparar:\n");
    exibirItem(maisRapido);
}

// AUXILIARES 

// Retorna a posicao do item com o codigo informado, ou -1
int buscarPosicao(ItemCardapio *vetor[], int codigo) {
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL && vetor[i]->codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Retorna a primeira posicao NULL do vetor, ou -1 se estiver cheio
int posicaoLivre(ItemCardapio *vetor[]) {
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] == NULL) {
            return i;
        }
    }
    return -1;
}

int contarItens(ItemCardapio *vetor[]) {
    int qtd = 0;
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) qtd++;
    }
    return qtd;
}

void exibirItem(ItemCardapio *item) {
    printf("  Codigo   : %d\n", item->codigo);
    printf("  Nome     : %s\n", item->nome);
    printf("  Categoria: %s\n", item->categoria);
    printf("  Preco    : R$ %.2f\n", item->preco);
    printf("  Preparo  : %d min\n", item->tempoPreparo);
}

// Le uma linha do teclado com fgets, tira o '\n' e troca ';' por ','
void lerTexto(char texto[], int tamanho) {
    if (fgets(texto, tamanho, stdin) == NULL) {
        texto[0] = '\0';
        return;
    }
    int len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    } else {
        // texto maior que o tamanho: descarta o resto da linha
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ';') texto[i] = ',';
    }
}

// Le um inteiro; repete enquanto o usuario digitar algo invalido
int lerInteiro(void) {
    char linha[TAM_LINHA];
    int valor;
    char sobra;
    while (1) {
        lerTexto(linha, TAM_LINHA);
        if (sscanf(linha, "%d %c", &valor, &sobra) == 1) {
            return valor;
        }
        if (feof(stdin)) return 0;
        printf("Valor invalido. Digite um numero inteiro: ");
    }
}

// Le um numero real; aceita virgula ou ponto como separador decimal
double lerReal(void) {
    char linha[TAM_LINHA];
    double valor;
    char sobra;
    while (1) {
        lerTexto(linha, TAM_LINHA); // lerTexto ja troca ';' por ','
        for (int i = 0; linha[i] != '\0'; i++) {
            if (linha[i] == ',') linha[i] = '.';
        }
        if (sscanf(linha, "%lf %c", &valor, &sobra) == 1) {
            return valor;
        }
        if (feof(stdin)) return 0;
        printf("Valor invalido. Digite um numero (ex.: 12.50): ");
    }
}

// Mostra as categorias e copia a escolhida para o campo
void lerCategoria(char categoria[]) {
    int opcao;
    printf("Categoria:\n");
    for (int c = 0; c < QTD_CATEGORIAS; c++) {
        printf("  %d - %s\n", c + 1, CATEGORIAS[c]);
    }
    printf("Escolha: ");
    opcao = lerInteiro();
    while (opcao < 1 || opcao > QTD_CATEGORIAS) {
        if (feof(stdin)) { opcao = 1; break; }
        printf("Opcao invalida. Escolha de 1 a %d: ", QTD_CATEGORIAS);
        opcao = lerInteiro();
    }
    strcpy(categoria, CATEGORIAS[opcao - 1]);
}
