#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/*codigo pra printar arte*/
#define MAX_LEN 128

void print_image(FILE *file) {
  char read_string[MAX_LEN];

  while (fgets(read_string, sizeof(read_string), file) != NULL)
    printf("%s", read_string);
}

void printa_txt(char *file_name) {
  FILE *file = NULL;
  file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "erro ao abrir %s\n", file_name);
  }

  print_image(file);
  fclose(file);
}

#include "celula.h"
Celula jogo[10][20];
int l, c, taml = 10, tamc = 20;

void comecarJogo() {
  /*Inicializa todas posições da matriz com o valor zerado*/
  for (l = 0; l < taml; l++) {
    for (c = 0; c < tamc; c++) {
      jogo[l][c].bomba = 0;
      jogo[l][c].abertas = 0;
      jogo[l][c].vizinhos = 0;
      jogo[l][c].marca = 0;
    }
  }
}

void sorteio(int n) {
  /*Define as posições das bombas de forma aleatória*/
  int i;
  srand(time(NULL));
  for (i = 1; i <= n; i++) {
    l = rand() % taml;
    c = rand() % tamc;
    if (jogo[l][c].bomba == 0) {
      jogo[l][c].bomba = 1;
    } else {
      i--;
    }
  }
}

int contagem(int l, int c) {
  /*Conta a quantidade de minas ao redor da casa*/
  int quant = 0, i, j;
  for (i = l - 1; i <= l + 1; i++) {
    for (j = c - 1; j <= c + 1; j++) {
      /*Verifica se a casa é válida e se é uma bomba*/
      if (i >= 0 && i < taml && j >= 0 && j < tamc && jogo[i][j].bomba) {
        quant++;
      }
    }
  }
  return quant;
}

void minas() {
  /*Percorre toda a matriz para contar todas as casas que não são bombas*/
  for (l = 0; l < taml; l++) {
    for (c = 0; c < tamc; c++) {
      jogo[l][c].vizinhos = contagem(l, c);
    }
  }
}

void impressao() {
  // imprime o campo minado
  printf("\n\n\t   ");
  for (c = 0; c < taml; c++) {
    printf("  %d ", c); // imprime primeira metade dos indices de colunas
  }
  for (c = taml; c < tamc; c++) {
    printf(" %d ", c); // imprime segunda metade dos indices de colunas
  }
  printf("\n\t   "
         "---------------------------------------------------------------------"
         "------------\n");
  for (l = 0; l < taml; l++) {
    printf("\t%d  |", l); // imprime indices de linhas
    for (c = 0; c < tamc; c++) {
      if (jogo[l][c].abertas) {
        if (jogo[l][c].bomba) {
          printf(" * "); // * para casas que forem abertas e possuirem bombas
        } else {
          printf(" %d ",
                 jogo[l][c]
                     .vizinhos); // inteiro para quantidade de bombas ao redor
        }
      } else if (jogo[l][c].marca) {
        printf(" x "); // x para casas que foram marcadas como bomba
      } else {
        printf("   "); // vazio para casas que ainda não foram abertas
      }
      printf("|");
    }

    printf("\n\t   "
           "-------------------------------------------------------------------"
           "--------------\n");
  }
}

void abertura(int a, int b) {
  if (a >= 0 && a < taml && b >= 0 && b < tamc &&
      jogo[a][b].abertas == 0) { // confere se a casa é válida e se está fechada
    jogo[a][b].abertas = 1;      // transforma ela em uma casa aberta
    if (jogo[a][b].vizinhos ==
        0) { // caso não possua bombas nas casas adjacentes, abre todas elas
             // recursivamente
      for (int i = a - 1; i <= a + 1; i++) {
        for (int j = b - 1; j <= b + 1; j++) {
          abertura(i, j);
        }
      }
    }
  }
}

int vitoria() {
  int contador = 0;
  for (l = 0; l < taml; l++) {
    for (c = 0; c < tamc; c++) {
      if (jogo[l][c].abertas == 0 && jogo[l][c].bomba == 0) {
        contador++; // conta quantas casas que não são bombas ainda não foram
                    // abertas
      }
    }
  }
  return contador;
}

