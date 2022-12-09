#ifndef ButtonRead
#define ButtonRead

typedef enum {
NoPressed=0,//0 raw =4095 
ButtonOK,//1 raw =3300
ButtonLEFT,//2 raw =1440
ButtonRIGHT,//3 raw =0-11
ButtonUP,//4 raw =660
ButtonDOWN,//5 raw =88
ScreenOffFlag
} 
buttonsOut;

uint8_t listenButtons(void);
#endif