#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>



extern volatile unsigned int CCR0_counter;
extern char display_line[4][11];
extern volatile unsigned int CCR1_counter;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

// Global Variables
char process_buffer[25]; // Size for appropriate Command Length
volatile unsigned int rx_A1;
volatile unsigned int temp_A1;
char tx_A1buf[LARGE_RING_SIZE];
char rx_A1buf[LARGE_RING_SIZE];
volatile char tx_A1;

volatile unsigned int rx_A0;
volatile unsigned int temp_A0;
char tx_A0buf[LARGE_RING_SIZE];
char rx_A0buf[LARGE_RING_SIZE];
volatile char tx_A0;

extern char IOT_state;
int i;

int command_rx;

extern char recieved_message[LARGE_RING_SIZE];
//volatile unsigned int iot_rx_wr;


//project 9
volatile unsigned int temp;
volatile unsigned int temp2;
extern volatile int p9_rx_state;
extern volatile int transmission_complete;



//char temp[LARGE_RING_SIZE];
// Serial
volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
volatile unsigned int iot_rx_wr;
unsigned int iot_rx_rd;
unsigned int direct_iot;
volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
volatile unsigned int usb_rx_wr;
unsigned int usb_rx_rd;
unsigned int direct_usb;

//p9
volatile char IOT_Ring_Rx[SMALL_RING_SIZE], iot_TX_buf[SMALL_RING_SIZE], USB_Ring_Rx[SMALL_RING_SIZE];
volatile unsigned int iot_rx_wr, iot_tx, usb_rx_wr;

volatile unsigned int cmnd_start;
volatile unsigned int cmnd_stop;
volatile unsigned int take_command = 0;
volatile unsigned int done_command = 0;

volatile int recieve_command;

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    //-----------------------------------------------------------------------------
    // Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA0IV,0x08)){
    case 0: break; // Vector 0 - no interrupt
    case 2:{ // Vector 2 - RXIFG
        temp = iot_rx_wr++;
        IOT_2_PC[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
        if (iot_rx_wr >= (sizeof(IOT_2_PC))){
            iot_rx_wr = BEGINNING; // Circular buffer back to beginning
        }

        if ((IOT_2_PC[temp] == '^')) { // my code
            if (take_command == 0) {
                cmnd_start = temp - 1;
            }
            take_command = 1;
        }

        if ((IOT_2_PC[temp] == '\n') && take_command) { // my code
            cmnd_stop = temp;
            done_command = 1;
            take_command = 0;
        }

        if ((IOT_2_PC[temp] == '\n')) {
            UCA1IE |= UCTXIE; // Enable Tx interrupt // change back to UCA1IE
        }
    }break;
    case 4:{ // Vector 4 - TXIFG
        UCA0TXBUF = PC_2_IOT[direct_iot++]; // PC_2_IOT
        if (direct_iot >= (sizeof(PC_2_IOT))){ // PC_2_IOT
            direct_iot = BEGINNING;
        }
        if (UCA0TXBUF == '\n'){ // change back to usb_rx_wr
            UCA0IE &= ~UCTXIE; // Disable TX interrupt
        }
    }break;
    default: break;
    }
    //------------------------------------------------------------------------------
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    //------------------------------------------------------------------------------
    // Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA1IV,0x08)){
    case 0: break; // Vector 0 - no interrupt
    case 2:{ // Vector 2 - RXIFG
        temp = usb_rx_wr++;
        PC_2_IOT[temp] = UCA1RXBUF; // Rx -> PC_2_IOT character array
        if (usb_rx_wr >= (sizeof(PC_2_IOT))){
            usb_rx_wr = BEGINNING; // Circular buffer back to beginning
        }
        if ((PC_2_IOT[temp] == '\n')) {
            UCA0IE |= UCTXIE; // Enable Tx interrupt
        }

    }break;
    case 4:{ // Vector 4 - TXIFG
        UCA1TXBUF = IOT_2_PC[direct_usb++];
        if (direct_usb >= (sizeof(IOT_2_PC))){
            direct_usb = BEGINNING; // Circular buffer back to beginning
        }
        if (direct_usb == iot_rx_wr){
            UCA1IE &= ~UCTXIE; // Disable TX interrupt
        }
    }break;
    default: break;
    }
}

