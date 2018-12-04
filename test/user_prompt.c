#include <stdio.h>
#include <stdlib.h>
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

void prompt_user(double *battery_capacity, double *battery_charge_level,
    double *charger_effect, double *max_discharge, int *auto_discharge)
{
	int done = 0;

	system(CLSCREEN);
	printf("Enter battery capacity(kWh): \n>");
	while (!done)
	{
		scanf(" %lf", battery_capacity);
		if (*battery_capacity > 0)
			done = 1;
		else
			printf("Error! Battery capacity cannot be negative.\n>");
	}

	system(CLSCREEN);
	printf("Enter battery charge level(%%): \n>");
	done = 0;
	while (!done)
	{
		scanf(" %lf", battery_charge_level);
		if ((*battery_charge_level <= 100) && (*battery_charge_level >= 0))
			done = 1;
		else
			printf("Error! Battery charge level can't be below 0%% or above 100%%.\n>");
	}

	system(CLSCREEN);
	printf("Enter charger effect(%%/h): \n>");
	done = 0;
	while (!done)
	{
		scanf(" %lf", charger_effect);
		if ((*charger_effect <= 100) && (*charger_effect >= 0))
			done = 1;
	  else
			printf("Error! Charger effect can't be below 0%%/h or above 100%%/h.\n>");

	}

	system(CLSCREEN);
  printf("Auto discharge option? yes = 1, no = 0: \n>");
	done = 0;
	while (!done)
	{
		scanf(" %d", auto_discharge);
		if ((*auto_discharge == 1) || (*auto_discharge == 0))
			done = 1;
		else
			printf("Error! You need to enter 1 or 0, please try again.\n>");

	}
  if(*auto_discharge == 1){
	   printf("Enter max discharge level(%%): \n>");
	    done = 0;
	     while (!done)
	      {
		    scanf(" %lf", max_discharge);
		      if ((*max_discharge <= 100) && (*max_discharge >= 0))
		         done = 1;
	        else
 	        printf("Error! Max discharge level can't be below 0%% or above 100%%.\n>");
	      }
	}
	system(CLSCREEN);
}
