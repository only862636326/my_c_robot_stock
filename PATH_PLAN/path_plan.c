
#include "path_plan.h" 


/**
  *****************************************************************
  * @file    PathPlan.c
  * @author  ShengNan Wang
  * @version V1.1.0
  * @date    2020.2.13
  * @brief   底盘路线规化, 以设定的速度跑点,
  *****************************************************************    
  * @更新说明:
  *      1.1.0  2020.2.13 1.重构,封装
  *    
  *    
  ******************************************************************
  */

/*******************************************************
 *
 * Function name ：PathPlan_Init
 * Description   : 路径规划初始化
 *                 
 * Parameter     ：pPlan_t:路经规化结构体指针
 *                 
 * Return        ：
                  
**********************************************************/

void PathPlan_Init(pTYPE_Path_Plan_t pPlan_t, pTYPE_Path_Info_t pPathInfo, ENUM_PathPlanMode mode)
{
    pPlan_t ->chassisInfo_t.x = 0;
    pPlan_t ->chassisInfo_t.y = 0;
    pPlan_t ->chassisInfo_t.theta = 0;
    
    pPlan_t ->prePointNum = 0;

    pPlan_t ->config_t.dead_Band       = 50;
    pPlan_t ->config_t.dead_Band_xx_2  = 2500;
    pPlan_t ->config_t.dead_Band_Theta = 2;
    pPlan_t ->config_t.a_ratio    = PATH_PLAN_A_RATIO;
    pPlan_t ->config_t.axis_ratio = PATH_PLAN_AXIS_RATIO;
    pPlan_t ->config_t.mode_enum  = mode;
    
    pPlan_t ->pPlanInfo = pPathInfo;
}


/*******************************************************
*
* Function name ：Is_Position_In_Dead_Band_1
* Description   : 计算当前位置是否在对应点死区内(街区距离)
                  
*                 
* Parameter     ：   pPlan_t:路经规化结构体指针
                    i:第i个点

* Return        ：1 在死区内
                  0 不在死区内
                 
**********************************************************/