//#pragma vector=EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    //-----------------------------------------------------------------------------
//    // Echo back RXed character, confirm TX buffer is ready first
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0: break; // Vector 0 - no interrupt
//    case 2:{ // Vector 2 - RXIFG
//        temp = iot_rx_wr++;
//        IOT_2_PC[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
//        if (iot_rx_wr >= (sizeof(IOT_2_PC))){
//            iot_rx_wr = BEGINNING; // Circular buffer back to beginning
//        }
//
//        if ((IOT_2_PC[temp] == '^')) { // my code
//            if (take_command == 0) {
////                if(recieve_command == 1){
////                    cmnd_start = 0;
////                }
////                else{
////                    cmnd_start = temp - 1;
////                }
//                cmnd_start = temp - 1;
//            }
//            take_command = 1;
//        }
//
//        if ((IOT_2_PC[temp] == '\n') && take_command) { // my code
////            if(recieve_command == 1){
////                cmnd_stop = 8;
////            }
////            else{
////                cmnd_stop = temp;
////            }
//            cmnd_stop = temp;
//            done_command = 1;
//            take_command = 0;
//        }
//
//        if ((IOT_2_PC[temp] == '\n')) {
//            UCA1IE |= UCTXIE; // Enable Tx interrupt // change back to UCA1IE
//        }
//    }break;
//    case 4:{ // Vector 4 - TXIFG
//        UCA0TXBUF = PC_2_IOT[direct_iot++]; // PC_2_IOT
//        if (direct_iot >= (sizeof(PC_2_IOT))){ // PC_2_IOT
//            direct_iot = BEGINNING;
//        }
//        if (UCA0TXBUF == '\n'){ // change back to usb_rx_wr
//            UCA0IE &= ~UCTXIE; // Disable TX interrupt
//        }
//    }break;
//    default: break;
//    }
//    //------------------------------------------------------------------------------
//}
//
//#pragma vector=EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){
//    //------------------------------------------------------------------------------
//    // Echo back RXed character, confirm TX buffer is ready first
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0: break; // Vector 0 - no interrupt
//    case 2:{ // Vector 2 - RXIFG
//        temp = usb_rx_wr++;
//        PC_2_IOT[temp] = UCA1RXBUF; // Rx -> PC_2_IOT character array
//        if (usb_rx_wr >= (sizeof(PC_2_IOT))){
//            usb_rx_wr = BEGINNING; // Circular buffer back to beginning
//        }
//        if ((PC_2_IOT[temp] == '\n')) {
//            UCA0IE |= UCTXIE; // Enable Tx interrupt
//        }
//
//    }break;
//    case 4:{ // Vector 4 - TXIFG
//        UCA1TXBUF = IOT_2_PC[direct_usb++];
//        if (direct_usb >= (sizeof(IOT_2_PC))){
//            direct_usb = BEGINNING; // Circular buffer back to beginning
//        }
//        if (direct_usb == iot_rx_wr){
//            UCA1IE &= ~UCTXIE; // Disable TX interrupt
//        }
//    }break;
//    default: break;
//    }
//}
////------------------------------------------------------------------------------



// USE THIS TO CONNECT TO WIFI


