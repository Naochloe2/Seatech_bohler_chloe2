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
#include <stdint.h>

//unsigned int ADCValue0;
//unsigned int ADCValue1;
//unsigned int ADCValue2;
unsigned char stateRobot;

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
            float volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
            volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;

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

//void OperatingSystemLoop(void) {
//    switch (stateRobot) {
//        case STATE_ATTENTE:
//            timestamp = 0;
//            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
//            stateRobot = STATE_ATTENTE_EN_COURS;
//
//        case STATE_ATTENTE_EN_COURS:
//            if (timestamp > 1000)
//                stateRobot = STATE_AVANCE;
//            break;
//
//        case STATE_AVANCE:
//            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
//            stateRobot = STATE_AVANCE_EN_COURS;
//            break;
//        case STATE_AVANCE_EN_COURS:
//            SetNextRobotStateInAutomaticMode();
//            break;
//
//        case STATE_TOURNE_GAUCHE:
//            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
//            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
//            break;
//        case STATE_TOURNE_GAUCHE_EN_COURS:
//            SetNextRobotStateInAutomaticMode();
//            break;
//
//        case STATE_TOURNE_DROITE:
//            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
//            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
//            break;
//        case STATE_TOURNE_DROITE_EN_COURS:
//            SetNextRobotStateInAutomaticMode();
//            break;
//
//        case STATE_TOURNE_SUR_PLACE_GAUCHE:
//            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
//            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
//            break;
//        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
//            SetNextRobotStateInAutomaticMode();
//            break;
//
//        case STATE_TOURNE_SUR_PLACE_DROITE:
//            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
//            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
//            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
//            break;
//        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
//            SetNextRobotStateInAutomaticMode();
//            break;
//
//        default:
//            stateRobot = STATE_ATTENTE;
//            break;
//    }
//}
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

uint16_t stateRobot = 0b000000001; // état courant
uint16_t oldStateRobot = 0b0000001; // ancien état hors recherche
uint16_t nextStateRobot = 0b0000001; // pochain état hors recherche
float a;
int b = 1;

int thresholdExtGauche1 = 20;
int thresholdGauche = 25;
int thresholdCentre = 35; // plutot mettre de l'info ici
int thresholdDroite = 25;
int thresholdExtDroite1 = 20;

uint16_t thresholdTpsBlocked = 5000; // tps max durant lequel on a pas eu 5 des capteurs à 0

