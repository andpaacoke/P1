#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/*typedef struct el_price_day el_price_day;*/

int open_practice_file(el_price_day el_price_day[]) {
  int i = 0;
  FILE *practice_data_file;
  practice_data_file = fopen("practice_data2.txt","r");                          /* placeholder filename */

  if (practice_data_file == NULL) {
    printf("Error opening file!\n");
    return (0);
  }

  while (fscanf(practice_data_file," %d-%d-%d %d - %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
          &el_price_day[i].day, &el_price_day[i].month, &el_price_day[i].year,
          &el_price_day[i].hour1, &el_price_day[i].hour2,
          &el_price_day[i].time_of_day.price_sys, &el_price_day[i].time_of_day.price_se1,
          &el_price_day[i].time_of_day.price_se2, &el_price_day[i].time_of_day.price_se3,
          &el_price_day[i].time_of_day.price_se4, &el_price_day[i].time_of_day.price_fi,
          &el_price_day[i].time_of_day.price_dk1, &el_price_day[i].time_of_day.price_dk2,
          &el_price_day[i].time_of_day.price_oslo, &el_price_day[i].time_of_day.price_kr_sand,
          &el_price_day[i].time_of_day.price_bergen, &el_price_day[i].time_of_day.price_molde,
          &el_price_day[i].time_of_day.price_tr_heim, &el_price_day[i].time_of_day.price_tromsoe,
          &el_price_day[i].time_of_day.price_ee, &el_price_day[i].time_of_day.price_lv,
          &el_price_day[i].time_of_day.price_lt) == 22){
    i++;
  }
  fclose(practice_data_file);
  return i - 1;
}
