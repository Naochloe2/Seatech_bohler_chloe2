#ifndef ADC_H
#define ADC_H
#define ADCConversionFinishedFlag
#define ADCResult

void InitADC1(void) ;
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) ;
void ADC1StartConversionSequence();
void ADCClearConversionFinishedFlag(void);

#endif 