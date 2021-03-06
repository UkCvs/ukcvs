#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>

/* needed for 2.4/makelinks 2.6 stuff, don't submit
   to mainline kernel */
#include <linux/version.h>

void dvb_kernel_thread_setup (const char *thread_name)
{
        lock_kernel ();

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,61))
        daemonize ();
	strncpy (current->comm, thread_name, sizeof(current->comm));
#else
        daemonize (thread_name);
#endif

/*      not needed anymore in 2.5.x, done in daemonize() */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))
        reparent_to_init();
#endif

        sigfillset (&current->blocked);
        unlock_kernel ();
}

/* if the miracle happens and "generic_usercopy()" is included into
   the kernel, then this can vanish. please don't make the mistake and
   define this as video_usercopy(). this will introduce a dependecy
   to the v4l "videodev.o" module, which is unnecessary for some
   cards (ie. the budget dvb-cards don't need the v4l module...) */
int dvb_usercopy(struct inode *inode, struct file *file,
	             unsigned int cmd, unsigned long arg,
		     int (*func)(struct inode *inode, struct file *file,
		     unsigned int cmd, void *arg))
{
        char    sbuf[128];
        void    *mbuf = NULL;
        void    *parg = NULL;
        int     err  = -EINVAL;

        /*  Copy arguments into temp kernel buffer  */
        switch (_IOC_DIR(cmd)) {
        case _IOC_NONE:
                parg = (void *)arg;
                break;
        case _IOC_READ: /* some v4l ioctls are marked wrong ... */
        case _IOC_WRITE:
        case (_IOC_WRITE | _IOC_READ):
                if (_IOC_SIZE(cmd) <= sizeof(sbuf)) {
                        parg = sbuf;
                } else {
                        /* too big to allocate from stack */
                        mbuf = kmalloc(_IOC_SIZE(cmd),GFP_KERNEL);
                        if (NULL == mbuf)
                                return -ENOMEM;
                        parg = mbuf;
                }

                err = -EFAULT;
                if (copy_from_user(parg, (void *)arg, _IOC_SIZE(cmd)))
                        goto out;
                break;
        }

        /* call driver */
        if ((err = func(inode, file, cmd, parg)) == -ENOIOCTLCMD)
                err = -EINVAL;

        if (err < 0)
                goto out;

        /*  Copy results into user buffer  */
        switch (_IOC_DIR(cmd))
        {
        case _IOC_READ:
        case (_IOC_WRITE | _IOC_READ):
                if (copy_to_user((void *)arg, parg, _IOC_SIZE(cmd)))
                        err = -EFAULT;
                break;
        }

out:
        if (mbuf)
                kfree(mbuf);

        return err;
}

EXPORT_SYMBOL(dvb_usercopy);
EXPORT_SYMBOL(dvb_kernel_thread_setup);
