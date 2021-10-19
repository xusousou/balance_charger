#ifndef __KEY_H_
#define __KEY_H_
#include "board.h"

//typedef enum 
//{
//    KEY_WAKEUP = 0,
//}key_typedef_enum;

//typedef enum 
//{
//    KEY_MODE_GPIO = 0,
//    KEY_MODE_EXTI = 1
//}keymode_typedef_enum;

void key_init(void);
uint8_t KEY_Scan(void);
//void gd_eval_key_init(key_typedef_enum keynum, keymode_typedef_enum keymode);
//uint8_t gd_eval_key_state_get(key_typedef_enum keynum);

#endif
