/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x79f3f3a8 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/FitkitSVN/apps/xtomal02/fpga/vigenere.vhd";
extern char *IEEE_P_3620187407;

unsigned char ieee_p_3620187407_sub_2546454082_3965413181(char *, char *, char *, int );
unsigned char ieee_p_3620187407_sub_2599155846_3965413181(char *, int , char *, char *);
char *ieee_p_3620187407_sub_436279890_3965413181(char *, char *, char *, char *, int );
char *ieee_p_3620187407_sub_436351764_3965413181(char *, char *, char *, char *, int );
char *ieee_p_3620187407_sub_767668596_3965413181(char *, char *, char *, char *, char *, char *);
char *ieee_p_3620187407_sub_767740470_3965413181(char *, char *, char *, char *, char *, char *);


static void work_a_1389154466_3212880686_p_0(char *t0)
{
    char t1[16];
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;

LAB0:    xsi_set_current_line(54, ng0);
    t2 = (t0 + 868U);
    t3 = *((char **)t2);
    t2 = (t0 + 4316U);
    t4 = ieee_p_3620187407_sub_436351764_3965413181(IEEE_P_3620187407, t1, t3, t2, 64);
    t5 = (t0 + 2576);
    t6 = (t5 + 32U);
    t7 = *((char **)t6);
    t8 = (t7 + 40U);
    t9 = *((char **)t8);
    memcpy(t9, t4, 8U);
    xsi_driver_first_trans_fast(t5);
    t2 = (t0 + 2516);
    *((int *)t2) = 1;

LAB1:    return;
}

static void work_a_1389154466_3212880686_p_1(char *t0)
{
    char t4[16];
    char *t1;
    char *t2;
    char *t3;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    unsigned int t10;
    unsigned int t11;
    unsigned char t12;

LAB0:    xsi_set_current_line(61, ng0);
    t1 = (t0 + 776U);
    t2 = *((char **)t1);
    t1 = (t0 + 1408U);
    t3 = *((char **)t1);
    t1 = (t3 + 0);
    memcpy(t1, t2, 8U);
    xsi_set_current_line(62, ng0);
    t1 = (t0 + 776U);
    t2 = *((char **)t1);
    t1 = (t0 + 4300U);
    t3 = (t0 + 1052U);
    t5 = *((char **)t3);
    t3 = (t0 + 4348U);
    t6 = ieee_p_3620187407_sub_767740470_3965413181(IEEE_P_3620187407, t4, t2, t1, t5, t3);
    t7 = (t0 + 1408U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t9 = (t4 + 12U);
    t10 = *((unsigned int *)t9);
    t11 = (1U * t10);
    memcpy(t7, t6, t11);
    xsi_set_current_line(63, ng0);
    t1 = (t0 + 1408U);
    t2 = *((char **)t1);
    t1 = (t0 + 4396U);
    t12 = ieee_p_3620187407_sub_2599155846_3965413181(IEEE_P_3620187407, 65, t2, t1);
    if (t12 != 0)
        goto LAB2;

LAB4:
LAB3:    xsi_set_current_line(66, ng0);
    t1 = (t0 + 1408U);
    t2 = *((char **)t1);
    t1 = (t0 + 2612);
    t3 = (t1 + 32U);
    t5 = *((char **)t3);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    memcpy(t7, t2, 8U);
    xsi_driver_first_trans_fast(t1);
    t1 = (t0 + 2524);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(64, ng0);
    t3 = (t0 + 1408U);
    t5 = *((char **)t3);
    t3 = (t0 + 4396U);
    t6 = ieee_p_3620187407_sub_436279890_3965413181(IEEE_P_3620187407, t4, t5, t3, 26);
    t7 = (t0 + 1408U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t9 = (t4 + 12U);
    t10 = *((unsigned int *)t9);
    t11 = (1U * t10);
    memcpy(t7, t6, t11);
    goto LAB3;

}

static void work_a_1389154466_3212880686_p_2(char *t0)
{
    char t4[16];
    char *t1;
    char *t2;
    char *t3;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    unsigned int t10;
    unsigned int t11;
    unsigned char t12;

LAB0:    xsi_set_current_line(73, ng0);
    t1 = (t0 + 776U);
    t2 = *((char **)t1);
    t1 = (t0 + 1476U);
    t3 = *((char **)t1);
    t1 = (t3 + 0);
    memcpy(t1, t2, 8U);
    xsi_set_current_line(74, ng0);
    t1 = (t0 + 776U);
    t2 = *((char **)t1);
    t1 = (t0 + 4300U);
    t3 = (t0 + 1052U);
    t5 = *((char **)t3);
    t3 = (t0 + 4348U);
    t6 = ieee_p_3620187407_sub_767668596_3965413181(IEEE_P_3620187407, t4, t2, t1, t5, t3);
    t7 = (t0 + 1476U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t9 = (t4 + 12U);
    t10 = *((unsigned int *)t9);
    t11 = (1U * t10);
    memcpy(t7, t6, t11);
    xsi_set_current_line(75, ng0);
    t1 = (t0 + 1476U);
    t2 = *((char **)t1);
    t1 = (t0 + 4412U);
    t12 = ieee_p_3620187407_sub_2546454082_3965413181(IEEE_P_3620187407, t2, t1, 90);
    if (t12 != 0)
        goto LAB2;

LAB4:
LAB3:    xsi_set_current_line(78, ng0);
    t1 = (t0 + 1476U);
    t2 = *((char **)t1);
    t1 = (t0 + 2648);
    t3 = (t1 + 32U);
    t5 = *((char **)t3);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    memcpy(t7, t2, 8U);
    xsi_driver_first_trans_fast(t1);
    t1 = (t0 + 2532);
    *((int *)t1) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(76, ng0);
    t3 = (t0 + 1476U);
    t5 = *((char **)t3);
    t3 = (t0 + 4412U);
    t6 = ieee_p_3620187407_sub_436351764_3965413181(IEEE_P_3620187407, t4, t5, t3, 26);
    t7 = (t0 + 1476U);
    t8 = *((char **)t7);
    t7 = (t8 + 0);
    t9 = (t4 + 12U);
    t10 = *((unsigned int *)t9);
    t11 = (1U * t10);
    memcpy(t7, t6, t11);
    goto LAB3;

}


extern void work_a_1389154466_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1389154466_3212880686_p_0,(void *)work_a_1389154466_3212880686_p_1,(void *)work_a_1389154466_3212880686_p_2};
	xsi_register_didat("work_a_1389154466_3212880686", "isim/vigenere_isim.exe.sim/work/a_1389154466_3212880686.didat");
	xsi_register_executes(pe);
}
