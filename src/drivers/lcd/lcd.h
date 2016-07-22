/**
 * @file
 * @brief
 *
 * @author  Vita Loginova
 * @date    02.04.2015
 */

#ifndef DRIVERS_LCD_H_
#define DRIVERS_LCD_H_

extern void lcd_init();
extern void lcd_demo();

extern void lcd_select_mode(int is_byte, int is_two_lines, int big_font);
extern void lcd_clear();
extern void lcd_cfg_shift(int screen, int right);
extern void lcd_on_off_control(int display_on, int cursor_on, int blinking_on);
extern void lcd_test_loop1(int);

extern void lcd_putc(char ch);

#endif /* !DRIVERS_LCD_H_ */
