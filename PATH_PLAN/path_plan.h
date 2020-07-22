#ifndef __PATH_PLAN__
#define __PATH_PLAN__


#include "POINTS.h"
#include "pid.h"
#include "user_math.h"
#include "math.h"
#include "uart.h"


#define PATH_PLAN_A_RATIO       0.2f
#define PATH_PLAN_AXIS_RATIO    10.0f
//相应函数重定义,以便更改
#define pathPlan_printf my_printf
#define pathPlan_cos    cosf
#define pathPlan_sin    sinf
#define pathPlan_sqrt   sqrtf 

typedef enum 
{
    PATH_PLAN_MODE_1,
    PATH_PLAN_MODE_2,
}ENUM_PathPlanMode;

typedef struct TYPE_PathPlan_Config_t
{
    float dead_Band;        //死区
    float dead_Band_xx_2;   //死区平方
    float dead_Band_Theta;  //角度死区
    
    ENUM_PathPlanMode mode_enum;
    float a_ratio;          //调角角度的KP
    float axis_ratio;       //速度的KP
}TYPE_PathPlan_Config_t, *pTYPE_PathPlan_Config_t;

typedef struct TYPE_PathPlan_ChassisInfo_t
{
    float x; 
    float y;       //当前位置
    float theta;            //底盘角度    
}TYPE_PathPlan_ChassisInfo_t, pTYPE_PathPlan_ChassisInfo_t;
typedef struct TYPE_PathPlan_Chassis_V
{
    float x;
    float y;
    float a;
}TYPE_PathPlan_Chassis_V,*pTYPE_PathPlan_Chassis_V;

typedef struct TYPE_Path_Plan_t
{
    TYPE_PathPlan_Config_t      config_t;        //设置
    pTYPE_Path_Info_t           pPlanInfo;       //路经信息指针
    
     uint16_t                   prePointNum;     //运行到第几点
    TYPE_PathPlan_ChassisInfo_t chassisInfo_t;   //底盘信息
    pTYPE_Point_Info_t          pPrePoint;       //当前点指针

    TYPE_PathPlan_Chassis_V     planV_t;         //规化的速度
}TYPE_Path_Plan_t, *pTYPE_Path_Plan_t;           


void PathPlan_SetChassisInfo    (pTYPE_Path_Plan_t pPlan_t, float x, float y, float theta);
void PathPlan_SetDeadBand       (pTYPE_Path_Plan_t pPlan_t, float dead_Band, float dead_Band_Theta);
void PathPlan_GetV_XYA          (pTYPE_Path_Plan_t pPlan_t, float *x, float *y, float *A);

void PathPlan_Run_One_Time      (pTYPE_Path_Plan_t pPlan_t);
void PathPlan_Cla_Theta         (pTYPE_Path_Plan_t pPlan_t);
void PathPlan_Cla_Plan_V        (pTYPE_Path_Plan_t pPlan_t);


void PathPlan_Init              (pTYPE_Path_Plan_t pPlan_t, pTYPE_Path_Info_t pPathInfo, ENUM_PathPlanMode mode);
void PathPlan_Point_Print       (pTYPE_Path_Plan_t pPlan_t, uint16_t num);

uint8_t Is_Position_In_Dead_Band_2(pTYPE_Path_Plan_t pPlan_t);
uint8_t Is_Position_In_Dead_Band_1(pTYPE_Path_Plan_t pPlan_t); 

#endif



