#include "types.h"
#include <stdio.h>
#include <string.h>

/*
 * Atividade: [av1:ap2]: (SIMULAÇÃO DE EMPRÉSTIMOS) Atividade Prática 2 - Usando structs e functions
 * 
 * Alunos:
 *   -> Matheus Nilton Biolowons
 */

int main() {
  int option;
  Client clients[10];
  int len = sizeof(clients) / sizeof(Client);

  // Inicializa os clientes com nome vazio e valores zerados, isso evita muitos erros!
  for (int i = 0; i < len; i++) {
    Loan loan = {0, 0, 0.0, 0};
    clients[i] = (Client){"", 0, loan};
  }

  do {
    printf("\n--- Menu Principal ---\n");
    printf("1. Selecionar Cliente\n");
    printf("2. Mostrar Resultados\n");
    printf("0. Sair\n");
    printf("Informe a opção desejada: ");
    option = getInt();
    clearScreen();

    switch (option) {
    case 1: {
      int clientIndex = selectClient(clients, len);
      int subOption;
      do {
        subOption = menu(&clients[clientIndex]);
        if (subOption == -1) continue;
        int error;
        switch (subOption) {
        case 1:
          // Se o cliente ainda não estiver cadastrado, realiza o cadastro
          if (clients[clientIndex].name[0] == '\0') {
            printf("Informe o nome: ");
            fgets(clients[clientIndex].name, sizeof(clients[clientIndex].name), stdin);
            clients[clientIndex].name[strcspn(clients[clientIndex].name, "\n")] = '\0';

            do {
              printf("Informe o salário: ");
              error = scanf("%f", &clients[clientIndex].salary);

              if (error == 0) {
                printf("Valor inválido. Por favor, tente novamente.\n");
                clearCache();
              }
              else if (clients[clientIndex].salary < 0) {
                printf("O salário deve ser maior ou igual a 0.\n");
                error = 0;
              }
            } while (error == 0);
            clearCache();

            printf("Cadastro efetuado com êxito!\n");
          } else {
            // Cadastro de solicitação de empréstimo para cliente já cadastrado
            printf("Cadastro de solicitação de empréstimo para %s\n", clients[clientIndex].name);

            do {
              printf("Informe a quantidade de meses para o empréstimo: ");
              error = scanf("%d", &clients[clientIndex].loan.months);
              clearCache();

              if (error == 0) {
                printf("Valor inválido. Por favor, tente novamente.\n");
              } else if (clients[clientIndex].loan.months <= 0) {
                printf("O número de meses deve ser maior que 0.\n");
                error = 0;
              }
            } while (error == 0);

            do {
              printf("Informe o valor do empréstimo: ");
              error = scanf("%f", &clients[clientIndex].loan.value);

              if (error == 0) {
                printf("Valor inválido. Por favor, tente novamente.\n");
                clearCache();
              } else if (clients[clientIndex].loan.value <= 0) {
                printf("O valor do empréstimo deve ser maior que 0.\n");
                error = 0;
              }
            } while (error == 0);

            clients[clientIndex].loan.plot = clients[clientIndex].loan.value / clients[clientIndex].loan.months;
            float maximumValue = clients[clientIndex].salary * 0.2;

            printf("\n--- Detalhes do Empréstimo ---\n");
            printf("Valor máximo permitido: R$%.2f\n", maximumValue);
            printf("Valor da parcela: R$%.2f\n", clients[clientIndex].loan.plot);
            printf("Valor solicitado: R$%.2f\n", clients[clientIndex].loan.value);

            if (clients[clientIndex].loan.plot > maximumValue) {
              printf("Empréstimo reprovado.\n");
              clients[clientIndex].loan.approved = 0;
              break;
            }

            clients[clientIndex].loan.approved = 1;
            printf("Empréstimo aprovado.\n");
            clearCache();
          }
          break;
        case 2:
          if (clients[clientIndex].name[0] == '\0') {
            printf("Cliente não cadastrado.\n");
            break;
          }

          if (clients[clientIndex].loan.value <= 0) {
            printf("Nenhuma solicitação de empréstimo registrada.\n");
            break;
          }

          printf("\n--- Visualização dos Detalhes do Empréstimo ---\n");
          printf("|-----------------------------------------|\n");
          printf("  -> Total de meses: %d\n", clients[clientIndex].loan.months);
          printf("  -> Valor solicitado: R$%.2f\n", clients[clientIndex].loan.value);
          printf("  -> Valor da parcela: R$%.2f\n", clients[clientIndex].loan.plot);
          printf("  -> Status: %s\n", clients[clientIndex].loan.approved ? "Aprovado" : "Não Aprovado");
          printf("|-----------------------------------------|\n");
          break;
        case 0:
          printf("Retornando ao Menu Principal...\n");
          break;
        default:
          printf("Opção inválida! Por favor, escolha uma opção válida.\n");
          break;
        }
      } while (subOption != 0);
    }
    break;
    case 2:
      printf("Resultado Geral: \n");

      int approved = 0;
      int registered = 0;

      for (int index = 0; index < len; index++) {
        if (clients[index].loan.approved == 1) approved++;
        if (clients[index].name[0] != '\0') registered++;
      }

      printf("Usuários registrados: %d\n", registered);
      printf("Usuários com Empréstimo: %d\n", approved);
      break;
    case 0:
      printf("Encerrando o programa. Obrigado!\n");
      break;
    default:
      printf("Opção inválida! Por favor, escolha uma opção válida.\n");
      break;
    }
  } while (option != 0);

  return 0;
}
