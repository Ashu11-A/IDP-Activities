#include "utils.h"
#define Aprovado 1
#define Reprovado 0

void liberar_memoria(Aluno *alunos, int num_alunos);
void limpar_buffer();

/*
 * Função para alocar ou realocar memória para o array de avaliacoes
 * Em caso de erro, o programa é encerrado!
 * 
 * @param alunos Deve ser um pontero, pois a função realloc retorna um ponteiro para um novo endereço (ou não).
 * @param novo_tamanho Isso é essencial, pois caso não passado,
 *  seria necessário usar aritimetica de ponteiro para calcular quantos elementos estão alocados na memoria,
 *  e no caso somar mais 1.
 * 
 * @warning Essa função deve ser implementada
 */

Aluno* realocar_memoria_aluno(Aluno *alunos, int novo_tamanho) {
  alunos = (Aluno *) realloc(alunos, (novo_tamanho * sizeof(Aluno)));

  if (alunos == NULL) {
    printf("Ocorreu um erro ao alocar a memoria!");
    exit(1);
  }

  return alunos; 
}

/*
 * Função para alocar ou realocar memória para o array de avaliacoes
 * @warning Essa função deve ser implementada
 */
Avaliacao* realocar_memoria_avaliacao(Avaliacao *avaliacoes, int novo_tamanho) {
  avaliacoes = (Avaliacao *) realloc(avaliacoes, (novo_tamanho * sizeof(Avaliacao)));

  if (avaliacoes == NULL) {
    printf("Ocorreu um erro ao alocar a memoria!");
    exit(1);
  }

  return avaliacoes; 
}

/*
 * Função para adicionar uma nova avaliação ao histórico de avaliações do aluno.
 * Aqui realocamos expaço suficiente para implementar mais uma avaliação
 * 
 * @fn realocar_memoria_avaliacao - usada para realocar memoria, que usa internamente a função realloc
 * 
 * @warning Essa função deve ser implementada
 */
void adicionar_avaliacoes(Aluno *aluno, Avaliacao avaliacao) {
  aluno->avaliacoes = realocar_memoria_avaliacao(aluno->avaliacoes, (aluno->num_avaliacoes + 1));
  aluno->avaliacoes[aluno->num_avaliacoes] = avaliacao;
  aluno->num_avaliacoes++;
}

/*
 * Essa função faz o parser do numero para que fique no intervalo de 1 a 10
 * 
 * @example int number = formatNumber(-1) // 0
 * @example int number = formatNumber(11) // 10
 */
float formatNumber (float number) {
  return number > 10
    ? 10
    : number < 0
      ? 0
      : number;
}

/* 
 * Função para calcular as notas finais do aluno,
 *
 * Estou usando aritimedica de ponteiros, primero aponto a variavel *avaliação para aluno->avaliacoes, que me retorna o primeiro ponteiro.
 * Para saber quanto eu posso andar eu faço o calculo de aluno->avaliacoes + aluno->num_avaliacoes
 * isso retorna o endereço de memoria do ultimo elemento, que possibilita eu fazer uma comparação de (A < B).
 * E para seguir para o proximo elemento, apenas incremento (avaliação++)
 * 
 * Essa função usa os parametros #define declarados no começo desse arquivo
 * eles são:
 * 
 * @def Aprovado // 1
 * @def Reprovado // 0
 * 
 * @warning Função para liberar a memória alocada para os alunos
 */
void calcular_notas(Aluno *aluno) {
  float notaFinal = 0;
  float bimestreTotal = 0;
  
  for (Avaliacao *avaliação = aluno->avaliacoes; avaliação < aluno->avaliacoes + aluno->num_avaliacoes; avaliação++) {
    avaliação->ap1 = formatNumber(avaliação->ap1);
    avaliação->ap2 = formatNumber(avaliação->ap2);
    avaliação->ap3 = formatNumber(avaliação->ap3);
    avaliação->np = formatNumber(avaliação->np);

    float notaAvaliação = (avaliação->ap1 + avaliação->ap2 + avaliação->ap3) / 3;
    float valorAvaliação = notaAvaliação * 0.3;
    float valorProva = avaliação->np * 0.7;

    bimestreTotal += valorAvaliação + valorProva;
  }

  notaFinal = formatNumber(bimestreTotal / 2);
  aluno->nf = notaFinal;
  aluno->status = aluno->nf >= 6 ? Aprovado : Reprovado;
}

/*
 * Aqui estamos usando aritimedica de ponteiros para percorrer todos os alunos e limpar as alocações.
 * Primeiro temos que limpar a memoria alocada para as avaliações.
 * Posteriormente limpamos a alocação feita para o aluno.
 * 
 * @warning Função para liberar a memória alocada para os alunos
 */
void liberar_memoria(Aluno *alunos, int num_alunos) {
  if (alunos != NULL) {
    for (Aluno *aluno = alunos; aluno < alunos + num_alunos; aluno++) {
      free(aluno->avaliacoes);
    }
    free(alunos);
  }
}

// Definição de macros para limpar a tela
void limpar_buffer(){
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}

