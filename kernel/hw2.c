
#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include "sched/sched.h"
#include <linux/slab.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_set_status(int status) { // what about errno????
    if(!(status == 0 || status == 1 )){
        return -EINVAL;
    }
    struct task_struct* curr = current;
    curr->faculty=status;
    return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_get_status(void) {
	struct task_struct* curr = current;
	return curr->faculty;
}

/////////////////////////////////////////////

asmlinkage long sys_register_process(void)
{
    struct list_head* list;
    struct task_struct* init_process;
    struct task_struct* task;
    int num_tasks;
    num_tasks = 0;
    init_process = find_task_by_vpid(1);

    if(init_process->recognized_size >=1)
    {
        list_for_each(list, &init_process->recognized) 
        {
            
            if(num_tasks >= init_process->recognized_size)
            {
                break;
            }
            task = list_entry(list, struct task_struct, recognized); //maybe sibling is a bug
            if(task->pid == current->pid)
            {
                return 0;
            }
            num_tasks++;
        }
        
        //how to add to the list_head???
    }
	list_add(&current->recognized,&init_process->recognized); // maybe bugged bcus of invalid use of list 
    init_process->recognized_size++;
	return 0;

    // return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_get_all_cs(void)
{
    struct list_head* list;
    struct task_struct* init_process;
    struct task_struct* task;
    long cs_pids;
    int num_tasks;
    num_tasks = 0;
    init_process = find_task_by_vpid(1);
    if(init_process->recognized_size == 0)
    {
        return -ENODATA;
    }

    cs_pids = 0;

    list_for_each(list, &init_process->recognized) 
    {
        num_tasks++;
        if(num_tasks > init_process->recognized_size)
        {
            break;
        }
        task = list_entry(list, struct task_struct, recognized); //maybe sibling is a bug

        if(task->faculty == 1)
        {   
            cs_pids += task->pid;
        }
        // cs_pids += task->pid;
    }
    return cs_pids;

    // return 0;

}



/////////////////////////////////////////////



