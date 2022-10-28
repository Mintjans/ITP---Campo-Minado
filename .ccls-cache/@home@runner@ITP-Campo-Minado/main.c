#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*bomba: Se a celula é bomba ou não; 1 caso seja e 0 caso não seja;
  abertas: Se a celula já foi revelada ou não pelo jogador; 1 caso esteja aberta e 0  caso esteja fechada;
  vizinhos: Conta quantos dos vizinhos são bombas, vai de 0 a 8.*/
typedef struct{
    int bomba;
    int abertas;
    int vizinhos;
}Celula;

Celula jogo[10][20];
int l, c, taml = 10, tamc = 20;

void comecarJogo(){
  /*Inicializa todas posições da matriz com o valor zerado*/
    for(l=0; l<taml; l++){
        for(c=0; c<tamc; c++){
            jogo[l][c].bomba = 0;
            jogo[l][c].abertas = 0;
            jogo[l][c].vizinhos = 0;
        }
    }
}

void sorteio(int n){
  /*Define as posições das bombas de forma aleatória*/
  int i;
  srand(time(NULL));
  for(i=1; i <= n; i++){
    l = rand() % taml;
    c = rand() % tamc;
    if(jogo[l][c].bomba == 0){
      jogo[l][c].bomba = 1;
    }else{
      i--;
    }
  }
}

int contagem(int l, int c){
  /*Conta a quantidade de minas ao redor da casa*/
  int quant = 0, i, j;
    for (i = l-1; i <= l+1; i++){
      for (j = c-1; j <= c+1; j++){
        if (l >= 0 && l < taml && c >= 0 && c < tamc && jogo[i][j].bomba){
          quant++;
        }
      }
    }
  return quant;
}

void minas(){
  /*Percorre toda a matriz para contar todas as casas que não são bombas*/
  for(l=0; l<taml; l++){
    for(c=0; c<tamc; c++){
      jogo[l][c].vizinhos = contagem(l, c);
    }
  }
}

void impressao(){
  // imprime o campo minado
    printf("\n\n\t   ");
    for(c=0; c<taml; c++){
        printf("  %d ", c); // imprime primeira metade dos indices de colunas
    }
    for(c = taml; c<tamc; c++){ 
        printf(" %d ", c); // imprime segunda metade dos indices de colunas
    }
    printf("\n\t   ---------------------------------------------------------------------------------\n");
    for(l=0; l<taml; l++){
        printf("\t%d  |", l); // imprime indices de linhas
        for(c=0; c<tamc; c++){
            if(jogo[l][c].abertas){
                if(jogo[l][c].bomba){
                    printf(" * "); // * para casas que forem abertas e possuirem bombas
                }else{
                    printf(" %d ", jogo[l][c].vizinhos); // inteiro para quantidade de bombas ao redor
                }
            }else{
                printf("   "); // vazio para casas que ainda não foram abertas
                }
        printf("|");
        }

    printf("\n\t   ---------------------------------------------------------------------------------\n");
    }
}

void abertura(int a, int b){
    if (a >= 0 && a < taml && b >= 0 && b < tamc && jogo[a][b].abertas == 0){ // confere se a casa é válida e se está fechada
            jogo[a][b].abertas = 1; // transforma ela em uma casa aberta
            if(jogo[a][b].vizinhos == 0){ // caso não possua bombas nas casas adjacentes abre todas elas recursivamente
                    for(l=a-1; l<=a+1; l++){
                        for(c=b-1; c<=b+1; c++){
                            abertura(l, c);
                        }
                    }
            }
    }
}

int vitoria(){
    int contador = 0;
    for(l=0; l<taml; l++){
        for(c=0; c<tamc; c++){
            if(jogo[l][c].abertas == 0 && jogo[l][c].bomba == 0){
                contador++; // conta quantas casas que não são bombas ainda não foram abertas
            }
        }
    }
    return contador;
}

void jogar(){
    int lin, col;
    do{
        impressao();
        printf("\nDigite as coordenadas de linha e coluna a serem abertas: ");
        scanf("%d%d", &lin, &col);
        abertura(lin, col);
    }while(vitoria() != 0 && jogo[lin][col].bomba == 0);

    if(jogo[lin][col].bomba == 1){
        impressao();
        printf("\t\nDERROTA!\n");
    }else{
        printf("\t\nVITÓRIA!\n");
    }

}

int main (){
   comecarJogo();
   sorteio(40);
   minas();
   jogar();

return 0;
}