//
//#pragma vector = EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    char usb_value;
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0:
//        break; //Vector 0 - no interrupt
//    case 2:
//    { // Vector 2 - Rx1IFG RECEIVE
//        usb_value = UCA0RXBUF;
//        UCA1TXBUF = usb_value;
//
//        if(usb_value == '\r')break;
//        if(usb_value == '\n'){
//            transmission_complete = 1;
//        }
//        else{
//            IOT_Ring_Rx[usb_rx_wr++] = usb_value; // Add to Ring Buffer
//        }
//        if(usb_rx_wr >= sizeof(IOT_Ring_Rx)){
//            usb_rx_wr = BEGINNING;
//        }
//    }
//    break;
//
//    case 4:
//    { // Vector 4 - TX1IFG TRANSMIT
//        UCA0TXBUF = USB_Ring_Rx[direct_iot];
//
//        if(USB_Ring_Rx[direct_iot++] == '\n'){ //  Disable interrupt whenever we have transmitted a '\n'
//            UCA0IE &= ~UCTXIE;
//            direct_iot = 0;
//            //Any code within this if statement only runs once the entire message has been transmitted
//        }
//        if(direct_iot >= sizeof(USB_Ring_Rx)){
//            direct_iot = BEGINNING;
//        }
//
//
//    }
//    break;
//    default:
//        break;
//    }
//    //------------------------------------------------------------------------------
//}
//
//
//#pragma vector = EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA1
//
//    char usb_value;
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0:
//        break; //Vector 0 - no interrupt
//    case 2:
//    { // Vector 2 - Rx1IFG RECEIVE
//        usb_value = UCA1RXBUF;
//        UCA0TXBUF = usb_value;
//
//        if(usb_value == '\r')break;
//        if(usb_value == '\n'){
//            //transmission_complete = 1;
//        }
//        else{
//            USB_Ring_Rx[usb_rx_wr++] = usb_value; // Add to Ring Buffer
//        }
//        if(usb_rx_wr >= sizeof(USB_Ring_Rx)){
//            usb_rx_wr = BEGINNING;
//        }
//    }
//    break;
//
//    case 4:
//    { // Vector 4 - TX1IFG TRANSMIT
//        UCA1TXBUF = USB_Ring_Rx[direct_iot++];
//
//        if(direct_iot == usb_rx_wr){
//            UCA1IE &= ~UCTXIE;
//            //Any code within this if statement only runs once the entire message has been transmitted
//        }
//        if(direct_iot >= sizeof(USB_Ring_Rx)){
//            direct_iot = BEGINNING;
//        }
//
//    }
//    break;
//    default:
//        break;
//    }
//    //------------------------------------------------------------------------------
//}
//
//





//
//#pragma vector = EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
//    char iot_receive;
////    char message[16] = "Dezmun\r\n";
//    char message[16];
//
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0: break; //Vector 0 - no interrupt
//    case 2:{ // Vector 2 – Rx0IFG
//        iot_receive = UCA0RXBUF; //want to use the temp values when comparing /temp is a single character
//        //UCA1IE |= UCTXIE; //
//        UCA1TXBUF = iot_receive;
//
//    }break;
//    case 4:{ // Vector 4 – Tx0IFG
//        //UCA1TXBUF = "A";
//
//        UCA0TXBUF = iot_TX_buf[iot_tx];
//        iot_TX_buf[iot_tx++] = 0;
//        if(iot_TX_buf[iot_tx] == 0x00){
//            UCA0IE &= ~UCTXIE;
//            iot_tx = 0;
//        }
//
//    }break;
//    default: break;
//    }
//}//------------------------------------------------------------------------------
//
//#pragma vector = EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){
//    p9_rx_state = ON;
//    char usb_value;
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0: break; //Vector 0 - no interrupt
//    case 2:{ // Vector 2 - Rx1IFG
//        p9_rx_state = ON;
//        usb_value = UCA1RXBUF;
//
//        USB_Ring_Rx[usb_rx_wr++] = usb_value; // Add to Ring Buffer
//        if(usb_rx_wr >= sizeof(USB_Ring_Rx)){
//            usb_rx_wr = BEGINNING;
//        }
//
//
//        UCA0TXBUF = usb_value;
//        //UCA0IE |= UCTXIE; //
//
//
//    }break;
//    case 4:{ // Vector 4 - TX1IFG
//       //UCA1TXBUF = "A";
//
//        if(p9_rx_state == 1){
//            UCA1TXBUF = IOT_Ring_Rx[direct_iot++];
//            // IOT_Ring_Rx[direct_iot++]= 0;
//            if(direct_iot >= sizeof(IOT_Ring_Rx)){
//                direct_iot = BEGINNING;
//            }
//            if(iot_rx_wr == direct_iot){
//                UCA1IE &= ~UCTXIE;
////                p9_rx_state = OFF;
//            }
//            p9_rx_state = OFF;
//
//            //p9_rx_state = OFF;
//        }
//
//    }break;
//    default:
//        break;
//    }
//    //------------------------------------------------------------------------------
//}






