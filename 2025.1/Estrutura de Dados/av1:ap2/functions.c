#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int selectClient(Client *clients, int numClients) {
  int selectedId;
  do {
    printf("\n--- Seleção de Clientes ---\n");
    for (int i = 0; i < numClients; i++) {
      if (clients[i].name[0] != '\0')
        printf("Id: %d - Nome: %s\n", i + 1, clients[i].name);
      else
        printf("Id: %d - Nome: [Não cadastrado]\n", i + 1);
    }
    printf("Digite o Id do cliente que deseja acessar: ");

    selectedId = getInt();
    if (selectedId < 1 || selectedId > numClients) {
      printf("Opção inválida. Tente novamente.\n");
    }
  
  } while (selectedId < 1 || selectedId > numClients);

  clearScreen();
  return selectedId - 1;
}

int menu(Client *client) {
  int option;

  printf("\n--- Menu do Cliente ---\n");
  if (client->name[0] == '\0') {
    printf("1. Realizar Cadastro\n");
    printf("0. Retornar ao Menu Principal\n");
  } else {
    printf("1. Solicitar Empréstimo\n");
    printf("2. Visualizar Empréstimo\n");
    printf("0. Retornar ao Menu Principal\n");
  }

  printf("Informe a opção desejada: ");
  option = getInt();
  clearScreen();

  return option;
}

int validateInt(const char *str) {
  char *endptr;
  strtol(str, &endptr, 10);

  while (isspace(*endptr))
    endptr++;
  return (*endptr == '\0' || *endptr == '\n');
}

int getInt() {
  char cache[32];

  if (fgets(cache, sizeof(cache), stdin) == NULL || !validateInt(cache)) {
    printf("Entrada inválida.\n");
    return -1;
  }

  return atoi(cache);
}

void clearCache(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}
