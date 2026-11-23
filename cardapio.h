#ifndef CARDAPIO_H
#define CARDAPIO_H

// Constantes
#define MAX 100                 // capacidade maxima 
#define TAM_NOME 51
#define TAM_CATEGORIA 21
#define ARQUIVO "dados.txt"     // arquivo texto de persistencia
#define TAM_LINHA 200

// Registro 
typedef struct {
    int codigo;                       // identificador unico
    char nome[TAM_NOME];              // nome do prato / bebida
    char categoria[TAM_CATEGORIA];    // Prato, Bebida, Lanche, Sobremesa
    double preco;                     // preco em R$
    int tempoPreparo;                 // tempo estimado em minutos
} ItemCardapio;

// Prototipos das operacoes do TAD 

// Memoria
void inicializarVetor(ItemCardapio *vetor[]);
void liberarMemoria(ItemCardapio *vetor[]);

// Arquivo
int carregarArquivo(ItemCardapio *vetor[]);
int salvarArquivo(ItemCardapio *vetor[]);

// Operacoes do menu
void cadastrarItem(ItemCardapio *vetor[]);
void listarItens(ItemCardapio *vetor[]);
void buscarItem(ItemCardapio *vetor[]);
void relatorio(ItemCardapio *vetor[]);

// Auxiliares
int buscarPosicao(ItemCardapio *vetor[], int codigo);
int posicaoLivre(ItemCardapio *vetor[]);
int contarItens(ItemCardapio *vetor[]);
void exibirItem(ItemCardapio *item);
void lerTexto(char texto[], int tamanho);
int lerInteiro(void);
double lerReal(void);
void lerCategoria(char categoria[]);

#endif
