#include <stdlib.h>

struct el_price_hour{
  double price_sys,
         price_se1,
         price_se2,
         price_se3,
         price_se4,
         price_fi,
         price_dk1,
         price_dk2,
         price_oslo,
         price_kr_sand,
         price_bergen,
         price_molde,
         price_tr_heim,
         price_tromsoe,
         price_ee,
         price_lv,
         price_lt;
};

struct el_price_day{
  int day;
  int month;
  int year;
  int hour1;
  int hour2;
  struct el_price_hour time_of_day;
};
