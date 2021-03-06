/*
	$Id: zapit_setup.h,v 1.3 2010/09/26 22:01:08 dbt Exp $

	zapit setup menue - Neutrino-GUI

	Copyright (C) 2009/2010 Tuxbox-Team

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

#ifndef __zapit_setup__
#define __zapit_setup__

#include <configfile.h>

#include <gui/widget/menue.h>
#include <gui/widget/icons.h>

#include <driver/framebuffer.h>
#include <zapit/client/zapitclient.h>

class CZapitSetup : public CMenuTarget
{
	private:

		CFrameBuffer *frameBuffer;

		int x, y, width, height, hheight, mheight;

		neutrino_locale_t menue_title;
		std::string menue_icon;

		void hide();
		void showSetup();
		void Init();

		int remainingChannelsBouquet;
		int saveaudiopids;
		int savelastchannel;
		int uncommitted_switch;

	protected:
		char CstartChannelRadio[30];
		char CstartChannelTV[30];

	public:	
		CZapitSetup(const neutrino_locale_t title = NONEXISTANT_LOCALE, const char * const IconName = NEUTRINO_ICON_SETTINGS);
		~CZapitSetup();

		int exec(CMenuTarget* parent, const std::string & actionKey);
		void InitZapitChannelHelper(CZapitClient::channelsMode mode);

};

class CZapitSetupNotifier : public CChangeObserver
{
	private:
		CMenuForwarder* toDisable[2];
	public:
		CZapitSetupNotifier(CMenuForwarder*, CMenuForwarder*);
		bool changeNotify(const neutrino_locale_t OptionName, void * data);
};

#endif
