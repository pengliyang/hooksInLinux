#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
MODULE_LICENSE("Dual BSD/GPL");

#define _DEBUG
#ifdef _DEBUG
#define kprintk(fmt,args...) printk(KERN_DEBUG fmt,##args)
#define kprintf(fmt,args...) printf(fmt,##args)
#define kperror(str) perror(str)
#else
#define kprintk
#define kprintf
#define kperror
#endif


long g_old_sys_mkdir=0;
long * g_sys_call_table=NULL;
long g_oldcr0=0;

struct _idtr{
    unsigned short  limit;
    unsigned int    base;
} __attribute__ ( ( packed ) );

// 中断描述符表结构
struct _idt_descriptor
{
    unsigned short offset_low;
    unsigned short sel;
    unsigned char  none, flags;
    unsigned short offset_high;
} __attribute__((packed));

unsigned int close_cr(void)
{
    unsigned int cr0 = 0;
    unsigned int ret;

    asm volatile ("movl %%cr0, %%eax"
            : "=a"(cr0)
            );
    ret = cr0;

    /*clear the 20th bit of CR0,*/
    cr0 &= 0xfffeffff;
    asm volatile ("movl %%eax, %%cr0"
            :
            : "a"(cr0)
            );
    return ret;
}

void  open_cr(unsigned int oldval)
{
    asm volatile ("movl %%eax, %%cr0"
            :
            : "a"(oldval)
            );
}

long * get_sys_call_table(void)
{

    struct _idt_descriptor * idt;
    struct _idtr idtr;
    unsigned int sys_call_off;
    int sys_call_table=0;
    unsigned char* p;
    int i;

    asm("sidt %0":"=m"(idtr));
    printk("addr of idtr: 0x%x\n", (unsigned int)&idtr);
    idt=(struct _idt_descriptor *)(idtr.base+8*0x80);
    sys_call_off=((unsigned int )(idt->offset_high<<16)|(unsigned int )idt->offset_low);
    printk("addr of idt 0x80: 0x%x\n", sys_call_off);
    p=(unsigned char *)sys_call_off;
    for (i=0; i<100; i++)
    {
        if (p[i]==0xff && p[i+1]==0x14 && p[i+2]==0x85)
        {
            sys_call_table=*(int*)((int)p+i+3);
            kprintk("addr of sys_call_table: 0x%x\n", sys_call_table);
            return (long*)sys_call_table;
        }
    }
    return 0;
}



asmlinkage long my_sys_mkdir(const char __user *pathname, int mode)
{

    kprintk("can't' mkidr ^ ^\n");
    return -1;
}


void start_hook(void)
{
    g_sys_call_table=get_sys_call_table();

    if(!g_sys_call_table)
    {
        kprintk("get sys_call_table error");
        return;
    }
    //检测获取到的地址是不是正确的
    if (g_sys_call_table[__NR_close] != (unsigned long)sys_close)
    {   
        kprintk("Incorrect sys_call_table address.\n");
        return ;
    }
    g_old_sys_mkdir=g_sys_call_table[__NR_mkdir];
    //hoot it
    g_oldcr0=close_cr();
    g_sys_call_table[__NR_mkdir]=my_sys_mkdir;
    open_cr(g_oldcr0);
    kprintk("new %08x %08x\n",(unsigned int)my_sys_mkdir,(unsigned int)g_sys_call_table[__NR_mkdir]);

}

int raider_init(void)
{
    kprintk("raider init\n");
    start_hook();
    return  0;
}

void raider_exit(void)
{
    kprintk("raider exit");
    if(g_sys_call_table && g_old_sys_mkdir)
    {
        g_oldcr0=close_cr();
        g_sys_call_table[__NR_mkdir]=g_old_sys_mkdir;
        kprintk("restore %08x\n",(unsigned int)g_sys_call_table[__NR_mkdir]);
        open_cr(g_oldcr0);
    }
}

