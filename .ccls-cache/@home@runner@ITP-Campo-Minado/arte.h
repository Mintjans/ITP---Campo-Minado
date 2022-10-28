#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 128

void print_image(FILE *file){
    char read_string[MAX_LEN];
 
    while(fgets(read_string,sizeof(read_string),file) != NULL)
        printf("%s",read_string);
}

void printa_txt(char* file_name){
  FILE *file = NULL;
  file = fopen(file_name,"r");
 
  if(file == NULL){
      fprintf(stderr,"erro ao abrir %s\n", file_name);
      return 1;
  }
 
  print_image(file);
  fclose(file);
}
 