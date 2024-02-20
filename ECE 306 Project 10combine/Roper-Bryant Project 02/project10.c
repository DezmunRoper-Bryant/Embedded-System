#include "msp430.h"
#include "ports.h"
#include "functions.h"
#include "macros.h"

extern volatile int left_value;
extern volatile int right_value;

int white_right_value;
int white_left_value;
int black_right_value;
int black_left_value;

char right_valu_string[4];
char left_valu_string[4];
char right_valu_string_bl[4];
char left_valu_string_bl[4];

int cal_screen;

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;


void NumtoBCD_right(int hex_value, char state){
    int value = 0;
    char valuestr[4] = '0000';
    int i ;

//    for(i=0; i < 4; i++) {
//        valuestr[i] = '0';
//    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        valuestr[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        valuestr[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        valuestr[2] = 0x30 + value;
    }
    valuestr[3] = 0x30 + hex_value;

    if(state == 'R'){
        strcpy(right_valu_string , valuestr);
    }
    else if(state == 'L'){
        strcpy(left_valu_string , valuestr);
    }
}


void NumtoBCD_black(int hex_value, char state){
    int value = 0;
    char valuestr[4] = '0000';
    int i ;

//    for(i=0; i < 4; i++) {
//        valuestr[i] = '0';
//    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        valuestr[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        valuestr[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        valuestr[2] = 0x30 + value;
    }
    valuestr[3] = 0x30 + hex_value;
    if(state == 'r'){
        strcpy(right_valu_string_bl , valuestr);
    }
    else if(state == 'l'){
        strcpy(left_valu_string_bl , valuestr);
    }

}

void NumtoBCD_left(int hex_value){
    int value;
    int i ;

    for(i=0; i < 4; i++) {
        left_valu_string[i] = '0';
    }
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        left_valu_string[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        left_valu_string[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        left_valu_string[2] = 0x30 + value;
    }
    left_valu_string[3] = 0x30 + hex_value;
}


void adc_line_10(char line, char right[4], char left[4]){
//-------------------------------------------------------------
    int i;
    unsigned int real_line;
    real_line = line - 1;
//    strcpy(display_line[1], "Thumb");
//    strcpy(display_line[2], "Right");
//    strcpy(display_line[3], "Left");
    for(i=0; i < 4; i++) {
        strcpy(display_line[0], "          ");
        display_line[real_line][0] = 'R';
        display_line[real_line][1] = ':';
        display_line[real_line][i + 2] = right[i];
        display_changed = TRUE;
        update_display = TRUE;
    }
    for(i=0; i < 4; i++) {
        display_line[real_line + 1][0] = 'L';
        display_line[real_line + 1][1] = ':';
        display_line[real_line + 1][i + 2] = left[i];
        display_changed = TRUE;
        update_display = TRUE;
    }
}
//
//void adc_line_10(char line, char location, char value[4]){
////-------------------------------------------------------------
//    int i;
//    unsigned int real_line;
//    real_line = line - 1;
////    strcpy(display_line[1], "Thumb");
////    strcpy(display_line[2], "Right");
////    strcpy(display_line[3], "Left");
//    for(i=0; i < 4; i++) {
//        display_line[real_line][0] = 'R';
//        display_line[real_line][1] = ':';
//        display_line[real_line][i+location] = value[i];
//        display_changed = TRUE;
//        update_display = TRUE;
//    }
//}
//
//void ADC_calibration(void){
//    //Display values
//    //First will be what white looks like
//
//    switch(cal_screen){
//    case 0:
//        strcpy(display_line[0], "White?    ");
//        if (!(P4IN & SW1)){
//            white_right_value = right_value;
//            white_left_value = left_value;
//            NumtoBCD_left(left_value);
//            NumtoBCD_right(right_value);
//            cal_screen = 1;
//        }
//        break;
//    case 1:
//        strcpy(display_line[0], "Selections");
//        strcpy(display_line[0], right_valu_string);
//        strcpy(display_line[0], left_valu_string);
//        strcpy(display_line[0], "Okay?");
//        break;
//    }
//
//    //Second will be what black line looks like
//
//}
