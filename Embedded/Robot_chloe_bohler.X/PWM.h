#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void PWMSetSpeed(float vitesseEnPourcents, int NumeroMoteur) ;
void InitPWM(void) ;
void PWMUpdateSpeed() ;

#endif	/* PWM_H */

