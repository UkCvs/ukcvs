/*
 * $Id: avia_gt_lirc.c,v 1.14 2003/09/30 05:45:35 obi Exp $
 *
 * lirc ir driver for AViA eNX/GTX (dbox-II-project)
 *
 * Homepage: http://www.tuxbox.org
 *
 * Copyright (C) 2002 Florian Schirmer (jolt@tuxbox.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/devfs_fs_kernel.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/wait.h>

#include "avia_gt_ir.h"
#include <dbox/lirc.h>

#ifndef CONFIG_DEVFS_FS
#error no devfs
#endif

static devfs_handle_t devfs_handle;
static lirc_t pulse_buffer[AVIA_GT_IR_MAX_PULSE_COUNT * 2];

static int avia_gt_lirc_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned long value = 0;
	int result = 0;

	switch (cmd) {
	case LIRC_GET_FEATURES:
		return put_user(LIRC_CAN_REC_MODE2 | LIRC_CAN_SEND_PULSE | LIRC_CAN_SET_SEND_CARRIER | LIRC_CAN_SET_SEND_DUTY_CYCLE, (unsigned long *)arg);

	case LIRC_GET_REC_MODE:
		return put_user(LIRC_MODE_MODE2, (unsigned long *)arg);

	case LIRC_GET_SEND_MODE:
		return put_user(LIRC_MODE_PULSE, (unsigned long *)arg);

	case LIRC_SET_REC_MODE:
		result = get_user(value, (unsigned long *)arg);
		if (result)
			return result;
		if (value != LIRC_MODE_MODE2)
			return -EINVAL;
		break;

	case LIRC_SET_SEND_CARRIER:
		result = get_user(value, (unsigned long *)arg);
		if (result)
			return result;
		avia_gt_ir_set_frequency(value);
		break;

	case LIRC_SET_SEND_DUTY_CYCLE:
		result = get_user(value, (unsigned long *)arg);
		if (result)
			return result;
		avia_gt_ir_set_duty_cycle(value);
		break;

	case LIRC_SET_SEND_MODE:
		result = get_user(value, (unsigned long *)arg);
		if (result)
			return result;
		if (value != LIRC_MODE_PULSE)
			return -EINVAL;
		break;

	default:
		return -ENOIOCTLCMD;
	}

	return 0;
}

static u8 got_pulse;

static unsigned int avia_gt_lirc_poll(struct file *file, poll_table *wait)
{
	if ((avia_gt_ir_get_rx_buffer_read_position() == avia_gt_ir_get_rx_buffer_write_position()) && (!got_pulse))
		poll_wait(file, avia_gt_ir_receive_data(), wait);

	if ((avia_gt_ir_get_rx_buffer_read_position() != avia_gt_ir_get_rx_buffer_write_position()) || (got_pulse))
		return (POLLIN | POLLRDNORM);
	else
		return 0;
}

static u32 pulse_len;
//static u8 got_next;
//static u32 next_high;
//static u32 next_low;

static ssize_t avia_gt_lirc_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
	u32 period_high = 0;
	u32 period_low = 0;
//	u32 rand_val;
	int result = 0;
	u32 done = 0;
	lirc_t *rx_buffer = (lirc_t *)buf;

	if (count % sizeof(lirc_t))
		return -EINVAL;

	while ((done * sizeof(lirc_t)) < count) {
	
		if (got_pulse) {
			rx_buffer[done] = pulse_len | PULSE_BIT;
			got_pulse = 0;
		}
		else {
/*			if (got_next) {
			
				period_low = next_low;
				period_high = next_high;
				
				got_next = 0;
			
			} else {*/
				if ((result = avia_gt_ir_receive_pulse(&period_low, &period_high, !(file->f_flags & O_NONBLOCK))))
					return result;
//			}

