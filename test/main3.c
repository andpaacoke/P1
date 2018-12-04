/* P1 Projekt Gruppe B2-4 */
/* Kompiler indstillinger:
gcc -Wall -pedantic -ansi -c main3.c open_practice_file.c prntfunc.c user_prompt.c invertering_af_matrix3.c finddata.c && gcc *.o -o main.exe -lm && rm *.o
*/

#include <stdio.h>                                                              /* Inkluderer biblioteker */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "main.h"

                                                                                /* Definer funktioner for windows */
#ifdef __unix__
#include <unistd.h>
#define CLSCREEN "clear && clear"
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#define CLSCREEN "cls"
#endif


#define PLACEHOLDER_VAL 9000                                                    /* Definere placeholderere til preprocessoren */
#define Hours_A_YEAR 9000
#define FULL 100
#define USAGE 0.8                                                               /* Random value */
#define NUM_DAYS 24
#define MAX_MATRIX_SIZE 24
#define QUANTI 1
#define MANUAL 2
#define CLOSE_PROG 0
#define DAYS 1
#define WEEKS 2
#define TOO_FAR_BACK -1
#define FALSE 0
#define TRUE 1
#define MAX_DATA_AMOUNT 100

void battery_choice(int num_of_data, el_price_day practice_data[]);
int math_choice(int num_of_data, el_price_day practice_data[]);
void calc_prediction(double *amount_of_predict, double *perc_of_predict, double prediction, double day_to_predict);
void prnt_prediction(int mode_choice, int count_exe, double prediction, double day_to_predict, double amount_of_predict, double perc_of_predict);



int main(void){
  int prog_choice,
      num_of_data = PLACEHOLDER_VAL;
  el_price_day practice_data[PLACEHOLDER_VAL];

  srand(time(NULL));                                                       /* Starter random funktionen*/
  num_of_data = open_practice_file(practice_data);                         /* Åbner datafil og importere data til struct */
  if (num_of_data == 0){
    printf("Error loading data. Program will shutdown now!\n" );
    return -1;
  }

  do {      /* Makes you able to run the program multiple times */
    do {       /* Makes sure the user chooses a valid option */
      printf("\nWhat program mode do you want to use?\n\n"
             "(1) For battery\n"
             "(2) For electricity price prediction\n"
             "(0) To exit the program\n\n" );
      scanf(" %d", &prog_choice);
      if(prog_choice < 0 || prog_choice >2){
        system(CLSCREEN);
        printf("That is not a valid value, please choose a valid option.\n\n" );
      }
    } while(prog_choice < 0 || prog_choice > 2);

    switch (prog_choice) {
      case 0: break;
      case 1: printf("\nYou chose battery\n\n" );
              battery_choice(num_of_data, practice_data);
              break;
      case 2: printf("\nYou chose electricity price prediction\n\n" );
              math_choice(num_of_data, practice_data);
              break;
    }
  } while(prog_choice != 0);
  return 0;
}


void battery_choice(int num_of_data, el_price_day practice_data[]){
  double battery_capacity = 0,                                                  /* Definere lokale variable i main */
         battery_charge_level = 0,
         charger_effect = 0,
         max_discharge = 0,
         battery_value = 120,                                                   /* FIX: Battery value */
         profit = 0;
  int auto_discharge = 0,
      i = 0;
  char charge[1] = "+";


  prompt_user(&battery_capacity, &battery_charge_level,                         /* Prompter brugeren for input -> Til batteryladningsdelene af programmet*/
              &charger_effect, &max_discharge, &auto_discharge);

  for(i = 0; i < num_of_data ; i++){
    if (practice_data[i].time_of_day.price_dk1 < battery_value){
      if (battery_charge_level < FULL){                                         /* Kontrollere om batteriet er ladet mindre end 100% */
        charge[0] = '+';                                                        /* Ændre ladningsretningen til positiv */
        battery_charge_level += charger_effect;                                 /* Ændre opladningsgraden med opladerefekten  */
        profit = practice_data[i].time_of_day.price_dk1 / battery_charge_level;
        battery_value += profit;
        /*printf("\n%f",battery_value);*/
      }
      if (battery_charge_level > FULL){                                         /* Hvis opladningsgraden oversiger 100% sættes den til præcis 100% */
        battery_charge_level = FULL;
        charge[0] = ' ';                                                        /* Fjerne ladningsretningen da batteriet er fuldt opladet */
      }
    }
  else{                                                                         /* Tilføjer et forbrug til batteiet for at simulere et hjem der bliver brugt */
    charge[0] = '-';                                                            /* Ændre ladningsretningen til negativ */
    battery_charge_level -= USAGE;                                              /* Tapper batteriet for el */
  }
  prntfunc(practice_data, i, PLACEHOLDER_VAL, &battery_capacity,                /* Kalder printfunktionen der sørger for udskriften på std. output */
          &battery_charge_level, &charger_effect, &max_discharge,
          auto_discharge, charge);
  sleep(2.5);                                                                   /* Sænker hastigheden af programmet */
  }
}


