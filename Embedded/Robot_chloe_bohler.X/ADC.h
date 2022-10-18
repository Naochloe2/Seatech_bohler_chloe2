#ifndef ADC_H
#define ADC_H

unsigned int * ADCGetResult(void);
void InitADC1(void);
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
void ADC1StartConversionSequence();
void ADCClearConversionFinishedFlag(void);
unsigned char ADCIsConversionFinished(void);

#endif  