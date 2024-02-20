#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "timers.h"

int seconds_elapsed = 0;
int demo_seconds = 0;
int s_counter = 0;
int d_counter = 0;

extern char start_moving;


char on = 'n';

//extern volatile int ADC_Channel;

//int start_p6;
extern volatile int lcd_counter;
extern volatile int left_value;
extern volatile int right_value;

//int black_line;
int cw_count;

extern volatile int stopwatch;

extern volatile int start_p6;

//hw8
extern volatile char b_rate[10];

unsigned int CCR0_counter;
unsigned int CCR0_counter2;
unsigned int CCR0_counter3;

extern char display_line[4][11];
extern volatile unsigned int CCR1_counter;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

volatile unsigned int start;
volatile unsigned int SW1_pressed;
volatile unsigned int SW2_pressed;
extern char test_rc[LARGE_RING_SIZE];
extern char state;
extern char IOT_state;

volatile char process_buffer[13];
extern volatile char tx_A1buf[LARGE_RING_SIZE];

int tbcounter;

extern volatile int run_line_follow;


extern volatile int left_value;
extern volatile int right_value;

extern volatile int white_right_value;
extern volatile int white_left_value;

extern volatile char right_valu_string[4];
extern volatile char left_valu_string[4];


extern volatile int black_right_value;
extern volatile int black_left_value;

extern volatile char right_valu_string_bl[4];
extern volatile char left_valu_string_bl[4];

extern volatile int cal_screen;


extern volatile int calibrate_adc;

//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
TB0CTL = TBSSEL__SMCLK; // SMCLK source
TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
TB0CTL |= MC__CONTINOUS; // Continuous up
TB0CTL |= ID__8; // Divide clock by 8
TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
TB0CCTL0 |= CCIE; // CCR0 enable interrupt
TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
// TB0CCTL1 |= CCIE; // CCR1 enable interrupt
TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
// TB0CCTL2 |= CCIE; // CCR2 enable interrupt
TB0CTL &= ~TBIE; // Disable Overflow Interrupt
TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0


    tbcounter += 1;
    if(tbcounter >= 2){
        CCR0_counter += 1;
        CCR0_counter2 += 1;
        CCR0_counter3 += 1;
//        if(CCR0_counter == 10){
//
//        }
        tbcounter = 0;
    }

    if(lcd_counter++ > 4 ){
        update_display = TRUE;
        lcd_counter = 0;
}

//    s_counter ++;
//    if(s_counter >= 20){
////        strcpy(tx_A1buf, "NCSU  #1\r\n");
////        UCA0IE |= UCTXIE;
//    //if(s_counter >= 5){
//        seconds_elapsed ++;
////        strcpy(display_line[0], "          "); //display UCA1 2 seconds after the switch is pressed
////        strcpy(display_line[1], "          ");
////        strcpy(display_line[2], "          ");
////        strcpy(display_line[3][5], seconds_elapsed + 0x30); //display the buad rate
////        display_changed = TRUE;
//
//        s_counter = 0;
//    }

        d_counter ++;
        if(d_counter >= 20){
            demo_seconds ++;
            d_counter = 0;
            seconds_elapsed++;
        }


    if(run_line_follow == 1){
        s_counter ++;
        if(s_counter >= 20){
        //if(s_counter >= 5){
            seconds_elapsed ++;
            s_counter = 0;
        }
    }



//----------------------------------------------------------------------------
}




//
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    // Switch 1
    if (P4IFG & SW1) {
//        SW1_pressed = PRESSED;
        //CCR1_counter = 0;
        P4IFG &= ~SW1; // IFG SW1 cleared
        P4IE &= ~SW1; // disable switch interrupt
        TB0CCTL1 |= CCIE; // enable timer CCR1
        UCA0IE |= UCTXIE;

        start_p6 = 1;
//        if(calibrate_adc){
//            switch(cal_screen){
//            case 0:
//                white_right_value = right_value;
//                white_left_value = left_value;
//                NumtoBCD_right(left_value , 'L');
//                NumtoBCD_right(right_value, 'R');
//                cal_screen = 1;
//                break;
//            case 1:
//                //SW1_pressed == PRESSED;
//                cal_screen = 0;
//                break;
//            case 2:
//                //SW1_pressed == PRESSED;
//                black_right_value = right_value;
//                black_left_value = left_value;
//                NumtoBCD_black(left_value , 'l');
//                NumtoBCD_black(right_value, 'r');
//                cal_screen = 3;
//                break;
//            case 3:
//                //SW1_pressed == PRESSED;
//                cal_screen = 0;
//                break;
//            case 4:
//                break;
//            }
//        }

        IOT_state = TRANSMIT;
    }
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
// Switch 2
  if (P2IFG & SW2) {
      run_line_follow = 1;
//      ADCCTL0 |= ADCSC;
//    SW2_pressed = PRESSED;
    //CCR1_counter = 0;
    P2IFG &= ~SW2; // IFG SW2 cleared
    P2IE &= ~SW2; // disable switch interrupt
    TB0CCTL1 |= CCIE; // enable timer CCR1
//    strcpy(display_line[0], "          ");
//    strcpy(display_line[1], "          ");
//    strcpy(display_line[2], "          ");
    //Init_Serial_UCA0('b');
//    if(calibrate_adc){
//        switch(cal_screen){
//        case 0:
//            break;
//        case 1:
//            //SW2_pressed == PRESSED;
//            cal_screen = 2;
//            break;
//        case 2:
//            break;
//        case 3:
//            //SW2_pressed == PRESSED;
//            //strcpy(display_line[0], "          ");
//            cal_screen = 4;
//            break;
//        case 4:
//            break;
//        }
//    }
  }
}



