#include <stdio.h>
#include "cardapio.h"

int main(void) {
    ItemCardapio *cardapio[MAX];   // vetor de ponteiros para a struct
    int opcao;

    inicializarVetor(cardapio);    // todas as posicoes = NULL

    int qtd = carregarArquivo(cardapio);
    printf("=== CARDAPIO DO RESTAURANTE / CAFETERIA ===\n");
    printf("%d item(ns) carregado(s) de %s\n", qtd, ARQUIVO);

    do {
        printf("\n========== MENU ==========\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Listar todos\n");
        printf("3 - Buscar por codigo\n");
        printf("4 - Relatorio\n");
        printf("5 - Salvar e sair\n");
        printf("Opcao: ");
        opcao = lerInteiro();
        if (feof(stdin)) opcao = 5;  // entrada encerrada - salva e sai

        switch (opcao) {
            case 1:
                cadastrarItem(cardapio);
                break;
            case 2:
                listarItens(cardapio);
                break;
            case 3:
                buscarItem(cardapio);
                break;
            case 4:
                relatorio(cardapio);
                break;
            case 5:
                if (salvarArquivo(cardapio)) {
                    printf("Dados salvos em %s.\n", ARQUIVO);
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 5);

    liberarMemoria(cardapio);      // free em cada posicao nao nula
    printf("Memoria liberada. Ate logo!\n");
    return 0;
}
