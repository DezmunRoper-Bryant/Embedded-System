#include "msp430.h"
#include "ports.h"
#include "functions.h"
#include "macros.h"

//#define BLACK_LINE (600)

void wheel_Off(void);
void wheel_Forward(int speed);
void wheel_Reverse(void);
void right_Forward(void);
void left_Forward(void);
void clockwise(void);
void counter_clockwise(void);
void p5_drive(void);
extern volatile int seconds_elapsed;

extern char display_line[4][11];
extern char *display[4];
char pwm_state;
extern char change;
char run_wheels;
extern volatile unsigned char display_changed;

extern volatile int left_value;
extern volatile int right_value;

char state = IDLE;
unsigned int state2;
extern volatile unsigned int CCR0_counter;
extern volatile unsigned int CCR0_counter2;
extern volatile unsigned int CCR0_counter3;
unsigned int first_time = 1;
unsigned int initial = 1;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern char display_line[4][11];
extern volatile unsigned int start;
extern char state_serial;

int p6_action = 0;
extern volatile int p7_action;
extern volatile int p7_action;
//int p7_action = 0;
int black_line = 0;

extern volatile int white_right_value;
extern volatile int white_left_value;

extern volatile char right_valu_string[4];
extern volatile char left_valu_string[4];


extern volatile int black_right_value;
extern volatile int black_left_value;

int blackLineDetected;
//int isBlackLineDetected(int leftValue, int rightValue) {
//    return (leftValue >= 650) && (rightValue >= 650);
//}

//int isBlackLineDetected(int leftValue, int rightValue) {
//    return (leftValue >= black_left_value - 25) && (rightValue >= black_right_value - 25 );
//}
int isBlackLineDetected(int leftValue, int rightValue) {
    return (leftValue >= BLACK_LEFT) && (rightValue >= BLACK_RIGHT);
}


int number_screen = 0;

extern volatile int line_not_done;

//The following 4 lines will ensure that all wheel drive directions are off.
    void wheel_Off(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 0;
        //Reverse
        RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 0;
//        P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
    }

    //To turn on Forward movement:
    void wheel_Forward(int speed){
        //Forwards
        LEFT_FORWARD_SPEED = 2600 * speed; //2500 * speed; //5000 * speed; //10000 * speed;
        RIGHT_FORWARD_SPEED = 2875* speed; //5750 * speed; //11500 * speed ;
        //Reverse
        RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 0;
//        P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
//        P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
    }

    void wheel_Reverse(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 0;
        //Reverse
        RIGHT_REVERSE_SPEED = 10000;
        LEFT_REVERSE_SPEED = 10000;
//        P6OUT |= L_REVERSE; // Set Port pin High [Wheel On]
//        P6OUT |= R_REVERSE; // Set Port pin High [Wheel On]
    }
    void right_Forward(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 0;
        //Reverse
        RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 0;
//        P6OUT |= R_FORWARD; // Set Port pin High [Wheel On]
//        P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
    }
    void left_Forward(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 0;
        //Reverse
        RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 0;
//        P6OUT |= L_FORWARD; // Set Port pin High [Wheel On]
//        P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]

    }
    void clockwise(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 5200; //5000; //15000;
        //Reverse
        RIGHT_REVERSE_SPEED = 6700;//6500; //15000;
        LEFT_REVERSE_SPEED = 0;
//        P6OUT |= L_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~R_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~L_REVERSE; // Set Port pin Low [Wheel Off]
//        P6OUT |= R_REVERSE; // Set Port pin Low [Wheel Off]
    }

    void counter_clockwise(void){
        //Forwards
        RIGHT_FORWARD_SPEED = 5500*2;//18500;//37000;
        LEFT_FORWARD_SPEED = 0;
        //Reverse
        RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 7000*2; // 12500; //25000;
//        P6OUT &= ~L_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT |= R_FORWARD; // Set Port pin Low [Wheel Off]
//        P6OUT |= L_REVERSE; // Set Port pin Low [Wheel Off]
//        P6OUT &= ~R_REVERSE; // Set Port pin Low [Wheel Off]
    }


    char action[15];

    void p5_display_change(action){
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], action);
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
        display_changed = TRUE;
    }

    void p5_drive(void){
        switch(seconds_elapsed){
            case 0:
                wheel_Forward(1);
                p5_display_change(" FORWARDS ");
                break;
            case 1:
                wheel_Off();
                break;
            case 2:
                wheel_Reverse();
                break;
            case 4:
                wheel_Off();
                break;
            case 5:
                wheel_Forward(1);
                break;
            case 6:
                wheel_Off();
                break;
            case 7:
                clockwise();
                break;
            case 10:
                wheel_Off();
                break;
            case 12:
                counter_clockwise();
                break;
            case 15:
                wheel_Off();
                break;
        }
    }


    void p6_drive(void){
        //p6_action = 0;
        switch(p6_action){
        case 0:

            P1OUT |= RED_LED; // Initial Value = Low / Off
            wheel_Forward(2);
            p6_action = 1;
            break;
        case 1:
            if(isBlackLineDetected(left_value, right_value)){
                strcpy(display_line[0], "BLACK LINE");
                display_changed = TRUE;
                wheel_Off();
                seconds_elapsed = 0;
                p6_action = 2;
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
                p6_action = 3;
            }
            break;
        case 3:
            clockwise();
            p6_action = 4;
            break;
        case 4:
            if(isBlackLineDetected(left_value, right_value)){
                strcpy(display_line[0], "BLACK LINE");
                display_changed = TRUE;
                wheel_Off();
                blackLineDetected = TRUE;
                p6_action = 5;
            }
            else{
                strcpy(display_line[0], "ALIGNING  ");
                //strcpy(display_line[0], "NOT BLACK ");
                display_changed = TRUE;
            }
//        case 7:
//            wheel_Reverse();
//
//            break;
        default:
            break;

        }
    }

