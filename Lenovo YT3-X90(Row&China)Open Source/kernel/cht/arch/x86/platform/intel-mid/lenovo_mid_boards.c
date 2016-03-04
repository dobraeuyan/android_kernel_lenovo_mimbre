/*
 * Copyright © 2015 lenovo Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *
 */

#include <linux/dmi.h>
#include <linux/printk.h>


int is_board_blade3_10a(void)
{
	const char *board_name;

	board_name= dmi_get_system_info(DMI_BOARD_NAME);
	pr_info("[Board]: %s", board_name);
	if(strcmp(board_name, "Blade3-10A") == 0)
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(is_board_blade3_10a);

int is_board_blade3_13a(void)
{
	const char *board_name;

	board_name= dmi_get_system_info(DMI_BOARD_NAME);
	pr_info("[Board]: %s", board_name);
	if(strcmp(board_name, "Blade3-13A") == 0)
		return 1;
	return 0;
}

EXPORT_SYMBOL_GPL(is_board_blade3_13a);




