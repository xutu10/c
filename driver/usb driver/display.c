#include <linux/usb.h>
#include <linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>           //kmalloc

#define VENDOR_ID 0x10cf
#define PRODUCT_ID 0x8101
#define MIN(a,b) ((a <= b) ? a : b)


static const struct usb_device_id id_table[] = {
	{USB_DEVICE(VENDOR_ID,PRODUCT_ID)},{},	
};

MODULE_DEVICE_TABLE(usb,id_table);

struct usb_display {
	struct usb_device *udev;
	unsigned char pixel;
	unsigned char line;
	unsigned char square;
	unsigned char text;
	unsigned char clearall;
};

//static struct usb_display *dev = NULL;      // global variable
//!!! doesn't work at macro

static unsigned char bulk_buf[100];

static unsigned char check_num(int size){
	int ret = 0;
	int i;
	for(i = 1; i < size; i++)
		ret += bulk_buf[i];

	ret = ret%256;	
	
	return ret;	
}

static void send_bulk_msg(struct usb_display* dev, int size){

	int act_len;
	int retval = 0;

	retval = usb_bulk_msg(dev->udev,usb_sndbulkpipe(dev->udev,0x02),
						  bulk_buf,MIN(size,100),&act_len,5000);
	if(retval < 0)
		printk(KERN_ERR "transfer error");
	if(act_len != size)
		printk(KERN_ERR "wrong size is transfered");
}
	


static void draw_pixel(struct usb_display* dev, const char* buf){

	bulk_buf[0] =0xAA;
	bulk_buf[1] = 0x08;
	bulk_buf[2] = 0x00;
	bulk_buf[3] = 0x09;
	sscanf(buf,"%d %d",&bulk_buf[4],&bulk_buf[5]);
	bulk_buf[6] = check_num(6);
	bulk_buf[7] = 0x55;

	send_bulk_msg(dev,8);
}

static void draw_line(struct usb_display* dev,const char* buf){

	bulk_buf[0] = 0xAA;
	bulk_buf[1] = 0x10;
	bulk_buf[2] = 0x00;
	bulk_buf[3] = 0x12;
	sscanf(buf,"%d %d %d %d",&bulk_buf[4],&bulk_buf[5],&bulk_buf[6],
		   &bulk_buf[7]);
	bulk_buf[8] = check_num(8);
	bulk_buf[9] = 0x55;

	send_bulk_msg(dev,10);
}

static void draw_square(struct usb_display* dev,const char* buf){

	bulk_buf[0] = 0xAA;
	bulk_buf[1] = 0x10;
	bulk_buf[2] = 0x00;
	bulk_buf[3] = 0x07;
	sscanf(buf,"%d %d %d %d",&bulk_buf[4],&bulk_buf[5],&bulk_buf[6],
		   &bulk_buf[7]);
	bulk_buf[8] = check_num(8);
	bulk_buf[9] = 0x55;

	send_bulk_msg(dev,10);
}


static void send_text(struct usb_display* dev,const char* buf,size_t count){

	int i = 0;
	bulk_buf[0] = 0xAA;
	bulk_buf[1] = 0xA1;
	bulk_buf[2] = 0x00;
	bulk_buf[3] = 0x05;
	bulk_buf[4] = 0x00;
	bulk_buf[5] = 0x00;
	bulk_buf[6] = 0x80;

	for(i = 0;i < count;i++)
		bulk_buf[7 + i] = buf[i];

	bulk_buf[7 + count] = 0x00;
	bulk_buf[7 + count + 1] = check_num(7+count+1);
	bulk_buf[7 + count + 2] = 0x55;

	send_bulk_msg(dev,7 + count + 3 );
}

static void clear_all(struct usb_display* dev){
	
	bulk_buf[0] = 0xAA;
	bulk_buf[1] = 0x06;
	bulk_buf[2] = 0x00;
	bulk_buf[3] = 0x02;
	bulk_buf[4] = 0x08;
	bulk_buf[5] = 0x55;

	send_bulk_msg(dev,6);
}

