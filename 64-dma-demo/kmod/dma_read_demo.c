#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/completion.h>

#define DEV_NAME "dma_read_demo"
#define BUF_SIZE 4096

static dev_t devno;
static struct cdev cdev;
static struct class *cls;

static struct dma_chan *chan;
static dma_addr_t src_dma, dst_dma;
static char *src_buf, *dst_buf;

static DECLARE_COMPLETION_ONSTACK(dma_done);

static void dma_complete_func(void *arg)
{
    complete(&dma_done);
}

static ssize_t demo_read(struct file *f, char __user *ubuf,
                         size_t len, loff_t *off)
{
    struct dma_async_tx_descriptor *tx;
    dma_cookie_t cookie;
    size_t copy_len = min(len, (size_t)BUF_SIZE);

    /* Prepare source buffer */
    memset(src_buf, 0x41, BUF_SIZE); /* 'A' */

    reinit_completion(&dma_done);

    tx = dmaengine_prep_dma_memcpy(
            chan,
            dst_dma,
            src_dma,
            BUF_SIZE,
            DMA_CTRL_ACK | DMA_PREP_INTERRUPT);

    if (!tx)
        return -EIO;

    tx->callback = dma_complete_func;
    tx->callback_param = NULL;

    cookie = tx->tx_submit(tx);
    if (dma_submit_error(cookie))
        return -EIO;

    dma_async_issue_pending(chan);
    wait_for_completion(&dma_done);

    if (copy_to_user(ubuf, dst_buf, copy_len))
        return -EFAULT;

    return copy_len;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = demo_read,
};

static int __init demo_init(void)
{
    dma_cap_mask_t mask;

    alloc_chrdev_region(&devno, 0, 1, DEV_NAME);
    cdev_init(&cdev, &fops);
    cdev_add(&cdev, devno, 1);

    cls = class_create(DEV_NAME);
    device_create(cls, NULL, devno, NULL, DEV_NAME);

    dma_cap_zero(mask);
    dma_cap_set(DMA_MEMCPY, mask);

    chan = dma_request_channel(mask, NULL, NULL);
    if (!chan)
        return -ENODEV;

    src_buf = dma_alloc_coherent(chan->device->dev,
                                 BUF_SIZE, &src_dma, GFP_KERNEL);
    dst_buf = dma_alloc_coherent(chan->device->dev,
                                 BUF_SIZE, &dst_dma, GFP_KERNEL);

    return 0;
}

static void __exit demo_exit(void)
{
    dma_free_coherent(chan->device->dev, BUF_SIZE, src_buf, src_dma);
    dma_free_coherent(chan->device->dev, BUF_SIZE, dst_buf, dst_dma);

    dma_release_channel(chan);

    device_destroy(cls, devno);
    class_destroy(cls);
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Minimal DMA read demo");