//#pragma vector = EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA0
//    //------------------------------------------------------------------------------
//    // Interrupt name: eUSCI_A0_ISR
//    // Description: This interrupt transmits and receives through UCA0
//    //------------------------------------------------------------------------------
//    char iot_receive;
//    // int temp_char;
//    // int i;
//    switch(__even_in_range(UCA0IV,0x08)){
//    case 0: break; //Vector 0 - no interrupt
//    case 2:{ // Vector 2 – Rx0IFG
//        iot_receive = UCA0RXBUF;
//        IOT_Ring_Rx[iot_rx_wr++] = iot_receive; // Add to Ring Buffer
//        if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){
//            iot_rx_wr = BEGINNING;
//        }
//        UCA1IE |= UCTXIE; //
//
//        UCA1TXBUF = iot_receive;
//
//    }break;
//    case 4:{ // Vector 4 – Tx0IFG
//        UCA0TXBUF = iot_TX_buf[iot_tx];
//        iot_TX_buf[iot_tx++] = 0;
//        if(iot_TX_buf[iot_tx] == 0x00){
//            UCA0IE &= ~UCTXIE;
//            iot_tx = 0;
//        }
//    }break;
//    default: break;
//    }
//}//------------------------------------------------------------------------------
//
//#pragma vector = EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA1
//    //------------------------------------------------------------------------------
//    // Interrupt name: eUSCI_A1_ISR
//    // Description: This interrupt transmits and receives through UCA1
//    //------------------------------------------------------------------------------
//    char usb_value;
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0: break; //Vector 0 - no interrupt
//    case 2:{ // Vector 2 - Rx1IFG
//        p9_rx_state = ON;
//        usb_value = UCA1RXBUF;
//        USB_Ring_Rx[usb_rx_wr++] = usb_value; // Add to Ring Buffer
//        if(usb_rx_wr >= sizeof(USB_Ring_Rx)){
//            usb_rx_wr = BEGINNING;
//        }
//        UCA0TXBUF = usb_value;
//        UCA0IE |= UCTXIE; //
//    }break;
//    case 4:{ // Vector 4 - TX1IFG
//        UCA1TXBUF = IOT_Ring_Rx[direct_iot++];
//        // IOT_Ring_Rx[direct_iot++]= 0;
//        if(direct_iot >= sizeof(IOT_Ring_Rx)){
//            direct_iot = BEGINNING;
//        }
//        if(iot_rx_wr == direct_iot){
//            UCA1IE &= ~UCTXIE;
//            p9_rx_state = OFF;
//        }
//        //p9_rx_state = OFF;
//
//    }break;
//    default:
//        break;
//    }
//    //------------------------------------------------------------------------------
//}