//
//    void p6_drive(void){
//        p6_action = 0;
//        switch(p6_action){
//        case 0:
//            P2OUT |= IR_LED;
//            P1OUT |= RED_LED; // Initial Value = Low / Off
//            wheel_Forward();
//            p6_action = 1;
//            break;
//        case 1:
//            while(black_line != 1){
//                if((left_value >= 400)&&(right_value >= 400)){
//                    strcpy(display_line[0], "BLACK LINE");
//                    display_changed = TRUE;
//                    wheel_Off();
//                    p6_action = 2;
//                    black_line = 1;
//                }
//                else{
//                    strcpy(display_line[0], "NOT BLACK ");
//                    display_changed = TRUE;
//                }
//             }
//             black_line = 0;
//             break;
//        case 2:
//            clockwise();
//            while(black_line != 1){
//                if((left_value >= 400)&&(right_value >= 400)){
//                    strcpy(display_line[0], "BLACK LINE");
//                    display_changed = TRUE;
//                    wheel_Off();
//                    black_line = 1;
//
//                }
//                else{
//                    strcpy(display_line[0], "NOT BLACK ");
//                    display_changed = TRUE;
//                }
//
//            }
//        }
//    }

//    void p6_drive(void){
//        if((left_value > 5)&&(right_value > 5)){
//            strcpy(display_line[0], "BLACK LINE");
//            display_changed = TRUE;
//
//        }
//        else{
//            strcpy(display_line[0], "NOT BLACK");
//            display_changed = TRUE;
//
//        }
//    }




    /*void go_forward(int time) {
        if (initial) {
            CCR0_counter3 = 0;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            RIGHT_FORWARD_SPEED = 9000;
            LEFT_FORWARD_SPEED = 10000;
            break;
        default:
            if (CCR0_counter3 == (time * 5) + 2) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
        // Increment the counter during movement
        CCR0_counter3++;
    }*/

