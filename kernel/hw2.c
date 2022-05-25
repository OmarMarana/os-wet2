
#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include "sched/sched.h"

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_set_weight(int weight) {
    if(weight<0){
        return -EINVAL;
    }
    struct task_struct* curr = current;
    curr->weight=weight;
    return 0;
}

/////////////////////////////////////////////

asmlinkage long sys_get_weight(void) {
	struct task_struct* curr = current;
	return curr->weight;
}

/////////////////////////////////////////////

int get_total_weight_aux(struct task_struct* curr){
	int sum=0;
    if(list_empty(&(curr->children))){
        return sum+curr->weight;
    }
	struct list_head* list;
	list_for_each(list, &curr->children) {
		struct task_struct* task;
        	task = list_entry(list, struct task_struct, sibling);
            /* task now points to one of current’s children */
             
            sum+=get_total_weight_aux(task);
    	}
	return sum;
}

asmlinkage long sys_get_leaf_children_sum(void) {
    if(list_empty(&(current->children))){
        return -ECHILD;
    }
    int total_sum=0;
        struct task_struct* curr = current;
    total_sum=get_total_weight_aux(curr);
        return total_sum;
}



/////////////////////////////////////////////

int get_max_ancestor_weight_aux(struct task_struct* curr){
	int max_weight = curr->weight;
	pid_t max_pid = curr->pid;
	while(curr->pid != 1){
		if(curr->weight > max_weight){
			max_weight=curr->weight;
			max_pid=curr->pid;
		}
		curr=curr->real_parent;
	}
	return max_pid;
}

asmlinkage long sys_get_heaviest_ancestor(void) {
	struct task_struct* curr = current;
        int max_pid = get_max_ancestor_weight_aux(curr);
	return max_pid;
}

