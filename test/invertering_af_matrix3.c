#include <stdio.h>
#include <math.h>
#include "main.h"

#define NUM_DAYS 24
#define MAX_MATRIX_SIZE 24
#define DAYS 1
#define WEEKS 2
#define FALSE 0
#define TRUE 1
#define QUANTI 1
#define MANUAL 2

double regression(int *question_asked, int mode_choice, int day_week_choice, double vector[NUM_DAYS], int res_reg_data, int *order){
  int i,
      number_of_days = 0;   /*Antal dage der ses tilbage*/
  double udreg_arr_1[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],
         udreg_trans[MAX_MATRIX_SIZE][NUM_DAYS],
         udreg_fac[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],
         udreg_invert[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
  double det,
         prediction = 0,
         result_vector[MAX_MATRIX_SIZE];


  static double udreg_total[MAX_MATRIX_SIZE][NUM_DAYS];

  if (mode_choice == MANUAL) {
    *question_asked = FALSE;
  }

  number_of_days = res_reg_data;
  if (*question_asked == FALSE) {
    printf("Amount of degrees?\n" );
    scanf(" %d", order);
    *order = *order + 1;
  }


  if (*question_asked == FALSE) {    /* Gør at man kun kører den conputerkrævende del igennem én gang */
    create_matrix(number_of_days, udreg_arr_1, udreg_trans, day_week_choice, order);
    det = determinant(udreg_arr_1, *order);
    cofactor(udreg_arr_1, udreg_fac, *order);
    transpose_and_invert(udreg_fac, udreg_invert, order, det);
    invert_point_trans(number_of_days, udreg_invert, udreg_trans, udreg_total, order);
    if (mode_choice == QUANTI) {
          *question_asked = TRUE;
    }

  }

  total_dot_vector(udreg_total, vector, result_vector, order);
/****************************************************************************************************
  for (j = 0; j < 4; j++) {
    printf("--%5.12f\n", result_vector[j]);
  }
  printf("vector\n");
  for (j = 0; j < 4; j++) {
    printf("--%5.12f\n", vector[j]);
  }
  ****************************************************************************************************/

  if (day_week_choice == WEEKS) {                   /* Gør at det er den korrekte mængde dage der bliver kigget tilbage*/
    number_of_days = (number_of_days + 7)/7;
  }
  for (i = 0; i < *order; i++) {
      prediction += result_vector[i] * pow(number_of_days, i);
  }
                                                                                  /* trouble shooting help
  for ( i = 0; i < order; i++) {
    printf("\n");
    for ( j = 0; j < 4; j++) {
      printf(" %f ", udreg_total[i][j]);
    }
  }
  for ( i = 0; i < order; i++) {
    printf("\n");
    for ( j = 0; j < 4; j++) {
      printf(" %f ", udreg_total[i][j]);
    }
  }*/
  return prediction;
}

void create_matrix(int number_of_days, double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_trans[][MAX_MATRIX_SIZE], int day_week_choice, int *order){
  int i, j, g, k = 0;

  for(j = 0; j < *order ; j++){
    for(g = 0; g < *order; g++){
      udreg_arr_1[j][g] = 0;
      if(day_week_choice == DAYS){
        for(i = 0; i < number_of_days; i++){
          udreg_arr_1[j][g] += pow(i,j) * pow(i,g);
          udreg_trans[j][i] = pow(i,j);
        }
      }
      else{
        for (i = 0; i < number_of_days; i += 7) {
          udreg_arr_1[j][g] += pow(i,j) * pow(i,g);
          udreg_trans[j][k] = pow(i,j);
          k++;
          if (i == 0) {
            i-= 1;
          }
      }
        k = 0;
      }
    }
  }
}

double determinant(double udreg_arr_1[][MAX_MATRIX_SIZE], int order){
  int y_val, pre_inc = 1;

  double det;
  double udreg_arr_1_copy[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
  if (order == 1){
    return udreg_arr_1[0][0];                                                   /* Når der er fundet en 2x2 matrix vil den rekursive funktion ende her */
  }
  else {
   det = 0;
   for ( y_val = 0; y_val < order; y_val++ ){
      find_smaller_matrix(0, y_val, order, udreg_arr_1, udreg_arr_1_copy);                                 /* Sender 0 med da det kun er nødvendigt at bruge en søjle når man finder determinanten for hele matrixen*/
      det = det + pre_inc * (udreg_arr_1[0][y_val] * determinant(udreg_arr_1_copy, order - 1));             /* Det er 0 af samme gund som før */ /* order ender med at være 1 og den vil så returnere  */
      pre_inc= (-1) * pre_inc;
      }
  }
  return det;
}


void cofactor(double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_fac[][MAX_MATRIX_SIZE], int order){
  int y_val, x_val,
      pre_inc = -1;
  double udreg_arr_1_copy[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

  for (x_val = 0; x_val < order; x_val++) {                                     /*Har x-værdie med da man skal finde determinanten for hver værdi i den oprindelige matrix*/
    for ( y_val = 0; y_val < order; y_val++ ){
     find_smaller_matrix(x_val, y_val, order, udreg_arr_1, udreg_arr_1_copy);
     if (((x_val + y_val) % 2) != 0){                                                            /*Ændre fortegn på bestemte punkter i matrixen så den kan inverteres*/
       udreg_fac[x_val][y_val] = pre_inc * determinant(udreg_arr_1_copy, order - 1);
     }
     else{
       udreg_fac[x_val][y_val] = determinant(udreg_arr_1_copy, order - 1);
     }
    }
  }
}

void find_smaller_matrix(int x_val, int y_val, int order, double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_arr_1_copy[][MAX_MATRIX_SIZE]){
  int i, j,
      m = 0,
      n = 0;

  for ( i = 0; i < order; i++ ){
     for ( j = 0; j < order; j++ ){
         udreg_arr_1_copy[i][j] = 0;                                            /* nulstiller pladsen i matrixen */
         if (i != x_val && j != y_val) {                                        /* skal være != 0 da det er den søjle vi arbejder i */
            udreg_arr_1_copy[m][n] = udreg_arr_1[i][j];                         /* overskriver nulstillingen  med det tal der skal være der */
            if (n < (order-2)){                                                 /* Soerge for at matrixen bliver 2x2 (og ikke mindre) så man kan finde determinanten */
              n++;
            } else {
              n = 0;
              m++;
            }
         }
      }
    }
}

/* transformere matrixen og dividere værdien med dens determinant og finder derved den iverse matrix*/
void transpose_and_invert(double udreg_fac[][MAX_MATRIX_SIZE], double udreg_invert[][MAX_MATRIX_SIZE], int *order, double det){
  int i, j;

  for (i = 0; i < *order; i++) {
    for (j = 0; j < *order; j++) {
      udreg_invert[i][j] = udreg_fac[j][i] / det;
    }
  }
}

/* takes the inverted matrix and the transformed original matrix and finds the dot product*/
void invert_point_trans(int number_of_days, double udreg_invert[][MAX_MATRIX_SIZE], double udreg_trans[][NUM_DAYS], double udreg_total[][NUM_DAYS], int *order){
  int i, j , m;

  for (i = 0; i < *order; i++) {
    for (j = 0; j < number_of_days; j++) {
      udreg_total[i][j] = 0;
      for (m = 0; m < *order; m++) {
          udreg_total[i][j] += udreg_invert[i][m] * udreg_trans[m][j];
      }
    }
  }
}

/*Now it takes the final matrix and the vector which is a data set and finds the dot product and the result vector*/
void total_dot_vector(double udreg_total[][NUM_DAYS], double vector[MAX_MATRIX_SIZE], double result_vector[NUM_DAYS], int *order){
  int i, j;

  for (i = 0; i < *order; i++) {
    result_vector[i] = 0;
    for (j = 0; j < *order; j++) {
      result_vector[i] += vector[j] * udreg_total[i][j];
    }
  }
}
