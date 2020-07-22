#ifndef __ROBOT_H
#define __ROBOT_H

#include "sys.h"
#include "joystic.h"


#define robot_printf my_printf

typedef enum
{
    Robot_Sta_None,          //¿Õ×´Ì¬
    Robot_Sta_Start,         //¿ªÊ¼×´Ì¬
    
}ENUM_Robot_sta;

typedef struct TYPE_Robot_DebugInfo_t
{
    
    
    
}

typedef struct TYPE_Robot_Contr_t
{
    
    
    
    
}TYPE_Robot_Contr_t, *pTYPE_Robot_Contr_t;


typedef struct TYPE_Robot_t{
    ENUM_Robot_sta          robot_Sta_Enum;
    TYPE_Robot_Contr_t      robot_Contr_t;
    TYPE_Robot_DebugInfo_t  robot_Debug_t;
}TYPE_Robot_t, *pTYPE_Robot_t;

extern TYPE_Robot_t g_Robot_t;
//extern (void*) p_Task_Funs[];

void Robot_Init(void);
void Robot_Task(void);
void Robot_None_Task(void);
void Robot_ReStart_Task(void);

void Robot_Sta_Check(void);
void Robot_Joysyick2Contr(void);
ENUM_Robot_sta Robot_Sta_Get(void);


#endif



