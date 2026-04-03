#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/timekeeping.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define PROC_ENTRY_NAME "kernel_lab"
#define MESSAGE_BUFFER_SIZE 256

static char greeting[MESSAGE_BUFFER_SIZE] = "hello from kernel_lab";
static int repeat = 1;
static char last_message[MESSAGE_BUFFER_SIZE] = "write something to /proc/kernel_lab";
static unsigned int write_count;
static unsigned long load_jiffies;
static DEFINE_MUTEX(kernel_lab_lock);
static struct proc_dir_entry *kernel_lab_entry;

module_param_string(greeting, greeting, sizeof(greeting), 0644);
MODULE_PARM_DESC(greeting, "Greeting text exposed by the kernel_lab module.");

module_param(repeat, int, 0644);
MODULE_PARM_DESC(repeat, "Demo integer parameter that can be changed through sysfs.");

static ssize_t kernel_lab_read(struct file *file,
                               char __user *user_buffer,
                               size_t count,
                               loff_t *position)
{
    char buffer[512];
    struct timespec64 now;
    unsigned long uptime_ms;
    ssize_t length;

    mutex_lock(&kernel_lab_lock);
    uptime_ms = jiffies_to_msecs(jiffies - load_jiffies);
    ktime_get_real_ts64(&now);

    length = scnprintf(buffer,
                       sizeof(buffer),
                       "module: kernel_lab\n"
                       "greeting: %s\n"
                       "repeat: %d\n"
                       "writes: %u\n"
                       "last_message: %s\n"
                       "uptime_ms: %lu\n"
                       "reader_pid: %d\n"
                       "reader_comm: %s\n"
                       "real_time_sec: %lld\n",
                       greeting,
                       repeat,
                       write_count,
                       last_message,
                       uptime_ms,
                       task_pid_nr(current),
                       current->comm,
                       (long long)now.tv_sec);
    mutex_unlock(&kernel_lab_lock);

    return simple_read_from_buffer(user_buffer, count, position, buffer, length);
}

static ssize_t kernel_lab_write(struct file *file,
                                const char __user *user_buffer,
                                size_t count,
                                loff_t *position)
{
    char buffer[MESSAGE_BUFFER_SIZE];
    size_t copy_length = min(count, (size_t)(MESSAGE_BUFFER_SIZE - 1));

    if (copy_from_user(buffer, user_buffer, copy_length))
        return -EFAULT;

    buffer[copy_length] = '\0';
    strim(buffer);

    mutex_lock(&kernel_lab_lock);
    strscpy(last_message, buffer, sizeof(last_message));
    write_count++;
    mutex_unlock(&kernel_lab_lock);

    pr_info("kernel_lab: updated message to \"%s\"\n", last_message);

    return count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops kernel_lab_proc_ops = {
    .proc_read = kernel_lab_read,
    .proc_write = kernel_lab_write,
};
#else
static const struct file_operations kernel_lab_proc_ops = {
    .owner = THIS_MODULE,
    .read = kernel_lab_read,
    .write = kernel_lab_write,
};
#endif

static int __init kernel_lab_init(void)
{
    load_jiffies = jiffies;
    strscpy(last_message, greeting, sizeof(last_message));

    kernel_lab_entry = proc_create(PROC_ENTRY_NAME, 0666, NULL, &kernel_lab_proc_ops);
    if (!kernel_lab_entry)
        return -ENOMEM;

    pr_info("kernel_lab: loaded, read /proc/%s to inspect the module\n", PROC_ENTRY_NAME);
    return 0;
}

static void __exit kernel_lab_exit(void)
{
    proc_remove(kernel_lab_entry);
    pr_info("kernel_lab: unloaded after %u writes\n", write_count);
}

module_init(kernel_lab_init);
module_exit(kernel_lab_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI Codex");
MODULE_DESCRIPTION("A small Linux kernel playground module with procfs and module parameters.");
MODULE_VERSION("0.1.0");