void media(int linha, int coluna, float *med, int *q_numericas, int *q_fechadas) {
  /*med: media dos valores numericos adjacentes a casa fechada
  q_numericas: quantidade de casas numericas ao redor da media*/
  int soma = 0;
  *med = 0;
  *q_numericas = 0;
  *q_fechadas = 0;

  for (int l = linha - 1; l <= linha + 1; l++) {
    if (l >= 0 && l < 10) { // garante que a linha existe
      for (int c = coluna - 1; c <= coluna + 1; c++) {
        if (c >= 0 && c < 20) { // garante que a coluna existe
          if (jogo[l][c].abertas == 1 && jogo[l][c].vizinhos != 0) {
            *q_numericas =
                *q_numericas +
                1; // conta a quantidade de valores numericos diferentes de zero
            soma = soma + jogo[l][c].vizinhos; // soma dos valores numericos
          } else if (jogo[l][c].abertas == 0) {
            *q_fechadas = *q_fechadas + 1;
          }
        }
      }
    }
  }

  if (*q_numericas != 0) { // se existirem valores numericos a media é calculada
    *med = ((float)soma / (float)*q_numericas) / (float)*q_fechadas;
  }
  return;
}

void marcar() {

  for (int i = 0; i < taml; i++) {
    for (int j = 0; j < tamc; j++) {
    int count = 0;

      if (jogo[i][j].abertas == 1) {
        for (int k = i - 1; k <= i + 1; k++) {
          for (int m = j - 1; m <= j + 1; m++) {
            /*Verifica se a casa é válida e se está aberta*/
            if (k >= 0 && k < taml && m >= 0 && m < tamc &&
                jogo[k][m].abertas == 1) {
              count++;
            }
          }
        }
        if(i == 0 || i == 9){
          count += 3;
        }
        if(j == 19 || j == 0){
          count += 3;
        }
        if((i == 0 || i == 9) && (j == 19 || j == 0)){
          count -= 1;
        }
        if (count + jogo[i][j].vizinhos == 9) {
          for (int k = i - 1; k <= i + 1; k++) {
            for (int m = j - 1; m <= j + 1; m++) {
              /*Procura a casa fechada para marcar*/
              if (k >= 0 && k < taml && m >= 0 && m < tamc && jogo[k][m].abertas == 0 && jogo[k][m].marca == 0) {
                jogo[k][m].marca = 1;
              }
            }
          }
        }
      }
    }
  }
}

int procurar(){
int deubom = 1;
  
  for (int i = 0; i < taml; i++) {
    for (int j = 0; j < tamc; j++) {
    int count = 0;

      if (jogo[i][j].abertas == 1) {
        for (int k = i - 1; k <= i + 1; k++) {
          for (int m = j - 1; m <= j + 1; m++) {
            /*Verifica se a casa é válida e se está marcada*/
            if (k >= 0 && k < taml && m >= 0 && m < tamc && jogo[k][m].marca == 1) {
              count++;
            }
          }
        }
        if (count == jogo[i][j].vizinhos) {
          for (int k = i - 1; k <= i + 1; k++) {
            for (int m = j - 1; m <= j + 1; m++) {
              /*Procura as casas fechadas para abrir*/
              if (k >= 0 && k < taml && m >= 0 && m < tamc && jogo[k][m].abertas == 0 && jogo[k][m].marca == 0) {
                deubom = 0;
                abertura(k, m);
                printf("\nCoordenada escolhida pelo computador: %d %d", k, m);
                impressao();
                sleep(1);
              }
            }
          }
        }
      }
    }
  }
  return deubom;
}

void ajuda(int *l_aju, int *c_aju) {
  /*med_antiga: menor media dos valores numericos adjacentes de uma casa fechada
  med_nova: media dos valores numericos adjacentes da celula com a coordenada
  i,j qn_nova e qn_antiga: quantidade de casas numericas ao redor da casa
  fechada, serve como criterio de desempate qf_nova e qf_antiga: quantidade de
  casas fechadas ao redor da casa fechada sendo analisada, serve como segundo
  criterio de desempate l_aju e c_aju: linha e coluna retornado pela ajuda*/
  float med_antiga, med_nova;
  int qn_nova, qn_antiga;
  int qf_nova, qf_antiga;

  med_antiga = 100;
  med_nova = 0;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      if (jogo[i][j].abertas == 0) {
        media(i, j, &med_nova, &qn_nova, &qf_nova);

        if (med_nova < med_antiga && med_nova != 0) {
          med_antiga = med_nova;
          qn_antiga = qn_nova;
          qf_antiga = qf_nova;
          *l_aju = i;
          *c_aju = j;
        } else if (med_nova == med_antiga) {
          if (qn_nova < qn_antiga) {
            qn_antiga = qn_nova;
            qf_antiga = qf_nova;
            med_antiga = med_nova;
            *l_aju = i;
            *c_aju = j;
          } else if (qn_nova == qn_antiga && qf_nova > qf_antiga) {
            qn_antiga = qn_nova;
            qf_antiga = qf_nova;
            med_antiga = med_nova;
            *l_aju = i;
            *c_aju = j;
          }
        }
      }
    }
  }
  return;
}

