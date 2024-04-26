/* 
 * File:   DS1307.h
 * Author: Dubai Store
 *
 * Created on April 21, 2024, 11:41 PM
 */


#ifndef DS1307_H
#define	DS1307_H

#include "../system/system.h"

typedef struct{
    uint8_t _Hour;
    uint8_t _Minute;
    uint8_t _Second;
    uint8_t _Year;
    uint8_t _Month;
    uint8_t _Day;
}DS1307_t;

DS1307_t Get_time_And_Date(void);

#endif	/* DS1307_H */

