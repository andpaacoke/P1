#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "main.h"

#define DAY 1
#define WEEKS 2
#define QUANTI 1
#define MANUAL 2
#define DAY_IN_HOURS 24 /* Fordi vi ser dage tilbage er det 24 timer*/
#define WEEK_IN_HOURS 24*7 /*Fordi vi ser uger tilbage er det 24*7 timer*/
#define DAYS 1
#define WEEKS 2
#define FALSE 0
#define TRUE 1
#define ERROR_IN_DATA -1


int regression_data(int *quanti_done, int day_week_choice, int mode_choice, double *day_to_predict, double y_vector[], el_price_day el_price_day[], int num_of_data){
  int how_long_back_return = 0, /*returnere hvor mange dage der bliver set tilbage, lige gyldigt om der bliver valgt uger eller dage*/
      time_of_day,
      day, month, year,
      hours_back_in_time,             /* Hvor mange timer der bliver set tilbage (da dataen er i timer) så hvis det er uger er det 24*7 timer tilbage, hvis det er dage er det 24*/
      how_long_back_factor,
      how_long_back_sub,
      res_find_date;
  static int how_long_back;          /* Beskriver hvor langt tilbage man vælger at se*/ /*fx. jeg vælger at se 3 datoer (uger/dage) tilbage så vil denne være 3 og y_vectoren vil have størrelsen 3*/

  set_day_week_variables(day_week_choice, &how_long_back_factor, &how_long_back_sub, &hours_back_in_time);

  day_week_prompt(mode_choice, &day, &month, &year, &time_of_day, &how_long_back, quanti_done);

  if (mode_choice == QUANTI) {  /* Finder længden af måneden */
    day = find_days_of_month(&month, &time_of_day, &year);
  }

  res_find_date = find_date(num_of_data, el_price_day, day, month, year, time_of_day, day_to_predict);   /*returnere -1 hvis datoen ikke findes i dataen*/

  if (res_find_date == ERROR_IN_DATA) {
    return ERROR_IN_DATA;
  }

  res_find_date = find_prev_dates(res_find_date, how_long_back, mode_choice, hours_back_in_time, y_vector, el_price_day);   /*genbruger en variabel der ikke skal bruges*/

  if (res_find_date == ERROR_IN_DATA) {
    return ERROR_IN_DATA;
  }

  printf("Prediction date: Time = %d Day = %d Month = %d\n", time_of_day, day, month);

  /* udregner hvor mange dage der ses tilbage*/
  how_long_back_return = how_long_back * how_long_back_factor - how_long_back_sub;
  /*ex.: ser 4 (how_long_back) uger tilbage, ganger det med 7 (how_long_back_factor) fordi det er uger, minuser med 7 (how_long_back_sub) fordi pas?? */

  /*******************************************************/
  /* skal kun bruge til bugfixing
  i--;
  */
                                              /* Printer y-vektoren *//*
  for ( i = 0; i < how_long_back; i++) {
    printf("y_%d = %f\n",i, y_vector[i]);
  }*/
  /* Udregner hvor langt tilbage der kigges*/
  /*********************************************************/

  return how_long_back_return;
}

/*hvis man vælger UGER skal den mængde DAGE der ses tilbage hver lig med den mængde DAGE der er mellem startstidspunktet og sluttidspunktet*/
void set_day_week_variables(int day_week_choice, int *how_long_back_factor, int *how_long_back_sub, int *hours_back_in_time){
  if (day_week_choice == DAYS) {
    *how_long_back_factor = 1;
    *how_long_back_sub = 0;
    *hours_back_in_time = DAY_IN_HOURS;
  }
  else if (day_week_choice == WEEKS) {
    *how_long_back_factor = 7;
    *how_long_back_sub = 7;
    *hours_back_in_time = WEEK_IN_HOURS;
  }
}

void day_week_prompt(int mode_choice, int *day, int *month, int *year, int *time_of_day, int *how_long_back, int *quanti_done){
  if (mode_choice == MANUAL) {
    printf("What date would you like to look at? (dd mm yyyy)\n" );
    scanf(" %d %d %d", &*day, &*month, &*year);

    printf("What time of day? (0 - 23)\n" );
    scanf(" %d", &*time_of_day);

    printf("How far do you want to look back? (weeks/days in numbers)\n");
    scanf(" %d", &*how_long_back);
  }
  else if (mode_choice == QUANTI && *quanti_done == FALSE) {
    printf("How far do you want to look back? (weeks/days as integer)\n");
    scanf(" %d", &*how_long_back);
    *quanti_done = TRUE;                                                         /* Stop med at spørge brugeren */
  }

}

int find_days_of_month(int *month, int *time_of_day, int* year){
  int number_of_days;

  *time_of_day = rand() % 24;
  *year = 2016;
  *month = (rand() % 11) + 1;
  switch (*month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      number_of_days = 31;
      break;
    case 4: case 6: case 9: case 11:
      number_of_days = 30;
      break;
    case 2:
      number_of_days = 29;
      break;
  }
  return (rand() % number_of_days) + 1;

}

int find_date(int num_of_data,el_price_day el_price_day[], int day, int month, int year, int time_of_day, double *day_to_predict){
  int found_date = FALSE,
      i = 0;        /*i er tallet på den data der findes*/ /* Finder den dato man beder programmet om at finde*/

  while ( i < num_of_data && found_date == FALSE) {        /*kører til at datoen er fundet eller alle dataen er kigget igennem*/
    if ((year == el_price_day[i].year) && (month == el_price_day[i].month) && (day == el_price_day[i].day) && (time_of_day == el_price_day[i].hour1)) { /*Hvis datoen er fundet*/
      found_date = TRUE;
      *day_to_predict = el_price_day[i].time_of_day.price_dk1;      /* Den el-pris vi prøver at forudsige */
    }
    i++;
  }
  if (i == num_of_data) {                                   /* Hvis datoen ikke er fundet i dataen lukkes funktionen ned og starter op igen*/
    printf("---Error, date not found in data ---\n" );
    return -1;
  }
  return i;
}

int find_prev_dates(int res_find_date, int how_long_back, int mode_choice, int hours_back_in_time, double y_vector[],el_price_day el_price_day[]){
  int j = 0,
      i = res_find_date,
      days_back= 0;
  /* finder datoerne til y_vectoren*/  /* relevant at lægge mærke til at den starter med den sidste indgang i vektoren*/
  for ( j = how_long_back - 1; j >= 0; j--) {                       /*Da vi lige over dette har placeret noget på den sidste indgang i vectoren bliver vi nødt til at -2 på how_long_back*/
    i -= hours_back_in_time;                                         /*Da i er vores startounkt går vi den mængde data tilbage og finder de dataer i fortiden vi skal bruge*/
    if (i < 0) {
      if (mode_choice == MANUAL) {                      /* Hvis der ikke kan ses langt nok tilbage, til manuel indtastning */
        printf("---Error! Looking back too far!---\n"
               "---Can only look back %d times!---\n\n", days_back);
    }
      return ERROR_IN_DATA;
    }
    days_back++;                                                    /* Bruges kun hvis man ikke kan se den valgte mængde datoer tilbage*/
    y_vector[j] = el_price_day[i].time_of_day.price_dk1;
  }
  return 0;
}
