/*
 * v4l2.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef V4L2_H_
#define V4L2_H_

#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-int-device.h>
#include <linux/videodev2.h>

int vuser_init_video_device (struct video_device *, const char *);
void vuser_destroy_video_device (struct video_device *);

#endif /* V4L2_H_ */
