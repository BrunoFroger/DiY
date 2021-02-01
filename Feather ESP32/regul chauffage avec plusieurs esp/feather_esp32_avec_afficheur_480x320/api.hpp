//
//
//  api.hpp
//
//

#ifndef __API
#define __API


typedef struct {
    char heureFormatee[50];
    char dateFormatee[50];
} chauffageDatas;

extern chauffageDatas *getDatas(void);
extern void updateHeure(void);
extern bool isDatasAvailables(void);

#endif