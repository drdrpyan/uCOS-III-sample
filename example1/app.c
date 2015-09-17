#include <includes.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

#define LED_TASK_STK_SIZE 128

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_SEM   AppSem; 

static  OS_TCB   AppTaskStartTCB; 
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

//추가 코드
static  OS_TCB  LED1TaskTCB;
static  OS_TCB  LED2TaskTCB;
static  OS_TCB  LED3TaskTCB;
static  OS_TCB  LED4TaskTCB;
static  CPU_STK LED1TaskStk[LED_TASK_STK_SIZE];
static  CPU_STK LED2TaskStk[LED_TASK_STK_SIZE];
static  CPU_STK LED3TaskStk[LED_TASK_STK_SIZE];
static  CPU_STK LED4TaskStk[LED_TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

//추가 코드
static void LED1Task(void *p_arg);
static void LED2Task(void *p_arg);
static void LED3Task(void *p_arg);
static void LED4Task(void *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    OSInit(&err);                                               /* Init uC/OS-III.                                      */
 
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR )AppTaskStart, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
  

    OSTaskCreate((OS_TCB     *)&LED1TaskTCB,                
                 (CPU_CHAR   *)"LED1 Task",
                 (OS_TASK_PTR )LED1Task, 
                 (void       *)0,
                 (OS_PRIO     )1,
                 (CPU_STK    *)&LED1TaskStk[0],
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&LED2TaskTCB,                
                 (CPU_CHAR   *)"LED2 Task",
                 (OS_TASK_PTR )LED2Task, 
                 (void       *)0,
                 (OS_PRIO     )2,
                 (CPU_STK    *)&LED2TaskStk[0],
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    OSTaskCreate((OS_TCB     *)&LED3TaskTCB,                
                 (CPU_CHAR   *)"LED3 Task",
                 (OS_TASK_PTR )LED3Task, 
                 (void       *)0,
                 (OS_PRIO     )3,
                 (CPU_STK    *)&LED3TaskStk[0],
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    OSTaskCreate((OS_TCB     *)&LED4TaskTCB,                
                 (CPU_CHAR   *)"LED4 Task",
                 (OS_TASK_PTR )LED4Task, 
                 (void       *)0,
                 (OS_PRIO     )4,
                 (CPU_STK    *)&LED4TaskStk[0],
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);


    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    CPU_TS  ts;
    

   (void)p_arg;

    OSSemCreate(&AppSem, "Test Sem", 0, &err);

    BSP_Init();                                                   /* Initialize BSP functions                         */
    CPU_Init();                                                   /* Initialize the uC/CPU services                   */

    cpu_clk_freq = BSP_CPU_ClkFreq();                             /* Determine SysTick reference freq.                */                                                                        
    cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;  /* Determine nbr SysTick increments                 */
    OS_CPU_SysTickInit(cnts);                                     /* Init uC/OS periodic time src (SysTick).          */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                                 /* Compute CPU capacity with no task running        */
#endif

    CPU_IntDisMeasMaxCurReset();
    
    BSP_LED_Off(0);

//    while (DEF_TRUE) {                                            /* Task body, always written as an infinite loop.   */
//        BSP_LED_Toggle(0);
//#if 0
//        OSTimeDlyHMSM(0, 0, 0, 100, 
////                      OS_OPT_TIME_HMSM_STRICT, 
//                      &err);
//#endif
//        OSSemPend(&AppSem,
//                  100,
//                  OS_OPT_PEND_BLOCKING,
//                  &ts,
//                  &err);
//    }
}

static  void  LED1Task (void *p_arg)
{
    OS_ERR      err;    

    while (DEF_TRUE) {
        BSP_LED_Toggle(1);
        OSTimeDlyHMSM(0, 0, 1, 0, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &err);
    }
}

static  void  LED2Task (void *p_arg)
{
    OS_ERR      err;    

    while (DEF_TRUE) {
        BSP_LED_Toggle(2);
        OSTimeDlyHMSM(0, 0, 2, 0, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &err);
    }
}

static  void  LED3Task (void *p_arg)
{
    OS_ERR      err;    

    while (DEF_TRUE) {
        BSP_LED_Toggle(3);
        OSTimeDlyHMSM(0, 0, 3, 0, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &err);
    }
}

static  void  LED4Task (void *p_arg)
{
    OS_ERR      err;    

    while (DEF_TRUE) {
        BSP_LED_Toggle(4);
        OSTimeDlyHMSM(0, 0, 4, 0, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &err);
    }
}