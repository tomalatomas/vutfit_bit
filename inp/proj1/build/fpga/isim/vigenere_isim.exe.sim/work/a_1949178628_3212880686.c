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
static const char *ng0 = "C:/FitkitSVN/apps/xtomal02/fpga/sim/tb.vhd";
extern char *IEEE_P_2592010699;
extern char *IEEE_P_3499444699;

unsigned char ieee_p_2592010699_sub_1690584930_503743352(char *, unsigned char );
char *ieee_p_3499444699_sub_2213602152_3536714472(char *, char *, int , int );


static void work_a_1949178628_3212880686_p_0(char *t0)
{
    char *t1;
    char *t2;
    int64 t3;
    int64 t4;
    char *t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;

LAB0:    xsi_set_current_line(56, ng0);

LAB3:    t1 = (t0 + 1224U);
    t2 = *((char **)t1);
    t3 = *((int64 *)t2);
    t4 = (t3 / 2);
    t1 = (t0 + 684U);
    t5 = *((char **)t1);
    t6 = *((unsigned char *)t5);
    t7 = ieee_p_2592010699_sub_1690584930_503743352(IEEE_P_2592010699, t6);
    t1 = (t0 + 2316);
    t8 = (t1 + 32U);
    t9 = *((char **)t8);
    t10 = (t9 + 40U);
    t11 = *((char **)t10);
    *((unsigned char *)t11) = t7;
    xsi_driver_first_trans_delta(t1, 0U, 1, t4);
    t12 = (t0 + 2316);
    xsi_driver_intertial_reject(t12, t4, t4);

LAB2:    t13 = (t0 + 2256);
    *((int *)t13) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_1949178628_3212880686_p_1(char *t0)
{
    char t3[16];
    char *t1;
    char *t2;
    char *t4;
    int t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    unsigned char t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    unsigned char t16;
    unsigned char t17;
    unsigned char t18;
    int t19;
    int t20;
    int t21;
    char *t22;
    char *t23;
    char *t24;

LAB0:    t1 = (t0 + 2060U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(61, ng0);
    t2 = (t0 + 1360U);
    t4 = *((char **)t2);
    t5 = (0 - 0);
    t6 = (t5 * 1);
    t7 = (1U * t6);
    t8 = (0 + t7);
    t2 = (t4 + t8);
    t9 = *((unsigned char *)t2);
    t10 = ieee_p_3499444699_sub_2213602152_3536714472(IEEE_P_3499444699, t3, ((int)(t9)), 8);
    t11 = (t0 + 2352);
    t12 = (t11 + 32U);
    t13 = *((char **)t12);
    t14 = (t13 + 40U);
    t15 = *((char **)t14);
    memcpy(t15, t10, 8U);
    xsi_driver_first_trans_fast(t11);
    xsi_set_current_line(62, ng0);
    t2 = (t0 + 1292U);
    t4 = *((char **)t2);
    t5 = (0 - 0);
    t6 = (t5 * 1);
    t7 = (1U * t6);
    t8 = (0 + t7);
    t2 = (t4 + t8);
    t9 = *((unsigned char *)t2);
    t10 = ieee_p_3499444699_sub_2213602152_3536714472(IEEE_P_3499444699, t3, ((int)(t9)), 8);
    t11 = (t0 + 2388);
    t12 = (t11 + 32U);
    t13 = *((char **)t12);
    t14 = (t13 + 40U);
    t15 = *((char **)t14);
    memcpy(t15, t10, 8U);
    xsi_driver_first_trans_fast(t11);
    xsi_set_current_line(63, ng0);

LAB6:    t2 = (t0 + 2264);
    *((int *)t2) = 1;
    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    t10 = (t0 + 2264);
    *((int *)t10) = 0;
    xsi_set_current_line(64, ng0);
    t2 = (t0 + 2424);
    t4 = (t2 + 32U);
    t10 = *((char **)t4);
    t11 = (t10 + 40U);
    t12 = *((char **)t11);
    *((unsigned char *)t12) = (unsigned char)2;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(65, ng0);
    t2 = (t0 + 4453);
    *((int *)t2) = 0;
    t4 = (t0 + 4457);
    *((int *)t4) = 7;
    t5 = 0;
    t19 = 7;

LAB11:    if (t5 <= t19)
        goto LAB12;

LAB14:    xsi_set_current_line(70, ng0);

LAB25:    *((char **)t1) = &&LAB26;
    goto LAB1;

LAB5:    t4 = (t0 + 660U);
    t16 = xsi_signal_has_event(t4);
    if (t16 == 1)
        goto LAB8;

LAB9:    t9 = (unsigned char)0;

LAB10:    if (t9 == 1)
        goto LAB4;
    else
        goto LAB6;

LAB7:    goto LAB5;

LAB8:    t10 = (t0 + 684U);
    t11 = *((char **)t10);
    t17 = *((unsigned char *)t11);
    t18 = (t17 == (unsigned char)3);
    t9 = t18;
    goto LAB10;

LAB12:    xsi_set_current_line(66, ng0);
    t10 = (t0 + 1360U);
    t11 = *((char **)t10);
    t10 = (t0 + 4453);
    t20 = xsi_vhdl_mod(*((int *)t10), 2);
    t21 = (t20 - 0);
    t6 = (t21 * 1);
    xsi_vhdl_check_range_of_index(0, 1, 1, t20);
    t7 = (1U * t6);
    t8 = (0 + t7);
    t12 = (t11 + t8);
    t9 = *((unsigned char *)t12);
    t13 = ieee_p_3499444699_sub_2213602152_3536714472(IEEE_P_3499444699, t3, ((int)(t9)), 8);
    t14 = (t0 + 2352);
    t15 = (t14 + 32U);
    t22 = *((char **)t15);
    t23 = (t22 + 40U);
    t24 = *((char **)t23);
    memcpy(t24, t13, 8U);
    xsi_driver_first_trans_fast(t14);
    xsi_set_current_line(67, ng0);
    t2 = (t0 + 1292U);
    t4 = *((char **)t2);
    t2 = (t0 + 4453);
    t20 = *((int *)t2);
    t21 = (t20 - 0);
    t6 = (t21 * 1);
    xsi_vhdl_check_range_of_index(0, 7, 1, *((int *)t2));
    t7 = (1U * t6);
    t8 = (0 + t7);
    t10 = (t4 + t8);
    t9 = *((unsigned char *)t10);
    t11 = ieee_p_3499444699_sub_2213602152_3536714472(IEEE_P_3499444699, t3, ((int)(t9)), 8);
    t12 = (t0 + 2388);
    t13 = (t12 + 32U);
    t14 = *((char **)t13);
    t15 = (t14 + 40U);
    t22 = *((char **)t15);
    memcpy(t22, t11, 8U);
    xsi_driver_first_trans_fast(t12);
    xsi_set_current_line(68, ng0);

LAB17:    t2 = (t0 + 2272);
    *((int *)t2) = 1;
    *((char **)t1) = &&LAB18;
    goto LAB1;

LAB13:    t2 = (t0 + 4453);
    t5 = *((int *)t2);
    t4 = (t0 + 4457);
    t19 = *((int *)t4);
    if (t5 == t19)
        goto LAB14;

LAB22:    t20 = (t5 + 1);
    t5 = t20;
    t10 = (t0 + 4453);
    *((int *)t10) = t5;
    goto LAB11;

LAB15:    t10 = (t0 + 2272);
    *((int *)t10) = 0;
    goto LAB13;

LAB16:    t4 = (t0 + 660U);
    t16 = xsi_signal_has_event(t4);
    if (t16 == 1)
        goto LAB19;

LAB20:    t9 = (unsigned char)0;

LAB21:    if (t9 == 1)
        goto LAB15;
    else
        goto LAB17;

LAB18:    goto LAB16;

LAB19:    t10 = (t0 + 684U);
    t11 = *((char **)t10);
    t17 = *((unsigned char *)t11);
    t18 = (t17 == (unsigned char)3);
    t9 = t18;
    goto LAB21;

LAB23:    goto LAB2;

LAB24:    goto LAB23;

LAB26:    goto LAB24;

}


extern void work_a_1949178628_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1949178628_3212880686_p_0,(void *)work_a_1949178628_3212880686_p_1};
	xsi_register_didat("work_a_1949178628_3212880686", "isim/vigenere_isim.exe.sim/work/a_1949178628_3212880686.didat");
	xsi_register_executes(pe);
}
