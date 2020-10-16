#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/* performs a depth-first traversal of the list of tasks in the system. */
void traverse_DFS(struct task_struct *ptr) {
/* fill in your code */
	struct list_head *list;
	struct task_struct *task;

	printk(KERN_INFO "[ ppid - %6d : pid - %6d : name - %s ]\n", ptr->parent->pid, ptr->pid, ptr->comm);
	list_for_each(list, &ptr->children){
		task = list_entry(list, struct task_struct, sibling);
		traverse_DFS(task);
	}
	if ((&ptr->children)->next == &(ptr->children)){
		return;
	}
	
}

void traverse_BFS(struct task_struct *ptr) {
/* fill in your code */
	struct list_head *list;
	struct list_head *list_;

	struct task_struct *task = list_entry(&ptr->children, struct task_struct, sibling);	
	struct task_struct *task_ = ptr;
	struct task_struct *task__ = ptr;

	struct task_struct *tasks[100];

	int wp, rp;
	wp=rp=0;
	
	list_for_each(list, &task -> sibling)
	{
		task_ = list_entry(list, struct task_struct, sibling);
		tasks[wp++] = task_;
		list_for_each(list_, &task_->children)
		{
			task__ = list_entry(list_, struct task_struct, sibling); 
			printk(KERN_INFO "[ ppid - %6d : pid - %6d : name - %s ]\n", task__->parent->pid, task__->pid, task__->comm);
		}
	}
	while(rp < wp) traverse_BFS(tasks[rp++]);
}

int simple_init(void)
{
	struct task_struct *task;
	struct list_head *list;
	
	struct task_struct *c;

        printk(KERN_INFO "Loading Module\n");

	traverse_DFS(&init_task);


       return 0;
}

void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

	struct list_head *list;
	struct task_struct *task;
	printk(KERN_INFO "[ ppid - %6d : pid - %6d : name - %s ]\n", init_task.parent->pid, init_task.pid, init_task.comm);
	list_for_each(list, &(init_task.children))
	{
		task = list_entry(list, struct task_struct, sibling);
		printk(KERN_INFO "[ ppid - %6d : pid - %6d : name - %s ]\n", task->parent->pid, task->pid, task->comm);
	}
	traverse_BFS(&init_task);
}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Module");
MODULE_AUTHOR("SGG");

