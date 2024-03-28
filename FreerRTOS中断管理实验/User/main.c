#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "LED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
#include "timer.h"
uint8_t num;
uint8_t NUM;
uint8_t KeyNum; 

 
//任务优先级
#define START_TASK_PRIO		 1
//任务堆栈大小	
#define START_TASK_STACK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void Start_Task(void *pvParameters);
 
//任务优先级
#define TASK1_PRIO		1
//任务堆栈大小	
#define TASK1_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task1_Handler;
////任务函数
void Task1(void *pvParameters);

//任务优先级
#define TASK2_PRIO		1
//任务堆栈大小	
#define TASK2_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task2_Handler;
////任务函数
void Task2(void *pvParameters);

 
int main(void)
{
    LED_Init();
	Key_Init();
    OLED_Init();
    Timer_Init();
  //创建开始任务
  xTaskCreate((TaskFunction_t )Start_Task,            //任务函数
              (const char*    )"Start_Task",          //任务名称
              (uint16_t       )START_TASK_STACK_SIZE,        //任务堆栈大小
              (void*          )NULL,                  //传递给任务函数的参数
              (UBaseType_t    )START_TASK_PRIO,       //任务优先级
              (TaskHandle_t*  )&StartTask_Handler);   //任务句柄  
              
  vTaskStartScheduler();          //开启任务调度
  
}
 
//开始任务任务函数
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//进入临界区
       
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )Task1,     	
                (const char*    )"TASK1",   	
                (uint16_t       )TASK1_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )TASK1_PRIO,	
                (TaskHandle_t*  )&Task1_Handler);  

    xTaskCreate((TaskFunction_t )Task2,     	
                (const char*    )"TASK2",   	
                (uint16_t       )TASK2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )TASK2_PRIO,	
                (TaskHandle_t*  )&Task2_Handler); 				
				
   
				
	
    
			

        vTaskDelete(NULL);
		taskEXIT_CRITICAL(); //退出临界区

}

void Task1(void *pvParameters)
{
	while(1)
	{
	  OLED_ShowString(2,1,"NUM:");
	  OLED_ShowNum(2,5,NUM,5);
	  OLED_ShowString(1,1,"num:");
	  OLED_ShowNum(1,5,num,5);
      
	  vTaskDelay(10);
	 
	}
	
}


void Task2(void *pvParameters)
{
	
	
	
	while(1)
	{
	   
		if(NUM==6){ \
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		//portDISABLE_INTERRUPTS(); //关闭中断 
	    }
      
	  vTaskDelay(50);
	 
	}
	
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		
		num ++;												//Num变量自增，用于测试定时中断
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{   
	    
		NUM ++;												//Num变量自增，用于测试定时中断
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}

