#include "utils.h"

#define TAXA_JUROS 0.05
#define LIMITE_PARCELA 0.20

void limpar_buffer();

/*
 * Isso retorna um ponteiro generico, que é modificado depois por um type cast
 * Ao usar um void * possibilita retornar um valor dinamico em tempo de execução
 * 
 * @param - type: Os paramentros como type devem ser do tipo: %d, %f, %c...
 * @param - size: é usado aqui a tipagem de size_t que significa que aceitamos apenas numeros positivos
 * @details Vale constar que não sabia que algo assim era possivel, apenas testei (falo do tipo dinamico em scanf(type, anyData))
 */
void *getAny (char *type, char *description, size_t size) {
  void *anyData = malloc(size);
  int error = 0;

  do {
    printf("%s", description);
    error = scanf(type, anyData);
    limpar_buffer();

    if (error == 0) {
      printf("Valor inválido. Por favor, tente novamente.\n");
    }
  } while (error == 0);

  return anyData;
}

/*
 * Aqui estou usando a biblioteca <stdint.h>,
 * essencial para definir o tamanho exato que eu desejo que o int tenha,
 * já que em diferentes sistemas operacionais, int pode ser definido dinamicamente pelo compilador.
 * Quero evitar que isso ocorra, então defini-mos quantos bits o int terá
 * existem:
 * int8_t -> 1 byte
 * int16_t -> 2 bytes
 * int32_t -> 4 bytes
 * int64_t -> 8 bytes
 * 
 * @ref há alguns dias atrás estava estudando sobre arvore binaria, então tinha visto types como essa, e fui atrás para entender
 */
int getNumber (char *description) {
  return *((int *) getAny("%d", description, sizeof(int32_t)));
}

float getFloat (char *description) {
  return *((float *) getAny("%f", description, sizeof(int64_t)));
}

void setString (char *output, char *description, size_t size) {
  printf("%s", description);
  fgets(output, sizeof(output), stdin);
  /*
   * A função fgets detecta o caracter \n (enter) como um input, por isso temos que remove-lo
   * Por que estamos usando aspas simples?
   * Bom, ao usar aspas duplas o C entende a variavel como char *, mas ao usar aspas simples, ele se torna char
   * se usarmos aspas duplas, teria que ser algo como: "\0"[0], mas com aspas simples isso não é necessário
   */
  output[strlen(output)] = '\0';

  return;
}

/* 
 * Função para alocar ou realocar memória para o array de clientes
 * @attention A função "realocar_memoria_cliente" deve ser implementada pelo aluno 
 */
Cliente* realocar_memoria_cliente(Cliente *clientes, int novo_tamanho) {
  printf("\nTamanho da realocação (cliente): %d", novo_tamanho);
  printf("\nEndereço antigo: %p", clientes);
  clientes = (Cliente *) realloc(clientes, novo_tamanho * sizeof(Cliente));
  printf("\nEndereço novo: %p\n", clientes);

  if (!clientes) {
    perror("Erro ao alocar memória para clientes");
    exit(1);
  }

  return clientes;
}

/* 
 * Função para alocar ou realocar memória para o array de emprestimos
 * @attention A função "realocar_memoria_emprestimo" deve ser implementada pelo aluno 
 */
Emprestimo* realocar_memoria_emprestimo(Emprestimo *emprestimos, int novo_tamanho) {
  printf("\nTamanho da realocação (emprestimo): %d", novo_tamanho);
  printf("\nEndereço antigo: %p", emprestimos);
  emprestimos = (Emprestimo *) realloc(emprestimos, novo_tamanho * sizeof(Emprestimo));
  printf("\nEndereço novo: %p\n", emprestimos);

  if (!emprestimos) {
    perror("Erro ao alocar memória para dos emprestimos");
    exit(1);
  }

  return emprestimos;
}

/* 
 * Cadastra um novo cliente e adiciona ao array de clientes
 * @attention A função "cadastrar_novo_cliente" deve ser implementada pelo aluno 
 */
