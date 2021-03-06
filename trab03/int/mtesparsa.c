//EDSON YUDI TOMA 9791305

#include "mtesparsa.h"
#include "item.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct NO {
   ITEM *item;//valores
   struct NO *right;//proximo para a direita
   struct NO *down;//proximo para baixo
} NO;

struct ESPARSA {
   NO *head;//no base da matriz
   NO *lastrow;//ultima linha modificada
   NO *lastcol;//ultima coluna modificada
   int rows;//numero de linhas
   int cols;//numero de colunas
};

NO *criarNO (int row, int col, int value) {
   NO *no = (NO *)malloc(sizeof(NO));

   if (no != NULL) {
      no->item = criar_item(row, col, value);
      no->right = no;
      no->down = no;
   }
   return no;
}

int apagarNO (NO *no) {
   if (no != NULL) {
      apagar_item(&(no->item));
      no->right = NULL;
      no->down = NULL;
      free(no);
      return 1;
   }
   return 0;
}

ESPARSA *criarMT (int rows, int cols) {
   ESPARSA *matrix = (ESPARSA*)malloc(sizeof(ESPARSA));

   if (matrix != NULL) {
      matrix->rows = rows;//linhas
      matrix->cols = cols;//colunas
      matrix->head = criarNO(-1, -1, 0);//cria um no cabeca com um item na pos (-1,-1)
      if (matrix->head != NULL) {
         NO *aux = matrix->head;
         for (int i = 0; i < cols; i++) {
            aux->right = criarNO(-1, i, 0);//declara as colunas
            aux = aux->right;
         }
         aux->right = matrix->head;
         aux = matrix->head;
         for (int i = 0; i < rows; i++) {
            aux->down = criarNO(i, -1, 0);//declara as linhas
            aux = aux->down;
         }
         aux->down = matrix->head;
         matrix->lastrow = matrix->head;//|
         matrix->lastcol = matrix->head;//|__auxiliares...
      }
   }
   return matrix;
}

int apagarMT (ESPARSA *matrix) {
   if (matrix != NULL) {
      NO *prem = matrix->head->down;
      NO *paux = matrix->head->down->down;
      NO *apagar;
      NO *proximo;

      while (prem != matrix->head) {//linha por linha
         apagar = prem->right;
         proximo = prem->right->right;
         while (apagar != prem) {//coluna por coluna
            apagarNO(apagar);//apaga-se o no
            apagar = proximo;
            proximo = proximo->right;
         }
         apagarNO(prem);//apaga-se o no q indica a linha(coluna -1)
         prem = paux;
         paux = paux->down;
      }
      prem = matrix->head->right;
      paux = matrix->head->right->right;
      while (prem != matrix->head) {//apagar a 1 linha
         apagarNO(prem);
         prem = paux;
         paux = paux->right;
      }
      apagarNO(matrix->head);
      free(matrix);//free final
      return 1;
   }
   return 0;
}

void printMT (ESPARSA *matrix) {
   if (matrix != NULL) {
      NO *rowprint = matrix->head->down;
      NO *rowaux = matrix->head->down->down;
      NO *colprint;
      NO *colaux;
      
      printf("-1 %d %d\n", matrix->rows, matrix->cols);

      while (rowprint != matrix->head) {
         colprint = rowprint->right;
         colaux = rowprint->right->right;
         while (colprint != rowprint) {
            imprimir_item(colprint->item);
            colprint = colaux;
            colaux = colaux->right;
         }
         rowprint = rowaux;
         rowaux = rowaux->down;
      }
   }
}

int insert(ESPARSA *matrix, int row, int col, int value) {//considera a insercao ja ordenada!
   if (value != 0 && matrix != NULL && row > -1 && col > -1 &&matrix->rows >= row && matrix->cols >= col) {
      NO *set = criarNO(row, col, value); 
      NO *auxROW;
      NO *auxCOL;

      if (row >= matrix->lastrow->item->row)            //|
         auxROW = matrix->lastrow;//ultima linha usada! //|
      else                                              //|
         auxROW = matrix->head->down;                   //|
                                                        //|
      if (col >= matrix->lastcol->item->col)            //|
         auxCOL = matrix->lastcol;//ultima coluna usada!//|
      else                                              //|
         auxCOL = matrix->head->right;                  //|___--->tentativa de agilizar...
 
      if (set != NULL) {
         while (auxROW->item->row < row)
            auxROW = auxROW->down;//desce nas linhas ate a linha desejada

         while (auxCOL->item->col < col)
            auxCOL = auxCOL->right;//continua para a direita ate a coluna desejada

         matrix->lastrow = auxROW;//|
         matrix->lastcol = auxCOL;//|__->salvar ultima linha e coluna para agilizar!

         NO *auxSET = auxROW;
         while (auxSET->right != auxROW)
            auxSET = auxSET->right;//na linha, vai para a direita ate o ultimo elemento!
         auxSET->right = set;
         set->right = auxROW;//apontar o elemento adicionado para o comeco da linha... 
          
         auxSET = auxCOL;
         while (auxSET->down != auxCOL)
            auxSET = auxSET->down;//na coluna, desce ate o ultimo elemento!
         auxSET->down = set;
         set->down = auxCOL;//apontar o elemento adicionado para o comeco da coluna...
         return 1;//deu certo!
      }
   }
   return 0;//deu errado!
}

