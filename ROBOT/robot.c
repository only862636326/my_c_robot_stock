
#include "robot.h" 

#define pass  ;
/**
  *****************************************************************
  * @file    PathPlan.c
  * @author  ShengNan Wang
  * @version V1.0.0
  * @date    2020.2.14
  * @brief   ��������������
  *****************************************************************    
  * @����˵��:
  *    
  *    
  *    
  ******************************************************************
  */


TYPE_Robot_t g_Robot_t;

void Robot_Init(void)
{
    
    
    
}
void Robot_None_Task(void)
{
    
    
    
    
    
}
void Robot_Sta_Check(uint8_t flag)
{
    if (flag == 1)
    {
        switch (g_Robot_t.robot_Sta_Enum)
        {
            case Robot_Sta_None:
                // g_Robot_t.robot_Sta_Enum = Robot_Sta_None;
                break;
            
            case Robot_Sta_Start:
                pass;
                break;
            
            default:
                pass;
        }
    }
    else
    {
        pass;
        
    }
}

void Robot_Task(void)
{
    uint8_t robot_flag;
    
    while (1)
    {
        Robot_Sta_Check(robot_flag);
        switch (g_Robot_t.robot_Sta_Enum)
        {
            case Robot_Sta_None:
                p_Task_Funs[Robot_Sta_None];
                Robot_None_Task();
                /******״̬��־*****/
                if ( == )
                    robot_flag = 1;
                else 
                    robot_flag = 0;
                break;
                
            /**********�Զ���״̬����*********/
            case Robot_Sta_Start:
                //����
                Robot_ReStart_Task();
                break;
            
            default:
                pass;
            
            /***********/
        }
    }
}
void Robot_Joysyick2Contr(void)
{
    
    
    
    
    
}

















