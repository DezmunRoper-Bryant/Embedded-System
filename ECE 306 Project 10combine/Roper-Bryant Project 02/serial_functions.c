#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>
#include  "ports.h"


// Global Variables
volatile extern char pb_index; // Index for process_buffer
int p9_rx_state = OFF;
volatile extern char tx_A1buf[LARGE_RING_SIZE];

//volatile unsigned int cmnd_start;
//volatile unsigned int cmnd_stop;
//volatile unsigned int take_command = 0;
//volatile unsigned int done_command = 0;


volatile unsigned int direct_iot;
volatile unsigned int new_iot;


void Init_Serial_UCA1(char speed){
    //------------------------------------------------------------------------------
    // TX error (%) RX error (%)
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
    //------------------------------------------------------------------------------
    // Configure eUSCI_A0 for UART mode
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN; // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
    if (speed == 's') {
        UCA1BRW = 4 ; // set to 4 for 115,200 baud
        UCA1MCTLW = 0x5551 ;
    }
    else if (speed == 'f') {
        UCA1BRW = 17 ; // set to 17 for 460800
        UCA1MCTLW = 0x4A00 ; // set to 0x4A00 for 460800
    }
    UCA1CTLW0 &= ~UCSWRST ; // release from reset
    UCA1TXBUF = 0x00; // Prime the Pump
    UCA1IE |= UCRXIE; // Enable RX interrupt
    //------------------------------------------------------------------------------
}

void Init_Serial_UCA0(char speed){
    //------------------------------------------------------------------------------
    // TX error (%) RX error (%)
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
    //------------------------------------------------------------------------------
    // Configure eUSCI_A0 for UART mode
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN; // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
    if (speed == 's') {
        UCA0BRW = 4 ; // 115,200 baud
        UCA0MCTLW = 0x5551 ;
    }
    else if (speed == 'f') {
        UCA0BRW = 17 ; // set to 17 for 460800
        UCA0MCTLW = 0x4A00 ; // set to 0x4A00 for 460800
    }
    UCA0CTLW0 &= ~UCSWRST ; // release from reset
    UCA0TXBUF = 0x00; // Prime the Pump
    UCA0IE |= UCRXIE; // Enable RX interrupt
    //------------------------------------------------------------------------------
}

void USCI_A1_transmit(void){ // Transmit Function for USCI_A0
    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    pb_index = 0; // Set Array index to first location
    UCA1IE |= UCTXIE; // Enable TX interrupt
}

void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    pb_index = 0; // Set Array index to first location
    UCA0IE |= UCTXIE; // Enable TX interrupt
}

void out_character(char character){
    //------------------------------------------------------------------------------
    // The while loop will stall as long as the Flag is not set [port is busy]
    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = character;
    //------------------------------------------------------------------------------
}


//
//void Init_Serial_UCA0(void){
//    //------------------------------------------------------------------------------
//    // TX error (%) RX error (%)
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
//    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
//    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
//    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//    //------------------------------------------------------------------------------
//    // Configure eUSCI_A0 for UART mode
//    UCA0CTLW0 = 0;
//    UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
//    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
//    UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
//    UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
//    UCA0CTLW0 &= ~UCPEN; // No Parity
//    UCA0CTLW0 &= ~UCSYNC;
//    UCA0CTLW0 &= ~UC7BIT;
//    UCA0CTLW0 |= UCMODE_0;
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // UCA?MCTLW = UCSx + UCFx + UCOS16
//    UCA0BRW = 4 ; // 115,200 baud
//    UCA0MCTLW = 0x5551 ;
//
//    UCA0CTLW0 &= ~UCSWRST ; // release from reset
//    UCA0IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
//    direct_iot = 0; // Set Array index to first location
//    UCA0IE |= UCTXIE; // Enable TX interrupt
//}
//
//
//void Init_Serial_UCA1(void){
//    // Configure eUSCI_A1 for UART mode
//    UCA1CTLW0 = 0;
//    UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
//    UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
//    UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
//    UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
//    UCA1CTLW0 &= ~UCPEN; // No Parity
//    UCA1CTLW0 &= ~UCSYNC;
//    UCA1CTLW0 &= ~UC7BIT;
//    UCA1CTLW0 |= UCMODE_0;
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // UCA?MCTLW = UCSx + UCFx + UCOS16
//    UCA1BRW = 4 ; // 115,200 baud
//    UCA1MCTLW = 0x5551 ;
//
//    UCA1CTLW0 &= ~UCSWRST ; // release from reset
//    UCA1IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//void USCI_A1_transmit(void){ // Transmit Function for USCI_A1
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
//    //direct_iot = 0; // Set Array index to first location
//    UCA0IE |= UCTXIE; // Enable TX interrupt
//}



















