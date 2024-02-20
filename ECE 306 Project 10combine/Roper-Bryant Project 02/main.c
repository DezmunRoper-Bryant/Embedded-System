//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

// RED LED 0
// GREEN LED 1
// 0 TEST PROBE

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;

char start_moving;
unsigned int moving;

unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred


int event = NONE; // Set the event to NONE
//extern volatile char state;

extern volatile int seconds_elapsed;

extern volatile int stopwatch;
//HW 8
#define SMALL_RING_SIZE (16)
volatile char USB_Char_Rx[SMALL_RING_SIZE] ;
extern volatile char message_done;


//p6
int start_p6;
int lcd_counter = 0;
extern volatile int left_value;
extern volatile int right_value;
//int black_line;
int cw_count;

//hw 8
//unsigned int CCR0_counter;
//unsigned int CCR1_counter;

extern volatile int seconds_elasped;
char array_of_characters[10] = "NCSU  #1";
char b_rate[10] = "  115200  ";

unsigned int Last_Time_Sequence = 0; // a variable to identify Time_Sequence has changed
unsigned int cycle_time = 0; // is a new time base used to control making shapes
unsigned int time_change = 0; // is an identifier that a change has occurred
volatile unsigned int left_motor_count;
volatile unsigned int right_motor_count;
extern volatile unsigned int segment_count;
extern volatile unsigned int state;
volatile unsigned int delay_start;
extern volatile char USB_Char_Rx[SMALL_RING_SIZE];
extern char flag;
//extern char message_done;
char pb_index;
extern char process_buffer[25];
char IOT_state = WAITING;
extern char tx_A1buf[LARGE_RING_SIZE];
extern char rx_A1buf[LARGE_RING_SIZE];
extern char rx_A0buf[LARGE_RING_SIZE];
extern volatile unsigned int rx_A0;
char recieved_message[LARGE_RING_SIZE];
extern unsigned int temp_A0;
volatile unsigned int wx_A0;
unsigned int temp_buff;
char wx_A0buf[LARGE_RING_SIZE];
unsigned int i;
unsigned int j;


int transmission_complete;


int run_line_follow = 0;

int p7_action;
extern volatile int blackLineDetected;

int line_not_done = 1;


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

extern volatile unsigned int SW1_pressed;
extern volatile unsigned int SW2_pressed;



int calibrate_adc;

//p9
extern volatile int p9_rx_state;
extern volatile unsigned int CCR0_counter;
extern volatile unsigned int CCR0_counter2;
extern volatile unsigned int CCR0_counter3;
extern volatile unsigned int CCR1_counter;

