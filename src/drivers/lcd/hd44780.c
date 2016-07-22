/**
 * @file
 * @brief Simple driver for Hitachi HD44780. There is some limitation:
 *	* Control pins must be on same gpio port
 *	* Data pins must be attached sequentially to same port
 *
 * @author  Anton Kozlov
 * @date    10.07.2012
 */

/* Some links:
 * http://easyelectronics.ru/avr-uchebnyj-kurs-podklyuchenie-k-avr-lcd-displeya-hd44780.html
 * http://pdf.datasheetarchive.com/indexerfiles/Datasheet-078/DSAE0065169.pdf
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <drivers/gpio.h>
#include <embox/unit.h>


EMBOX_UNIT_INIT(lcd_test);

/* low-level defines */

/* below defines control pins */
#define CTRL_PORT OPTION_GET(STRING,control_port)
#define RS (1 << OPTION_GET(NUMBER, reg_select_pin))
#define RW (1 << OPTION_GET(NUMBER, rw_pin))
#define E (1 << OPTION_GET(NUMBER, clock_pin))
#define RS_CMD  0   // when we send cmd
#define RS_DATA 1   // when we set data

/* Below defines data port */
#define DATA_PORT OPTION_GET(STRING,data_port)
#define DATA_PINS_OFFSET OPTION_GET(NUMBER,data_pins_offset)
#define BUSY_FLAG (1 << (7 + DATA_PINS_OFFSET))

/* high-level defines */
#define LINE_N OPTION_GET(NUMBER,line_n)
#define WIDTH_N OPTION_GET(NUMBER,line_width)
#if 0
static char buf[LINE_N * WIDTH_N];
static int pos = 0;
static int line_first = 0;
#endif
/* low-level routines */

static int lcd_read_busyflag() {
	// busy flag is highest bit
	int val;
	gpio_conf_as_input((void *)DATA_PORT, GPIO_PIN_9);

	gpio_set_level((void *)CTRL_PORT, RS, RS_CMD);
	gpio_set_level((void *)CTRL_PORT, RW, 1);
	usleep(1); // Tsu
	gpio_set_level((void *)CTRL_PORT, E, 1);
	usleep(1); // Td
	val = gpio_get_level((void *)DATA_PORT, GPIO_PIN_9);
	gpio_set_level((void *)CTRL_PORT, E, 0);
	usleep(1);

	return val;

}
#if 0
static int lcd_read(int reg) {
	int val;
	gpio_conf_as_input((void *)DATA_PORT, 0xff << DATA_PINS_OFFSET);

	gpio_set_level((void *)CTRL_PORT, RS, reg);
	gpio_set_level((void *)CTRL_PORT, RW, 1);
	usleep(1); // Tsu
	gpio_set_level((void *)CTRL_PORT, E, 1);
	usleep(1); // Td
	val = gpio_get_level((void *)DATA_PORT, (gpio_mask_t)0xff << DATA_PINS_OFFSET);
	gpio_set_level((void *)CTRL_PORT, E, 0);
	usleep(1);

	return val;
}
#endif
static void lcd_write(int reg, int val, int delay_us) {
	//gpio_settings((void *)DATA_PORT, 0xff << DATA_PINS_OFFSET, GPIO_MODE_OUTPUT);

#if 0
	printf("Going to RS = %d, RW = %d, val = 0b ", reg, RW_TO_DISPLAY);
	for (int i=7; i>=0; i--) {
		printf("%d", (val >> i) & 1);
	}
	printf("\n");
#endif
	gpio_set_level((void *)DATA_PORT, 0xff << DATA_PINS_OFFSET, 0);
	gpio_set_level((void *)CTRL_PORT, RS, reg);
	gpio_set_level((void *)CTRL_PORT, RW, 0);

	gpio_set_level((void *)DATA_PORT, val << DATA_PINS_OFFSET, 1);
	usleep(1); // TSU1
	gpio_set_level((void *)CTRL_PORT, E, 1);
	usleep(1); // Tw
	//usleep(delay_us);
	gpio_set_level((void *)CTRL_PORT, E, 0);
	usleep(1); // Tw
}

static void lcd_wait_busy(void) {
	unsigned r = 0;
	for (;;) {
		r = lcd_read_busyflag();
		printf(". r=%u\n", r);
		if (!r)
			break;
	}
	printf("| r=%u\n", r);
}

static void lcd_write_wait(int reg, char ch, int delay_us) {
	lcd_wait_busy();
	lcd_write(reg, ch, 0);
	//usleep(2000000);
	lcd_wait_busy();
}

void lcd_clear(void) {
	#define CLEAR_DISPLAY_TIMEOUT_US (2*1000)
	lcd_write_wait(RS_CMD, 0x01, CLEAR_DISPLAY_TIMEOUT_US);
}