/*			if ((period_high > 350) && (period_high < 750))
				period_high = 562;

			if ((period_high > 8000) && (period_high < 10000))
				period_high = 9000;

			if ((period_low > 350) && (period_low < 750))
				period_low = 562;

			if ((period_low > 1400) && (period_low < 1900))
				period_low = 1687;

			if ((period_low > 1900) && (period_low < 2500))
				period_low = 2250;

			if ((period_low > 3500) && (period_low < 5500))
				period_low = 4500;
*/			
/*			if (period_low > 16000) {
			
				get_random_bytes(&rand_val, sizeof(rand_val));
				//period_low = rand_val * 100 + 150000;
				//period_low = 120000;
				
				if (avia_gt_ir_get_rx_read_position() == avia_gt_ir_get_rx_buffer_position())
					udelay(7500);
				
				if (avia_gt_ir_get_rx_read_position() != avia_gt_ir_get_rx_buffer_position()) {

					if ((result = avia_gt_ir_receive_pulse(&next_low, &next_high, 1)))
						return result;
					
					got_next = 1;
				
				//	if ((next_low > 1500) && (next_low < 3000))
				//		period_low = 95000 + ((jiffies << 3) & 0xFFF);

				//	if ((next_low > 3000) && (next_low < 5500))
				//		period_low = 600000 + ((jiffies << 2) & 0xFFFF);
						
				} else {
				
					printk("avia_gt_lirc: Sigh!\n");
					
				}
				
			}*/

			got_pulse = 1;
			
			if (period_high & ~PULSE_MASK)
				pulse_len = PULSE_MASK;
			else
				pulse_len = period_high;

			if (period_low & ~PULSE_MASK)
				rx_buffer[done] = PULSE_MASK;
			else
				rx_buffer[done] = period_low;
		}

		done++;
	}

	return (done * sizeof(lirc_t));
}

static ssize_t avia_gt_lirc_write(struct file *file, const char *buf, size_t count, loff_t *offset)
{
	u32 pulse_count = 0;
	u32 pulse_nr = 0;
	int result = 0;

	if (count % sizeof(lirc_t))
		return -EINVAL;

	if (count > (AVIA_GT_IR_MAX_PULSE_COUNT * sizeof(lirc_t)))
		count = AVIA_GT_IR_MAX_PULSE_COUNT * sizeof(lirc_t);

	if (copy_from_user(pulse_buffer, buf, count))
		return -EFAULT;

	pulse_count = count / sizeof(lirc_t);

	for (pulse_nr = 0; pulse_nr < pulse_count; pulse_nr += 2)
		if ((result = avia_gt_ir_queue_pulse(pulse_buffer[pulse_nr], pulse_buffer[pulse_nr + 1], !(file->f_flags & O_NONBLOCK))))
			return result;

	return avia_gt_ir_send_buffer(!(file->f_flags & O_NONBLOCK));
}

static struct file_operations avia_gt_lirc_fops = {
	.owner = THIS_MODULE,
	.ioctl = avia_gt_lirc_ioctl,
	.poll = avia_gt_lirc_poll,
	.read = avia_gt_lirc_read,
	.write = avia_gt_lirc_write,
};

static int __init avia_gt_lirc_init(void)
{
	printk(KERN_INFO "avia_gt_lirc: $Id: avia_gt_lirc.c,v 1.14 2003/09/30 05:45:35 obi Exp $\n");

	if (avia_gt_ir_init() < 0)
		return -EIO;

	devfs_handle = devfs_register(NULL, "lirc", DEVFS_FL_DEFAULT, 0, 0, S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, &avia_gt_lirc_fops, NULL);

	if (!devfs_handle) {
		printk(KERN_ERR "avia_gt_lirc: devfs_register failed\n");
		return -EIO;
	}

	avia_gt_ir_set_frequency(38000);
	avia_gt_ir_set_duty_cycle(33);

	return 0;
}

static void __exit avia_gt_lirc_exit(void)
{
	devfs_unregister (devfs_handle);
	avia_gt_ir_exit();
}

module_init(avia_gt_lirc_init);
module_exit(avia_gt_lirc_exit);

MODULE_AUTHOR("Florian Schirmer <jolt@tuxbox.org>");
MODULE_DESCRIPTION("AViA eNX/GTX LIRC driver");
MODULE_LICENSE("GPL");