ITEM *get(ESPARSA *matrix, int row, int col) {
   if (row > -1 && col > -1 && matrix != NULL && matrix->rows >= row && matrix->cols >= col) {
      if (matrix->rows >= matrix->cols) {//tentar ir por um caminho menor
         NO *auxCOL = matrix->head->right;
         NO *parada;
         while (auxCOL->item->col < col)//vai ate a coluna e para
            auxCOL = auxCOL->right;
         parada = auxCOL;
         
         auxCOL = auxCOL->down;
         while (auxCOL != parada && auxCOL->item->row < row)//vai ate a linha ou uma linha a mais
            auxCOL = auxCOL->down;

         if (auxCOL->item->row == row)//se for a linha desejada, retorna-se o item
            return auxCOL->item;
      }
      else {
         NO *auxROW = matrix->head->down;
         NO *parada;
         while (auxROW->item->row < row)//vai ate a linha e para
            auxROW = auxROW->down;
         parada = auxROW;

         auxROW = auxROW->right;
         while (auxROW != parada && auxROW->item->col < col)//vai ate a coluna ou uma coluna a mais
            auxROW = auxROW->right;

         if (auxROW->item->col == col)//se for a coluna desejada, retorna-se o item
            return auxROW->item;
      }
   }
   return NULL;//nao encontrou!
}

ESPARSA *add(ESPARSA *m1, ESPARSA *m2) {
   if (m1 != NULL && m2 != NULL && m1->rows == m2->rows && m1->cols == m2->cols) {
      NO *auxm1 = m1->head->down;//1 linha
      NO *auxm2 = m2->head->down;//1 linha
      ESPARSA *m3 = criarMT(m1->rows, m1->cols);//aloca a matriz
  
      if (m3 != NULL) {//segurança..
         while (auxm1 != m1->head) {//enquanto nao recomeçar...
            auxm1 = auxm1->right;//|
            auxm2 = auxm2->right;//|___--->sair da coluna -1 e ir para a coluna 0
            while (auxm1->item->col != -1 || auxm2->item->col != -1) {//se ambos forem -1 para--->estao nos nos iniciais!
               if (auxm1->item->col != -1 && (auxm1->item->col < auxm2->item->col)) {//se nao for no inicial e a col for menor, insere
                  insert(m3, auxm1->item->row, auxm1->item->col, auxm1->item->valor);
                  auxm1 = auxm1->right;//como nao e um no inicial, desloca-se
               }
               else if (auxm2->item->col != -1 && (auxm1->item->col > auxm2->item->col)) {//mesma coisa
                  insert(m3, auxm2->item->row, auxm2->item->col, auxm2->item->valor);
                  auxm2 = auxm2->right;
               }
               else if (auxm1->item->col == -1 && auxm2->item->col != -1) {//se um for -1, significa q esta linha ja acabou, insere o outro!
                  insert(m3, auxm2->item->row, auxm2->item->col, auxm2->item->valor);
                  auxm2 = auxm2->right;
               }
               else if (auxm1->item->col != -1 && auxm2->item->col == -1) {//mesmca coisa
                  insert(m3, auxm1->item->row, auxm1->item->col, auxm1->item->valor);
                  auxm1 = auxm1->right;
               }
               else if (auxm1->item->col == auxm2->item->col) {//se forem iguais, soma
                  int soma = auxm1->item->valor + auxm2->item->valor;
                  if (soma != 0)
                     insert(m3, auxm1->item->row, auxm1->item->col, soma);
                  auxm1 = auxm1->right;
                  auxm2 = auxm2->right;
               }
            }
            auxm1 = auxm1->down;//
            auxm2 = auxm2->down;//___--->movem-se
         }
      }
      return m3;
   }
   return NULL;
}

ESPARSA *mult(ESPARSA *m1, ESPARSA *m2) {
   if (m1 != NULL && m2 != NULL && m1->cols == m2->rows) {
      NO *auxm1 = m1->head->down;
      NO *auxm2 = m2->head;
      ESPARSA *m3 = criarMT(m1->rows, m2->cols);
      int result = 0;//acumular resultados!
  
      if (m3 != NULL) {//segurança!
         while (auxm1 != m1->head) {
            auxm2 = auxm2->right;
            while (auxm2 != m2->head) {
               auxm1 = auxm1->right;
               auxm2 = auxm2->down;
               while (auxm1->item->col != -1 || auxm2->item->row != -1) {//mesmo while da adicao sem as insercoes!
                  if (auxm1->item->col != -1 && (auxm1->item->col < auxm2->item->row))
                     auxm1 = auxm1->right;
                  else if (auxm2->item->row != -1 && (auxm1->item->col > auxm2->item->row))
                     auxm2 = auxm2->down;
                  else if (auxm1->item->col == -1 && auxm2->item->row != -1)
                     auxm2 = auxm2->down;
                  else if (auxm1->item->col != -1 && auxm2->item->row == -1)
                     auxm1 = auxm1->right;
                  else if (auxm1->item->col == auxm2->item->row) {
                     result += auxm1->item->valor * auxm2->item->valor;
                     auxm1 = auxm1->right;
                     auxm2 = auxm2->down;
                  }
               }
               if (result != 0) {
                  insert(m3, auxm1->item->row, auxm2->item->col, result);
                  result = 0;
               }
               auxm2 = auxm2->right;
            }
            auxm1 = auxm1->down;
         }
      }
      return m3;
   }
   return NULL;
}

