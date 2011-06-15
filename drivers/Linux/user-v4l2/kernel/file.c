/*
 * file.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#include <linux/module.h>
#include <linux/fs.h>


#include "common.h"
#include "control.h"

#define VUSER_MAJOR 40
#define VUSER_DEVICE_NAME "user-v4l2"


static int vuser_open (struct inode *i, struct file *filp) {
	struct vuser_data *data;

	if ( (data = vuser_data_alloc()) == NULL) {
		return -ENOMEM;
	}

	vuser_data_get(data);
	filp->private_data = (void *)data;
	return 0;
}

static int vuser_release(struct inode *i, struct file *filp) {
	struct vuser_data *data;

	// TODO: Clean up task
	data = (struct vuser_data *) filp->private_data;
	vuser_data_put(data);
	return 0;
}

static long vuser_ioctl (struct file * filp, unsigned int cmd, unsigned long param) {
	struct vuser_data *data;

	data = (struct vuser_data *)filp->private_data;

	switch (cmd) {
	case VUSER_INIT: {
		if (data->state != VUSER_STATE_UNINITIALIZED) {
			return -EINVAL;
		}
		break;
	}
	case VUSER_DESTROY: {
		break;
	}
	default: {
		return -EINVAL;
	}
	}

	return 0;
}

static int vuser_mmap (struct file * filp, struct vm_area_struct * vma) {
	return 0;
}

static struct file_operations vuser_fops = {
	.open		= vuser_open,
	.release	= vuser_release,
	.mmap		= vuser_mmap,
	.compat_ioctl		= vuser_ioctl
};

static  int vuser_module_init (void) {
	int rc = 0;
	if ( (rc=register_chrdev(VUSER_MAJOR,VUSER_DEVICE_NAME,&vuser_fops)) != 0) {
		return rc;
	}

	return rc;
}

static void vuser_module_exit(void) {
	unregister_chrdev(VUSER_MAJOR, VUSER_DEVICE_NAME);
}

module_init(vuser_module_init);
module_exit(vuser_module_exit);
MODULE_AUTHOR("Fanchao Liu (admin@simophin.net)");
MODULE_LICENSE("GPL");
