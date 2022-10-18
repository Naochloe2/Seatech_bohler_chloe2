#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    
    InitTimer23();
    InitTimer1();
    SetFreqTimer4(1000);
    InitTimer4();
    
    InitPWM();
    InitADC1();

    PWMSetSpeedConsigne(20, MOTEUR_DROIT);
    PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);

    LED_BLANCHE = 0;
    LED_BLEUE = 0;
    LED_ORANGE = 0;


    while (1) {
        if (ADCIsConversionFinished()) {
            unsigned int * result = ADCGetResult();
            ADCClearConversionFinishedFlag();
            float volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34/ volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34/ volts - 5;
            volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34/ volts - 5;
            
//            ADCValue0 = result[0]*3.2;
//            ADCValue1 = result[1]*3.2;
//            ADCValue2 = result[2]*3.2;
//            
//            if (ADCValue0 > 1011)
//                LED_ORANGE = 1;
//            else 
//                LED_ORANGE = 0;
//            if (ADCValue1 > 1011)
//                LED_BLEUE = 1;
//            else 
//                LED_BLEUE = 0;
//            if (ADCValue2 > 1011)
//                LED_BLANCHE = 1;
//            else 
//                LED_BLANCHE = 0;
        }
    }
}