//
//    void p7_drive(void){
//        if (initial) {
//            CCR0_counter3 = 0;
//            initial = 0;
//        }
//        ADCCTL0 |= ADCSC;
//        //p7_action = 0;
//        //seconds_elapsed = 0;
//
//
//        switch(p7_action){
//        case 0:
//            P1OUT |= RED_LED; // Initial Value = Low / Off
//            wheel_Forward(3);
//            p7_action = 1;
//            if(seconds_elapsed)
//                break;
//        case 1:
//            if(isBlackLineDetected(left_value, right_value)){
//                strcpy(display_line[0], "BLACK LINE");
//                display_changed = TRUE;
//                wheel_Off();
//                seconds_elapsed = 0;
//                p7_action = 2;
//            }
//            else{
//                strcpy(display_line[0], "FORWARDS   ");
//                //strcpy(display_line[0], "NOT BLACK ");
//                display_changed = TRUE;
//            }
//            break;
//        case 2:
//            strcpy(display_line[0], "WAITING...");
//            if(seconds_elapsed >= 3){
//            //            if(seconds_elapsed >= 3){
//                //p6_action = 7;
//                p7_action = 3;
//            }
//            break;
//        case 3:
//            clockwise();
//            p7_action = 4;
//            break;
//        case 4:
//            if(isBlackLineDetected(left_value, right_value)){
//                strcpy(display_line[0], "BLACK LINE");
//                display_changed = TRUE;
//                wheel_Off();
//                blackLineDetected = TRUE;
//                seconds_elapsed = 0;
//                p7_action = 5;
//            }
//            else{
//                strcpy(display_line[0], "ALIGNING  ");
//                //strcpy(display_line[0], "NOT BLACK ");
//                display_changed = TRUE;
//            }
//            break;
//        case 5:
//            strcpy(display_line[0], "WAITING...");
//            if(seconds_elapsed >= 3){
//                p7_action = 6;
//            }
//            break;
//        case 6:
//            strcpy(display_line[0], "FOLLOWING ");
//            display_changed = TRUE;
//            //            LEFT_FORWARD_SPEED = 0;
//            //            RIGHT_FORWARD_SPEED = 0;
//            if (seconds_elapsed < 40){
//                if (left_value >= BLACK_LINE && right_value >= BLACK_LINE){
//                    strcpy(display_line[0], "STRAIGHT  ");
//                    wheel_Forward(3);
//                    //wheel_Forward(2);
//                }
//                else if (left_value < BLACK_LINE && right_value >= BLACK_LINE){
//                    strcpy(display_line[0], "TURN LEFT ");
//                    LEFT_FORWARD_SPEED = 7000; //6000;
//                    RIGHT_FORWARD_SPEED = 0;
//                }
//                else if (left_value >= BLACK_LINE && right_value < BLACK_LINE){
//                    strcpy(display_line[0], "TURN RIGHT ");
//                    LEFT_FORWARD_SPEED = 0;
//                    RIGHT_FORWARD_SPEED = 7500; //6500;
//                }
//                else{
//                    strcpy(display_line[0], "CRAP       ");
//                    RIGHT_FORWARD_SPEED = 0;
//                    LEFT_FORWARD_SPEED = 0; //15000;
//                    //Reverse
//                    RIGHT_REVERSE_SPEED = 2875 * 4; //15000;
//                    LEFT_REVERSE_SPEED = 2450 * 4;
//
//                }
//            }
//            else {
//                strcpy(display_line[0], "TIMES UP  ");
//                LEFT_FORWARD_SPEED = 0;
//                RIGHT_FORWARD_SPEED = 0;
//                //p7_action = OFF_LINE;
//                seconds_elapsed = 0;
//                p7_action = 7;
//
//            }
//            break;
//        case 7:
//            if(seconds_elapsed <= 2){
//                clockwise();
//            }
//            else{
//                seconds_elapsed = 0;
//                p7_action = 8;
//            }
//            break;
//        case 8:
//            wheel_Forward(3);
//            if(seconds_elapsed >= 2){
//                LEFT_FORWARD_SPEED = 0;
//                RIGHT_FORWARD_SPEED = 0;
//
//                //p7_action = 9;
//            }
//            break;
//        case 9:
//            break;
//        default:
//            break;
//
//        }
//
//        if (CCR0_counter3 == (50)) {
//            //wheel_Off();
//            state_serial = CLEAR;
//            initial = 1;
//        }
//    }



