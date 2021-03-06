/*
	$Id: osdlang_setup.h,v 1.2 2010/09/30 20:13:59 dbt Exp $

	OSD-Language Setup implementation - Neutrino-GUI

	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Copyright (C) 2010 T. Graf 'dbt'
	Homepage: http://www.dbox2-tuning.net/

	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __osdlang_setup__
#define __osdlang_setup__

#include <global.h>
#include <neutrino.h>

#include <gui/widget/menue.h>

#include <driver/framebuffer.h>

#include <system/setting_helpers.h>

#include <string>


class COsdLangSetup : public CMenuTarget, CChangeObserver
{	
	private:

		CFrameBuffer *frameBuffer;
		
		int x, y, width, height, menue_width, hheight, mheight;

		void hide();
		void showSetup();

	public:
		COsdLangSetup();
		~COsdLangSetup();
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

class COsdLangNotifier : public COsdLangSetup
{
	private:

		CMenuForwarder* toModifi;

	public:
		COsdLangNotifier();

		bool changeNotify(const neutrino_locale_t, void * Data);
};

#endif
