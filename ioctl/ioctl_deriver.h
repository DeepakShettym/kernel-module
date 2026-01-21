#ifndef _AVNG_IOCTL_H_
#define _AVNG_IOCTL_H_

#include <linux/ioctl.h>

#ifdef __KERNEL__
    #include <linux/types.h>
#else
    #include <stdint.h>
#endif


/* Magic number */
#define AVNG_MAGIC 'a'

/* IOCTL commands */
#define WR_VALUE _IOW(AVNG_MAGIC, 1, int32_t)
#define RD_VALUE _IOR(AVNG_MAGIC, 2, int32_t)

#endif /* _AVNG_IOCTL_H_ */

