#ifndef __setting_helpers__
#define __setting_helpers__

/*
	$Id: setting_helpers.h,v 1.105 2010/07/30 20:54:13 dbt Exp $

	Neutrino-GUI  -   DBoxII-Project

	Copyright (C) 2001 Steffen Hehn 'McClean'
	Homepage: http://dbox.cyberphoria.org/

	Kommentar:

	Diese GUI wurde von Grund auf neu programmiert und sollte nun vom
	Aufbau und auch den Ausbaumoeglichkeiten gut aussehen. Neutrino basiert
	auf der Client-Server Idee, diese GUI ist also von der direkten DBox-
	Steuerung getrennt. Diese wird dann von Daemons uebernommen.


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

#ifndef __SETTING_HELPERS__
#define __SETTING_HELPERS__

#include <config.h>
#include <gui/widget/menue.h>
#include <zapit/client/zapitclient.h>

#include <string>

unsigned long long getcurrenttime();
std::string getPidof(const std::string& process_name);
std::string getFileEntryString(const char* filename, const std::string& filter_entry, const int& column_num);
std::string getInterface();

// extern char CstartChannelRadio[30]; /* defined in setting_helpers.cpp */
// extern char CstartChannelTV[30];

class CSatDiseqcNotifier : public CChangeObserver
{
	private:
		CMenuItem* satMenu;
		CMenuItem* extMenu;
		CMenuItem* extMotorMenu;
		CMenuItem* repeatMenu;
	protected:
		CSatDiseqcNotifier( ) : CChangeObserver(){};  // prevent calling constructor without data we need
	public:
		CSatDiseqcNotifier( CMenuItem* SatMenu, CMenuItem* ExtMenu, CMenuItem* ExtMotorMenu, CMenuItem* RepeatMenu) : CChangeObserver()
		{ satMenu = SatMenu; extMenu = ExtMenu; extMotorMenu = ExtMotorMenu; repeatMenu = RepeatMenu;};
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CTP_scanNotifier : public CChangeObserver
{
	private:
		CMenuOptionChooser* toDisable1[2];
		CMenuForwarder* toDisable2[2];
		CMenuOptionStringChooser* toDisable3[1];		
	public:
		CTP_scanNotifier(CMenuOptionChooser*, CMenuOptionChooser*, CMenuForwarder*, CMenuForwarder*, CMenuOptionStringChooser*);
		bool changeNotify(const neutrino_locale_t, void *);
};

class CScanSettingsSatManNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *Data);
};

class CDHCPNotifier : public CChangeObserver
{
	private:
		CMenuForwarder* toDisable[5];
	public:
		CDHCPNotifier( CMenuForwarder*, CMenuForwarder*, CMenuForwarder*, CMenuForwarder*, CMenuForwarder*);
		bool changeNotify(const neutrino_locale_t, void * data);
};
class CStreamingNotifier : public CChangeObserver
{
	private:
		CMenuItem* toDisable[11];
	public:
		CStreamingNotifier( CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*);
		bool changeNotify(const neutrino_locale_t, void *);
};

class COnOffNotifier : public CChangeObserver
{
	private:
		int number;
		CMenuItem* toDisable[5];
	public:
		COnOffNotifier (CMenuItem* a1,CMenuItem* a2 = NULL,CMenuItem* a3 = NULL,CMenuItem* a4 = NULL,CMenuItem* a5 = NULL);
		bool changeNotify(const neutrino_locale_t, void *Data);
};

class CRecordingNotifier : public CChangeObserver
{
	private:
		CMenuItem* toDisable[9];
	public:
		CRecordingNotifier(CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem*);
		bool changeNotify(const neutrino_locale_t OptionName, void*);
};

class CRecordingSafetyNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CZaptoSafetyNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CRecordingNotifier2 : public CChangeObserver
{
	private:
		CMenuItem* toDisable[1];
	public:
		CRecordingNotifier2( CMenuItem*);
		bool changeNotify(const neutrino_locale_t, void *);
};

class CMiscNotifier : public CChangeObserver
{
	private:
		CMenuItem* toDisable[4];
	public:
		CMiscNotifier( CMenuItem*, CMenuItem*, CMenuItem*, CMenuItem* );
		bool changeNotify(const neutrino_locale_t, void *);
};

class CLcdNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

#ifndef TUXTXT_CFG_STANDALONE
class CTuxtxtCacheNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};
#endif

class CPauseSectionsdNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

#ifdef ENABLE_RADIOTEXT
class CRadiotextNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};
#endif

#ifdef HAVE_DBOX_HARDWARE
class CSPTSNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};
#endif
		
class CSectionsdConfigNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * );
};

class CTouchFileNotifier : public CChangeObserver
{
	const char * filename;
 public:
	inline CTouchFileNotifier(const char * file_to_modify)
		{
			filename = file_to_modify;
		};
	bool changeNotify(const neutrino_locale_t, void * data);
};

class CColorSetupNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CAudioSetupNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t OptionName, void *);
};

class CAudioSetupNotifier2 : public CChangeObserver
{
	private:
		CMenuItem* toDisable[1];
	public:
		CAudioSetupNotifier2( CMenuItem* );
		bool changeNotify(const neutrino_locale_t, void *);
};

class CUserMenuNotifier : public CChangeObserver
{
	private:
		CMenuForwarder *menuitem;
	public:
		CUserMenuNotifier(CMenuForwarder *item = NULL);
		void setItem(CMenuForwarder *item);
		bool changeNotify(const neutrino_locale_t, void *);
};

class CKeySetupNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CShutdownCountNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CConsoleDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CFdxChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CTimingSettingsNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t OptionName, void *);
};

class CFontSizeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void *);
};

class CRecAPIDSettingsNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t OptionName, void*);
};

class CAPIDChangeExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

void showSubchan(const std::string & subChannelName);
class CNVODChangeExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

class CStreamFeaturesChangeExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

class CMoviePluginChangeExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

class CUCodeCheckExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};


class CDVBInfoExec : public CMenuTarget
{
	public:
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

void testNetworkSettings(const char* ip, const char* netmask, const char* broadcast, const char* gateway, const char* nameserver, bool ip_static);
void showCurrentNetworkSettings();
class CNetAdapter
{
	private:
		long mac_addr_sys ( u_char *addr);	
	public:
		std::string getMacAddr(void);
};



#endif
#endif
