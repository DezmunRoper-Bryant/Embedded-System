//------------------------------------------------------------------------------
//
//  Description: This file contains the Switch Control
//
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include  "ports.h"

#include "macros.h"

// Function Prototypes
void Init_Switches(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void Switches_Process(void);
void lcd_180(void);


//------------------------------------------------------------------------------
// Switch Configurations
extern char display_line[4][11];
extern char *display[4];
char pwm_state;
extern char change;
char run_wheels;
extern volatile unsigned char display_changed;

extern char start_moving;
extern unsigned int moving;

extern volatile unsigned int event;
extern volatile int start_p6;
int stopwatch;
volatile int seconds_elasped;

extern volatile char b_rate[10];
int baud__ = 115200;

void Switches_Process(void){
    //------------------------------------------------------------------------------
//      if (!(P4IN & SW1)){
//          baud__ = 115200;
//          strcpy(b_rate, "  115200  ");
//          //UCA0TXBUF = 0x00; // Prime the Pump
//          //UCA0IE |= UCTXIE;
//          USCI_A0_transmit();
//          //Init_Serial_UCA0();
//
//    ////      strcpy(display_line[0], "          "); //display UCA1 2 seconds after the switch is pressed
//    ////      strcpy(display_line[1], "          ");
//    ////      strcpy(display_line[2], "          ");
//    ////      strcpy(display_line[3], b_rate); //display the buad rate
//    //      display_changed = TRUE;
//    //    start_moving = 1; //Added this because of the project file
//    //    event = CIRCLE;
//          //stopwatch = 1;
////          Init_Serial_UCA0();
//
//      }
//
//      if (!(P2IN & SW2)) {
//          baud__ = 460800;
//          strcpy(b_rate, "  460800  ");
//          //UCA0TXBUF = 0x00; // Prime the Pump
//          //UCA0IE |= UCTXIE;
//          USCI_A0_transmit();
//          //Init_Serial_UCA0();
//
//    ////      strcpy(display_line[0], "          "); //display UCA1 2 seconds after the switch is pressed
//    ////      strcpy(display_line[1], "          ");
//    ////      strcpy(display_line[2], "          ");
//    ////      strcpy(display_line[3], b_rate); //display the buad rate
//    //      display_changed = TRUE;
//
//    //    event = EIGHT;
//
//          start_p6 = 1; //Added this because of the project file  //was start_moving before
//
//
//      }
//
//  if ((!(P4IN & SW1) & !(P2IN & SW2))){
//      event = CIRCLE;
//  }
//------------------------------------------------------------------------------
}