// Carrega os alunos do arquivo CSV e retorna um array de alunos
// O número de alunos é retornado através do ponteiro num_alunos
Aluno *carregar_alunos(const char *nome_arquivo, int *num_alunos) {
    // Abre o arquivo para leitura
    FILE *arquivo = fopen(nome_arquivo, "r"); 
    // Verifica se o arquivo foi aberto corretamente
    // Se não, imprime uma mensagem de erro e retorna NULL    
    if (!arquivo) { 
        perror("Erro ao abrir arquivo de alunos");
        return NULL;
    }

    Aluno *alunos   = NULL;                 // Inicializa o ponteiro para alunos
    char linha[256];                        // Buffer para ler cada linha do arquivo
    fgets(linha, sizeof(linha), arquivo);   // Ler e descartar o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
        Aluno novo_aluno;       // Inicializa um novo aluno
        Avaliacao avaliacao1;   // Inicializa a avaliação 1
        Avaliacao avaliacao2;   // Inicializa a avaliação 2
        // Lê os dados do aluno da linha e armazena em novo_aluno
        // O sscanf retorna o número de itens lidos com sucesso
        float av1ap1, av1ap2, av1ap3, np1, av2ap1, av2ap2, av2ap3, np2;
        if (sscanf(linha, "%d,%[^,],%f,%f,%f,%f,%f,%f,%f,%f", &novo_aluno.matricula, novo_aluno.nome, &avaliacao1.ap1,&avaliacao1.ap2, &avaliacao1.ap3, &avaliacao1.np, &avaliacao2.ap1, &avaliacao2.ap2,&avaliacao2.ap3, &avaliacao2.np) == 10) {

            /* 
                ATENÇÃO: Essa função deve ser implementada
            */             
            // Realoca memória para mais um aluno
            alunos = realocar_memoria_aluno(alunos, (*num_alunos + 1));

            // Verifica se a realocação foi bem-sucedida
            if (!alunos) {
                perror("Erro ao alocar memória para alunos");
                fclose(arquivo);
                return NULL;
            }

            // Adiciona o novo aluno ao array
            novo_aluno.avaliacoes = NULL; // Inicializa o histórico de avaliações
            novo_aluno.num_avaliacoes = 0; // Inicializa o número de avaliacoes

            /* 
                ATENÇÃO: Essa função deve ser implementada
            */             
            adicionar_avaliacoes(&novo_aluno, avaliacao1); // Adiciona a primeira avaliação
            adicionar_avaliacoes(&novo_aluno, avaliacao2); // Adiciona a segunda avaliação

            /* 
                ATENÇÃO: Essa função deve ser implementada
            */ 
            calcular_notas(&novo_aluno); // Calcula as notas do aluno

            // Adiciona o novo aluno ao array de alunos
            // O ponteiro alunos é atualizado para apontar para o novo array
            alunos[*num_alunos] = novo_aluno;
            (*num_alunos)++; // Incrementa o número de alunos

        } else { // Se a leitura falhar, imprime uma mensagem de erro
            fprintf(stderr, "Erro ao ler linha do arquivo de alunos: %s", linha);
        }
    }

    fclose(arquivo);
    return alunos;
}

// Função para ordenar os alunos por nota final (NF)
void ordenar_alunos(Aluno *alunos, int num_alunos) {
    for (int i = 0; i < num_alunos - 1; i++) {
        for (int j = 0; j < num_alunos - i - 1; j++) {
            if (alunos[j].nf < alunos[j + 1].nf) {
                Aluno temp = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = temp;
            }
        }
    }
}

void listar_alunos(const Aluno *alunos, int num_alunos) {
    if (num_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n");
    printf("%-10s | %-30s | %-8s | %-8s | %-8s | %-8s | %-8s | %-8s | %-8s | %-8s | %-8s | %-10s\n",
           " Matrícula", "Nome", "AV1:AP1", "AV1:AP2", "AV1:AP3", "NP1", "AV2:AP1", "AV2:AP2", "AV2:AP3", "NP2", "NF", "Status");
    printf("-----------|--------------------------------|----------|----------|----------|----------|----------|----------|----------|----------|----------|------------\n");

    for (int i = 0; i < num_alunos; i++) {
        if (alunos[i].status == 0) { // Reprovado
            printf(RED_TEXT);
        }
        printf("%-10d | %-30s | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-8.2f | %-10s",
                alunos[i].matricula,
                alunos[i].nome,
                alunos[i].avaliacoes[0].ap1,
                alunos[i].avaliacoes[0].ap2,
                alunos[i].avaliacoes[0].ap3,
                alunos[i].avaliacoes[0].np,
                alunos[i].avaliacoes[1].ap1,
                alunos[i].avaliacoes[1].ap2,
                alunos[i].avaliacoes[1].ap3,
                alunos[i].avaliacoes[1].np,
                alunos[i].nf,
                alunos[i].status == 1 ? "Aprovado" : "Reprovado");
        if (alunos[i].status == 0) {
            printf(RESET_TEXT);
        }
        printf("\n");
    }
    printf("\n");
}
