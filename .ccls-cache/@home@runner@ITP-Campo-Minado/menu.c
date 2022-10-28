1#include "Funcs.h"
#include "jogar.h"
 
int main(){
  int modo_de_jogo;
  while(1==1){
    printa_txt("logo.txt");
    printf("\n");
    printf("\n");
    printa_txt("menu.txt");
  
    scanf("%d", &modo_de_jogo);
  
    if(modo_de_jogo == 1){
      mogo_jogo();
    }else if (modo_de_jogo == 2){
      modo_autonomo();
    }else if (modo_de_jogo == 3){
      return 0;
    }else{
      printf("\n Escolha um modo valido! \n");
    }
  }
}