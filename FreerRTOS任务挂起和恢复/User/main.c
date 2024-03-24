#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "LED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
 

 

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


//任务优先级
#define TASK3_PRIO		1
//任务堆栈大小	
#define TASK3_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task3_Handler;
////任务函数
void Task3(void *pvParameters);

 
int main(void)
{
    LED_Init();
	Key_Init();
    Serial_Init();
   
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
				
	xTaskCreate((TaskFunction_t )Task3,     	
                (const char*    )"TASK3",   	
                (uint16_t       )TASK3_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )TASK3_PRIO,	
                (TaskHandle_t*  )&Task3_Handler);
			
    
			

        vTaskDelete(NULL);
				taskEXIT_CRITICAL(); //退出临界区

}

void Task1(void *pvParameters)
{
	while(1)
	{
	  
	  LED_Turn(GPIO_Pin_1);
	  vTaskDelay(500);
	 
	}
	
}

void Task2(void *pvParameters)
{
	 while(1)
	 {
	 
	  LED_Turn(GPIO_Pin_2);
	  vTaskDelay(500); 
	  
	 }
	
}

void Task3(void *pvParameters)
{
      uint8_t KeyNum;
	 while(1)
	 {
	   KeyNum=Key_GetNum();
	     if(KeyNum==1)
		 {
			 vTaskSuspend(Task1_Handler);//挂起任务1
				           
		 }
		 if(KeyNum==2)
		 {
			 
			vTaskResume(Task1_Handler); 
		 }
		
		 if(KeyNum==3)
		 {
			
			 BaseType_t xYieldRequired;
             xYieldRequired = xTaskResumeFromISR( Task1_Handler );			 
			 if(xYieldRequired==pdTRUE) 
			 {
		       portYIELD_FROM_ISR( xYieldRequired );
		     }
		 }
		 
	   vTaskDelay(10);
}
	 
}