//#pragma vector=PORT4_VECTOR
//__interrupt void switch_interrupt(void) {
//    // Switch 1
//        if (P4IFG & SW1) {
//            CCR1_counter = 0;
//            P4IFG &= ~SW1;                      // IFG SW1 cleared
//            P4IE &= ~SW1;                       // SW1 interrupt Enabled
//            TB0CCR1 += TB0CCR1_INTERVAL;        // Add Offset to TBCCR1
//            TB0CCTL1 &= ~ CCIFG;
//            TB0CCTL1 |= CCIE;
//            IOT_state = TRANSMIT;


//            UCA0BRW = 4;
//            UCA0MCTLW = 0x5551;
//            USCI_A0_transmit();
//            strcpy(b_rate, "  115200  ");
//            display_changed = TRUE;
//            update_display = TRUE;
//
////                strcpy(display_line[2], "   Baud   ");
////                strcpy(display_line[3], " 115,200  ");
//
//
//        }
//}
//
//#pragma vector=PORT2_VECTOR
//__interrupt void switchP2_interrupt(void){
//    // Switch 2
//    if (P2IFG & SW2) {
//        P2IFG &= ~SW2; // IFG SW2 cleared
//        P2IE &= ~SW2;
//        TB0CCTL1 &= ~CCIFG; // Disables Flag
//        TB0CCR2 += TB0CCR2_INTERVAL;
//        TB0CCTL2 |= CCIE; // CCR1 enable interrupt
//        CCR1_counter = 0;
//        Init_Serial_UCA0('b');

//
//        UCA0BRW = 17;
//        UCA0MCTLW = 0x4A00;
//        USCI_A0_transmit();
//
////        strcpy(display_line[2], "   Baud   ");
////        strcpy(display_line[3], " 115,200  ");
//        strcpy(b_rate, "  460800  ");
//        display_changed = TRUE;
//        update_display = TRUE;
//    }
//}

#pragma vector = TIMER0_B1_VECTOR
//interrupt void TIMER_B0_CCR1_2_OVFL_ISR(void){
interrupt void Timer0_B1_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 1-3, overflow Interrupt Vector (TAIV) handler
  switch(__even_in_range(TB0IV,14)){
    case  0: break;                    // No interrupt
    case  2:                           // CCR1 SW1 Debounce
      P4IFG &= ~SW1;                  // Clear P4.1 interrupt flags
      P4IE |= SW1;                    // P4.1 Enable SW2 Interrupt

      TB0CCTL1 &= ~CCIFG;             // Clear CCR1 interrupt flag
      TB0CCTL1 &= ~CCIE;              // Disable CCR1 interrupt
      break;
    case  4:                           // CCR2 display update every 200msec
      P2IFG &= ~SW2;                  // Clear P2.6 interrupt flags
      P2IE |= SW2;                    // P2.6 Enable SW2 Interrupt

      TB0CCTL2 &= ~CCIFG;             // Clear CCR2 interrupt flag
      TB0CCTL2 &= ~CCIE;              // Disable CCR2 interrupt
      break;
    case 14:                           // overflow
      break;
    default: break;

  }

//------------------------------------------------------------------------------

}











//------------------------------------------------------------------------------
// Timer B1 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B1(void) {
TB1CTL = TBSSEL__SMCLK; // SMCLK source
TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
TB1CTL |= MC__CONTINOUS; // Continuous up
TB1CTL |= ID__8; // Divide clock by 8
TB1EX0 = TBIDEX__8; // Divide clock by an additional 8
TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
//TB1CCTL0 |= CCIE; // CCR0 enable interrupt
// TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
// TB0CCTL1 |= CCIE; // CCR1 enable interrupt
// TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
// TB0CCTL2 |= CCIE; // CCR2 enable interrupt
TB1CTL &= ~TBIE; // Disable Overflow Interrupt
TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB1 0 Interrupt handler
//----------------------------------------------------------------------------
TB1CCR0 += TB1CCR0_INTERVAL; // Add Offset to TBCCR0
ADCCTL0 |= ADCSC;

//if(run_line_follow){
//    ADCCTL0 |= ADCSC;
//}
//if(start_moving_ == 1){
//    P6OUT ^= L_FORWARD; // Toggle the state of the L_FORWARD pin
//    P6OUT ^= R_FORWARD; // Toggle the state of the R_FORWARD pin
//}
//----------------------------------------------------------------------------
}


#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB1IV,14)){
case 0: break; // No interrupt
case 2: // CCR1 not used
//...... Add What you need happen in the interrupt ......
TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
break;
case 4: // CCR2 not used
//...... Add What you need happen in the interrupt ......
TB1CCR2 += TB1CCR2_INTERVAL; // Add Offset to TBCCR2
break;
case 14: // overflow
//...... Add What you need happen in the interrupt ......
break;
default: break;
}
//----------------------------------------------------------------------------
}





//PWM

void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 LCD_BACKLITE
// TB3.2 P6.1 R_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE
//------------------------------------------------------------------------------
    TB3CTL = TBSSEL__SMCLK; // SMCLK
    TB3CTL |= MC__UP; // Up Mode
    TB3CTL |= TBCLR; // Clear TAR
    PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]
    TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
    LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle
    TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM duty cycle
    TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.3 Left Forward PWM duty cycle
    TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}

