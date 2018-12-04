#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void prntfunc(el_price_day el_price_day[], int i, int end_of_for_loop, double *battery_capacity, double *battery_charge_level, double *charger_effect, double *max_discharge, int auto_discharge, char *charge){

  int battery = *battery_charge_level,
      count = battery / 5; /* Hver grafisk streg er 5% */
  int count2 = 20-count; /* Placering af batteriets dut (plus-ende) */
  int j;
  double avgprice = ((el_price_day[i].time_of_day.price_dk1 + el_price_day[i-1].time_of_day.price_dk1 + el_price_day[i-2].time_of_day.price_dk1)/3);

  system("clear && clear");

  printf("\n\n                        B2-4 P1 Fra eksistende software til modeller \n");

  printf("         _______________________________________________________________________\n");
  printf("        |            |         |        |        |        |         |           |\n");
  printf("        |    Date    |   Hour  |   Sys  |   DK1  |   DK2  |  Charge |  Battery  |\n");
  printf("        |____________|_________|________|________|________|_________|___________|\n");
  printf("        |            |         |        |        |        |         |           |\n");
  printf("        | %2.2d-%2.2d-%2.2d | %2.2d - %2.2d | %6.2f | %6.2f | %6.2f |    %c    |  %6.2f   |\n",
          el_price_day[i].day, el_price_day[i].month,
          el_price_day[i].year, el_price_day[i].hour1, el_price_day[i].hour2,
          el_price_day[i].time_of_day.price_sys,
          el_price_day[i].time_of_day.price_dk1, el_price_day[i].time_of_day.price_dk2, *charge, *battery_charge_level);
  printf("        |____________|_________|________|________|________|_________|___________|\n");


/* Forsøg med at tjekke om den skal købe eller sælge */
  printf("\nPrevious 5 prices: %2.2f %2.2f %2.2f %2.2f %2.2f\n",
    el_price_day[i].time_of_day.price_dk1, el_price_day[i-1].time_of_day.price_dk1,
    el_price_day[i-2].time_of_day.price_dk1, el_price_day[i-3].time_of_day.price_dk1,
    el_price_day[i-4].time_of_day.price_dk1);

  printf("Average of previous 5 prices: %2.2f\n", avgprice);
  printf("Current price: %2.2f\n", el_price_day[i].time_of_day.price_dk1);
  printf("Charge / De-charge? ");
  (avgprice > el_price_day[i].time_of_day.price_dk1) ? printf("CHARGE!!!\n") : printf("DE-CHARGE!!!\n");


  printf("\n                                       Battery");
  printf("\n                                 ____________________ \n");
  printf("                                |      %6.2f%%       |_\n", *battery_charge_level);



  for ( i = 0; i < 3 ; i++) {
    printf("                                |");
    for (j = 0; j < count; j++) {
      printf("|");
    }
    for(j = 0 ; j < count2 ; j++){
      printf(" ");
    }
    if(i == 0)
    printf("| |\n" );
    else if(i == 1)
    printf("| |\n");
    else
    printf("|_|\n");
  }
  printf("                                |____________________|\n");

}
