#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include <stdlib.h>
#include <stdbool.h>

//unsigned int ADCValue0;
//unsigned int ADCValue1;
//unsigned int ADCValue2;



int thresholdExtGauche1 = 25;
int thresholdGauche = 35;
int thresholdCentre = 30; // plutot mettre de l'info ici
int thresholdDroite = 35;
int thresholdExtDroite1 = 25;

int nextStateRobot;
int stateRobot;

unsigned char sensorState;

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
    
    InitUART();
    // InitTx();

    //    PWMSetSpeedConsigne(20, MOTEUR_DROIT);
    //    PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);

    LED_BLANCHE = 0;
    LED_BLEUE = 0;
    LED_ORANGE = 0;
    
    

    while (1) {
        
//        SendMessageDirect((unsigned char*) "Bonjour", 7);
//        
//        SendMessage((unsigned char*) "Bonjour", 7);
//        __delay32(40000000);
        
        int i;
        for(i=0; i< CB_RX1_GetDataSize(); i++)
        {
            unsigned char c = CB_RX1_Get();
            SendMessage(&c,1);
        }
        __delay32(1000);

        
        if (ADCIsConversionFinished()) {
            unsigned int * result = ADCGetResult();
            ADCClearConversionFinishedFlag();
            float volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;

            if (robotState.distanceTelemetreExtremeGauche < thresholdExtGauche1 || robotState.distanceTelemetreGauche < thresholdGauche)
                LED_BLANCHE  = 1;
            else
                LED_BLANCHE = 0;

            if (robotState.distanceTelemetreCentre < thresholdCentre)
                LED_BLEUE = 1;
            else
                LED_BLEUE = 0;

            if (robotState.distanceTelemetreExtremeDroit < thresholdExtDroite1 || robotState.distanceTelemetreDroit < thresholdDroite)
                LED_ORANGE = 1;
            else
                LED_ORANGE = 0;

            sensorState = 0;
            if (robotState.distanceTelemetreExtremeGauche < thresholdExtGauche1)
                sensorState = sensorState | 0b10000;
            if (robotState.distanceTelemetreGauche < thresholdGauche)
                sensorState = sensorState | 0b01000;
            if (robotState.distanceTelemetreCentre < thresholdCentre)
                sensorState = sensorState | 0b00100;
            if (robotState.distanceTelemetreDroit < thresholdDroite)
                sensorState = sensorState | 0b00010;
            if (robotState.distanceTelemetreExtremeDroit < thresholdExtDroite1)
                sensorState = sensorState | 0b00001;

        }
    }
}

//
//unsigned char nextStateRobot = 0;
//
//void SetNextRobotStateInAutomaticMode() {
//    //    unsigned char positionObstacle = PAS_D_OBSTACLE;
//
//    //Détermination de la position des obstacles en fonction des télémètres
//    //    if (robotState.distanceTelemetreDroit < 30 &&
//    //            robotState.distanceTelemetreCentre > 20 &&
//    //            robotState.distanceTelemetreGauche > 30) //Obstacle à droite
//    //        positionObstacle = OBSTACLE_A_DROITE;
//    //    else if (robotState.distanceTelemetreDroit > 30 &&
//    //            robotState.distanceTelemetreCentre > 20 &&
//    //            robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
//    //        positionObstacle = OBSTACLE_A_GAUCHE;
//    //    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
//    //        positionObstacle = OBSTACLE_EN_FACE;
//    //    else if (robotState.distanceTelemetreDroit > 30 &&
//    //            robotState.distanceTelemetreCentre > 20 &&
//    //            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
//    //        positionObstacle = PAS_D_OBSTACLE;
//
//
//
//    //Détermination de l?état à venir du robot
//    //    if (positionObstacle == PAS_D_OBSTACLE)
//        nextStateRobot = STATE_AVANCE;
//    else if (positionObstacle == OBSTACLE_A_DROITE)
//        nextStateRobot = STATE_TOURNE_GAUCHE;
//    else if (positionObstacle == OBSTACLE_A_GAUCHE)
//        nextStateRobot = STATE_TOURNE_DROITE;
//    else if (positionObstacle == OBSTACLE_EN_FACE)
//        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

//Si l?on n?est pas dans la transition de l?étape en cours

//    if (nextStateRobot != stateRobot - 1)
//        stateRobot = nextStateRobot;

void OperatingSystemLoop(void) {

    switch (sensorState) {
        case 0b00000:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10001:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
    }
    
    SetNextRobotStateInAutomaticMode();
}


void SetNextRobotStateInAutomaticMode() {

    // Change l'etat courant en fonction
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;

        switch (stateRobot) {
            case STATE_ATTENTE:
                timestamp = 0;
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_ATTENTE_EN_COURS;

            case STATE_ATTENTE_EN_COURS:
                if (timestamp > 1000)
                    stateRobot = STATE_AVANCE;
                break;

            case STATE_AVANCE:
                PWMSetSpeedConsigne(30, MOTEUR_DROIT);
                PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
                stateRobot = STATE_AVANCE_EN_COURS;
                break;
            case STATE_AVANCE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_GAUCHE:
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
                
            case STATE_TOURNE_LEGER_GAUCHE:
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_LEGER_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_LEGER_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_DROITE:
                PWMSetSpeedConsigne(5, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
                
            case STATE_TOURNE_LEGER_DROITE:
                PWMSetSpeedConsigne(15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_LEGER_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_LEGER_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_GAUCHE:
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_DROITE:
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            default:
                stateRobot = STATE_ATTENTE;
                break;
        }
    }
}