/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

 #include "common.h"
 #include "syscall.h"
 #include "stdio.h"
 #include "libmem.h"
 #include "queue.h"
 #include <string.h> // để dùng strcmp()
 #include<pthread.h>
 static pthread_mutex_t sys_lock = PTHREAD_MUTEX_INITIALIZER;
 int __sys_killall(struct pcb_t *caller, struct sc_regs *regs)
 {
    pthread_mutex_lock(&sys_lock);
         char proc_name[100];
     uint32_t data;
 
     // hardcode for demo only
     uint32_t memrg = regs->a1;
     printf("Killall syscall: memrg = %d\n", memrg);
 
     /* TODO: Get name of the target proc */
     // proc_name = libread..
     int i = 0;
     data = 0;
     while (data != -1)
     {
         libread(caller, memrg, i, &data);
         // printf(data);
         proc_name[i] = data;
         if (data == -1)
             proc_name[i] = '\0';
         i++;
     }
     printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);
 
     /* TODO: Traverse proclist to terminate the proc
      *       stcmp to check the process match proc_name
      */
     // caller->running_list
     // caller->mlq_ready_queu
 
     /* TODO Maching and terminating
      *       all processes with given
      *        name in var proc_name
      */
     struct queue_t *running_list = caller->running_list;
     struct queue_t *mlq_ready_queue = caller->mlq_ready_queue;
     struct queue_t *ready_queue = caller->ready_queue;
     for (int i = 0; i < running_list->size; i++)
     {
         struct pcb_t *proc = running_list->proc[i];
         uint32_t name_memrgid = -1;
         for (int j = 0; j < proc->code->size; j++)
         {
 
             name_memrgid = proc->code->text[j].arg_1; // hoặc giá trị cụ thể cho mỗi process
         }
         if (name_memrgid == memrg)
         {
             printf("Terminate process %s\n", proc->path);
             running_list->size--;
             for (int k = i; k < running_list->size; k++)
             {
                 running_list->proc[k] = running_list->proc[k + 1];
             }
             running_list->proc[running_list->size] = NULL;
         }
     }
     for (int i = 0; i < mlq_ready_queue->size; i++)
     {
         // printf("MLQ ready queue %d: %s\n", i, mlq_ready_queue->proc[i]->path);
         struct pcb_t *proc = mlq_ready_queue->proc[i];
         uint32_t name_memrgid = -1;
         for (int j = 0; j < proc->code->size; j++)
         {
             name_memrgid = proc->code->text[j].arg_1; // hoặc giá trị cụ thể cho mỗi process
         }
         if (name_memrgid == memrg)
         {
             printf("Terminate process %s\n", proc->path);
             mlq_ready_queue->size--;
             for (int k = i; k < mlq_ready_queue->size; k++)
             {
                 mlq_ready_queue->proc[k] = mlq_ready_queue->proc[k + 1];
             }
             mlq_ready_queue->proc[mlq_ready_queue->size] = NULL;
         }
     }
 
     for (int i = 0; i < ready_queue->size; i++)
     {
         // printf("Ready queue %d: %s\n", i, ready_queue->proc[i]->path);
         struct pcb_t *proc = ready_queue->proc[i];
         uint32_t name_memrgid = -1;
         for (int j = 0; j < proc->code->size; j++)
         {
             name_memrgid = proc->code->text[j].arg_1; // hoặc giá trị cụ thể cho mỗi process
         }
         if (name_memrgid == memrg)
         {
             printf("Terminate process %s\n", proc->path);
             ready_queue->size--;
             for (int k = i; k < ready_queue->size; k++)
             {
                 ready_queue->proc[k] = ready_queue->proc[k + 1];
             }
             ready_queue->proc[ready_queue->size] = NULL;
         }
     }
     pthread_mutex_unlock(&sys_lock);
     return 0;
 }
 