Cliente *cadastrar_novo_cliente(Cliente *clients, int *num_clientes) {
  Cliente newClient;
  
  setString(newClient.nome, "Informe o nome: ", sizeof(newClient.nome));
  newClient.salario = getFloat("Informe o salário: ");
  newClient.id = *num_clientes + 1;
  newClient.num_emprestimos = 0;
  newClient.historico_emprestimos = NULL;
  
  clients = realocar_memoria_cliente(clients, *num_clientes + 1);
  clients[*num_clientes] = newClient;
  (*num_clientes)++;
  
  printf("Cadastro efetuado com êxito!\n");

  return clients;
}

/*
 * Aqui estou usando aritimedica de ponteiros
 * Algo que pratiquei na monitoria
 * @ref https://github.com/samuka7abr/Monitoria-EDA/blob/84c613d68418ece60490bc2f808260a1e3ac71fa/Exerc%C3%ADcios/A1/Ex1/entregas/Matheus%20Nilton%20Biolowons/q3.c
 */
void showUsers (Cliente *clients, size_t len) {
  for (Cliente *client = clients; client < clients + len; client++) {
    printf(" - Id: %d | Name: %s \n", client->id, client->nome);
  }
}

/* 
 * Solicita um novo empréstimo para um cliente
 * @attention A função "solicitar_novo_emprestimo" deve ser implementada pelo aluno 
 */
void solicitar_novo_emprestimo(Cliente *clientes, int num_clientes) {
  showUsers(clientes, num_clientes);
  int id = getNumber("Digite o Id do usuário desejado: ");

  Cliente* client = buscar_cliente_por_id(clientes, num_clientes, id);
  Emprestimo emprestimo;

  emprestimo.valor_emprestimo = getFloat("Informe o valor do emprestimo: ");
  emprestimo.num_parcelas = getNumber("Informe quantas parcelas: ");
  calcular_valor_parcela(&emprestimo);
  aprovar_reprovar_emprestimo(client, &emprestimo);
  adicionar_emprestimo_historico(client, emprestimo);
}

/* 
 * Calcula o valor da parcela do empréstimo
 * @attention A função "calcular_valor_parcela" deve ser implementada pelo aluno 
 */
void calcular_valor_parcela(Emprestimo *emprestimo) {
  float valor_emprestimo = emprestimo->valor_emprestimo + (emprestimo->valor_emprestimo * TAXA_JUROS);
  float valor_parcela = valor_emprestimo / emprestimo->num_parcelas;

  printf("\nValor total do emprestimo mais %.0f porcento de taxa é de: %.2f", TAXA_JUROS * 100, valor_emprestimo);
  printf("\nO valor da parcela será de: %.2f", valor_parcela);

  emprestimo->valor_emprestimo = valor_emprestimo;
  emprestimo->valor_parcela = valor_parcela;
}

/*
 * Criei isso para evitar redundancia, fazendo assim uma otimização pequena em "aprovar_reprovar_emprestimo"
 */
int checkLoan (float max, float value, Cliente *client, Emprestimo *loan) {
  if (value > max) {
    printf("\n\nEmprestimo do cliente %s, não foi aprovado!", client->nome);
    printf(
      "\nValor da parcela passa a cota maxima de %.0f porcento.\nValor Maximo: %f\nValor da solicitação: %f\n",
      LIMITE_PARCELA * 100,
      max,
      value
    );
  
    loan->ativo = 0;
    loan->aprovacao = 0;

    return 1;
  }

  loan->ativo = 1;
  loan->aprovacao = 1;

  return 0;
}

/* 
 * Aprova ou reprova o empréstimo com base no salário do cliente
 * @attention A função "aprovar_reprovar_emprestimo" deve ser implementada pelo aluno 
 */
