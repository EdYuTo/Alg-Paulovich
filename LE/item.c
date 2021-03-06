#include "item.h"
#include <stdlib.h>
#include <stdio.h>

ITEM *criar_item(int chave, int valor) {
   ITEM *item = (ITEM *)malloc(sizeof(ITEM));

   if (item != NULL){
      item->chave = chave;
      item->valor = valor;
   }
   return item;
}

void apagar_item(ITEM *item) {
   if (item != NULL)
      free(item);
}

void imprimir_item(ITEM *item) {
   if (item != NULL)
	printf("item[%d] = %d\n", item->chave, item->valor);
}