int math_choice(int num_of_data, el_price_day practice_data[]){
  int mode_choice,
      day_week_choice,
      order = 0,
      repetitions,
      finddata_quanti_done = FALSE,
      res_reg_data,     /* Hvor langt der bliver set tilbage i dataen */
      quanti_question_asked = FALSE,
      question_asked = FALSE,
      count_exe = 0; /* Counts number of times the process has been executed*/
  double day_to_predict = 0,    /* elprisen for den day der skal forudsiges*/
         amount_of_predict,     /* Hvor meget vi ramte ved siden af */
         perc_of_predict,       /* Hvor mange procent vi ramte veed siden af*/
         sum_perc_of_predict = 0,
         sum_amount_of_predict = 0,
         sum_day_to_predict = 0,
         prediction,
         y_vector[MAX_DATA_AMOUNT];

  do {                                              /* Løkken kører 1 gang hvis man vælger quanti, eller så længe man vil hvis man vælger manuel */
    if (quanti_question_asked == FALSE) {           /*Gør at den kun kører en gang hvis man vælger kvanti*/
      do {
        printf("Press:\n"
               "(0) To return to menu.\n"
               "(1) For quantitive data.\n"      /* Prompts user to choose prog mode*/
               "(2) For manual input.\n");
        scanf("%d", &mode_choice);
        if (mode_choice < 0 || mode_choice > 2) {
          printf("\nThat is not a valid choice, try again.\n\n" );
        }
      } while(mode_choice < 0 || mode_choice > 2);

    }

    if (mode_choice == CLOSE_PROG) {                     /*closes program---------- måske ændre return så main ved at den skal lukke*/
      return 0;
    }

    if (mode_choice == QUANTI && quanti_question_asked == FALSE) {
      printf("How many repetitions?\n");
      scanf(" %d", &repetitions);
    }
    do {
      if (quanti_question_asked == FALSE) {               /* Do-while der sikre at man indtaster noget gyldigt */
        printf("\n"
               "Press 1 for days in sequence. \n"
               "Press 2 for weekdays in sequence.\n");
        scanf("%d", &day_week_choice);
      }
      if (day_week_choice > 2 || day_week_choice < 1) {
        printf("\nThat is not a valid choice, try again.\n\n" );
      }
    } while(day_week_choice > 2 || day_week_choice < 1);

    if (mode_choice == QUANTI) {
      quanti_question_asked = TRUE;
    }

    do {                                              /* Hvis der ikke kan ses langt nok tilbage, køres funtionen igen */
      res_reg_data = regression_data(&finddata_quanti_done, day_week_choice, mode_choice,
                                     &day_to_predict, y_vector, practice_data, num_of_data);
    } while(res_reg_data == TOO_FAR_BACK);
    prediction = regression(&question_asked, mode_choice, day_week_choice, y_vector, res_reg_data, &order);

    calc_prediction(&amount_of_predict, &perc_of_predict,
                    prediction, day_to_predict);
    prnt_prediction(mode_choice, count_exe, prediction,
                    day_to_predict, amount_of_predict, perc_of_predict);

    sum_amount_of_predict += amount_of_predict;
    sum_day_to_predict += day_to_predict;
    sum_perc_of_predict += perc_of_predict;
    count_exe++;
  } while(count_exe < repetitions && 1); /* Når du kører manual mode skal løkken også køre.. HOTFIX! */

  sum_perc_of_predict = sum_perc_of_predict / repetitions;
  sum_amount_of_predict = sum_amount_of_predict / repetitions;
  printf("Average amount diversed from the real value:     %4.3f\n"                /* Printer den gennemsnitlige afvigelse efter alle test er done*/
         "Average percentace diversed from the real value: %4.3f%%\n", sum_amount_of_predict, sum_perc_of_predict);

  return 0;  /* Vær obs på hvad der returneres */
}

void calc_prediction(double* amount_of_predict, double* perc_of_predict, double prediction, double day_to_predict){
  *amount_of_predict = prediction - day_to_predict;
  if (*amount_of_predict < 0) {
    *amount_of_predict = *amount_of_predict * (-1);
  }
  *perc_of_predict = (prediction - day_to_predict) / day_to_predict * 100;
  if (*perc_of_predict < 0) {
    *perc_of_predict = *perc_of_predict * (-1);
  }
}

  void prnt_prediction(int mode_choice, int count_exe, double prediction, double day_to_predict, double amount_of_predict, double perc_of_predict){
  printf("\nPrediction:    %15.3f\nReal value:    %15.3f\nAmount off:    %15.3f\nPercentage of: %15.3f%%\n\n",
          prediction, day_to_predict, amount_of_predict, perc_of_predict);
  if(mode_choice == QUANTI){
    printf("Test nr: %5d\n", count_exe + 1 );
  }
  printf("\n------------------------------------------------------------------\n\n");
}
