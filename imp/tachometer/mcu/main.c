/*******************************************************************************
   main.c: LCD + keyboard demo
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#define WINDOWSIZE 6  //Pick number, where 60%WINDOWSIZE is 0
char last_ch;         //naposledy precteny znak
unsigned int currentRevs = 0;
int window[WINDOWSIZE];  //Array with rev count, every index contains number of revs per second

/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void) {
}

/*******************************************************************************
 * Print counter
*******************************************************************************/
void tachometer_print() {
    LCD_clear();
    int rpm = getRPM();
    char str[10];
    itoa(rpm, str, 10);
    LCD_append_string(str);
    LCD_append_string(" RPM");
}

/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle() {
    char ch;
    ch = key_decode(read_word_keyboard_4x4());
    if (ch != last_ch) {
        last_ch = ch;
        if (ch != 0) {
            currentRevs++;
            tachometer_print();
        }
    }
    return 0;
}

/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd) {
    return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized() {
    LCD_init();
    LCD_clear();
    LCD_append_string("Tachometer ");
}
/*******************************************************************************
 * Init window
*******************************************************************************/
void initWindow() {
    int i;
    for (i = 0; i < WINDOWSIZE; i++) {
        window[i] = 0;
    }
}

/*******************************************************************************
 * Summarization of window
*******************************************************************************/
int getRPM() {
    int buffer = 0;
    int i;
    for (i = 0; i < WINDOWSIZE; i++) {
        buffer += window[i];
    }
    int remainder = 60 / WINDOWSIZE;
    return buffer * remainder;
}

/*******************************************************************************
 * Shift and add last rev count to array
*******************************************************************************/
void updateWindow() {
    int i;
    for (i = 0; i < WINDOWSIZE; i++) {
        window[i] = window[i + 1];             // Shifting left
        window[WINDOWSIZE - 1] = currentRevs;  // Append currentrevs to the end of the array
    }
}

/*******************************************************************************
 * Obsluha preruseni casovace timer A0
********************************************************************************/
interrupt(TIMERA0_VECTOR) Timer_A(void) {
    updateWindow();
    currentRevs = 0;  // Reset counter
    tachometer_print();
    CCR0 += 0x8000;  // nastav po kolika ticich (32768 = 0x8000, tj. za 1 s) ma dojit k dalsimu preruseni
}
/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void) {
    TACTL = TASSEL_1 + MC_2;  // ACLK (f_tiku = 32768 Hz = 0x8000 Hz), nepretrzity rezim
    CCTL0 = CCIE;             // povol preruseni pro casovac (rezim vystupni komparace)
    CCR0 = 0x8000;            // nastav po kolika ticich (32768 = 0x8000, tj. za 1 s) ma dojit k preruseni
    last_ch = 0;

    initWindow();  // Fills window with zeroes
    initialize_hardware();
    keyboard_init();

    while (1) {
        keyboard_idle();  // obsluha klavesnice
        terminal_idle();  // obsluha terminalu
    }
}
