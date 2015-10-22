#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>


typedef unsigned int u32;

static void get_ttbr1(u32 *ttbr1)
{
  asm
  (
    "mrc p15,0,%0,c2,c0,1\n"
    :"=r"(*ttbr1)
    :
    :
  );
/*
 * ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0211h/Bhchbicd.html
MRC p15, 0, <Rd>, c2, c0, 1            ; Read Translation Table Base Register 1
MCR p15, 0, <Rd>, c2, c0, 1            ; Write Translation Table Base Register 1 
*/
}

static void get_ttbr0(u32 *ttbr0)
{
  u32 a;
  asm
  (
    "mrc p15,0,%0,c2,c0,0\n"
    :"=r"(a)
    :
    :
  );
  *ttbr0 = a;
  printk(KERN_ALERT "a: %x\n", a);
}


static void get_ttbcr(u32 *reg)
{
  asm
  (
    "mrc p15,0,%0,c2,c0,2\n"
    :"=r"(*reg)
    :
    :
  );

#if 0
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0211k/Bihgfcgf.html
MRC p15, 0, <Rd>, c2, c0, 2      ; Read Translation Table Base Control Register
MCR p15, 0, <Rd>, c2, c0, 2      ; Write Translation Table Base Control Register
#endif
}


SYSCALL_DEFINE1(get_mmu_reg, unsigned int, reg_type)
{
  int reg = 0x12345678;
  printk(KERN_ALERT "reg_type: %x\n", reg_type);
  switch (reg_type)
  {
    case 0:
    {
      get_ttbr0(&reg);
      break;
    }
    case 1:
    {
      get_ttbr1(&reg);
      break;
    }
    case 2:
    {
      get_ttbcr(&reg);
      break;
    }
    default:
    {
      printk(KERN_ALERT "not support reg_type: %x\n", reg_type);
      return -5;
      break;
    }
  }
  return reg;
}

