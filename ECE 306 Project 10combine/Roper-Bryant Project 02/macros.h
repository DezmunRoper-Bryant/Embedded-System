
#define ALWAYS                  (1)
#define GRN_LED              (0x40) 
#define RED_LED              (0x01) 
#define RESET_STATE             (0)
#define TEST_PROBE           (0x01) 
#define TRUE                 (0x01) 
#define FALSE                 (0x02)


#define STRAIGHT 1
#define CIRCLE 2
#define EIGHT 3
#define TRIANGLE 4
#define NONE 0


#define WAIT ('A')//('W')
#define START ('B')//('S')
#define RUN ('C')//('R')
#define RIGHT ('D')//('T')
#define SHARP ('E')//('Z')
#define END ('F')//('E')
#define WHEEL_COUNT_TIME (10)

#define RIGHT_COUNT_TIME (5)//was just 6
#define LEFT_COUNT_TIME (8)


#define RIGHT_COUNT_TIME_Sharp (15)//(15 FOR RECT)(20)was just 6
#define LEFT_COUNT_TIME_Sharp (6)//(~6 FOR RECT)(1)


#define RIGHT_COUNT_TIME_RightTurn (7)//(9)//(7)//(7)
#define LEFT_COUNT_TIME_RightTurn (3)//(5)//(2)//(8)

#define RIGHT_COUNT_TIME_LeftTurn (1)//(7)//(7)
#define LEFT_COUNT_TIME_LeftTurn (15)//(2)//(8)

#define TRAVEL_DISTANCE (103)//(103)//(50)//(2)
#define TRAVEL_DISTANCE_xxx (206)//(103)//(50)//(2)
#define TRAVEL_DISTANCE_2 (17)//(50)//(2)

#define WAITING2START (50)


//PWM
#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR4)
#define RIGHT_REVERSE_SPEED (TB3CCR5)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED (TB3CCR3)
#define PERCENT_80 (45000)


//p8
#define LARGE_RING_SIZE (100)
#define SMALL_RING_SIZE (120)//(16)
#define BEGINNING        (0)
#define WAITING ('O')
#define RECEIVED ('P')
#define TRANSMIT ('Q')

//p9
#define ON (1)
#define OFF (0)
#define CLEAR ('R')
#define DECODE ('S')
#define IDLE ('T')

#define BLACK_LINE (750)

#define BLACK_RIGHT (700)
#define BLACK_LEFT (625)