uint8_t Is_Position_In_Dead_Band_1(pTYPE_Path_Plan_t pPlan_t)
{
    float distence;
    distence = __fabs(pPlan_t ->chassisInfo_t.x - pPlan_t ->pPrePoint ->x)
              +__fabs(pPlan_t ->chassisInfo_t.y - pPlan_t ->pPrePoint ->y);
    if (distence < pPlan_t ->config_t.dead_Band)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*******************************************************
*
* Function name ：Is_Position_In_Dead_Band_2
* Description   : 计算当前位置是否在对应点死区内(曼哈顿距离)
                  会增大计算量,但效果会更好
*                 
* Parameter     ：pPlan_t:路经规化结构体指针

* Return        ：1 在死区内
                  0 不在死区内
                 
**********************************************************/

uint8_t Is_Position_In_Dead_Band_2(pTYPE_Path_Plan_t pPlan_t)
{
    float distence;
    distence =  (pPlan_t ->chassisInfo_t.x - pPlan_t ->pPrePoint ->x) *
                (pPlan_t ->chassisInfo_t.x - pPlan_t ->pPrePoint ->x)
              + (pPlan_t ->chassisInfo_t.y - pPlan_t ->pPrePoint ->y) *
                (pPlan_t ->chassisInfo_t.y - pPlan_t ->pPrePoint ->y);
    if (distence < pPlan_t ->config_t.dead_Band_xx_2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/*******************************************************
*
* Function name ：PathPlan_Read_V
* Description   : 读结构的点到缓存中 
                 
*                 
* Parameter     ：pPlan_t:路经规化结构体指针
*                       i:第i个点

* Return        ：
                  
                 
**********************************************************/



void PathPlan_Read_V(pTYPE_Path_Plan_t pPlan_t)
{
    pPlan_t ->pPrePoint = &pPlan_t ->pPlanInfo ->pPoints[pPlan_t ->prePointNum];
    
}

/*******************************************************
*
* Function name ：PathPlan_Cla_Set_Chassis_V
* Description   : 计算当前位置的点到第g_PathPlan_ChassisInfo_t.point_num个点
                  x,y轴方向上的速度
*                 因无坐标转换,角度只能在0度左右浮动,
*                 
* Parameter     ：pPlan_t:路经规化结构体指针

* Return        ：
                  
                 
**********************************************************/



void PathPlan_Cla_Plan_V(pTYPE_Path_Plan_t pPlan_t)
{
    float sin_theta, cos_theta;
  
    //弦长,当前位置到目标的距离
    float chord_len =(float)pathPlan_sqrt(
        (pPlan_t ->chassisInfo_t.x - pPlan_t ->pPrePoint ->x) *
        (pPlan_t ->chassisInfo_t.x - pPlan_t ->pPrePoint ->x)
        +
        (pPlan_t ->chassisInfo_t.y - pPlan_t ->pPrePoint ->y) *
        (pPlan_t ->chassisInfo_t.y - pPlan_t ->pPrePoint ->y)
    );
    cos_theta = ( pPlan_t ->pPrePoint ->x - pPlan_t ->chassisInfo_t.x) / chord_len;
    sin_theta = ( pPlan_t ->pPrePoint ->y - pPlan_t ->chassisInfo_t.y) / chord_len;
    if (pPlan_t ->config_t.mode_enum == PATH_PLAN_MODE_1)
    {
        pPlan_t ->planV_t.x =  pPlan_t ->pPrePoint ->v * cos_theta;
        pPlan_t ->planV_t.y =  pPlan_t ->pPrePoint ->v * sin_theta;
    }
    else if(pPlan_t ->config_t.mode_enum == PATH_PLAN_MODE_2)
    {
        //if (Is_Position_In_Dead_Band_1(pPlan_t);
        chord_len *= pPlan_t ->config_t.axis_ratio;
        pPlan_t ->planV_t.x =  chord_len * cos_theta ;
        pPlan_t ->planV_t.y =  chord_len * sin_theta ;
    }    
}

//用Kp 调角度,达到角度锁定的效果
void PathPlan_Cla_Theta(pTYPE_Path_Plan_t pPlan_t)
{
    //uint16_t i = pPlan_t ->prePointNumnum;
    float Min = -pPlan_t ->config_t.dead_Band_Theta;
    float Max =  pPlan_t ->config_t.dead_Band_Theta;
    float det_theta = pPlan_t ->pPrePoint ->theta - pPlan_t ->chassisInfo_t.theta;
    
    if ((det_theta >= Min) && (det_theta <= Max))
    {
        pPlan_t ->planV_t.a = 0;
    }
    else
    {
        pPlan_t ->planV_t.a = det_theta * pPlan_t ->config_t.a_ratio;
        
    }
}
/*******************************************************
*
* Function name ：PathPlan_Run_One_Time
* Description   : 路经规化单次任务,
                  
*                 
*                 
* Parameter     ：pPlan_t:路经规化结构体指针

* Return        ：
                  
                 
**********************************************************/



void PathPlan_Run_One_Time(pTYPE_Path_Plan_t pPlan_t)
{
    if(pPlan_t ->prePointNum >= pPlan_t ->pPlanInfo ->point_num)
    {
        //规化完成
            // pPlan_t ->planV_t.x = 0;
            // pPlan_t ->planV_t.y = 0;
        
            PathPlan_Cla_Plan_V(pPlan_t);
            PathPlan_Cla_Theta(pPlan_t);
            return;
    }
    if (Is_Position_In_Dead_Band_1(pPlan_t))
    {
        //在死区内,
        pPlan_t ->prePointNum ++;
        
        if(pPlan_t ->prePointNum >= pPlan_t ->pPlanInfo ->point_num)
        {   
            //规化完成
            pPlan_t ->planV_t.x = 0;
            pPlan_t ->planV_t.y = 0;
            pPlan_t ->config_t.mode_enum = PATH_PLAN_MODE_2; //锁定
            return;
        }
        else
        {
            //规化未完成, 加载下一个点
            pPlan_t ->pPrePoint = pPlan_t ->pPlanInfo ->pPoints + pPlan_t ->prePointNum;
            // PathPlan_Read_V(pPath_t);
        }
    }
    else
    {
        //不在死区内,计算
        PathPlan_Cla_Plan_V(pPlan_t);
        PathPlan_Cla_Theta(pPlan_t);
    }   
}


//设置死区的API
void PathPlan_SetDeadBand(pTYPE_Path_Plan_t pPlan_t, float dead_Band, float dead_Band_Theta)
{
    pPlan_t ->config_t.dead_Band = dead_Band;
    pPlan_t ->config_t.dead_Band_xx_2 = dead_Band * dead_Band;
    pPlan_t ->config_t.dead_Band_Theta =dead_Band_Theta;
}
//设置底盘当前信息的API
void PathPlan_SetChassisInfo(pTYPE_Path_Plan_t pPlan_t, float x, float y, float theta)
{
    pPlan_t ->chassisInfo_t.x = x;
    pPlan_t ->chassisInfo_t.y = y;
    pPlan_t ->chassisInfo_t.theta = theta;
}                        
//获取计算速度的API
void PathPlan_GetV_XYA(pTYPE_Path_Plan_t pPlan_t, float *x, float *y, float *A)
{
    *x = pPlan_t ->planV_t.x;
    *y = pPlan_t ->planV_t.y;
    *A = pPlan_t ->planV_t.a;
}
//打印定义中第num个点数据
void PathPlan_Point_Print(pTYPE_Path_Plan_t pPlan_t, uint16_t num)
{
    
    pathPlan_printf(" %11.5f, %11.5f, %11.5f, %11.5f, %11.5f, %11.5f,\r\n",
                    pPlan_t ->pPrePoint ->x, pPlan_t ->pPrePoint ->y, pPlan_t ->pPrePoint ->v, 
                    pPlan_t ->pPrePoint ->theta, pPlan_t ->pPrePoint ->vx,pPlan_t ->pPrePoint ->vy); 
}


/***

void test(void)
{
    TYPE_Path_Plan_t plan_t;
    PathPlan_SetChassisInfo((pTYPE_Path_Plan_t pPlan_t, x, y, theta);
    PathPlan_Run_One_Time((pTYPE_Path_Plan_t pPlan_t);
    PathPlan_GetV_XYA((pTYPE_Path_Plan_t pPlan_t, &x, &y, &A);
}

*/

 
