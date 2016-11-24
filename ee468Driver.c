#include <linux/init.h>
//#include <linux/config.h>  // see http://askubuntu.com/a/295142
//#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h> 	/* printk() */
#include <linux/slab.h> 	/* kmalloc() */
#include <linux/fs.h> 		/* everything... */
#include <linux/errno.h> 	/* error codes */
#include <linux/types.h> 	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> 	/* O_ACCMODE */
//#include <asm/system.h> 	/* cli(), *_flags */  see http://superuser.com/a/783289
#include <asm/uaccess.h> 	/* copy_from/to_user */

#define MODNAME "ee468Driver"

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  read: memory_read,
  write: memory_write,
  open: memory_open,
  release: memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);
/* Global variables of the driver */
/* Major number */
int memory_major = 60;
/* Buffer to store data */
char *memory_buffer;



/**
 * Memory Init Module
 */
int memory_init(void) {
	int result;
	/* Registering device */
	// to make a corresponding device node use console command $mknod /dev/memory c 60 0
	result = register_chrdev(memory_major, MODNAME, &memory_fops);
	if (result < 0) {
		printk("<1>memory: cannot obtain major number %d\n", memory_major);
		return result;
	}

	/* Allocating memory for the buffer */
	memory_buffer = kmalloc(1, GFP_KERNEL);
	if (!memory_buffer) {
		result = -ENOMEM;
		goto fail;
	}
	memset(memory_buffer, 0, 1);
	printk("<1>Inserting %s module\n", MODNAME);
	return 0;

	fail:
		memory_exit();
		return result;
}



/**
 * Memory Exit Module
 */
void memory_exit(void) {
	/* Freeing the major number */
	unregister_chrdev(memory_major, MODNAME);

	/* Freeing buffer memory */
	if (memory_buffer) {
		kfree(memory_buffer);
	}
	printk("<1>Removing %s module\n", MODNAME);
}



/**
 * Memory Open
 */
int memory_open(struct inode *inode, struct file *filp) {
	/* Success */
	return 0;
}



/**
 * Memory Release
 */
int memory_release(struct inode *inode, struct file *filp) {
	/* Success */
	return 0;
}



/**
 * Memory Read
 */
ssize_t memory_read(struct file *filp, char *buf,
                    size_t count, loff_t *f_pos) {
  /* Transfering data to user space */
  copy_to_user(buf,memory_buffer,1);
  /* Changing reading position as best suits */
  if (*f_pos == 0) {
	  *f_pos+=1;
	  return 1;
  } else {
	  return 0;
  }
}



/**
 * Memory Write
 */
ssize_t memory_write( struct file *filp, char *buf,
                      size_t count, loff_t *f_pos) {
	char *tmp;
	tmp=buf+count-1;
	copy_from_user(memory_buffer,tmp,1);
	return 1;
}