//    void go_forward(int time) {
//        if (initial) {
//            CCR0_counter3 = 0;
//            initial = 0;
//        }
//        switch (CCR0_counter3) {
//        case 2:
//            RIGHT_FORWARD_SPEED = 11000;
//            LEFT_FORWARD_SPEED = 11000;
//            break;
//        default:
//            if (CCR0_counter3 == (time * 10) + 2) {
//                wheel_Off();
//                state_serial = CLEAR;
//                initial = 1;
//            }
//            break;
//        }
//    }
//
//
//    void go_back(int time) {
//        if (initial) {
//            CCR0_counter3 = 0;
//            initial = 0;
//        }
//        switch (CCR0_counter3) {
//        case 2:
//            RIGHT_REVERSE_SPEED = 11000;
//            LEFT_REVERSE_SPEED = 11000;
//            break;
//        default:
//            if (CCR0_counter3 == (time * 10) + 2) {
//                wheel_Off();
//                state_serial = CLEAR;
//                initial = 1;
//            }
//            break;
//        }
//    }
//
    void go_right(int time) {
        if (initial) {
            CCR0_counter3 = 0;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            RIGHT_FORWARD_SPEED = 11000;
            LEFT_FORWARD_SPEED = 14000;
            break;
        default:
            if (CCR0_counter3 == (4)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }

    void go_left(int time) {
        if (initial) {
            CCR0_counter3 = 0;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            RIGHT_FORWARD_SPEED = 20000;
            LEFT_FORWARD_SPEED = 9000;
            break;
        default:
            if (CCR0_counter3 == (4)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }

    void display_screen_up(int time){
        if (initial) {
            CCR0_counter3 = 0;
            number_screen += 1;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            if (number_screen == 1) {
                strcpy(display_line[0], "Arrived 01");
            } else if (number_screen == 2) {
                strcpy(display_line[0], "Arrived 02");
            } else if (number_screen == 3) {
                strcpy(display_line[0], "Arrived 03");
            } else if (number_screen == 4) {
                strcpy(display_line[0], "Arrived 04");
            } else if (number_screen == 5) {
                strcpy(display_line[0], "Arrived 05");
            } else if (number_screen == 6) {
                strcpy(display_line[0], "Arrived 06");
            } else if (number_screen == 7) {
                strcpy(display_line[0], "Arrived 07");
            } else if (number_screen == 8) {
                strcpy(display_line[0], "Arrived 08");
            }
            display_changed = TRUE;

            strcpy(display_line[1], "  DEZZY   ");
            strcpy(display_line[2], "  MOBILE  ");
            strcpy(display_line[3], "----------");
            display_changed = TRUE;
            break;
        default:
            if (CCR0_counter3 == (30)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }

    void display_screen_down(int time){
        if (initial) {
            CCR0_counter3 = 0;
            number_screen -= 1;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            if (number_screen == 1) {
                strcpy(display_line[0], "Arrived 01");
            } else if (number_screen == 2) {
                strcpy(display_line[0], "Arrived 02");
            } else if (number_screen == 3) {
                strcpy(display_line[0], "Arrived 03");
            } else if (number_screen == 4) {
                strcpy(display_line[0], "Arrived 04");
            } else if (number_screen == 5) {
                strcpy(display_line[0], "Arrived 05");
            } else if (number_screen == 6) {
                strcpy(display_line[0], "Arrived 06");
            } else if (number_screen == 7) {
                strcpy(display_line[0], "Arrived 07");
            } else if (number_screen == 8) {
                strcpy(display_line[0], "Arrived 08");
            }
            display_changed = TRUE;
            strcpy(display_line[1], "  DEZZY   ");
            strcpy(display_line[2], "  MOBILE  ");
            strcpy(display_line[3], "----------");
            display_changed = TRUE;
            break;
        default:
            if (CCR0_counter3 == (30)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }







    void go_CW(int time) {
        if (initial) {
            CCR0_counter3 = 0;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            RIGHT_FORWARD_SPEED = 0;
            LEFT_FORWARD_SPEED = 5200*2; //5000; //15000;
            //Reverse
            RIGHT_REVERSE_SPEED = 6700*2;//6500; //15000;
            LEFT_REVERSE_SPEED = 0;
            break;
        default:
            if (CCR0_counter3 == (4)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }

    void go_CCW(int time) {
        if (initial) {
            CCR0_counter3 = 0;
            initial = 0;
        }
        switch (CCR0_counter3) {
        case 2:
            RIGHT_FORWARD_SPEED = 5000*2;//18500;//37000;
            LEFT_FORWARD_SPEED = 0;
            //Reverse
            RIGHT_REVERSE_SPEED = 0;
            LEFT_REVERSE_SPEED = 6500*2; // 12500; //25000;
            break;
        default:
            if (CCR0_counter3 == (4)) {
                wheel_Off();
                state_serial = CLEAR;
                initial = 1;
            }
            break;
        }
    }




    void go_forward(int speed) {
            if (initial) {
                CCR0_counter3 = 0;
                initial = 0;
            }
            switch (CCR0_counter3) {
            case 2:
                LEFT_FORWARD_SPEED = 2600 * 8;
                RIGHT_FORWARD_SPEED = 2875 * 8;
                RIGHT_REVERSE_SPEED = 0;
                LEFT_REVERSE_SPEED = 0;
                break;
            default:
                if (CCR0_counter3 == (4)) {
                    //wheel_Off();
                    state_serial = CLEAR;
                    initial = 1;
                }
                break;
            }
        }


        void go_back(int time) {
            if (initial) {
                CCR0_counter3 = 0;
                initial = 0;
            }
            switch (CCR0_counter3) {
            case 2:
                RIGHT_FORWARD_SPEED = 0;
                LEFT_FORWARD_SPEED = 0;
                RIGHT_REVERSE_SPEED = 11000;
                LEFT_REVERSE_SPEED = 11000;
                break;
            default:
                if (CCR0_counter3 == (4)) {
                    //wheel_Off();
                    state_serial = CLEAR;
                    initial = 1;
                }
                break;
            }
        }

//        void go_right(int time) {
//            if (initial) {
//                CCR0_counter3 = 0;
//                initial = 0;
//            }
//            switch (CCR0_counter3) {
//            case 2:
//                RIGHT_FORWARD_SPEED = 0;
//                LEFT_REVERSE_SPEED = 0;
//                LEFT_FORWARD_SPEED = 11000;
//                RIGHT_REVERSE_SPEED = 11000;
//                break;
//            case 4:
//                LEFT_FORWARD_SPEED = 2600 * 6;
//                RIGHT_FORWARD_SPEED = 2875 * 6;
//                RIGHT_REVERSE_SPEED = 0;
//                LEFT_REVERSE_SPEED = 0;
//                break;
//            default:
//                if (CCR0_counter3 == (6)) {
//                    //wheel_Off();
//                    state_serial = CLEAR;
//                    initial = 1;
//                }
//                break;
//            }
//        }
//
//        void go_left(int time) {
//            if (initial) {
//                CCR0_counter3 = 0;
//                initial = 0;
//            }
//            switch (CCR0_counter3) {
//            case 2:
//                LEFT_FORWARD_SPEED = 0;
//                RIGHT_REVERSE_SPEED = 0;
//                RIGHT_FORWARD_SPEED = 11000;
//                LEFT_REVERSE_SPEED = 11000;
//                break;
//            case 4:
//                LEFT_FORWARD_SPEED = 2600 * 6;
//                RIGHT_FORWARD_SPEED = 2875 * 6;
//                RIGHT_REVERSE_SPEED = 0;
//                LEFT_REVERSE_SPEED = 0;
//                break;
//            default:
//                if (CCR0_counter3 == (6)) {
//                    //wheel_Off();
//                    state_serial = CLEAR;
//                    initial = 1;
//                }
//                break;
//            }
//        }
//
//        void black_line_detection(int time){
//            if (initial) {
//                CCR0_counter3 = 0;
//
//                initial = 0;
//            }
//


//            switch (CCR0_counter3) {
//            case 2:
//
//                break;
//            default:
//                if (CCR0_counter3 == (6)) {
//                    //wheel_Off();
//                    state_serial = CLEAR;
//                    initial = 1;
//                }
//                break;
//
//            }

//        }



        void no_go(int time){
            if (initial) {
                CCR0_counter3 = 0;
                initial = 0;
            }
            switch (CCR0_counter3) {
            case 2:
                LEFT_FORWARD_SPEED = 0;
                RIGHT_REVERSE_SPEED = 0;
                RIGHT_FORWARD_SPEED = 0;
                LEFT_REVERSE_SPEED = 0;
                break;
            default:
                if (CCR0_counter3 == (6)) {
                    state_serial = CLEAR;
                    initial = 1;
                }
                break;
            }
        }