void jogar() {
  int check, lin, col, aux, c_aju, l_aju, l_b, c_b;
  time_t begin = time(NULL);
  time_t end;
  double duracao;

  impressao();

  do {
  INICIO:
    printf("\nComandos:\n");
    printf("\n[1] — Abrir uma casa\n");
    printf("[2] — Ajuda\n");
    printf("[3] — Marcar uma casa como bomba\n");
    printf("[4] — Ver tempo de jogo\n");
    printf("[5] — Voltar ao menu principal\n");
    printf("\nDigite seu comando: ");
    scanf(" %d", &check);

    switch (check) {
    case 1:
      impressao();
      printf("\nDigite as coordenadas de linha e coluna a serem abertas: ");
      scanf("%d%d", &lin, &col);
      while (lin > 9 || lin < 0 || col > 19 || col < 0) {
        printf("\nCoordenada inválida!\n");
        printf("\nDigite as coordenadas de linha e coluna a serem abertas: ");
        scanf("%d%d", &lin, &col);
      }
      abertura(lin, col);
      impressao();
      break;

    case 2:
      ajuda(&l_aju, &c_aju);
      printf("\nO computador acha que você deve abrir a seguinte posição:\n");
      printf("Linha: %d\n", l_aju);
      printf("Coluna: %d\n", c_aju);
      break;

    case 3:
      impressao();
      printf("\nDigite as coordenadas de linha e coluna a serem marcadas: ");
      scanf("%d%d", &l_b, &c_b);
      while (l_b > 9 || l_b < 0 || c_b > 19 || c_b < 0) {
        printf("\nCoordenada inválida!\n");
        printf("\nDigite as coordenadas de linha e coluna a serem marcadas: ");
        scanf("%d%d", &l_b, &c_b);
      }
      jogo[l_b][c_b].marca = 1;
      impressao();
      goto INICIO;
      break;
      
    case 4:
      end = time(NULL);
        printf("\nDuração do jogo: %.2ld segundos. \n", (end - begin));
      break;

    case 5:
        return;
      break;
      
    default:
      printf("\nDigite um comando válido.\n");
      sleep(1);
      goto INICIO;
      break;
    }
  } while (vitoria() != 0 && jogo[lin][col].bomba == 0);

  if (jogo[lin][col].bomba == 1) {
    impressao();
    time_t end = time(NULL);
    printf("\nDuração do jogo: %.2ld segundos. \n", (end - begin));
    printa_txt("derrota.txt");
  } else {
    impressao();
    time_t end = time(NULL);
    duracao = (end - begin) / 60;
    printf("\nDuração do jogo: %.2ld segundos.\n", (end - begin));
    printa_txt("vitoria.txt");
  }
}

void autonomo() {
  int lin, col, aux;

  /*Define aleatoriamente uma posição inicial com 0 bombas próximas para começar*/
  do {
    impressao();

    do {
      lin = rand() % taml;
      col = rand() % tamc;
    } while (jogo[lin][col].abertas != 0);

    abertura(lin, col);
    printf("\nCoordenada escolhida pelo computador: %d %d", lin, col);
    sleep(1);
  } while (jogo[lin][col].vizinhos >= 1 && jogo[lin][col].bomba == 0);

  impressao();
  sleep(1);
  
  if (jogo[lin][col].bomba == 0) {
    do {
      marcar();
      if(procurar()==1){
        ajuda(&lin, &col);
        if(jogo[lin][col].marca == 1){
          do {
            lin = rand() % taml;
            col = rand() % tamc;
          } while (jogo[lin][col].abertas == 1 && jogo[lin][col].marca == 1);
        }
        printf("\nCoordenada escolhida pelo computador: %d %d", lin, col);
        abertura(lin, col);
        impressao();
      }
      sleep(1);
    } while (vitoria() != 0 && jogo[lin][col].bomba == 0);
  }

  if (jogo[lin][col].bomba == 1) {
    printa_txt("derrota.txt");
  } else {
    printa_txt("vitoria.txt");
  }
}