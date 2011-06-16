/*
 * v4l2.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */


#include "v4l2.h"
#include "common.h"
#include "ioreq.h"

#define GET_VUSER_DATA(vd) \
	(container_of((vd),struct vuser_data,vdev))


static int vuser_vidioc_querycap(struct file *filp, void *fh, struct v4l2_capability *cap) {
	struct vuser_data *data = 0;
	struct ioreq * req = 0;
	struct user_response *response = 0;
	int ret = 0;


	req = kzalloc(ioreq_calc_total_size(0), GFP_KERNEL);
	if (req == NULL) return -ENOMEM;

	req->cmd = IOREQ_QUERYCAP;

	data =  GET_VUSER_DATA(video_drvdata(filp));
	ret = vuser_data_do_request_wait_response(data,req,&response,-1);

	if (ret == 0) {
		if (response->request->size != sizeof(*cap)) {
			ret = -EINVAL;
		}else{
			memcpy(cap, ioreq_get_data(response->request), response->request->size);
		}
	}

	ioreq_user_put_response(response);

	return 0;
}

static int vuser_v4l2_open (struct file *filp) {
	struct vuser_data * data = 0;

	data = GET_VUSER_DATA(video_drvdata(filp));
	data = vuser_data_get(data);

	return 0;
}

static int vuser_v4l2_release( struct file *filp) {
	struct vuser_data * data = 0;

	data = GET_VUSER_DATA(video_drvdata(filp));
	vuser_data_put(data);

	return 0;
}

static void vuser_v4l2_video_release (struct video_device *dev) {
}

static int vuser_v4l2_mmap (struct file * filp, struct vm_area_struct * vma){
	return 0;
}

const static struct v4l2_file_operations vuser_video_fops =
{
		.open 			= vuser_v4l2_open,
		.release		= vuser_v4l2_release,
		.mmap			= vuser_v4l2_mmap,

		.owner		=	THIS_MODULE,
		.ioctl		=	video_ioctl2,
};

const static struct v4l2_ioctl_ops vuser_v4l2_ioctls = {
		.vidioc_querycap = vuser_vidioc_querycap,
};

int vuser_init_video_device (struct video_device * dev, const char * name) {
	int rc;
	dev->vfl_type = VFL_TYPE_GRABBER;
	strncpy(dev->name, name, sizeof(dev->name));
	dev->name [sizeof(dev->name)-1] = 0;
	dev->ioctl_ops = &vuser_v4l2_ioctls;
	dev->fops = &vuser_video_fops;
	dev->release = vuser_v4l2_video_release;

	rc = video_register_device(dev, VFL_TYPE_GRABBER, 1);
	return rc;
}


void vuser_destroy_video_device (struct video_device *dev) {
	video_unregister_device(dev);
}