void OperatingSystemLoop(void) {
    // Détermination de l état des caméras du robot
    // 00 0 0 0 00  0  0
    // EG G C D ED tps spe

    if (stateRobot == 0b000000001) // Mode Recherche (activé lorsque le dernier état à été effectué) => update stateRobot
        SetNextRobotStateInAutomaticMode();

    if ((stateRobot & (1 << 1)) >> 1) { // case blocked since thresholdTpsBlocked
        PWMSetSpeedConsigne(100, MOTEUR_DROIT);
        PWMSetSpeedConsigne(-100, MOTEUR_GAUCHE);
        stateRobot = 0b111111101; // MODE JE TOURNE
        timestamp = 0;
    } else {
        switch (stateRobot) {
                // CAS NO UPDATE DONE
            case 0b000000001:
                break;

                ////////////////////////////////////
                // LEEEEESSSSSTTTTTT GOOOOOOOOOOOO
                ////////////////////////////////////

            case 0b000000000:
                PWMSetSpeedConsigne(V_AVANCE_PLUS, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_AVANCE_PLUS, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                break;

                /////////////////////////////////////////////
                // CAS OBSTACLE BORD, RESTE LIBRE -> On continu d'avancer mais on tourne legerement
                /////////////////////////////////////////////
            case 0b110000000:
                PWMSetSpeedConsigne(V_AVANCE, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_AVANCE_PLUS, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;
                break;

            case 0b000001100:
                PWMSetSpeedConsigne(V_AVANCE_PLUS, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_AVANCE, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 0;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b110001100:
                if (b) {
                    PWMSetSpeedConsigne(V_AVANCE, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_AVANCE_MOINS, MOTEUR_GAUCHE);
                } else {
                    PWMSetSpeedConsigne(V_AVANCE_MOINS, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_AVANCE, MOTEUR_GAUCHE);
                }
                oldStateRobot = stateRobot;
                b = !b;
                stateRobot = 0b000000001;

          
                break;

                /////////////////////////////////////////////
                // CAS OBSTACLE DEVANT, COTE LIBRE -> tourne
                /////////////////////////////////////////////
            case 0b000100000:
                a = (double) rand() / (double) RAND_MAX;
                if (a > 0.5) {
                    PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                } else {
                    PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_GAUCHE);
                }
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1 + (a < 0.5);
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b111000000:
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 2;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001000000:
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 2;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b000110000:
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b000111100:
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b000101100:
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b000010000:
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b000011100:
                PWMSetSpeedConsigne(V_Rota_moins, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota_moins, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 1;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

                /////////////////////////////////////////////
                // CAS OBSTACLE DEVANT + COTE -> demi-tour //  
                /////////////////////////////////////////////
            case 0b001010000:
                a = (double) rand() / (double) RAND_MAX;
                if (a > 0.5) {
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                } else {
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                }
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001110000:
                a = (double) rand() / (double) RAND_MAX;
                if (a > 0.5) {
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                } else {
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                }
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b111110000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b111010000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b110010000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b110100000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b110110000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001100000:
                a = (double) rand() / (double) RAND_MAX;
                if (a < 0.8) {
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                } else {
                    PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                }
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b111100000:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001011100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001101100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001111100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            case 0b001001100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b000000001;

                payloadState[0] = 3;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

                //////////////////////////////////
                // CAS CUL SAC -> demi tour //////
                //////////////////////////////////

                // CAS JE TOURNE SUR PLACE
            case 0b111111101:
                if (timestamp > TPS_DEMITOUR) { // tps pour faire un demi tour
                    stateRobot = 0b000000001;
                    oldStateRobot = 0b000000001;
                    timestamp = 0;
                    robotState.tpsBlocked = 0;
                } else
                    break;
                break;

            case 0b110011100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place

                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);

                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b110101100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place

                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b110111100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place
                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b111001100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place
                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b111011100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place
                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b111101100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place
                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

            case 0b111111100:
                PWMSetSpeedConsigne(V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                stateRobot = 0b111111101; // MODE JE TOURNE sur place
                payloadState[0] = 4;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                timestamp = 0;
                robotState.tpsBlocked = 0;
                break;

                // RECUL
            case 0b011111101:
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-V_Rota, MOTEUR_GAUCHE);
                oldStateRobot = stateRobot;
                payloadState[0] = 5;
                payloadState[1] = (timestamp >> 24) & 0xFF;
                payloadState[2] = (timestamp >> 16) & 0xFF;
                payloadState[3] = (timestamp >> 8) & 0xFF;
                payloadState[4] = timestamp & 0xFF;
                UartEncodeAndSendMessage(0x0050, sizeof (payloadState) / sizeof (char), payloadState);
                break;

            default:
                stateRobot = 0b000000001;
                break;
        }
    }
}

void SetNextRobotStateInAutomaticMode() {

    // Condition particulière (à souhait)

    // Détermination de l état des caméras du robot
    // 00 0 0 0 00  0  0
    // EG G C D ED tps spe
    int bitsExGauche;
    if (robotState.distanceTelemetreExtremeGauche < thresholdExtGauche1)
        bitsExGauche = 0b11;
    else // robotState.distanceTelemetreExtremeGauche > thresholdExtGauche3
        bitsExGauche = 0b00;

    int bitsExDroite;
    if (robotState.distanceTelemetreExtremeDroite < thresholdExtDroite1)
        bitsExDroite = 0b11;
    else // robotState.distanceTelemetreExtremeDroite > thresholdExtDroite3
        bitsExDroite = 0b00;

    nextStateRobot = (bitsExGauche << 7) +
            ((int) (robotState.distanceTelemetreGauche < thresholdGauche) << 6) +
            ((int) (robotState.distanceTelemetreCentre < thresholdCentre) << 5) +
            ((int) (robotState.distanceTelemetreDroite < thresholdDroite) << 4) +
            (bitsExDroite << 2) +
            ((int) (robotState.tpsBlocked > thresholdTpsBlocked) << 1);


    // Change l'etat courant en fonction
    if (nextStateRobot != oldStateRobot) {
        stateRobot = nextStateRobot;
        oldStateRobot = nextStateRobot;

        // CUSTOM LED DEBUG
        if (((nextStateRobot & (1 << 8)) >> 8) || ((nextStateRobot & (1 << 6)) >> 6)) LED_BLANCHE = 1;
        else LED_BLANCHE = 0;
        payloadLED[0] = 0;
        payloadLED[1] = LED_BLANCHE;
        UartEncodeAndSendMessage(0x0020, 2, payloadLED);

        if ((nextStateRobot & (1 << 5)) >> 5) LED_BLEUE = 1;
        else LED_BLEUE = 0;
        payloadLED[0] = 1;
        payloadLED[1] = LED_BLEUE;
        UartEncodeAndSendMessage(0x0020, 2, payloadLED);

        if (((nextStateRobot & (1 << 2)) >> 2) || ((nextStateRobot & (1 << 4)) >> 4))
            LED_ORANGE = 1;
        else LED_ORANGE = 0;
        payloadLED[0] = 2;
        payloadLED[1] = LED_ORANGE;
        UartEncodeAndSendMessage(0x0020, 2, payloadLED);
    } else; // on reste dans l'état recherche

    // Reset le timer si on est pas blocké
    if (nextStateRobot == 0)
        robotState.tpsBlocked = 0;


}

}