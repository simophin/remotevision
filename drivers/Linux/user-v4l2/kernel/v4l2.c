/*
 * v4l2.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */


#include "v4l2.h"
#include "common.h"

#define GET_VUSER_DATA (vd) \
	(container_of(vd,vuser_data,vdev))

int vuser_init_video_device (struct video_device * dev, const char * name) {
	return 0;
}


void vuser_destroy_video_device (struct video_device *dev) {

}
