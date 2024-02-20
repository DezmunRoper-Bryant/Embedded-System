#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>
#include  "ports.h"

// Global Variables
volatile extern char pb_index; // Index for process_buffer
extern volatile unsigned int CCR0_counter2;
extern volatile unsigned char display_changed;
extern volatile unsigned int usb_rx_wr;
volatile extern char pb_index; // Index for process_buffer
//char command[4][4];
char command[4][7];
extern char display_line[4][11];
unsigned int command_it;
extern volatile unsigned int take_command;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern unsigned int direct_iot;
unsigned int command_in;
unsigned int cmnd_line;
unsigned int cmnd_index;
unsigned int cmnd_start_line;
unsigned int cmnd_num;
unsigned int cmnd_execute;
extern volatile unsigned int cmnd_start;
extern volatile unsigned int cmnd_stop;
extern volatile unsigned int done_command;
char state_serial;
unsigned int exec_cmnd;
unsigned int once = 1;
unsigned int single_run1 = 1;
unsigned int single_run2 = 1;
unsigned int single_run3 = 1;
unsigned int single_run4 = 1;
unsigned int run;
char configure_cmnd1[13] = "AT+CIPMUX=1\r\n";
char configure_cmnd2[21] = "AT+CIPSERVER=1,7883\r\n";
char configure_cmnd3[10] = "AT+CIFSR\r\n";
//char configure_cmnd4[] = ;
//char configure_cmnd5[] = ;
//char configure_cmnd6[] = ;

int i;
int j;

int thousands;
int hundreds;
int tens;
int ones;

int recieve_command;

extern volatile int line_not_done;
extern volatile int run_line_follow;

extern volatile int p7_action;

int first_command;
extern volatile int demo_seconds;

void load_command(void) {
    if(first_command){
        demo_seconds = 0;
        first_command = FALSE;
    }
    if (done_command) {
        done_command = 0;
        cmnd_index = 0;
        cmnd_num = 0;
        cmnd_line = 0;
        exec_cmnd = 0;
        state_serial = DECODE;
        for (i = cmnd_start; i < cmnd_stop; i++) {
            if (IOT_2_PC[i] == '^') {
                command[cmnd_line][cmnd_index] = IOT_2_PC[++i];
                if ((IOT_2_PC[i] != 'f') && (IOT_2_PC[i] != 's') && (IOT_2_PC[i] != '^')) {
                    command[cmnd_line][++cmnd_index] = IOT_2_PC[++i];
                }
                cmnd_num++;
                cmnd_line++;
                cmnd_index = 0;
            }
        }
    }
}

void perform_command(void) {
    if (cmnd_num) { //cmnd_execute
        switch(state_serial) {
        case IDLE:
            break;

        case DECODE:
            switch (command[exec_cmnd][0]) {
            case '^': // check
                strcpy(display_line[0], "   HERE   ");
                display_changed = TRUE;
                state_serial = CLEAR;
                break;
            case 'f': //fast
                Init_Serial_UCA1('f');
                Init_Serial_UCA0('f');
                state_serial = CLEAR;
                break;
            case 's': //slow
                Init_Serial_UCA1('s');
                Init_Serial_UCA0('s');
                state_serial = CLEAR;
                break;
            case 'F': //go forward state
                go_forward(command[exec_cmnd][1] - 48);
                break;
            case 'B':
                go_back(command[exec_cmnd][1] - 48);
                break;
            case 'R':
                go_right(command[exec_cmnd][1] - 48);
                break;
            case 'L':
                go_left(command[exec_cmnd][1] - 48);
                break;
            case 'X':
                run_line_follow = 1;
                line_not_done = 1;
                p7_action = 30;
                state_serial = CLEAR;
                //p7_drive();
                break;
            case 'Z':
                no_go(command[exec_cmnd][1] - 48);
                break;
            case 'N':
                line_not_done = 0;
                state_serial = CLEAR;
                break;
            case 'c':
                go_CW(command[exec_cmnd][1] - 48);
                break;
            case 'C':
                go_CCW(command[exec_cmnd][1] - 48);
                break;
            case 'D':
                display_screen_up(command[exec_cmnd][1] - 48);
                break;
            case 'd':
                display_screen_down(command[exec_cmnd][1] - 48);
                break;
            default:
                strcpy(display_line[0], "  ERROR!  ");
                display_changed = TRUE;
            }
            break;

            case CLEAR:
                for (i = 0; i < 4; i++) {
                    command[exec_cmnd][i] = '\0';
                }
                cmnd_num--;
                exec_cmnd++;
                if (cmnd_num) {
                    state_serial = DECODE;
                }
                else {
                    state_serial = IDLE;
                }
                break;
        }
    }
}


//char - 48 = number
//
//        _ _ _ _
//
//        t h t o
//
//        ((t)-48)*1000
//
//        ((command[exec_cmnd][1] - 48)*1000) + ((command[exec_cmnd][2] - 48)*100) + ((command[exec_cmnd][3] - 48)*10) + ((command[exec_cmnd][4] - 48)*1)


void configure_IOT (void) {
    if (once) {
        once = 0;
        CCR0_counter2 = 0;
        run = 1;
    }
    if (run) {
        if (CCR0_counter2 == 40) {
            if (single_run1) {
                for (i = 0; i < 13; i++) {
                    PC_2_IOT[i] = configure_cmnd1[i];
                    single_run1 = 0;
                }
                usb_rx_wr = 13;
                UCA0IE |= UCTXIE;
            }
        }
        else if (CCR0_counter2 == 60) {
            if (single_run2) {
                for (i = 13; i < 34; i++) {
                    PC_2_IOT[i] = configure_cmnd2[i-13];
                    single_run2 = 0;
                }
                usb_rx_wr = 34;
                UCA0IE |= UCTXIE;
            }
        }
        else if (CCR0_counter2 == 80) {
            if (single_run3) {
                for (i = 34; i < 44; i++) {
                    PC_2_IOT[i] = configure_cmnd3[i-34];
                    single_run3 = 0;
                }
                usb_rx_wr = 44;
                UCA0IE |= UCTXIE;
            }
        }
        else if (CCR0_counter2 == 100) {
            for (i = 0; i < SMALL_RING_SIZE; i++) {
                if ((IOT_2_PC[i] == 'S') && (IOT_2_PC[i + 1] == 'T') && (IOT_2_PC[i + 2] == 'A') && (IOT_2_PC[i + 3] == 'I') && (IOT_2_PC[i + 4] == 'P')) {
                    for (j = 0; j < 8; j++) {
                        display_line[2][j] = IOT_2_PC[j + i + 7];
                        display_changed = TRUE;
                    }
                    for (j = 0; j < 6; j++) {
                        display_line[3][j] = IOT_2_PC[j + i + 14];
                        display_changed = TRUE;

                    }

                }
            }
        }
        else if (CCR0_counter2 == 120) {
            if (single_run4) {
                strcpy(display_line[0], " IOT READY ");
                display_changed = TRUE;
                single_run4 = 0;
            }
        }
        else if (CCR0_counter2 == 130){
            first_command = TRUE;
        }


        else if (CCR0_counter2 > 130) {
//            first_command = TRUE;
            recieve_command = TRUE;
            run = 0;
        }
    }
}