//
//void Init_Serial_UCA1(void){
//    //------------------------------------------------------------------------------
//    // TX error (%) RX error (%)
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
//    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
//    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
//    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//    //------------------------------------------------------------------------------
//    // Configure eUSCI_A0 for UART mode
//    UCA1CTLW0 = 0;
//    UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
//    UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
//    UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
//    UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
//    UCA1CTLW0 &= ~UCPEN; // No Parity
//    UCA1CTLW0 &= ~UCSYNC;
//    UCA1CTLW0 &= ~UC7BIT;
//    UCA1CTLW0 |= UCMODE_0;
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // UCA?MCTLW = UCSx + UCFx + UCOS16
//
//    UCA1BRW = 4 ; // set to 4 for 115,200 baud
//    UCA1MCTLW = 0x5551 ;
//
//    p9_rx_state = OFF; //Stays off until a character is received
//
//    UCA1CTLW0 &= ~UCSWRST ; // release from reset
//    //UCA1TXBUF = 0x00; // Prime the Pump
//    UCA1IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//
//void Init_Serial_UCA0(void){
//    //------------------------------------------------------------------------------
//    // TX error (%) RX error (%)
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
//    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
//    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
//    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//    //------------------------------------------------------------------------------
//    // Configure eUSCI_A0 for UART mode
//    UCA0CTLW0 = 0;
//    UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
//    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
//    UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
//    UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
//    UCA0CTLW0 &= ~UCPEN; // No Parity
//    UCA0CTLW0 &= ~UCSYNC;
//    UCA0CTLW0 &= ~UC7BIT;
//    UCA0CTLW0 |= UCMODE_0;
//    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
//    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
//    // UCA?MCTLW = UCSx + UCFx + UCOS16
//
//    UCA0BRW = 4 ; // set to 4 for 115,200 baud
//    UCA0MCTLW = 0x5551 ;
//
//    p9_rx_state = OFF; //Stays off until a character is received
//
//
////    if (speed == 'a') {
////        UCA0BRW = 4 ; // 115,200 baud
////        UCA0MCTLW = 0x5551 ;
////    }
////    else if (speed == 'b') {
////        UCA0BRW = 17 ; // set to 17 for 460800
////        UCA0MCTLW = 0x4A00 ; // set to 0x4A00 for 460800
//    //}
//    UCA0CTLW0 &= ~UCSWRST ; // release from reset
//    //UCA0TXBUF = 0x00; // Prime the Pump
//    UCA0IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//
//void USCI_A1_transmit(void){ // Transmit Function for USCI_A0
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
//    //strcpy(tx_A1buf, "Testing\r\n");
//    pb_index = 0; // Set Array index to first location
//    UCA1IE |= UCTXIE; // Enable TX interrupt
//}
//
//void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
//    pb_index = 0; // Set Array index to first location
//    UCA0IE |= UCTXIE; // Enable TX interrupt
//}
//
//void out_character(char character){
//    //------------------------------------------------------------------------------
//    // The while loop will stall as long as the Flag is not set [port is busy]
//    while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
//    UCA0TXBUF = character;
//    //------------------------------------------------------------------------------
//}
