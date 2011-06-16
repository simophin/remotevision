/*
 * file.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>



#include "common.h"
#include "control.h"
#include "ioreq.h"

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

	default: {
		return -EINVAL;
	}
	}

	return 0;
}

static unsigned int vuser_poll(struct file * filp, struct poll_table_struct * table) {
	unsigned int ret = 0;
	struct vuser_data *data = 0;

	data = (struct vuser_data *)filp->private_data;

	poll_wait(filp, &data->kernel_requests_wait, table);

	mutex_lock(&data->kernel_request_mutex);
	if (!vuser_data_is_kernel_request_empty(data)) {
		ret |= POLLIN | POLLRDNORM;
	}
	mutex_unlock(&data->kernel_request_mutex);

	return ret;
}

ssize_t vuser_read (struct file * filp, char __user *userptr, size_t size, loff_t * off) {
	ssize_t ret = 0;
	struct vuser_data *data = 0;
	struct kernel_request *req = 0;
	size_t request_size = 0;

	data = (struct vuser_data *)filp->private_data;

	mutex_lock(&data->kernel_request_mutex);
	if (!vuser_data_is_kernel_request_empty(data)) {
		req = vuser_data_shift_request(data);
	}else{
		if (!(filp->f_flags & O_NONBLOCK)) {
			mutex_unlock(&data->kernel_request_mutex);
			wait_event_interruptible(data->kernel_requests_wait,
					!vuser_data_is_kernel_request_empty(data));
			mutex_lock(&data->kernel_request_mutex);
			if (!vuser_data_is_kernel_request_empty(data)) {
					req = vuser_data_shift_request(data);
			}
		}
	}
	mutex_unlock(&data->kernel_request_mutex);

	if (req != 0) {
		request_size = ioreq_get_total_size(req->request);
		if (size >= request_size) {
			copy_to_user(userptr, (void *)req->request, request_size);
			ret = request_size;
		}

		ioreq_kernel_put_request(req);
	}


	return ret;
}

ssize_t vuser_write (struct file * filp, const char __user * userptr, size_t size, loff_t * off) {
	ssize_t ret = 0;
	struct vuser_data *data = 0;
	struct user_response *response = 0;
	struct ioreq * io = 0;

	/* Check the input data */
	if (ioreq_get_total_size(userptr) != size) {
		return -EINVAL;
	}

	data = (struct vuser_data *)filp->private_data;

	response = ioreq_user_create_response();
	if (response == NULL) return -ENOMEM;

	io = kzalloc(sizeof(*io), GFP_KERNEL);
	if (io == NULL) { ioreq_user_free_request(response); return -ENOMEM; }

	copy_from_user(io, userptr, size);
	ret = size;

	ioreq_user_get_response(response);
	response->request = io;
	vuser_data_append_response(data, response);
	ioreq_user_put_response(response);

	return ret;
}

static int vuser_mmap (struct file * filp, struct vm_area_struct * vma) {
	return 0;
}

static struct file_operations vuser_fops = {
	.open		= vuser_open,
	.read		= vuser_read,
	.release	= vuser_release,
	.mmap		= vuser_mmap,
	.poll		= vuser_poll,
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
