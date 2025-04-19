// main.c
#include "utils.c"

/*
 * Activity: [av1ap3] Atividade Prática 3 - Cadastro de Clientes e Solicitação de Empréstimos
 * Alunos:
 *  - Matheus Nilton Biolowons
 *  - João Antônio Barbosa Pereira
 * Date: 16/04/2025
 */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <clientes.csv> <emprestimos.csv>\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo_clientes = argv[1];
    const char *nome_arquivo_emprestimos = argv[2];

    int num_clientes = 0;
    Cliente *clientes = carregar_clientes(nome_arquivo_clientes, &num_clientes);
    if (!clientes) {
      return 1;
    }

    // Os empréstimos são carregados e adicionados ao histórico dos clientes
    carregar_emprestimos(nome_arquivo_emprestimos, clientes, num_clientes);

    int opcao;
    do {
      printf("\n----------------𝐄𝐧𝐭𝐫𝐚𝐝𝐚------------------------------\n");
      printf("1 - Cadastrar Novo Cliente\n");
      printf("2 - Solicitar Novo Empréstimo\n");
      printf("3 - Listar Clientes e seus Empréstimos\n");
      printf("4 - Listar todos os Empréstimos Carregados\n");
      printf("0 - Sair\n");
      printf("-------------------------------------------------------\n");
      printf("Digite a opção desejada: ");
      scanf("%d", &opcao);
      getchar(); // Limpar o buffer do teclado

      switch (opcao) {
        case 1:
          Cliente *temp_clientes = cadastrar_novo_cliente(clientes, &num_clientes);
          if (temp_clientes) {
            clientes = temp_clientes;
          } else {
            fprintf(stderr, "Erro ao cadastrar novo cliente.\n");
          }
          break;
        case 2:
          solicitar_novo_emprestimo(clientes, num_clientes);
          break;
        case 3:
          listar_clientes(clientes, num_clientes);
          break;
        case 4:
          listar_emprestimos(clientes, num_clientes);
          break;
        case 0:
          printf("Saindo...\n");
          break;
        default:
          printf("Opção inválida. Tente novamente.\n");
      }
  } while (opcao != 0);

  liberar_memoria(clientes, num_clientes);

  return 0;
}