void lcd_return_home(void) {
	lcd_write_wait(0, 1<<1, 2000);
}

void lcd_cfg_shift(int addr_increment_dir, int shift_display) {
	int val = 0b100 +
			(addr_increment_dir? 0b010 : 0) +
			(shift_display ? 1 : 0);
	lcd_write_wait(RS_CMD, val, 40);
}
void lcd_on_off_control(int display_on, int cursor_on, int blinking_on)
{
	int val = 0b1000 + (display_on?  0b100:0) +
										 (cursor_on ?  0b010:0) +
										 (blinking_on? 0b001:0);
	lcd_write_wait(RS_CMD, val, 40);
}
// {screen 1, cursor 0}, { right 1, left 0 }s
void lcd_do_shift(int screen, int right) {
	int val = 0b10000 + (screen? 0b1000 : 0) + (right ? 0b100 : 0);
	lcd_write_wait(RS_CMD, val, 40);
}
// {1-byte,0-half byte} {0-single line,1-two lines} {0- 5x8 font, 1 - 5x10}
void lcd_select_mode(int is_byte, int is_two_lines, int big_font)
{
	int val =           0b100000 +
			(is_byte?       0b010000 : 0) +
			(is_two_lines ? 0b001000 : 0) +
			(big_font?      0b000100 : 0);
	lcd_write_wait(RS_CMD, val, 40);
}

void lcd_test_loop1(int us) {
	printf("us = %d\n", us);
	for (int i=0; i<6; ++i) {
		gpio_set_level((void *)CTRL_PORT, RS, 1);
		usleep(us);
		gpio_set_level((void *)CTRL_PORT, RS, 0);
	}
}


#if 0
/* high-level routines */
static inline void print_line(int n) {
	for (int j = 0; j < WIDTH_N; j++) {
		lcd_write_wait(1, buf[n * WIDTH_N + j]);
	}
}
static void refresh(void) {
	int cnt = 0;

	for (int i = line_first+1; i < LINE_N; i++) {
		go_to(cnt++, 0);
		print_line(i);

	}
	for (int i = 0; i <= line_first; i++) {
		go_to(cnt++, 0);
		print_line(i);
	}
}

static void newline(void) {

	line_first = (line_first + 1) % LINE_N;
	memset(buf + WIDTH_N * line_first, ' ', WIDTH_N);
	refresh();
	pos = 0;
	go_to(LINE_N - 1, 0);
}
#endif

void lcd_putc(char ch) {
//	if (ch == '\n') {
//		newline();
//		return;
//	}

//	if (pos >= WIDTH_N) {
//		newline();
//	}

//	buf[line_first * WIDTH_N + pos] = ch;
//	pos ++;

	lcd_write_wait(1, ch, 45);
}

void lcd_init() {
	lcd_select_mode(1, 1, 0);
	lcd_clear();
	lcd_cfg_shift(1,0);

	lcd_on_off_control(1,0,0);
	lcd_clear();
	lcd_do_shift(0, 1);

		lcd_putc(0b10111110);
		lcd_putc(0b10111000);
		lcd_putc(0b10110111);

		lcd_putc(0b11100011);
		lcd_putc(0b01100001);
}

void lcd_demo()
{
	printf("%s", __func__);
	lcd_select_mode(1, 1, 0);
	lcd_clear();
	lcd_cfg_shift(1,0);

	lcd_on_off_control(1,0,0);
	lcd_clear();
	lcd_do_shift(0, 1);

	lcd_putc('1'); lcd_putc('2'); lcd_putc('3');
	lcd_clear();

//	for (int i=0; i<3; ++i) {
//		int ans = lcd_read(1);
//		if (ans != i+'0')
//			printf("PIZDA %d\n", i);
//	}
//	for(int i=0; i<128; ++i) {
//		int val = i | 0x80;
//		lcd_write_wait(0, val);
//		lcd_putc('X');
//	}
}

static int lcd_test(void) {
	//char demo[] = "      Embox     \n   Rev unknown  ";

	gpio_set_level((void *)CTRL_PORT, E | RS | RW, 0);
	//gpio_settings((void *)CTRL_PORT, RS | E | RW, GPIO_MODE_OUTPUT);
#if 0
	lcd_write(0, 0x30);
	nsdelay(39000);
	lcd_write(0, 0x30);
	nsdelay(39000);
	lcd_write_wait(0, 0x0e);
	lcd_write_wait(0, 0x01);

	memset(buf, ' ', LINE_N * WIDTH_N);
	go_to(LINE_N - 1, 0);
#endif
#if 0 /* example */
	for (int i = 0; i < sizeof(demo) - 1; i++) {
		lcd_putc(demo[i]);
	}
#endif
	return 0;
}