static void select_function(struct usb_display* dev,const char* buf,size_t count ){

	if(dev->pixel){
		draw_pixel(dev,buf);
	    dev->pixel = 0;
	}
	if(dev->line){
		draw_line(dev,buf);
	    dev->line = 0;
	}
	if(dev->square){
		draw_square(dev,buf);
	    dev->square = 0;
	}
	if(dev->text){
		send_text(dev,buf,count);
	    dev->text = 0;
	}
	if(dev->clearall){
		clear_all(dev);
	    dev->clearall = 0;
	}
}

	
#define show_set(value) \
static ssize_t show_##value(struct device *udev, struct device_attribute* \
							attr,char* buf)                            \
{																		\
    struct usb_interface *intf = to_usb_interface(udev);					\
    struct usb_display *dev = usb_get_intfdata(intf);                      \
	return sprintf(buf,"%d\n",dev->value);      \
}          \
static ssize_t set_##value(struct device *udev, struct device_attribute* \
							   attr,const char *buf, size_t count)     \
{															  \
    struct usb_interface *intf = to_usb_interface(udev);      \
    struct usb_display *dev = usb_get_intfdata(intf);        \
															  \
    dev->value = 1;										  \
    select_function(dev,buf, count);					  \
    return count;										\
}                                                     \
static DEVICE_ATTR(value, S_IRUGO | S_IWUSR, show_##value,set_##value);
show_set(pixel);
show_set(line);
show_set(square);
show_set(text);
show_set(clearall);

static int display_probe(struct usb_interface *interface,
						 const struct usb_device_id *id){

	struct usb_device *udev =  interface_to_usbdev(interface);
	int retval = -ENOMEM;
	unsigned char* init_msg;
	
    struct usb_display* dev =  kmalloc(sizeof(struct usb_display), GFP_KERNEL);
	if(NULL == dev){
		printk("initialise device failure");
		return 1;
	}
	
    dev->udev = usb_get_dev(udev);
	usb_set_intfdata(interface, dev);

	retval = device_create_file(&interface->dev, &dev_attr_pixel);
	if (retval){
		printk("create file failure");
		return 1;
	}
	retval = device_create_file(&interface->dev, &dev_attr_line);
	if (retval){
		printk("create file failure");
		return 1;
	}
	retval = device_create_file(&interface->dev, &dev_attr_square);
	if (retval){
		printk("create file failure");
		return 1;
	}
	retval = device_create_file(&interface->dev, &dev_attr_text);
	if (retval){
		printk("create file failure");
		return 1;
	}
	retval = device_create_file(&interface->dev, &dev_attr_clearall);
	if (retval){
		printk("create file failure");
		return 1;
	}
	
	init_msg = kmemdup("\x77\x65\x6c\x63\x6f\x6d\x65",7,GFP_KERNEL);  //welcome
	if(!init_msg){
		printk("create init_msg failure");
		retval = -ENOMEM;
		return 1;
	}

	retval = usb_control_msg(dev->udev,usb_sndctrlpipe(dev->udev,0),
							 0x32,
							 0x21,
							 0,
							 0,
							 init_msg,
							 7,
							 2000);
	if(!retval){
		printk("transfer error");
		return 1;
	}
		
	
	kfree(init_msg);
	if(7 != retval){
		printk("transfer error");
		return 1;
	}

	printk("USB Display now attached\n");

	return 0;
}

static void display_disconnect(struct usb_interface *interface){
	struct usb_display *dev;

	dev = usb_get_intfdata(interface);

	device_remove_file(&interface->dev, &dev_attr_pixel);
	device_remove_file(&interface->dev, &dev_attr_line);
	device_remove_file(&interface->dev, &dev_attr_square);
	device_remove_file(&interface->dev, &dev_attr_text);
	device_remove_file(&interface->dev, &dev_attr_clearall);
	
	usb_set_intfdata(interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);
	printk("USB Display now disconnected\n");
}

static struct usb_driver display_driver = {
	.name = "display",
	.probe = display_probe,
	.disconnect = display_disconnect,
	.id_table = id_table,
};

module_usb_driver(display_driver);

MODULE_LICENSE("GPL");





