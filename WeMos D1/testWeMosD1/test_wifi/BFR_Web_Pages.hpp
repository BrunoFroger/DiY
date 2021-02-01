// ESP32_horloge
// BFR_Web_Pages.hpp
// (c) B. Froger 2020

#include <Arduino.h>

#ifndef __BFR_WEB_PAGES
#define __BFR_WEB_PAGES

#include "BFR_serveurWeb.hpp"

extern String page;

extern void web_Pages_Init();
extern void web_HomePage();
extern void web_ConfigPage();

#endif