//#include  "functions.h"
//#include  "msp430.h"
//#include "macros.h"
//#include <string.h>
//
//// Global Variables
//volatile extern char pb_index; // Index for process_buffer
//char process_buffer[25]; // Size for appropriate Command Length
//volatile unsigned int rx_A1;
//volatile unsigned int temp_A1;
//char tx_A1buf[LARGE_RING_SIZE];
//char rx_A1buf[LARGE_RING_SIZE];
//volatile char tx_A1;
//
//volatile unsigned int rx_A0;
//volatile unsigned int temp_A0;
//char tx_A0buf[LARGE_RING_SIZE];
//char rx_A0buf[LARGE_RING_SIZE];
//volatile char tx_A0;
//
//extern char IOT_state;
//int i;
//
//extern char recieved_message[LARGE_RING_SIZE];
//
//extern char display_line[4][11];
//extern volatile unsigned char display_changed;
//extern volatile unsigned char update_display;
//
//
//void Init_Serial_UCA1(char speed){
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
//    if (speed == 'a') {
//        UCA1BRW = 4 ; // set to 4 for 115,200 baud
//        UCA1MCTLW = 0x5551 ;
//    }
//    else if (speed == 'b') {
//        UCA1BRW = 17 ; // set to 17 for 460800
//        UCA1MCTLW = 0x4A00 ; // set to 0x4A00 for 460800
//    }
//    UCA1CTLW0 &= ~UCSWRST ; // release from reset
//    UCA1TXBUF = 0x00; // Prime the Pump
//    UCA1IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//
//void Init_Serial_UCA0(char speed){
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
//    if (speed == 'a') {
//        strcpy(display_line[0], "          ");
//        strcpy(display_line[1], "          ");
//        strcpy(display_line[2], "BAUD: 115k");
//        UCA0BRW = 4 ; // 115,200 baud
//        UCA0MCTLW = 0x5551 ;
//    }
//    else if (speed == 'b') {
//        strcpy(display_line[0], "          ");
//        strcpy(display_line[1], "          ");
//        strcpy(display_line[2], "BAUD: 460k");
//        UCA0BRW = 17 ; // set to 17 for 460800
//        UCA0MCTLW = 0x4A00 ; // set to 0x4A00 for 460800
//    }
//    UCA0CTLW0 &= ~UCSWRST ; // release from reset
//    UCA0TXBUF = 0x00; // Prime the Pump
//    UCA0IE |= UCRXIE; // Enable RX interrupt
//    //------------------------------------------------------------------------------
//}
//
//void USCI_A1_transmit(void){ // Transmit Function for USCI_A0
//    // Contents must be in process_buffer
//    // End of Transmission is identified by NULL character in process_buffer
//    // process_buffer includes Carriage Return and Line Feed
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
//
//#pragma vector=EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){
//    //------------------------------------------------------------------------------
//    // Echo back RXed character, confirm TX buffer is ready first
//    unsigned int temp_A1;
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0: // Vector 0 - no interrupt
//        break;
//    case 2: // Vector 2 - RXIFG
//        temp_A1 = rx_A1++;
//        rx_A1buf[temp_A1] = UCA1RXBUF;
//        if (rx_A1 >= (LARGE_RING_SIZE)){
//            rx_A1 = BEGINNING; // Circular buffer back to beginning
//        }
//        /*if (rx_A1buf[BEGINNING] != '\0') {
//            P1OUT ^= RED_LED;
//            IOT_state = RECEIVED;
//        }*/
//        tx_A1buf[temp_A1] = rx_A1buf[temp_A1] ; // Transmit out the other port
//        break;
//    case 4: // Vector 4 - TXIFG
//        tx_A1++;
//        UCA1TXBUF = tx_A1buf[tx_A1];
//        if (tx_A1 >= (LARGE_RING_SIZE)){
//            tx_A1 = BEGINNING; // Circular buffer back to beginning
//            UCA1IE &= ~UCTXIE;
//        }
//
//        /*
//  UCA1TXBUF = test[pb_index]; // Transmit Current Indexed value // change iot_rx_wr to pb_index
//  test[pb_index++] = NULL; // Null Location of Transmitted value // change back to process_buffer
//  if(test[pb_index] == NULL){ // Is the next pb_index location NULL - End of Command
//    UCA1IE &= ~UCTXIE; // Disable TX interrupt
//  }
//         */
//
//        break;
//    default: break;
//    }
//    //------------------------------------------------------------------------------
//}
//
//
//#pragma vector=EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    //------------------------------------------------------------------------------
//    // Echo back RXed character, confirm TX buffer is ready first
//    unsigned int temp_A0;
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0: // Vector 0 - no interrupt
//        break;
//    case 2: // Vector 2 - RXIFG
//        if(UCA0RXBUF==0)break;
//
//        temp_A0 = rx_A0++;
//
//        rx_A0buf[temp_A0] = UCA0RXBUF; // Rx -> IOT_2_PC character array
//        recieved_message[temp_A0]=rx_A0buf[temp_A0];
//
//        if (rx_A0 >= (LARGE_RING_SIZE)){
//            //recieved_message[temp_A0] = " ";
//            rx_A0 = BEGINNING; // Circular buffer back to beginning
//        }
//        if (temp_A0 >= (LARGE_RING_SIZE) || rx_A0buf[temp_A0] == '\n') {
//            P1OUT ^= RED_LED;
//            IOT_state = RECEIVED;
//           // UCA0IE &= ~UCRXIE;
//            /*for (i = 0; i<10;++i) {
//                recieved_message[i]=rx_A1buf[i];
//           }*/
//            //rx_A1buf[LARGE_RING_SIZE];
//           //rx_A0 = BEGINNING;
//        }
//        //UCA1TXBUF = IOT_2_PC[temp] ; // Transmit out the other port
//        break;
//
//    case 4: // Vector 4 - TXIFG
//        temp_A0 = tx_A0++;
//        tx_A0buf[temp_A0] = recieved_message[temp_A0];
//        UCA0TXBUF = tx_A0buf[temp_A0];
//        if (temp_A0 >= (LARGE_RING_SIZE) || tx_A0buf[temp_A0] == '\n'){ // || tx_A0buf[temp_A0] == '\n' || tx_A0buf[temp_A0] == '\r'
//            tx_A0 = BEGINNING; // Circular buffer back to beginning
//            UCA0IE &= ~UCTXIE;
//            //IOT_state = TRANSMIT;
//        }
//
//        /*
//  UCA1TXBUF = test[pb_index]; // Transmit Current Indexed value // change iot_rx_wr to pb_index
//  test[pb_index++] = NULL; // Null Location of Transmitted value // change back to process_buffer
//  if(test[pb_index] == NULL){ // Is the next pb_index location NULL - End of Command
//    UCA1IE &= ~UCTXIE; // Disable TX interrupt
//  }
//         */
//
//        break;
//    default: break;
//    }
//    //------------------------------------------------------------------------------
//}
