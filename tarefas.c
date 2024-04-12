// tarefas.c

#include "tarefass.h"
#include <stdio.h>
#include <string.h>
#define TOTAL 100

void tira_quebra_linha(char *str) {
  int len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}

ERROS criar(Tarefa tarefas[], int *pos) {
  if (*pos >= TOTAL)
    return MAX_TAREFA;

  printf("Entre com a prioridade: ");
  scanf("%d", &tarefas[*pos].prioridade);
  if (tarefas[*pos].prioridade < 1 || tarefas[*pos].prioridade > 10) {
    printf("Prioridade inválida , o limite é entre 1 e 10 tente novamente!\n");
    return OK; // Retorna OK aqui para indicar que a tarefa não foi criada devido à prioridade inválida
  }

  clearBuffer(); // Limpa o buffer do teclado para evitar problemas de entrada

  printf("Entre com a categoria: ");
  fgets(tarefas[*pos].categoria, 100, stdin);
  tira_quebra_linha(tarefas[*pos].categoria);

  printf("Entre com a descricao: ");
  fgets(tarefas[*pos].descricao, 300, stdin);
  tira_quebra_linha(tarefas[*pos].descricao);

  *pos = *pos + 1;

  return OK;
}

ERROS deletar(Tarefa tarefas[], int *pos) {
  // teste se existem tarefas
  if (*pos == 0)
    return SEM_TAREFAS;

  // verifica se a tarefa escolhida existe
  int pos_deletar;
  printf("Entre com a posicao da tarefa a ser deletada: ");
  scanf("%d", &pos_deletar);
  pos_deletar--; // garantir posicao certa no array
  if (pos_deletar >= *pos || pos_deletar < 0)
    return NAO_ENCONTRADO;

  for (int i = pos_deletar; i < *pos - 1; i++) {
    tarefas[i].prioridade = tarefas[i + 1].prioridade;
    strcpy(tarefas[i].categoria, tarefas[i + 1].categoria);
    strcpy(tarefas[i].descricao, tarefas[i + 1].descricao);
  }

  *pos = *pos - 1;

  return OK;
}

ERROS listar(Tarefa tarefas[], int pos) {
    if (pos == 0)
        return SEM_TAREFAS;

    char categoria[100];
    printf("Entre com a categoria das tarefas que deseja listar (ou deixe vazio para listar todas as tarefas): ");
    fgets(categoria, 100, stdin);
    tira_quebra_linha(categoria);

    int encontradas = 0;
    for (int i = 0; i < pos; i++) {
        if (categoria[0] == '\0' || strcmp(tarefas[i].categoria, categoria) == 0) {
            printTarefas(tarefas[i], i);
            encontradas++;
        }
    }

    if (encontradas == 0)
        printf("Nenhuma tarefa encontrada com a categoria especificada.\n");

    return OK;
}


ERROS salvar(Tarefa tarefas[], int *pos) {
  FILE *f = fopen("tarefas.bin", "wb");
  if (f == NULL)
    return ABRIR;

  int qtd = fwrite(tarefas, *pos, sizeof(Tarefa), f);
  if (qtd != *pos)
    return ESCREVER;

  qtd = fwrite(pos, sizeof(int), 1, f);
  if (qtd != 1)
    return ESCREVER;

  if (fclose(f) != 0)
    return FECHAR;

  return OK;
}

ERROS carregar(Tarefa tarefas[], int *pos) {
  FILE *f = fopen("tarefas.bin", "rb");
  if (f == NULL)
    return ABRIR;

  int qtd = fread(tarefas, *pos, sizeof(Tarefa), f); // Corrigido para ler a quantidade correta de tarefas
  if (qtd == 0)
    return LER;

  qtd = fread(pos, sizeof(int), 1, f); // Corrigido para ler a posição correta
  if (qtd != 1)
    return LER;

  if (fclose(f) != 0)
    return FECHAR;

  return OK;
}

void printTarefas(Tarefa t, int pos) {
  printf("Posição da tarefa: %d\n", pos + 1); // Corrigido para imprimir a posição correta
  printf("Prioridade: %d\n", t.prioridade);
  printf("Categoria: %s\n", t.categoria);
  printf("Descrição: %s\n", t.descricao);
}

void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    continue;
  }
}


// para rodar o codigo basta usar o comando gcc *.c && ./a.out