void aprovar_reprovar_emprestimo(Cliente *cliente, Emprestimo *novo_emprestimo) {
  float totalValue = 0;
  float maxValue = cliente->salario * LIMITE_PARCELA;

  // Caso o valor da parcela já seja maior que o valor maximo, já retorne, evitando o loop O(n)
  if (checkLoan(maxValue, novo_emprestimo->valor_parcela, cliente, novo_emprestimo)) return;

  for (size_t index = 0; index < cliente->num_emprestimos; index++) {
    if (cliente->historico_emprestimos[index].ativo == 0) continue;

    totalValue += cliente->historico_emprestimos[index].valor_parcela;
  }

  checkLoan(maxValue, novo_emprestimo->valor_parcela + totalValue, cliente, novo_emprestimo);
  return;
}

// Carrega os clientes do arquivo CSV e retorna um array de clientes
// O número de clientes é retornado através do ponteiro num_clientes
// O arquivo deve ter o formato: id,nome,salario
// Exemplo: 1,João,3000.00
Cliente *carregar_clientes(const char *nome_arquivo, int *num_clientes) {
    FILE *arquivo = fopen(nome_arquivo, "r"); // Abre o arquivo para leitura
    if (!arquivo) { // Verifica se o arquivo foi aberto corretamente
        // Se não, imprime uma mensagem de erro e retorna NULL
        perror("Erro ao abrir arquivo de clientes");
        return NULL;
    }

    Cliente *clientes = NULL;   // Inicializa o ponteiro para clientes
    *num_clientes = 0;          // Inicializa o número de clientes   
    char linha[256];            // Buffer para ler cada linha do arquivo
    fgets(linha, sizeof(linha), arquivo); // Ler e descartar o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
        Cliente novo_cliente; // Inicializa um novo cliente
        // Lê os dados do cliente da linha e armazena em novo_cliente
        // O formato esperado é: id,nome,salario
        // Exemplo: 1,João,3000.00
        // O sscanf retorna o número de itens lidos com sucesso
        if (sscanf(linha, "%d,%[^,],%f", &novo_cliente.id, novo_cliente.nome, &novo_cliente.salario) == 3) {

            /* 
                ATENÇÃO: A função "realocar_memoria_cliente" deve ser implementada pelo aluno 
            */

            // Realoca memória para mais um cliente
            clientes = realocar_memoria_cliente(clientes, (*num_clientes + 1));

            // Verifica se a realocação foi bem-sucedida
            if (!clientes) {
                perror("Erro ao alocar memória para clientes");
                fclose(arquivo);
                return NULL;
            }
            // Adiciona o novo cliente ao array
            novo_cliente.historico_emprestimos = NULL; // Inicializa o histórico de empréstimos
            novo_cliente.num_emprestimos = 0; // Inicializa o número de empréstimos
            // Adiciona o novo cliente ao array de clientes
            // O ponteiro clientes é atualizado para apontar para o novo array
            clientes[*num_clientes] = novo_cliente;
            (*num_clientes)++; // Incrementa o número de clientes
        } else { // Se a leitura falhar, imprime uma mensagem de erro
            fprintf(stderr, "Erro ao ler linha do arquivo de clientes: %s", linha);
        }
    }

    fclose(arquivo);
    return clientes;
}


