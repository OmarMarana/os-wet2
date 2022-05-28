
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
    printk("entered sys_set_status \n");
    struct task_struct* curr = current;
    curr->faculty=status;
    struct list_head* list;
    struct task_struct* init_process;
    struct s_task_node* task;
    int num_tasks;
    num_tasks = 0;
    init_process = find_task_by_vpid(1);
    if(init_process->recognized_size >=1 )
    {
        printk("entered sys_set_status and rec.size isnt 0\n");
        list_for_each(list, &init_process->recognized) 
        {
            printk("entered sys_set_status and rec.size isnt 0 enterd foreach\n");
            num_tasks++;
            if(num_tasks > init_process->recognized_size)
            {
                break;
            }
            task = list_entry(list, struct s_task_node, sibling); //maybe sibling is a bug
            printk("task num %d in list is %d  \n",num_tasks,task->pid);
            // printk("entered sys_set_status and rec.size isnt 0 enterd foreach curr pid is %d \n",current->pid);
            if(task->pid == current->pid)
            {
                printk("status to be set is %d \n",status);
                printk("task->faculty before supposed update %d \n",task->faculty);
                // task->faculty = -1;
                task->faculty = status;
                printk("task->faculty after supposed update %d \n",task->faculty);
                return 0;
            }

        }
        
        //how to add to the list_head???
    }
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
    struct s_task_node* task;
    int num_tasks;
    num_tasks = 0;
    init_process = find_task_by_vpid(1);
    //check if its already in the list

    if(init_process->recognized_size >=1)
    {
        list_for_each(list, &init_process->recognized) 
        {
            
            if(num_tasks >= init_process->recognized_size)
            {
                break;
            }
            task = list_entry(list, struct s_task_node, sibling); //maybe sibling is a bug
            if(task->pid == current->pid)
            {
                return 0;
            }
            num_tasks++;
        }
        
        //how to add to the list_head???
    }
    // struct s_task_node* new_task = kmalloc(sizeof(*new_task), GFP_KERNEL);
    struct s_task_node* new_task = kmalloc(sizeof(struct s_task_node), GFP_KERNEL); 
    new_task->faculty = 0; 
    new_task->pid = current->pid;
    printk("registering process pid : %d \n",new_task->pid);
	list_add(&new_task->sibling,&init_process->recognized); // maybe bugged bcus of invalid use of list 
    init_process->recognized_size++;
	return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_get_all_cs(void)
{
    struct list_head* list;
    struct task_struct* init_process;
    struct s_task_node* task;
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
        task = list_entry(list, struct s_task_node, sibling); //maybe sibling is a bug
        if(task->faculty == 1)
        {   
            cs_pids += task->pid;
        }
        // cs_pids += task->pid;
    }
    return cs_pids;

}



/////////////////////////////////////////////



