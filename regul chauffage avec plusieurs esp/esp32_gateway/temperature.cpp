//
//
//  temperature.cpp
//
//

int temperatureCourante = 200;

//=========================================
//
//          getTemperature
//
//=========================================
int getTemperature(void){
    temperatureCourante = temperatureCourante + 1;
    if (temperatureCourante > 210) temperatureCourante = 200;
    return temperatureCourante;
}