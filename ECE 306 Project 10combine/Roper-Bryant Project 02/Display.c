extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

#include "macros.h"
#include "msp430.h"
#include "functions.h"
#include "macros.h"



void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
