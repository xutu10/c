#include <linux/usb.h>
#include<linux/kernel.h> 

#define VENDOR_ID 0x10cf
#define PRODUCT_ID 0x8101

struct usb_display{
	struct usb_device *udev;
	unsigned char* buffer;	
};
// add id of the device, so that linux can verify this device
// when it works then probe is called. 
static const struct usb_device_id display_table[] = {
	{USB_DEVICE(VENDOR_ID,PRODUCT_ID)},{}	
}

MODULE_DIVICE_TABLE(usb,id_table);


static int display_open(struct inode *inode, struct file *file){

	struct usb_display *dev = NULL;
	struct usb_interface *interface;
	int retval = 0;
	int minor;

	minor = iminor(inode);
	interface = usb_find_interface(&display_driver,minor);

	dev = usb_get_intfdata(interface);

	++dev->open_count;
	file->private_data = dev;       //???
	
}

static int display_release(struct inode *inode, struct file *file){

	struct usb_display *dev = NULL;
	int retval = 0;

	dev = file ->private_data;

	--dev->open_count;
	
}

static ssize_t display_write (struct file *file, const char _user *user_buf,
						  size_t count, loff_t *ppos){

	struct usb_display *dev;
	int retval = 0;

	u8 buf[8];
	__u8 cmd = ;             //???

	dev = file -> private_data;

	memset(&buf, 0. sizeof(buf));

	retval = usb_control_msg(dev->udev,
							 usb_sndctrlpipe(dev->udev,0),
							 request,requestTyp,value,index
							 &buf,sizeof(buf),timeout);      //???
	
}

static struct file_operations display_fops = {
	owner = THIS_MODULE,
	write = display_write,
	open = display_open,
	release = display_release,
};
	

static int display_probe(struct usb_interface *interface,
						 const struct usb_device_id *id){

	struct usb_device *udev =  interface_to_usbdev(interface);
	struct usb_display *dev = NULL;
	struct usb_host_interface *iface_desc;               //????
	struct usb_endpoint_descriptor *endpoint;
	int retval = -ENOMEM;

	dev =  kmalloc(sizeof(struct usb_display), GFP_KERNEL);
	if(NULL == dev){
		printk("initialise device failure");
	}
	
	dev->udev = usb_get_dev(udev);

	usb_set_intfdata(interface, dev);

	retval = device_create_file(&interface->dev, ??? funcs);
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

static struct usb_driver display_driver = {
	name = "display",
	probe = display_probe,
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





