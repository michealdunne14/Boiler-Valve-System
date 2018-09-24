/*
 * main.c
 *
 *  Created on: 2018 Mar 27 13:47:31
 *  Author: michealdunne14
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
//Gets the result of the ADC measure
uint32_t result;
//Opens the valve to a position
uint32_t valvePosition;
// Range the values down
float range;
// Checks if it is going down
bool calc = false;

void MeasureIRQHandler(void){
	//Measures the values
	result = ADC_MEASUREMENT_GetGlobalResult();
	//Puts a limit on how high the value can go
	result = (result) & 0xfff;
	// reduces the range
	range = result/81.9;
	//Checks if value is below 25
		if(range < 25){
			valvePosition = 0;
			PWM_SetDutyCycle(&LED, valvePosition*100);
		}
		//Checks if the value if between 25 and 30 if this is the case opens the valve by 25%
		else if ((range >= 25) && (range <= 29)) {
			valvePosition = 25;
			PWM_SetDutyCycle(&LED, valvePosition*100);
			calc = true;
		}
		//Checks if the value if between 30 and 40 if this is the case opens the valve by 50%
		else if ((range >= 30) && (range <= 40)){
			valvePosition = 50;
			PWM_SetDutyCycle(&LED, valvePosition*100);
			calc = true;
		}
		//Checks if the value is above 40% if so valve is open 100%
		else if(range > 40){
			valvePosition = 100;
			PWM_SetDutyCycle(&LED, valvePosition*100);
			calc = true;
		}
		// if the value is above 22 and it has gone up above 25 then keep the valve at 100% until it goes below 22/
		else if(range > 22 && calc == true){
				PWM_SetDutyCycle(&LED, 100*100);
				calc = false;
		}
}
//WatchDog
void Watchdog_Handler(void){
	WATCHDOG_Service();
}



int main(void)
{

  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */
	ADC_MEASUREMENT_StartConversion(&ADCMEASURE);
  if(status != DAVE_STATUS_SUCCESS)
  {
		//Starts the watchDog
	 WATCHDOG_Start();
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	
  }
}
