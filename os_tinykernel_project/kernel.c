#include "isr.h"
#include "idt.h"
#include "utility.h"
#include "keyboard.h"
#include "mouse.h"
#include "screen.h"
#include "pic.h"


u8 lclik = 0;
u8 keypress = 0;
u32 clicks = 0;

void keypressed(){
        /* char *video_address = (char*)0xb8000;
        //print interrupt num____________
        video_address[0] = 'A';//address[1] sets forground and background color of character
        video_address[2] = ':';
        video_address[4] = 'p';
        video_address[6] = 'r';
        video_address[8] = 'e';
        video_address[10] = 's';
        video_address[12] = 's';
        video_address[14] = 'e';
        video_address[16] = 'd';*/
        /*for(u8 i = 0; i < 20; i++){
            disp_char('A', i, 0, 0x6f);
        }*/
        keypress = 1;

}
void left_click(){
        lclik = 1;
        clicks++;
        /*for(u8 i = 0; i < 20; i++){
            disp_char('a', i, 4, 0x6f);
        }*/
}

//extern void wait1();

void main_loop(){
    while(1){
        clear_screen(0x0e);
        if(keypress)    disp_string("keyboard working bre", 1, 2, 0x67);
        if(lclik)   disp_string("left click working bre", 1, 4, 0x6f);
        disp_char_absolute('+', mouse_x, mouse_y, 0x6f);
        disp_string("score:", 1, 3, 0x6f);
        disp_int(clicks, 48, 24, 0x6f);
        draw_screen();
    }
}


void main() {

    isr_install();


    keyboard_init();
    mouse_install();

     clear_screen(0x0e);
    __asm__ __volatile__("int $19");
     draw_screen();

    main_loop();
}


