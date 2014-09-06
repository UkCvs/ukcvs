/*
 * $Id: color.h,v 1.2 2012/06/29 21:26:01 rhabarber1848 Exp $
 *
 * tuxwetter - d-box2 linux project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef __color__
#define __color__

#define COL_MAXFREE			254-8*7 - 1
#define COL_INFOBAR_SHADOW		254-8*7
#define COL_INFOBAR			254-8*6
#define COL_MENUHEAD			254-8*5
#define COL_MENUCONTENT			254-8*4
#define COL_MENUCONTENTDARK		254-8*3
#define COL_MENUCONTENTSELECTED		254-8*2
#define COL_MENUCONTENTINACTIVE		254-8*1

#define COL_BACKGROUND 			255

#define COL_INFOBAR_SHADOW_PLUS_0       (COL_INFOBAR_SHADOW + 0)
#define COL_INFOBAR_SHADOW_PLUS_1       (COL_INFOBAR_SHADOW + 1)
#define COL_INFOBAR_PLUS_0              (COL_INFOBAR + 0)
#define COL_INFOBAR_PLUS_1              (COL_INFOBAR + 1)
#define COL_INFOBAR_PLUS_3              (COL_INFOBAR + 3)
#define COL_INFOBAR_PLUS_7              (COL_INFOBAR + 7)
#define COL_MENUHEAD_PLUS_0             (COL_MENUHEAD + 0)
#define COL_MENUCONTENT_PLUS_0          (COL_MENUCONTENT + 0)
#define COL_MENUCONTENT_PLUS_1          (COL_MENUCONTENT + 1)
#define COL_MENUCONTENT_PLUS_2          (COL_MENUCONTENT + 2)
#define COL_MENUCONTENT_PLUS_3          (COL_MENUCONTENT + 3)
#define COL_MENUCONTENT_PLUS_4          (COL_MENUCONTENT + 4)
#define COL_MENUCONTENT_PLUS_5          (COL_MENUCONTENT + 5)
#define COL_MENUCONTENT_PLUS_6          (COL_MENUCONTENT + 6)
#define COL_MENUCONTENT_PLUS_7          (COL_MENUCONTENT + 7)
#define COL_MENUCONTENTDARK_PLUS_0      (COL_MENUCONTENTDARK + 0)
#define COL_MENUCONTENTDARK_PLUS_2      (COL_MENUCONTENTDARK + 2)
#define COL_MENUCONTENTSELECTED_PLUS_0  (COL_MENUCONTENTSELECTED + 0)
#define COL_MENUCONTENTSELECTED_PLUS_2  (COL_MENUCONTENTSELECTED + 2)
#define COL_MENUCONTENTINACTIVE_PLUS_0  (COL_MENUCONTENTINACTIVE + 0)
#define COL_BACKGROUND_PLUS_0           (COL_BACKGROUND + 0)

enum {	COL_WHITE=0x60, COL_RED, COL_GREEN, COL_BLUE, COL_YELLOW, COL_BLACK };

#endif
