#include <linux/usb.h>
#include <linux/kernel.h> 

#define VENDOR_ID 0x10cf
#define PRODUCT_ID 0x8101

struct usb_display{
	struct usb_device *udev;
	unsigned char* buffer;	// constant size 8 bytes
	unsigned char* input_from_user;
	unsigned char pixel;
};

// a list of different types of usb devices that this driver supports
// usb core will decide which driver to give a device according to this table
static const struct usb_device_id display_table[] = {
	{USB_DEVICE(VENDOR_ID,PRODUCT_ID)},{},	
};


MODULE_DIVICE_TABLE(usb,id_table);

struct usb_display *dev = NULL;      // global variable
static unsigned char bulk_buf[100];

static void select_functions(const char* buf,size_t count ){

	if(dev->pixel){
		draw_pixel(&buf,count);
		dev->pixel = 0;
	}
	if(dev->line){
		draw_line();
		dev->line = 0;
	}
	if(dev->square){
		draw_square();
		dev->square = 0;
	}
	if(dev->text){
		send_text();
		dev->text = 0;
	}
	if(dev->clear_all){
		clear_all();
		dev->clear_all = 0;
	}
}


static unsigned char check_num(int size){
	int ret = 0;
	int i;
	for(i = 1; i < size + 1; i++)
		ret += bulk_buf[i];

	ret = ret%256;	
	
	return ret;	
}

static void send_bulk_msg(int size){

	int act_len;
	int retval = 0;

	retval = usb_bulk_msg(dev->udev,sndctrlpipe(dev->udev,0x02),
						  bulk_buff,size,&act_len,5000);
	if(retval < 0)
		printk(KERN_ERR "transfer error");
	if(act_len != size)
		printk(KERN_ERR "wrong size is transfered");
}


static void draw_pixel(const char* buf){

	bulk_buf[0] = "\xAA";
	bulk_buf[1] = "\x08";
	bulk_buf[2] = "\x00";
	bulk_buf[3] = "\x09";
	sscanf(buf,"%d %d",&bulk_buf[4],&bulk_buf[5]);
	bulk_buf[6] = check_num(&bulk_buf,5);
	bulk_buf[7] = "\x55";

	send_bulk_msg(8);
}

#define set(value)\
static ssize_t set_##value(struct device *udev, struct device_attribute\
							   *attr,const char *buf, size_t count){    \

    struct usb_interface *intf = to_usb_interface(udev);      \
    struct usb_dislplay *dev = usb_get_intfdata(intf);        \
															  \
    dev-> value = 1;	 \
    select_functions(&buf, count);					\
    return count;										\
}\

static DEVICE_ATTR(value,S_IGUGO | S_IWUSR,NULL,set_##value);
set(pixel);

static int display_probe(struct usb_interface *interface,
						 const struct usb_device_id *id){

	struct usb_device *udev =  interface_to_usbdev(interface);
	struct usb_host_interface *iface_desc;               //????
	struct usb_endpoint_descriptor *endpoint;
	int retval = -ENOMEM;

	dev =  kmalloc(sizeof(struct usb_display), GFP_KERNEL);
	if(NULL == dev){
		printk("initialise device failure");
	}
	
	dev->udev = usb_get_dev(udev);

	usb_set_intfdata(interface, dev);

	retval = device_create_file(&interface->dev,funcs);
	if (retval)
		printk("create file failure");

	unsigned char* init_msg;

	init_msg = kmemdup("hallo",5,GFP_KERNEL);
	if(!init_msg){
		printk("create init_msg failure");
		retval = -ENOMEM;
	}

	retval = usb_control_msg(dev->udev,usb_sndctrllpipe(dev->udev,0),
							 0x32,
							 0x21,
							 0,
							 0,
							 init_msg,
							 5,
							 2000,);

	kfree(init_msg);
	if(5 != retval)
		printk("transfer error");

	printk(&interface->dev, "USB Display now attached\n");

	return 0;
}

static void display_disconnect(struct usb_interface *interface){
	struct usb_display *dev;

	dev = usb_get_intfdata(interface);

	device_remove_file(&interface->dev, ??? funcs);
	usb_set_intfdata(interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);
	printk(&interface->dev, "USB Display now disconnected\n");
}

// which decribes the usb driver
static struct usb_driver display_driver = {
	name = "display",
	probe = display_probe,  // usb core call this function
	disconnect = display_disconnect,
	id_table = id_table,
};

module_usb_driver(display_driver);

/* // register in usb subsystem */
/* static void __init usb_display_init(void){ */
/* 	usb_register(&display_driver); */
	
/* } */

/* static void __exit usb_display_exit(void){ */
/* 	usb_deregister(&display_driver); */
/* } */

/* module_init(usb_display_init); */
/* module_exit(usb_display_exit); */

MODULE_LICENSE("GPL");





