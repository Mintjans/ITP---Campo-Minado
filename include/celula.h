/*A definição de Celula tá aqui pq vamos precisar dela tbm no modo autonomo*/
/*bomba: Se a celula é bomba ou não; 1 caso seja e 0 caso não seja;
  abertas: Se a celula já foi revelada ou não pelo jogador; 1 caso esteja aberta e 0  caso esteja fechada;
  vizinhos: Conta quantos dos vizinhos são bombas, vai de 0 a 8.
  marca: Se a celula está marcada como bomba ou não; 1 caso esteja marcada e 0 caso não;*/
typedef struct{
    int bomba;
    int abertas;
    int vizinhos;
    int marca;
}Celula;