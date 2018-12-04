#include "struct.h"
#define NUM_DAYS 24
#define MAX_MATRIX_SIZE 24

typedef struct el_price_day el_price_day;
typedef struct el_price_hour el_price_hour;
void prompt_user(double *battery_capacity, double *battery_charge_level,                                  /* Prototyper */
                 double *charger_effect, double *max_discharge, int *auto_discharge);
int open_practice_file(el_price_day el_price_day[]);
void prntfunc(el_price_day el_price_day[], int i, int end_of_for_loop, double *battery_capacity, double *battery_charge_level, double *charger_effect, double *max_discharge, int auto_discharge, char *charge);

double regression(int *question_asked, int mode_choice, int day_week_choice, double vector[NUM_DAYS], int res_reg_data, int *order);
void create_matrix(int number_of_days, double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_trans[][MAX_MATRIX_SIZE], int day_week_choice, int *order);
double determinant(double udreg_arr_1[][MAX_MATRIX_SIZE], int order);
void cofactor(double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_fac[][MAX_MATRIX_SIZE], int order);
void transpose_and_invert(double udreg_fac[][MAX_MATRIX_SIZE], double udreg_invert[][MAX_MATRIX_SIZE], int *order, double det);
void invert_point_trans(int number_of_days, double udreg_invert[][MAX_MATRIX_SIZE], double udreg_trans[][NUM_DAYS], double udreg_total[][NUM_DAYS], int *order);
void total_dot_vector(double udreg_total[][NUM_DAYS], double vector[MAX_MATRIX_SIZE], double result_vector[NUM_DAYS], int *order);
void find_smaller_matrix(int x_val, int y_val, int order, double udreg_arr_1[][MAX_MATRIX_SIZE], double udreg_arr_1_copy[][MAX_MATRIX_SIZE]);

int regression_data(int * quanti_done, int day_week_choice, int choice2, double *day_to_predict, double y_vector[], el_price_day el_price_day[], int num_of_data);
void set_day_week_variables(int day_week_choice, int *how_long_back_factor, int *how_long_back_sub, int *jump_back_in_time);
void day_week_prompt(int mode_choice, int *day, int *month, int *year, int *time_of_day, int *how_long_back, int *quanti_done);
int find_date(int num_of_data,el_price_day el_price_day[], int day, int month, int year, int time_of_day, double *day_to_predict);
int find_prev_dates(int res_find_date, int how_long_back, int mode_choice, int hours_back_in_time, double y_vector[],el_price_day el_price_day[]);


int find_days_of_month(int *month, int *time_of_day, int* year);