//
//#pragma vector=EUSCI_A1_VECTOR
//__interrupt void eUSCI_A1_ISR(void){
//    //------------------------------------------------------------------------------
//    unsigned int temp_A1;
//    switch(__even_in_range(UCA1IV,0x08)){
//    case 0: // Vector 0 - no interrupt
//        break;
//    case 2: // Vector 2 - RXIFG
//        p9_rx_state = ON;
//        temp_A1 = rx_A1++;
//        rx_A1buf[temp_A1] = UCA1RXBUF;
//        if (rx_A1 >= (LARGE_RING_SIZE)){
//            rx_A1 = BEGINNING; // Circular buffer back to beginning
//        }
//        tx_A1buf[temp_A1] = rx_A1buf[temp_A1] ; // Transmit out the other port
////        temp = UCA1RXBUF;
////        UCA0TXBUF = temp;
//        break;
//    case 4: // Vector 4 - TXIFG
//        tx_A1++;
//        UCA1TXBUF = tx_A1buf[tx_A1];
//        if (tx_A1 >= (LARGE_RING_SIZE)){
//            tx_A1 = BEGINNING; // Circular buffer back to beginning
//            UCA1IE &= ~UCTXIE;
//            p9_rx_state = OFF;
//        }
//
//
//        break;
//    default: break;
//    }
//    //------------------------------------------------------------------------------
//}
//
//#pragma vector=EUSCI_A0_VECTOR
//__interrupt void eUSCI_A0_ISR(void){
////-----------------------------------------------------------------------------
//// Echo back RXed character, confirm TX buffer is ready first
//switch(__even_in_range(UCA0IV,0x08)){
//     case 0: break; // Vector 0 - no interrupt
//     case 2:{ // Vector 2 - RXIFG
//         temp = iot_rx_wr++;
//         IOT_2_PC[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
//         if (iot_rx_wr >= (sizeof(IOT_2_PC))){
//             iot_rx_wr = BEGINNING; // Circular buffer back to beginning
//         }
////         temp2 = UCA0RXBUF;
////         UCA1TXBUF = temp2;
//         UCA1IE |= UCTXIE; // Enable Tx interrupt
//
//     }break;
//     case 4:{ // Vector 4 - TXIFG
//         UCA0TXBUF = PC_2_IOT[direct_iot++];
//         if (direct_iot >= (sizeof(PC_2_IOT))){
//             direct_iot = BEGINNING;
//         }
//         if (direct_iot == usb_rx_wr){
//             UCA0IE &= ~UCTXIE; // Disable TX interrupt
//         }
////         temp = UCA1RXBUF;
////         UCA1TXBUF = temp;
//
//
//
//     }break;
//     default: break;
// }
////------------------------------------------------------------------------------
//}


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
//        temp_A0 = rx_A0++;
//        rx_A0buf[temp_A0] = UCA0RXBUF; // Rx -> IOT_2_PC character array
//        recieved_message[temp_A0]=rx_A0buf[temp_A0];
//        if (rx_A0 >= (LARGE_RING_SIZE)){
//            rx_A0 = BEGINNING; // Circular buffer back to beginning
//        }
//        if (temp_A0 >= (LARGE_RING_SIZE) || rx_A0buf[temp_A0] == '\n') {
//            P1OUT ^= RED_LED;
//            IOT_state = RECEIVED;
//        }
//        break;
//
//    case 4: // Vector 4 - TXIFG
//        temp_A0 = tx_A0++;
//        tx_A0buf[temp_A0] = recieved_message[temp_A0];
//        UCA0TXBUF = tx_A0buf[temp_A0];
//        if (temp_A0 >= (LARGE_RING_SIZE) || tx_A0buf[temp_A0] == '\n'){ // || tx_A0buf[temp_A0] == '\n' || tx_A0buf[temp_A0] == '\r'
//            tx_A0 = BEGINNING; // Circular buffer back to beginning
//            UCA0IE &= ~UCTXIE;
//        }
//        //project 9 step 9
//        temp = UCA1RXBUF;
//        UCA1TXBUF = temp;
//
//        break;
//    default: break;
//    }
//    //------------------------------------------------------------------------------
//}
