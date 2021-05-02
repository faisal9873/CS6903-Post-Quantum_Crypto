#include "time_helper.h"

double tv_to_seconds(struct timeval* tv){
    return tv->tv_sec + (double)tv->tv_usec/1000000;
}