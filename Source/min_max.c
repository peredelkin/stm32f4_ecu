#include "min_max.h"

uint16_t min_max_u16(uint16_t data,uint16_t min,uint16_t max) {
    if(data > max) {
        return max;
    } else {
        if(data < min) {
            return min;
        } else {
            return data;
        }
    }
}
