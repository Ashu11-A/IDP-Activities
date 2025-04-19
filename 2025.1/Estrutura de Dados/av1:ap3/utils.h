#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <stdint.h>

#ifdef _WIN32
    #define LIMPAR_TELA "cls"
#else
    #define LIMPAR_TELA "clear"
#endif

// Definição de cores ANSI (suportado em alguns terminais)
#define COR_VERMELHO "\033[31m"
#define COR_RESET "\033[0m"

#define MAX_NOME 50

typedef struct Emprestimo {
    int cliente_id;
    float valor_emprestimo;
    int num_parcelas;
    float valor_parcela;
    int aprovacao;          // 1 para "Aprovado". 0 para "Reprovado"
    int ativo;              // 1 para "Ativo", 0 para "Inativo"
} Emprestimo;

typedef struct Cliente {
    int id;
    char nome[MAX_NOME];
    float salario;
    Emprestimo *historico_emprestimos;
    int num_emprestimos;
} Cliente;

// Protótipos das funções em utils.c

// Funções já implementadas
void limpar_buffer();
Cliente *carregar_clientes(const char *nome_arquivo, int *num_clientes);
Emprestimo *carregar_emprestimos(const char *nome_arquivo, Cliente *clientes, int num_clientes);
void adicionar_emprestimo_historico(Cliente *cliente, Emprestimo emprestimo);
void listar_clientes(const Cliente *clientes, int num_clientes);
void listar_emprestimos(const Cliente *clientes, int num_clientes);
Cliente *buscar_cliente_por_id(Cliente *clientes, int num_clientes, int id);
void liberar_memoria(Cliente *clientes, int num_clientes);


// ATENÇÃO: As funções abaixo devem ser implementadas pelo aluno

/*
    ATENÇÃO: As funções "realocar_memoria_cliente" e "realocar_memoria_emprestimo" devem ser implementadas pelo aluno
    Essas funções devem alocar ou realocar memória para os arrays de clientes e empréstimos, respectivamente.
*/
Cliente *realocar_memoria_cliente(Cliente *clientes, int novo_tamanho);
Emprestimo* realocar_memoria_emprestimo(Emprestimo *emprestimos, int novo_tamanho);

/*
    ATENÇÃO: As funções "cadastrar_novo_cliente" e "solicitar_novo_emprestimo" devem ser implementadas pelo aluno
    Essas funções devem permitir o cadastro de novos clientes e a solicitação de novos empréstimos, respectivamente.
*/
Cliente *cadastrar_novo_cliente(Cliente *clientes, int *num_clientes);
void solicitar_novo_emprestimo(Cliente *clientes, int num_clientes);

/* 
    ATENÇÃO: A função "calcular_valor_parcela" e "aprovar_reprovar_emprestimo" devem ser implementadas pelo aluno
    Essas funções devem calcular o valor da parcela do empréstimo e aprovar ou reprovar o empréstimo, respectivamente.
*/
void calcular_valor_parcela(Emprestimo *emprestimo);
void aprovar_reprovar_emprestimo(Cliente *cliente, Emprestimo *novo_emprestimo);

#endif
