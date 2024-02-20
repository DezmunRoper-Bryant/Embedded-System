#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"


unsigned char display_changed;
char one_time;
unsigned int Time_Sequence;
extern unsigned int cycle_time;
unsigned int delay_start;
unsigned int left_motor_count;
unsigned int right_motor_count;
unsigned int segment_count = 0 ;
unsigned int p4_state = WAIT; ///////
extern unsigned int time_change;

extern volatile int event;

extern char display_line[4][11];
extern char *display[4];

void Run_Straight(void);
void wait_case(void);
void start_case(void);
void end_case(void);
void Forward_On(void);
void Forward_Off(void);
void run_case(void);
void Forward_Move(void);
void Run_Eight(void);

void Forward_On(void){};
void Forward_Move(void){};
void Forward_Off(void){};

void run_case_Circle_Left(void);
void run_case_Circle_Right(void);

int sides = 1;
int edge = 1;
int wait = 0;
int turn = 0;


int left_count = 2;
int right_count = 1;
int eights = 0;





void Run_Eight(void){
    strcpy(display_line[0], "   Eight   ");
      strcpy(display_line[1], " 8 ");
      strcpy(display_line[2], "    ");
      strcpy(display_line[3], "    ");
      display_changed = TRUE;
    switch(p4_state){

     case WAIT: // Begin
         wait_case();
         break; //
     case START: // Begin
         start_case();
         break; //
     case RUN: // Run
         run_case_Circle_Left();
         break;
     case RIGHT:
         run_case_Circle_Right();
         break; //
     case END: // End
         end_case();
         int sides = 1;
         int edge = 1;
         event = TRIANGLE;
         break; //
     default: break;
     }
}

void Run_Straight(void){
 switch(p4_state){
     case WAIT: // Begin
         wait_case();
         break; //
     case START: // Begin
         start_case();
         break; //
     case RUN: // Run
         run_Sharp();
         //run_case();
         break;
     case END: // End
         end_case();
         break; //
     default: break;
     }
}

void Run_Triangle(void){
    strcpy(display_line[0], "   Triangle   ");
       strcpy(display_line[1], " 3 ");
       strcpy(display_line[2], "    ");
       strcpy(display_line[3], "    ");

    switch(p4_state){
     case WAIT: // Begin
         wait_case();
         break; //
     case START: // Begin
         start_case();
         break; //
     case RUN: // Run
         run_case();
         break;
     case SHARP:
         run_Sharp();
         break;
     case END: // End
         end_case();
         event = CIRCLE;
         break; //
     default: break;
     }
}





void Run_Circle(void){
    strcpy(display_line[0], "   Circle   ");
       strcpy(display_line[1], " 0 ");
       strcpy(display_line[2], "    ");
       strcpy(display_line[3], "    ");
    switch(p4_state){

     case WAIT: // Begin
         wait_case();
         break; //
     case START: // Begin
         start_case();
         break; //
     case RUN: // Run
         run_case_Circle_Right1();
         break; //
     case END: // End
         end_case();
         break; //
     default: break;
 }
}




void wait_case(void){
 if(time_change){
 time_change = 0;
 if(delay_start++ >= WAITING2START){
 delay_start = 0;
 p4_state = START;
 }
 }
}


void start_case(void){
 cycle_time = 0;
 right_motor_count = 0;
 left_motor_count = 0;
 wheel_Forward();
 //Forward_On();
 segment_count = 0;
 p4_state = RUN;
}


void run_case(void){
 if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE_2){
     if(right_motor_count++ >= RIGHT_COUNT_TIME){
         P6OUT &= ~R_FORWARD;
     }
     if(left_motor_count++ >= LEFT_COUNT_TIME){
         P6OUT &= ~L_FORWARD;
     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         wheel_Forward();
         //Forward_Move();
     }
 }else{
     sides++;
     wait = 1;
     turn = 1;
     if(sides < 7){
         p4_state = SHARP;
     }
     else{
         p4_state = END;
     }
 }
 }
}


void run_Sharp(void){
    if(time_change){
 time_change = 0;
// P6OUT &= ~R_FORWARD;
// P6OUT &= ~L_FORWARD;
 if(segment_count <= 30){
//     if(right_motor_count++ >= RIGHT_COUNT_TIME_Sharp){
//         P6OUT &= ~R_FORWARD;
//     }
//     if(left_motor_count++ >= LEFT_COUNT_TIME_Sharp){
//         P6OUT &= ~L_FORWARD;
//     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         right_Forward();
         //Forward_Move();
     }
 }else{
     segment_count = 0;
     if(sides < 7){
         p4_state = RUN;
     }
     else{
     p4_state = END;}
;
 }
 }
}

void run_case_Circle_Right(void){
    if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE){
     if(right_motor_count++ >= RIGHT_COUNT_TIME_RightTurn){
         P6OUT &= ~R_FORWARD;
     }
     if(left_motor_count++ >= LEFT_COUNT_TIME_RightTurn){
         P6OUT &= ~L_FORWARD;
     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         wheel_Forward();
         //Forward_Move();
     }
 }else{
     segment_count = 0;
     eights ++;
     if(eights < 2){
         p4_state = RUN;
     }
     else{
         p4_state = END;
     }

 }
 }
}

void run_case_Circle_Left1(void){
    if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE){
     if(right_motor_count++ >= RIGHT_COUNT_TIME_LeftTurn){
         P6OUT &= ~R_FORWARD;
     }
     if(left_motor_count++ >= LEFT_COUNT_TIME_LeftTurn){
         P6OUT &= ~L_FORWARD;
     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         wheel_Forward();
         //Forward_Move();
     }
 }else{
     segment_count = 0;
 p4_state = END;
 }
 }
}


void run_case_Circle_Right1(void){
    if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE_xxx){
     if(right_motor_count++ >= RIGHT_COUNT_TIME_RightTurn){
         P6OUT &= ~R_FORWARD;
     }
     if(left_motor_count++ >= LEFT_COUNT_TIME_RightTurn){
         P6OUT &= ~L_FORWARD;
     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         wheel_Forward();
         //Forward_Move();
     }
 }else{
     segment_count = 0;
 p4_state = END;
 }
 }
}


void run_case_Circle_Left(void){
    if(time_change){
 time_change = 0;
 if(segment_count <= TRAVEL_DISTANCE){
     if(right_motor_count++ >= RIGHT_COUNT_TIME_LeftTurn){
         P6OUT &= ~R_FORWARD;
     }
     if(left_motor_count++ >= LEFT_COUNT_TIME_LeftTurn){
         P6OUT &= ~L_FORWARD;
     }
     if(cycle_time >= WHEEL_COUNT_TIME){
         cycle_time = 0;
         right_motor_count = 0;
         left_motor_count = 0;
         segment_count++; //segment count i being incremented
         wheel_Forward();
         //Forward_Move();
     }
 }else{
     segment_count = 0;
 p4_state = RIGHT;
 }
 }
}


void end_case(void){
  wheel_Off();
  //Forward_Off();
 p4_state = WAIT;
 event = NONE;
}




