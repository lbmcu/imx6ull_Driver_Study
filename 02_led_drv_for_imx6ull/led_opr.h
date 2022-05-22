#ifndef _LED_OPR_H
#define	_LED_OPR_H

struct led_oprations {
	int num;
	int(*init)(int which);	/* 初始化LED，which-哪一个LED */
	int(*ctl)(int which, char status);	/* 控制LED，which-哪一个LED, status:1-亮, 0-灭 */
};

struct led_oprations* get_board_led_opr(void);

#endif