// Carrega os empréstimos do arquivo e associa aos clientes
// O número de empréstimos é retornado através do ponteiro num_emprestimos
// O arquivo deve ter o formato: cliente_id,valor_emprestimo,num_parcelas
// Exemplo: 1,1000.00,12
Emprestimo *carregar_emprestimos(const char *nome_arquivo, Cliente *clientes, int num_clientes) {
    FILE *arquivo = fopen(nome_arquivo, "r"); // Abre o arquivo para leitura
    if (!arquivo) { // Verifica se o arquivo foi aberto corretamente
        perror("Erro ao abrir arquivo de emprestimos");
        return NULL;
    }

    Emprestimo *todos_emprestimos = NULL;   // Inicializa o ponteiro para todos os empréstimos
    int num_emprestimos_total = 0;          // Inicializa o número total de empréstimos
    char linha[256];                        // Buffer para ler cada linha do arquivo
    fgets(linha, sizeof(linha), arquivo);   // Ler e descartar o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
        Emprestimo novo_emprestimo; // Inicializa um novo empréstimo
        // Lê os dados do empréstimo da linha e armazena em novo_emprestimo
        // O formato esperado é: cliente_id,valor_emprestimo,num_parcelas
        // Exemplo: 1,1000.00,12
        // O sscanf retorna o número de itens lidos com sucesso
        if (sscanf(linha, "%d,%f,%d", &novo_emprestimo.cliente_id, &novo_emprestimo.valor_emprestimo, &novo_emprestimo.num_parcelas) == 3) {

            /* 
                ATENÇÃO: A função "calcular_valor_parcela" deve ser implementada pelo aluno 
            */

            // Calcula o valor da parcela do empréstimo
            calcular_valor_parcela(&novo_emprestimo);   // Calcula o valor da parcela
            novo_emprestimo.ativo = 1;                  // Assume como ativo ao carregar

            // Busca o cliente correspondente ao ID do empréstimo
            // O cliente é buscado no array de clientes
            // Se o cliente for encontrado, aprova ou reprova o empréstimo
            // e adiciona ao histórico do cliente
            // Se o cliente não for encontrado, imprime uma mensagem de aviso
            // e não adiciona o empréstimo ao histórico
            Cliente *cliente = buscar_cliente_por_id(clientes, num_clientes, novo_emprestimo.cliente_id); // Busca o cliente
            // Se o cliente for encontrado, aprova ou reprova o empréstimo
            if (cliente) {

                /* 
                    ATENÇÃO: A função "aprovar_reprovar_emprestimo" deve ser implementada pelo aluno 
                */
                aprovar_reprovar_emprestimo(cliente, &novo_emprestimo);     // Aprova ou reprova o empréstimo
                adicionar_emprestimo_historico(cliente, novo_emprestimo);   // Adiciona o empréstimo ao histórico do cliente

                /* 
                    ATENÇÃO: A função "realocar_memoria_emprestimo" deve ser implementada pelo aluno 
                */
                todos_emprestimos = realocar_memoria_emprestimo(todos_emprestimos, (num_emprestimos_total + 1));

                if (!todos_emprestimos) {
                    perror("Erro ao alocar memória para todos os emprestimos");
                    fclose(arquivo);
                    return NULL;
                }
                todos_emprestimos[num_emprestimos_total] = novo_emprestimo;
                num_emprestimos_total++;
            } else {
                fprintf(stderr, "Aviso: Cliente com ID %d não encontrado para o empréstimo.\n", novo_emprestimo.cliente_id);
            }
        } else {
            fprintf(stderr, "Erro ao ler linha do arquivo de emprestimos: %s", linha);
        }
    }

    fclose(arquivo);
    return todos_emprestimos;
}


// Adiciona um novo empréstimo ao histórico do cliente
void adicionar_emprestimo_historico(Cliente *cliente, Emprestimo emprestimo) {

    /* 
        ATENÇÃO: A função "realocar_memoria_emprestimo" deve ser implementada pelo aluno
    */
    cliente->historico_emprestimos = realocar_memoria_emprestimo(cliente->historico_emprestimos, cliente->num_emprestimos + 1);

    // Verifica se a realocação foi bem-sucedida
    if (!cliente->historico_emprestimos) { // Se a realocação falhar, imprime uma mensagem de erro
        perror("Erro ao alocar memória para histórico de empréstimos");
        if (!cliente->historico_emprestimos) { // Se o histórico de empréstimos for NULL
            // Tenta alocar memória para o primeiro empréstimo
            perror("Erro ao alocar memória para histórico de empréstimos");
            if (cliente->num_emprestimos == 0) { // Se não houver empréstimos
                // Aloca memória para o primeiro empréstimo
                cliente->historico_emprestimos = malloc(sizeof(Emprestimo));
                if (!cliente->historico_emprestimos) {
                    perror("Erro crítico ao alocar memória para o primeiro empréstimo");
                    return;
                }
            } else {
                return; // Já existe memória alocada, erro ao realocar
            } 
        }       
    }
    // Adiciona o novo empréstimo ao histórico
    cliente->historico_emprestimos[cliente->num_emprestimos] = emprestimo;
    // não existia essa parte do código, então me dei direito a coloca-lo
    cliente->historico_emprestimos[cliente->num_emprestimos].cliente_id = cliente->id;
    cliente->num_emprestimos++;
}

