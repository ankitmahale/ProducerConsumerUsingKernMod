#include<linux/module.h>
#include<linux/string.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/semaphore.h>
#include<linux/delay.h>
#include<linux/miscdevice.h>

struct semaphore mutex;
struct semaphore full;
struct semaphore empty;

MODULE_LICENSE("GPL");	

int no_of_strings=20;
static char* string;
static int no_of_times=0;
static int size=0;


//open the device and keeps count of how many times the device is opened
static int my_open(struct inode *inod, struct file *fil)
{
	
	printk(KERN_ALERT"%d no_of_times the file was opened\n",no_of_times++);
	return 0;
}
//performs the read operation
static ssize_t my_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	int iterator1=0,iterator2=0,ret_val;
	while(iterator1<len)
	{
		//if the operation is interrupted, the function returns a nonzero value	
		if(down_interruptible(&full)<0)
		{
			printk(KERN_ALERT "READ INTERRUPT FULL");
			return -1;
		}
		//if the operation is interrupted, the function returns a nonzero value	
		if(down_interruptible(&mutex)<0)
		{
			printk(KERN_ALERT "READ INTERRUPT MUTEX");
			return -1;
		}
		ret_val = copy_to_user(buff+iterator1,&string[0],1);
                 
                if(ret_val<0)
                {
			printk(KERN_ALERT "Copy to user failed\n");
                        return ret_val;
 		}
               
                iterator1=iterator1+1;
		for(iterator2=0;iterator2<size;iterator2++)
		{
			string[iterator2]= string[iterator2+1];
		}
                size=size-1;
		up(&mutex);
		up(&empty);
	  
	}
	string[size+1] = '\0';
	return size;
}

//performs the write operation
static ssize_t my_write(struct file *filp,const char *buff, size_t len, loff_t *off)
{
	int iterator1=0,ret_val;
	if(size<0)
	size = 0;
	
	for(iterator1=0;iterator1<len; iterator1++)
	{
		//if the operation is interrupted, the function returns a nonzero value		
		if(down_interruptible(&empty)<0)
		{
			printk(KERN_ALERT "WRITE INTERRUPT EMPTY");
			return -1;
		}
		//if the operation is interrupted, the function returns a nonzero value	
		if(down_interruptible(&mutex)<0)
		{
			printk(KERN_ALERT "WRITE INTERRUPT MUTEX");
			return -1;
		}
		ret_val = copy_from_user(&string[size],&buff[iterator1],1);
                 if(ret_val<0)
                {
			printk(KERN_ALERT "Copy from user failed\n");
                        return ret_val;
 		}
                size=size+1;
		up(&mutex);
                up(&full);
                

	}
	return size;
}
//this realeases the device
static int my_release(struct inode *inod, struct file *fil)
{
	printk(KERN_ALERT"CLOSED\n");
	return 0;
}

static struct file_operations my_fops =
{
        .owner= THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_release,
        .llseek=noop_llseek
};
//device struct
static struct miscdevice my_misc_device =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mypipe",
	.fops = &my_fops
};
//register the device with kernel 
int __init init_module(void)
{
	int retval=misc_register(&my_misc_device);//register the device
	if(retval>=0){
		printk(KERN_ALERT "Successfully registered the device\n");		
	}
	no_of_strings=no_of_strings*5-4;
        sema_init(&mutex,1);
	sema_init(&full,0);
	sema_init(&empty,no_of_strings);	
        string = kmalloc(no_of_strings, GFP_KERNEL);
        if(!string)
     	{     
                printk(KERN_ALERT "Cannot allocate the memory");
	}
	
	return retval;
}
//unregister the device when removing the module
void __exit cleanup_module(void)
{
	kfree(string);
        printk(KERN_ALERT "Device Exited\n");
	misc_deregister(&my_misc_device);//unregister the device
}



