#ifndef EXAM_ALERT_H
#define EXAM_ALERT_H

static int shutdown_thread(void *data);
static void get_time_string(char *buf, size_t size);
static int usb_notify(struct notifier_block *nb, unsigned long action, void *data);
static int fb_notify(struct notifier_block *nb, unsigned long event, void *data);
int exam_alert_init(void);
void exam_alert_exit(void);

//extern struct notifier_block usb_nb; 
//extern  struct notifier_block fb_nb;

#endif