// Lista todos os clientes e seus empréstimos
void listar_clientes(const Cliente *clientes, int num_clientes) {
    system(LIMPAR_TELA);
    printf("\n--- Lista de Clientes ---\n");
    for (int i = 0; i < num_clientes; i++) { 

        printf("ID: %d, Nome: %s, Salário: %.2f, Empréstimos: %d\n",
               clientes[i].id, clientes[i].nome, clientes[i].salario, clientes[i].num_emprestimos);
        if (clientes[i].historico_emprestimos && clientes[i].num_emprestimos > 0) {
            printf("  Histórico de Empréstimos:\n");
            for (int j = 0; j < clientes[i].num_emprestimos; j++) {
                printf("    ID Cliente: %d, Valor: %.2f, Parcelas: %d, Parcela: %.2f, Status: ",
                       clientes[i].historico_emprestimos[j].cliente_id,
                       clientes[i].historico_emprestimos[j].valor_emprestimo,
                       clientes[i].historico_emprestimos[j].num_parcelas,
                       clientes[i].historico_emprestimos[j].valor_parcela);
                    
                    if (clientes[i].historico_emprestimos[j].aprovacao) {
                        printf("Aprovado");
                    } else {
                        printf(COR_VERMELHO "Reprovado" COR_RESET);
                    }

                    printf(", Ativo: %s\n", clientes[i].historico_emprestimos[j].ativo ? "Sim" : "Não");
            }
        } else {
            printf("  Sem histórico de empréstimos.\n");
        }
    }
    printf("-------------------------\n");
}

// Lista todos os empréstimos carregados
void listar_emprestimos(const Cliente *clientes, int num_clientes) {
    system(LIMPAR_TELA);
    printf("\n--- Lista de Empréstimos Carregados ---\n");
    int total_emprestimos = 0;
    for (int i = 0; i < num_clientes; i++) {
        total_emprestimos += clientes[i].num_emprestimos;
        if (clientes[i].historico_emprestimos) {
            for (int j = 0; j < clientes[i].num_emprestimos; j++) {
                printf("Cliente ID: %d, Valor: %.2f, Parcelas: %d, Parcela: %.2f, Status: ",
                       clientes[i].historico_emprestimos[j].cliente_id,
                       clientes[i].historico_emprestimos[j].valor_emprestimo,
                       clientes[i].historico_emprestimos[j].num_parcelas,
                       clientes[i].historico_emprestimos[j].valor_parcela);
                
                        if (clientes[i].historico_emprestimos[j].aprovacao) {
                            printf("Aprovado");
                        } else {
                            printf(COR_VERMELHO "Reprovado" COR_RESET);
                        }
                        printf(", Ativo: %s\n", clientes[i].historico_emprestimos[j].ativo ? "Sim" : "Não");
            }
        }
    }
    printf("Total de empréstimos carregados: %d\n", total_emprestimos);
    printf("---------------------------------------\n");
}

// Busca um cliente pelo ID
Cliente *buscar_cliente_por_id(Cliente *clientes, int num_clientes, int id) {
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].id == id) {
            return &clientes[i];
        }
    }
    return NULL;
}

// Libera a memória alocada para os clientes e seus históricos de empréstimos
void liberar_memoria(Cliente *clientes, int num_clientes) {
    if (clientes) {
        for (int i = 0; i < num_clientes; i++) {
            free(clientes[i].historico_emprestimos);
        }
        free(clientes);
    }
}

// Definição de macros para limpar a tela
void limpar_buffer(){
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}