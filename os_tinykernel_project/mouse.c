//wip, heavily based on SANiK's mouse driver

#include "isr.h"
#include "utility.h"
#include "io.h"
#include "pic.h"
#include "keyboard.h"
#include "kernel.h"

u8 mouse_cycle=0;     //unsigned char
u8 mouse_byte[3];    //signed char
u8 mouse_x=0;         //signed char
u8 mouse_y=0;         //signed char

//Mouse functions
void mouse_handler(registers_t *r) //struct regs *a_r (not used but just there)
{
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inb(0x60);
      mouse_x=mouse_byte[1];
      mouse_y=mouse_byte[2];
      mouse_cycle=0;
      break;
  }
}

void mouse_wait(u8 a_type) //unsigned char
{
  u32 _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(u8 a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

u8 mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void mouse_install()
{
  u8 _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);

  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  _status=(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, _status);

  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge

  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  irq_install_handler(12, mouse_handler);
}
