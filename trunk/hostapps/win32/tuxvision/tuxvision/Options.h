//
//  TuxVision
//  
//  Rev.0.0 Bernd Scharping 
//  bernd@transputer.escape.de
//
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

BOOL CreatePropertySheet(HWND hWndParent, HINSTANCE hInst, int StartPage);
BOOL CALLBACK DlgProc_DBOX(HWND hDlg, UINT msg, WPARAM  wParam, LPARAM  lParam);
BOOL CALLBACK DlgProc_AUDIO(HWND hDlg, UINT msg, WPARAM  wParam, LPARAM  lParam);
BOOL CALLBACK DlgProc_MCE(HWND hDlg, UINT msg, WPARAM  wParam, LPARAM  lParam);
BOOL CALLBACK DlgProc_MISC(HWND hDlg, UINT msg, WPARAM  wParam, LPARAM  lParam);
BOOL CALLBACK DlgProc_ABOUT(HWND hDlg, UINT msg, WPARAM  wParam, LPARAM  lParam);
