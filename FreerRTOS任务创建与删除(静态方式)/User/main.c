#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "LED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "OLED.h"

 

 

 
//任务优先级
#define START_TASK_PRIO		 1
//任务堆栈大小	
#define START_TASK_STACK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;

StackType_t  Start_task_stack[START_TASK_STACK_SIZE]  ;

StaticTask_t Start_task_tcb;

//任务函数
void Start_Task(void *pvParameters);
 
//任务优先级
#define TASK1_PRIO		2
//任务堆栈大小	
#define TASK1_STACK_SIZE 		128  
//任务堆栈
StackType_t TASK1_STACK[TASK1_STACK_SIZE];
//任务控制块
StaticTask_t  TASK1_TCB;
//任务句柄
TaskHandle_t Task1_Handler;
////任务函数
void Task1(void *pvParameters);

 //任务优先级
#define TASK2_PRIO		3
//任务堆栈大小	
#define TASK2_STACK_SIZE 		128
//任务堆栈
StackType_t TASK2_STACK[TASK2_STACK_SIZE];
//任务控制块
StaticTask_t  TASK2_TCB;
//任务句柄
TaskHandle_t Task2_Handler;
////任务函数
void Task2(void *pvParameters);

//任务优先级
#define TASK3_PRIO		4
//任务堆栈大小	
#define TASK3_STACK_SIZE 		128
//任务堆栈
StackType_t TASK3_STACK[TASK2_STACK_SIZE];
//任务控制块
StaticTask_t  TASK3_TCB;
//任务句柄
TaskHandle_t Task3_Handler;
////任务函数
void Task3(void *pvParameters);


StaticTask_t  idle_task_tcb;
StackType_t   idle_task_stack[START_TASK_STACK_SIZE];
//空闲任务内存分配
void vApplicationGetIdleTaskMemory(     StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize )
{
		*ppxIdleTaskTCBBuffer=&idle_task_tcb;//空闲任务控制块	
        *ppxIdleTaskStackBuffer=idle_task_stack;//空闲任务堆栈	
		*pulIdleTaskStackSize=START_TASK_STACK_SIZE;//空闲任务堆栈大小									
}



StaticTask_t  Timer_task_tcb;
StackType_t   Timer_task_stack[START_TASK_STACK_SIZE];
//软件定时器内存分配
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
		*ppxTimerTaskTCBBuffer=&Timer_task_tcb;//空闲任务控制块	
        *ppxTimerTaskStackBuffer=Timer_task_stack;//空闲任务堆栈	
		*pulTimerTaskStackSize=START_TASK_STACK_SIZE;//空闲任务堆栈大小									 								 
}
int main(void)
{
    LED_Init();
    Key_Init();
    
     
  //创建开始任务
   StartTask_Handler= xTaskCreateStatic( (TaskFunction_t ) Start_Task,            //任务函数
	                                     (char *         ) "Start_Task",          //任务名称
                                         (uint32_t       ) START_TASK_STACK_SIZE, //堆栈大小
                                         (void *         ) NULL,                  //接口函数
                                         (UBaseType_t    ) START_TASK_PRIO,       //任务优先级
                                         (StackType_t *  ) Start_task_stack,      //堆栈
                                         (StaticTask_t *) &Start_task_tcb );     //开始任务控制块
              
  vTaskStartScheduler();          //开启任务调度
  
}
 
//开始任务任务函数
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//进入临界区
                                            
    //创建TASK1任务
 Task1_Handler= xTaskCreateStatic( (TaskFunction_t ) Task1,            //任务函数
	                               (char *         ) "Task1",          //任务名称
                                   (uint32_t       ) TASK1_STACK_SIZE, //堆栈大小
                                   (void *         ) NULL,                  //接口函数
                                   (UBaseType_t    ) TASK1_PRIO,       //任务优先级
                                   (StackType_t *  ) TASK1_STACK,      //堆栈
                                   ( StaticTask_t *) &TASK1_TCB );     //开始任务控制块
					   
 Task2_Handler=xTaskCreateStatic( (TaskFunction_t ) Task2,            //任务函数
	                              (char *         ) "Task2",          //任务名称
                                  (uint32_t       ) TASK2_STACK_SIZE, //堆栈大小
                                  (void *         ) NULL,                  //接口函数
                                  (UBaseType_t    ) TASK2_PRIO,       //任务优先级
                                  (StackType_t *  ) TASK2_STACK,      //堆栈
                                  ( StaticTask_t *) &TASK2_TCB );     //开始任务控制块
					 
 Task3_Handler=xTaskCreateStatic( (TaskFunction_t ) Task3,            //任务函数
	                              (char *         ) "Task3",          //任务名称
                                  (uint32_t       ) TASK3_STACK_SIZE, //堆栈大小
                                  (void *         ) NULL,                  //接口函数
                                  (UBaseType_t    ) TASK3_PRIO,       //任务优先级
                                  (StackType_t *  ) TASK3_STACK,      //堆栈
                                  ( StaticTask_t *) &TASK3_TCB );     //开始任务控制块 
			

        vTaskDelete(StartTask_Handler);
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
	   KeyNum=Key_GetNum(GPIO_Pin_11);
	      if(KeyNum==1){
			 LED_OFF(GPIO_Pin_1);
			 vTaskDelete(Task1_Handler); 
			 Task1_Handler = NULL;
		  }
	   vTaskDelay(10);
	 }
	 
}


