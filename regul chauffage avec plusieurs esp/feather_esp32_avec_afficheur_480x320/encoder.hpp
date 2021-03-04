// chauffage
// encoder.hpp
// (c) B. Froger 2020

#include <Arduino.h>

#ifndef __ENCODER
#define __ENCODER


extern void initEncoder(void);
extern void checkEncoder(void);
extern int getEncoder(void);
extern void setEncoder(int);
extern void resetEncoder();
extern boolean isButtonPressed(void);


#endif