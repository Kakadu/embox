#include <drivers/lcd/lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	//printf("SUKA\n");
//	lcd_clear();
//	for (int i=0; i<16; i++)
//		lcd_putc(0xe2);

	if (argc == 3 && !strcmp("loop", argv[1])) {
		int d = atoi(argv[2]);
		lcd_test_loop1(d);
	} else
	if (argc == 2 && !strcmp("init", argv[1])) {
		lcd_init();
	} else if (argc==5 && !strcmp("mode", argv[1])) {
		int is_byte =     !strcmp("1", argv[2]) ? 1 : 0;
		int is_2_lines  = !strcmp("1", argv[3]) ? 1 : 0;
		int is_big_font = !strcmp("1", argv[4]) ? 1 : 0;
		lcd_select_mode(is_byte, is_2_lines, is_big_font);
	} else if (argc==2 && !strcmp("clear", argv[1])) {
		lcd_clear();
	} else if (argc==4 && !strcmp("shift", argv[1])) {
		int screen = !strcmp("1", argv[2]) ? 1 : 0;
		int right  = !strcmp("1", argv[3]) ? 1 : 0;
		lcd_cfg_shift(screen, right);
	} else if (argc==5 && !strcmp("cfgdisplay", argv[1])) {
		int screen_on   = !strcmp("1", argv[2]) ? 1 : 0;
		int cursor_on   = !strcmp("1", argv[3]) ? 1 : 0;
		int blinking_on = !strcmp("1", argv[4]) ? 1 : 0;
		lcd_on_off_control(screen_on, cursor_on, blinking_on);
	} else if (argc==2 && !strcmp("demo", argv[1])) {
		lcd_demo();
	}
	return 0;
}
