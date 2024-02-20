#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "timers.h"

int ADC_Channel;
volatile long ADC_Left_Detect;
volatile long ADC_Right_Detect;
volatile long ADC_Thumb_Detect;

extern char display_line[4][11];
extern char *display[4];

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

char adc_char[4];

int left_value;
int right_value;

//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD(int hex_value){
    int value;
    int i ;

    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;

//    if(lrt == 'l'){
//        strcpy(left_value, adc_char);
//    }
//    if(lrt == 'r'){
//        strcpy(right_value, adc_char);
//    }
}
//-----------------------------------------------------------------


//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
//-------------------------------------------------------------
    int i;
    unsigned int real_line;
    real_line = line - 1;
//    strcpy(display_line[1], "Thumb");
//    strcpy(display_line[2], "Right");
//    strcpy(display_line[3], "Left");
    for(i=0; i < 4; i++) {
//enable

                //display_line[real_line][i+location] = adc_char[i];
        //display_changed = TRUE;
        //update_display = TRUE;
    }
}
//-------------------------------------------------------------

void Init_ADC(void){
//------------------------------------------------------------------------------
// V_DETECT_L (0x04) // Pin 2 A2
// V_DETECT_R (0x08) // Pin 3 A3
// V_THUMB (0x20) // Pin 5 A5
//------------------------------------------------------------------------------
// ADCCTL0 Register
    ADCCTL0 = 0; // Reset
    ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
    ADCCTL0 |= ADCMSC; // MSC
    ADCCTL0 |= ADCON; // ADC ON
// ADCCTL1 Register
    ADCCTL1 = 0; // Reset
    ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
    ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
    ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
    ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
    ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
// ADCCTL1 & ADCBUSY identifies a conversion is in process
// ADCCTL2 Register
    ADCCTL2 = 0; // Reset
    ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
    ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
    ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
    ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
// ADCMCTL0 Register
    ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
    ADCMCTL0 |= ADCINCH_5; // V_THUMB (0x20) Pin 5 A5
    ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
    ADCCTL0 |= ADCENC; // ADC enable conversion.
    //ADCCTL0 |= ADCSC; // ADC start conversion.
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
            // before its previous conversion result was read.
            break;
        case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
            break;
        case ADCIV_ADCHIIFG: // Window comparator interrupt flags
            break;
        case ADCIV_ADCLOIFG: // Window comparator interrupt flag
            break;
        case ADCIV_ADCINIFG: // Window comparator interrupt flag
            break;


        case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
            ADCCTL0 &= ~ADCENC; // Disable ENC bit.
            switch (ADC_Channel++){
                case 0x00: // Channel A2 Interrupt
                    ADC_Left_Detect = ADCMEM0; // Move result into Global
                    ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
                    left_value = ADC_Left_Detect;
                    HEXtoBCD(ADC_Left_Detect); // Convert result to String
                    adc_line(4,0); // Place String in Display
                    ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                    ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
                    ADCCTL0 |= ADCSC; // Start next sample
                    break;
                case 0x01: // Channel A3 Interrupt
                    ADC_Right_Detect = ADCMEM0; // Move result into Global
                    ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
                    right_value = ADC_Right_Detect;
                    HEXtoBCD(ADC_Right_Detect); // Convert result to String
                    adc_line(3,0); // Place String in Display
                    ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
                    ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
                    ADCCTL0 |= ADCSC; // Start next sample
                    break;
                case 0x02:
                    ADC_Thumb_Detect = ADCMEM0; // Move result into Global Values
                    ADC_Thumb_Detect = ADC_Thumb_Detect >> 2; // Divide the result by 4
                    HEXtoBCD(ADC_Thumb_Detect); // Convert result to String
                    //adc_line(2,0); // Place String in Display
                    ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A?
                    ADCMCTL0 |= ADCINCH_2; // Enable First channel 2
                    ADC_Channel=0;

                    break;
                default:
                    break;
            }
            ADCCTL0 |= ADCENC; // Enable Conversions
            break;

        default:
            break;
    }
}
