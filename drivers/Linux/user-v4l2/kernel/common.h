/*
 * common.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <linux/atomic.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-int-device.h>
#include <linux/videodev2.h>


struct vuser_data {
	atomic_t ref;
	unsigned char state;
	struct video_device vdev;
};


struct vuser_data * vuser_data_alloc(void);
void vuser_data_free (struct vuser_data *);

static inline struct vuser_data * vuser_data_get (struct vuser_data * d)
	{ atomic_inc(&d->ref); return d;  }
void vuser_data_put (struct vuser_data *);

enum video_state {
	VUSER_STATE_UNINITIALIZED,
	VUSER_STATE_READY,
};

#endif /* COMMON_H_ */
