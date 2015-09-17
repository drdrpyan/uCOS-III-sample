#ifndef __HW2_INTERRUPT__
#define __HW2_INTERRUPT__

#include <os.h>
#include <cpu.h>
#include <bsp.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>

#define HW2_TASK_LED_STK_SIZE 128
#define HW2_TASK_LED_PRIO 4
OS_TCB HW2TaskLEDTCB;
CPU_STK HW2TaskLEDStack[HW2_TASK_LED_STK_SIZE];
void HW2TaskLED(void *p_arg);


#define HW2_TASK_BUTTON_STK_SIZE 128
#define HW2_TASK_BUTTON_PRIO 5
OS_TCB HW2TaskButtonTCB;
CPU_STK HW2TaskButtonStack[HW2_TASK_BUTTON_STK_SIZE];
void HW2TaskButton(void *p_arg);

OS_SEM  ButtonSem;

void HW2ButtonHandler(void);
                      
void HW2TaskLED(void *p_arg) {
  OS_ERR err;
  BSP_LED_Off(0);
  while (DEF_TRUE) {
    BSP_LED_Off(0);
    BSP_LED_Toggle(0);
    OSTimeDlyHMSM(0, 0, 10, 0, OS_OPT_TIME_HMSM_STRICT, &err);
  }
}

void HW2TaskButton(void *p_arg) {
  OS_ERR err;
  CPU_TS ts;
  int ledNum = 0;

  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  BSP_PeriphEn(BSP_PERIPH_ID_IOPA);             
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  BSP_IntVectSet(BSP_INT_ID_EXTI0, HW2ButtonHandler);
  BSP_IntEn(BSP_INT_ID_EXTI0);
  
  OSSemCreate(&ButtonSem, "Button Semaphore", 0, &err);
    
  while (DEF_TRUE) {
    (void)OSSemPend(&ButtonSem, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    BSP_LED_Off(0);
    ledNum = (ledNum+1)%5;
    BSP_LED_Toggle(ledNum);
  }
}

void HW2ButtonHandler(void) {
  OS_ERR err;
  OS_SEM_CTR ctr;
  ctr = OSSemPost(&ButtonSem, OS_OPT_POST_1, &err);
  EXTI_ClearITPendingBit(EXTI_Line0);
}
#endif

