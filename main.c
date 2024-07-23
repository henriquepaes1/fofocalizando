
#include <stdio.h>
#include "bcm.h"
#include "sched.h"

/*
 * Ponto de entrada do sistema.
 */
void system_main(void) {
   sched_init();
   asm volatile ("b task_switch");           // transfere o controle ao primeiro thread
}

/*
 * Ponto de entrada do primeiro task.
 */
void user1_main(void) {
   int i;
   printf("to na task 1");
   for(;;) {
      for(i=0; i<100; i++) {
         asm volatile ("nop");
         yield();
      }
   }
}

/*
 * Ponto de entrada do segundo task.
 */
void user2_main(void) {
   int i;
   printf("to na task 2");
   for(;;) {
      for(i=0; i<150; i++) {
         asm volatile ("nop");
         yield();
      }
   }
}