void main(void){
    //------------------------------------------------------------------------------
    // Main Program
    // This is the main routine for the program. Execution of code starts here.
    // The operating system is Back Ground Fore Ground.
    //
    //------------------------------------------------------------------------------
    PM5CTL0 &= ~LOCKLPM5;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings

    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();                   // Initialize Variables and Initial Conditions
    //Init_Timers();                       // Initialize Timers
    Init_LCD();                          // Initialize LCD
    Init_Timer_B0();
    // This line identifies the text on the Display is to change.
    display_changed = TRUE;
    Init_Timer_B1();
    Init_Timer_B3();
    Init_ADC();

    Init_Serial_UCA0('s');
    Init_Serial_UCA1('s');
    //p7_action = 0;

//    Init_Serial_UCA0();
//    Init_Serial_UCA1();//Communicate at 115200 baud rate

    // Place the contents of what you want on the display, in between the quotes
    // Limited to 10 characters per line
//    strcpy(display_line[0], "Arrived 0");
//    display_line[0][9], 's';
//    strcpy(display_line[1], "BLACK LINE");
//    strcpy(display_line[2], "BLACK LINE");
//    strcpy(display_line[3], "BLACK LINE");
    //    strcpy(display_line[0], "          ");
//    strcpy(display_line[1], "          ");
//    strcpy(display_line[2], "BAUD: 115K");
//    strcpy(display_line[3], "          ");




    //movement_stop();

    CCR0_counter = 0;
    CCR0_counter2 = 0;
    CCR0_counter3 = 0;
    wx_A0 = 0;
    unsigned int linear_buffer_index = 0;

    p7_action = 21;
    calibrate_adc = 1;

//    seconds_elapsed = 0;
//    if(seconds_elapsed >= 1){
//        strcpy(display_line[0], "IOT IS ON ");
//        display_changed = TRUE;
//        P3OUT |= IOT_EN; // Initial Value = Low / Off
//    }

    //------------------------------------------------------------------------------
    // Beginning of the "While" Operating System
    //------------------------------------------------------------------------------
    while(ALWAYS) {                      // Can the Operating system run
        Display_Process();
        P3OUT ^= TEST_PROBE;
        P2OUT |= IR_LED;

//        if(seconds_elapsed == 1){
//            P3OUT |= IOT_EN; // Initial Value = Low / Off
        //            seconds_elapsed = 0;
        //        }
        //P3OUT |= IOT_EN;

        if (CCR0_counter == 10){
            P3OUT |= IOT_EN;
        }

        ADCCTL0 |= ADCSC;
//        LEFT_FORWARD_SPEED = 27000;
//        RIGHT_FORWARD_SPEED = 14500;
        configure_IOT();
        load_command();
        perform_command();







//
//        if(calibrate_adc){
//            switch(cal_screen){
//            case 0:
//                strcpy(display_line[0], "White?    ");
//                display_changed = TRUE;
//                update_display = TRUE;
//                break;
//            case 1:
//                strcpy(display_line[0], "Selections");
//                adc_line_10(2, right_valu_string, left_valu_string);
//                strcpy(display_line[3], "Okay?");
//                display_changed = TRUE;
//                update_display = TRUE;
//                break;
//            case 2:
//                strcpy(display_line[0], "Black Line?");
//                display_changed = TRUE;
//                update_display = TRUE;
//                break;
//            case 3:
//                strcpy(display_line[0], "Selections");
//                strcpy(display_line[2], "          ");
//                strcpy(display_line[3], "          ");
//                adc_line_10(2, right_valu_string_bl, left_valu_string_bl);
//                strcpy(display_line[3], "Okay?");
//                display_changed = TRUE;
//                update_display = TRUE;
//                break;
//            case 4:
//                //            strcpy(display_line[0], "          ");
//                adc_line_10(1, right_valu_string, left_valu_string);
//                adc_line_10(3, right_valu_string_bl, left_valu_string_bl);
//                display_changed = TRUE;
//                update_display = TRUE;
//                seconds_elapsed = 0;
//                cal_screen = 5;
//                break;
//            case 5:
//                if(seconds_elapsed >= 2){
//                    wheel_Forward(3);
//                    cal_screen = 6;
//                }
//                break;
//            case 6:
//                if(isBlackLineDetected(left_value, right_value)){
//                    strcpy(display_line[0], "BLACK LINE");
//                    display_changed = TRUE;
//                    wheel_Off();
//                    calibrate_adc = 0;
//                }
//                break;
//            default:
//                break;
//            }
//        }

//        if(calibrate_adc == 0){

        //}


//        strcpy(display_line[0], "Arrived 0");
//        strcpy(display_line[0][9], "Z");
//        strcpy(display_line[1], "BLACK LINE");
//        strcpy(display_line[2], "BLACK LINE");
//        strcpy(display_line[3], "BLACK LINE");

//
//        |Arrived 0X|
//        |          |
//        |          |
//        |F0001 999s|

        switch(p7_action){
        case 0:
            P1OUT |= RED_LED; // Initial Value = Low / Off
            wheel_Forward(4);
            p7_action = 1;
            if(seconds_elapsed)
                break;
        case 1:
            if(isBlackLineDetected(left_value, right_value)){
                strcpy(display_line[0], "BLACK LINE");
                display_changed = TRUE;
                wheel_Off();
                seconds_elapsed = 0;
                p7_action = 2;
            }
            else{
                strcpy(display_line[0], "FORWARDS   ");
                //strcpy(display_line[0], "NOT BLACK ");
                display_changed = TRUE;
            }
            break;
        case 2:
            strcpy(display_line[0], "WAITING...");
            if(seconds_elapsed >= 5){
                //            if(seconds_elapsed >= 3){
                //p6_action = 7;
                p7_action = 3;
            }
            break;
        case 3:
            counter_clockwise();
            //clockwise();
            p7_action = 4;
            break;
        case 4:
            if(isBlackLineDetected(left_value, right_value)){
                strcpy(display_line[0], "BLACK LINE");
                display_changed = TRUE;
                wheel_Off();
                blackLineDetected = TRUE;
                seconds_elapsed = 0;
                p7_action = 5;
            }
            else{
                strcpy(display_line[0], "ALIGNING  ");
                //strcpy(display_line[0], "NOT BLACK ");
                display_changed = TRUE;
            }
            break;
        case 5:
            strcpy(display_line[0], "WAITING...");
            if(seconds_elapsed >= 5){
                p7_action = 6;
            }
            break;
        case 6:
            strcpy(display_line[0], "FOLLOWING ");
            display_changed = TRUE;
            //            LEFT_FORWARD_SPEED = 0;
            //            RIGHT_FORWARD_SPEED = 0;
            if(line_not_done == 1){
                //if (seconds_elapsed < 40){
                if (left_value >= BLACK_LEFT && right_value >= BLACK_RIGHT){
                    strcpy(display_line[0], "STRAIGHT  ");
                    wheel_Forward(6); //3
                    //wheel_Forward(2);
                }
                else if (left_value < BLACK_LEFT && right_value >= BLACK_RIGHT){
                    strcpy(display_line[0], "TURN LEFT ");
                    LEFT_FORWARD_SPEED = 10500; //7000; //6000;
                    RIGHT_FORWARD_SPEED = 0;
                }
                else if (left_value >= BLACK_LEFT && right_value < BLACK_RIGHT){
                    strcpy(display_line[0], "TURN RIGHT ");
                    LEFT_FORWARD_SPEED = 0;
                    RIGHT_FORWARD_SPEED = 11000; //7500; //6500;
                }
                else{
                    strcpy(display_line[0], "CRAP       ");
                    RIGHT_FORWARD_SPEED = 0;
                    LEFT_FORWARD_SPEED = 0; //15000;
                    //Reverse
                    RIGHT_REVERSE_SPEED = 2875 * 5; //15000;
                    LEFT_REVERSE_SPEED = 2450 * 5;

                }
            }
            else {
                strcpy(display_line[0], "Finished!!");
                LEFT_FORWARD_SPEED = 0;
                RIGHT_FORWARD_SPEED = 0;
                //p7_action = OFF_LINE;
                seconds_elapsed = 0;
                p7_action = 7;

            }
            break;
        case 7:
            if(seconds_elapsed <= 5){
                LEFT_FORWARD_SPEED = 5200*2; //5000; //15000;
                //Reverse
                RIGHT_REVERSE_SPEED = 6700*2;//6500; //15000;
                //clockwise();
            }
            else{
                seconds_elapsed = 0;
                p7_action = 8;
            }
            break;
        case 8:
            wheel_Forward(6);
            if(seconds_elapsed >= 4){
                LEFT_FORWARD_SPEED = 0;
                RIGHT_FORWARD_SPEED = 0;

                //p7_action = 9;
            }
            break;
        case 9:
            break;

        case 30:
            LEFT_FORWARD_SPEED = 26500;
            RIGHT_FORWARD_SPEED = 17000;
            seconds_elapsed  = 0;
            p7_action = 32;
            break;
        case 31:
            if(seconds_elapsed >= 3){
                if(left_value <= 100 && right_value <= 100){
                    p7_action = 0;
                }
            }
            break;
        case 32:
            if(seconds_elapsed >= 3){
                if(isBlackLineDetected(left_value, right_value)){
                    strcpy(display_line[0], "BLACK LINE");
                    display_changed = TRUE;
                    wheel_Off();
                    seconds_elapsed = 0;
                    p7_action = 2;
                }
            }
            break;


        default:
            break;

        }

//        if(line_not_done == 0){
//            LEFT_FORWARD_SPEED = 0;
//            RIGHT_FORWARD_SPEED = 0;
//            LEFT_FORWARD_SPEED = 0;
//            RIGHT_REVERSE_SPEED = 0;
//        }

        //        if(run_line_follow == 1){
        //             p7_drive();
        //         }


        //LEFT_REVERSE_SPEED = 12000;
        //RIGHT_REVERSE_SPEED = 12000;

        //        P3OUT |= IOT_EN; // Initial Value = Low / Off

        //
        //        for (i = 0; i < 32; i++){
        //            if(IOT_RING_Rx[i] == '^'){
        //                receive_command = IOT_RING_Rx[i+1];
        //
        //                P6OUT  ^= GRN_LED;
        //            }
        //        }


        //
        //        if(UCA1RXBUF == '^^'){
        //            UCA1TXBUF = 'HELLOOOO! Can you see me?';
        //        }
        //        if(UCA1RXBUF == '^F'){
        //            UCA1TXBUF = '115,200';
        //        }
        //        if(UCA1RXBUF == '^S'){
        //            UCA1TXBUF = '9,600';
        //        }




        //        strcpy(tx_A1buf, "NCSU  #1\r\n");
        //                UCA0IE |= UCTXIE;
        //
        //        if(p9_rx_state == 1){
        ////            strcpy(tx_A1buf, "NCSU  #1\r\n");
        ////            UCA1TXBUF = "A";
        ////            USCI_A1_transmit();
        //
        //        }



        //Project 8
        /*
        switch (IOT_state) {
        case WAITING:
            strcpy(display_line[0], "  WAITING ");
            display_changed = TRUE;
            break;
        case RECEIVED:
            strcpy(display_line[0], " RECEIVED ");
            for (i = rx_A0buf[temp_A0]; i != '\n'; i++){
                if (i >= (LARGE_RING_SIZE)){
                    rx_A0 = BEGINNING; // Circular buffer back to beginning
                }
            }
            strncpy(display_line[3], recieved_message, 10);
            for (j = 0; j < 10; j++){
                if (display_line[3][j] == NULL){
                    display_line[3][j] = ' ';
                }
                if (display_line[3][j] == '\r'){
                    display_line[3][j] = ' ';
                }
                if (display_line[3][j] == '\n'){
                    display_line[3][j] = ' ';
                }
            }
            strcpy(display_line[1], "          ");
            display_changed = TRUE;
            break;
        case TRANSMIT:
            strcpy(display_line[0], " TRANSMIT ");
            strncpy(display_line[1], recieved_message, 10);
            strcpy(display_line[3], "          ");
            for (j = 0; j < 10; j++){
                if (display_line[1][j] == NULL){
                    display_line[1][j] = ' ';
                }
                if (display_line[1][j] == '\r'){
                    display_line[1][j] = ' ';
                }
                if (display_line[1][j] == '\n'){
                    display_line[1][j] = ' ';
                }

            }
            display_changed = TRUE;
            break;
        }
         */
    }


    //------------------------------------------------------------------------------
}
