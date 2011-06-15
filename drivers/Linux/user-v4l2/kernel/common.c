/*
 * common.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#include "common.h"

#include <linux/kernel.h>
#include <linux/slab.h>


struct vuser_data * vuser_data_alloc(void) {
	struct vuser_data *ret;

	ret = kzalloc(sizeof (*ret), GFP_KERNEL);
	atomic_set(&ret->ref,0);
	ret->state = VUSER_STATE_UNINITIALIZED;

	return ret;
}


void vuser_data_free (struct vuser_data * data) {
	kfree((void *)data);
}

void vuser_data_put (struct vuser_data * d) {
	if (!atomic_dec_and_test(&d->ref)) {
		vuser_data_free (d);
	}
}
