/*
        $Id: personalize.cpp,v 1.24 2010/09/30 18:11:54 dbt Exp $

        Customization Menu - Neutrino-GUI

        Copyright (C) 2007 Speed2206
        and some other guys

        Kommentar:

        This is the customization menu, as originally showcased in
        Oxygen. It is a more advanced version of the 'user levels'
        patch currently available.


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
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <global.h>
#include <neutrino.h>
#include <driver/fontrenderer.h>
#include <driver/rcinput.h>
#include <driver/screen_max.h>
#include <daemonc/remotecontrol.h>
#include <gui/widget/helpbox.h>
#include "widget/menue.h"
#include "widget/messagebox.h"
#include "widget/hintbox.h"
#include "widget/lcdcontroler.h"
#include "widget/keychooser.h"
#include "widget/stringinput.h"
#include "widget/stringinput_ext.h"
#include "color.h"
#include "personalize.h"

#define PERSONALIZE_STD_OPTION_COUNT 3
#define PERSONALIZE_EDP_OPTION_COUNT 3
#define PERSONALIZE_EOD_OPTION_COUNT 2
#define PERSONALIZE_YON_OPTION_COUNT 2

const CMenuOptionChooser::keyval PERSONALIZE_STD_OPTIONS[PERSONALIZE_STD_OPTION_COUNT] =
{
	{ 0, LOCALE_PERSONALIZE_NOTVISIBLE      },                                      // The option is NOT visible on the menu's
	{ 1, LOCALE_PERSONALIZE_VISIBLE         },                                      // The option is visible on the menu's
	{ 2, LOCALE_PERSONALIZE_PIN      },                                      // PIN Protect the item on the menu
};

const CMenuOptionChooser::keyval PERSONALIZE_EDP_OPTIONS[PERSONALIZE_EDP_OPTION_COUNT] =
{
	{ 0, LOCALE_PERSONALIZE_DISABLED        },                                      // The menu is NOT enabled / accessible
	{ 1, LOCALE_PERSONALIZE_ENABLED         },                                      // The menu is enabled / accessible
	{ 2, LOCALE_PERSONALIZE_PIN      },                                      // The menu is enabled and protected with PIN
};

const CMenuOptionChooser::keyval PERSONALIZE_EOD_OPTIONS[PERSONALIZE_EOD_OPTION_COUNT] =
{
	{ 0, LOCALE_PERSONALIZE_DISABLED        },                                      // The option is NOT enabled / accessible
	{ 1, LOCALE_PERSONALIZE_ENABLED         },                                      // The option is enabled / accessible
};

const CMenuOptionChooser::keyval PERSONALIZE_YON_OPTIONS[PERSONALIZE_YON_OPTION_COUNT] =
{
	{ 0, LOCALE_PERSONALIZE_NOTPROTECTED    },                                      // The menu/option is NOT protected
	{ 1, LOCALE_PERSONALIZE_PINPROTECT      },                                      // The menu/option is protected by a PIN
};



CPersonalizeGui::CPersonalizeGui()
: configfile('\t')
{
	frameBuffer = CFrameBuffer::getInstance();
	
	width 	= w_max (710, 100);
	hheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	mheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
	height 	= hheight+13*mheight+ 10;
	x	= getScreenStartX (width);
	y	= getScreenStartY (height);
}

int CPersonalizeGui::exec(CMenuTarget* parent, const std::string & actionKey)
{
	int res = menu_return::RETURN_REPAINT;

	if (parent)
	{
		parent->hide();
	}

	if(actionKey=="mainmenu_options") {                                     // Personalize the Main Menu
		ShowMainMenuOptions();
		return res;
	}

	if(actionKey=="settings_options") {                                     // Personalize the Settings Menu
		ShowSettingsOptions();
		return res;
	}

	if (actionKey=="service_options") {                                     // Personalize the Service Menu
		ShowServiceOptions();
		return res;
	}
				
	if (actionKey=="personalize_help") {                                     // Personalize help
		ShowHelpPersonalize();
		return res;
	}

	ShowPersonalizationMenu();                                              // Show main Personalization Menu
	return res;
}


void CPersonalizeGui::hide()
{
	frameBuffer->paintBackgroundBoxRel(x,y, width,height);
}

void CPersonalizeGui::ShowPersonalizationMenu()
{
/*      This is the main personalization menu. From here we can go to the other sub-menu's and enable/disable
        the PIN code feature, as well as determine whether or not the EPG menu/Features menu is accessible. */

	manageSettings();

	CMenuWidget* pMenu = new CMenuWidget(LOCALE_PERSONALIZE_HEAD,NEUTRINO_ICON_PROTECTING, width);
	CPINChangeWidget * pinChangeWidget = new CPINChangeWidget(LOCALE_PERSONALIZE_PINCODE, g_settings.personalize_pincode, 4, LOCALE_PERSONALIZE_PINHINT);

	pMenu->addItem(GenericMenuSeparator);
	pMenu->addItem(GenericMenuBack);
	pMenu->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_PERSONALIZE_ACCESS));

	pMenu->addItem(new CMenuOptionChooser(LOCALE_PERSONALIZE_PINSTATUS, (int *)&g_settings.personalize_pinstatus, PERSONALIZE_YON_OPTIONS, PERSONALIZE_YON_OPTION_COUNT, true, NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	pMenu->addItem(new CMenuForwarder(LOCALE_PERSONALIZE_PINCODE, true, g_settings.personalize_pincode, pinChangeWidget, NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	pMenu->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, 	LOCALE_PERSONALIZE_MENUCONFIGURATION));

	pMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_HEAD, true, NULL, this, "mainmenu_options", CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
	pMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_SETTINGS, true, NULL, this, "settings_options", CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));
	pMenu->addItem(new CMenuForwarder(LOCALE_MAINMENU_SERVICE, true, NULL, this, "service_options", CRCInput::RC_1));

	pMenu->addItem(GenericMenuSeparatorLine);
	pMenu->addItem(new CMenuOptionChooser(LOCALE_INFOVIEWER_STREAMINFO, (int *)&g_settings.personalize_bluebutton, PERSONALIZE_EOD_OPTIONS, PERSONALIZE_EOD_OPTION_COUNT, true, NULL, CRCInput::RC_2));
	pMenu->addItem(new CMenuOptionChooser(LOCALE_INFOVIEWER_EVENTLIST, (int *)&g_settings.personalize_redbutton, PERSONALIZE_EOD_OPTIONS, PERSONALIZE_EOD_OPTION_COUNT, true, NULL, CRCInput::RC_3));

	pMenu->addItem(GenericMenuSeparatorLine);
	pMenu->addItem(new CMenuForwarder(LOCALE_PERSONALIZE_HELP, true, NULL, this, "personalize_help", CRCInput::RC_help, NEUTRINO_ICON_BUTTON_HELP));

	pMenu->exec (NULL, "");
	pMenu->hide ();
	delete pMenu;
	
	if (haveChangedSettings())
	{
		if (ShowMsgUTF(LOCALE_PERSONALIZE_HEAD, g_Locale->getText(LOCALE_PERSONALIZE_SAVERESTART), CMessageBox::mbrNo, CMessageBox::mbYes | CMessageBox::mbNo, NEUTRINO_ICON_PROTECTING) == CMessageBox::mbrYes)
			SaveAndRestart();
		else
			if (ShowMsgUTF(LOCALE_PERSONALIZE_HEAD, g_Locale->getText(LOCALE_MESSAGEBOX_DISCARD), CMessageBox::mbrNo, CMessageBox::mbYes | CMessageBox::mbNo, NEUTRINO_ICON_PROTECTING) == CMessageBox::mbrYes)
				restoreSettings();
	}
		
}

void CPersonalizeGui::ShowMainMenuOptions()
{
/*      Here we give the user the option to enable, disable, or PIN protect items on the Main Menu.
        We also provide a means of PIN protecting the menu itself. */

	int shortcut = 1;

	CMenuWidget* pMMMenu = new CMenuWidget(LOCALE_MAINMENU_HEAD,NEUTRINO_ICON_PROTECTING, width);

	pMMMenu->addItem(GenericMenuSeparator);
	pMMMenu->addItem(GenericMenuBack);
	pMMMenu->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_PERSONALIZE_ACCESS));

	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_TVMODE, (int *)&g_settings.personalize_tvmode, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_RADIOMODE, (int *)&g_settings.personalize_radiomode, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_SCARTMODE, (int *)&g_settings.personalize_scartmode, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_GAMES, (int *)&g_settings.personalize_games, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));
	pMMMenu->addItem(GenericMenuSeparatorLine);

#ifdef ENABLE_AUDIOPLAYER
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_AUDIOPLAYER, (int *)&g_settings.personalize_audioplayer,PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
#ifdef ENABLE_INTERNETRADIO
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_INETRADIO_NAME, (int *)&g_settings.personalize_inetradio,PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
#endif
#endif
#ifdef ENABLE_ESD
	//check if esound is available
	if (access("/bin/esd", X_OK) == 0 || access("/var/bin/esd", X_OK) == 0) {
		pMMMenu->addItem(new CMenuOptionChooser(LOCALE_ESOUND_NAME, (int *)&g_settings.personalize_esound,PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
	}
#endif
#ifdef ENABLE_MOVIEPLAYER
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_MOVIEPLAYER, (int *)&g_settings.personalize_movieplayer, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
#endif
#ifdef ENABLE_PICTUREVIEWER
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_PICTUREVIEWER, (int *)&g_settings.personalize_pictureviewer, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
#endif
#ifdef ENABLE_UPNP
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_UPNPBROWSER, (int *)&g_settings.personalize_upnpbrowser, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
#endif
#if defined(ENABLE_AUDIOPLAYER) || defined(ENABLE_INTERNETRADIO) || defined(ENABLE_ESD) || defined(ENABLE_MOVIEPLAYER) || defined(ENABLE_PICTUREVIEWER) || defined(ENABLE_UPNP)
	pMMMenu->addItem(GenericMenuSeparatorLine);
#endif
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_SLEEPTIMER, (int *)&g_settings.personalize_sleeptimer, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_REBOOT, (int *)&g_settings.personalize_reboot, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::convertDigitToKey(shortcut++)));
	pMMMenu->addItem(new CMenuOptionChooser(LOCALE_MAINMENU_SHUTDOWN, (int *)&g_settings.personalize_shutdown, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_standby, NEUTRINO_ICON_BUTTON_POWER));

	pMMMenu->addItem(GenericMenuSeparator);

	pMMMenu->exec (NULL, "");
	pMMMenu->hide ();
	delete pMMMenu;

}

void CPersonalizeGui::ShowSettingsOptions()
{
/*      Here we give the user the option to enable, disable, or PIN protect items on the Settings Menu.
        We also provide a means of PIN protecting the menu itself. */

	CMenuWidget* pSTMenu = new CMenuWidget(LOCALE_MAINMENU_SETTINGS,NEUTRINO_ICON_PROTECTING, width);

	pSTMenu->addItem(GenericMenuSeparator);
	pSTMenu->addItem(GenericMenuBack);
	pSTMenu->addItem(GenericMenuSeparatorLine);

	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_PERSONALIZE_SETUPMENUWITHPIN, (int *)&g_settings.personalize_settings, PERSONALIZE_YON_OPTIONS, PERSONALIZE_YON_OPTION_COUNT, true, NULL));
	pSTMenu->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_PERSONALIZE_ACCESS));

	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_VIDEO, (int *)&g_settings.personalize_video, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_1));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_AUDIO, (int *)&g_settings.personalize_audio, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_2));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_PARENTALLOCK_PARENTALLOCK, (int *)&g_settings.personalize_youth, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_3));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_NETWORK, (int *)&g_settings.personalize_network, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_4));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_RECORDING, (int *)&g_settings.personalize_recording, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_5));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_OSD, (int *)&g_settings.personalize_colors, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_6));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_LCD, (int *)&g_settings.personalize_lcd, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_7));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_KEYBINDING, (int *)&g_settings.personalize_keybinding, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_8));
#ifdef ENABLE_DRIVE_GUI
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_DRIVE_SETUP_HEAD, (int *)&g_settings.personalize_drive_setup_stat, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_9));
#endif
#if defined(ENABLE_AUDIOPLAYER) || defined(ENABLE_PICTUREVIEWER) || defined(ENABLE_ESD) || defined(ENABLE_MOVIEPLAYER)
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MEDIAPLAYERSETTINGS_GENERAL, (int *)&g_settings.personalize_mediaplayer, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));
#endif
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_DRIVER, (int *)&g_settings.personalize_driver, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	pSTMenu->addItem(new CMenuOptionChooser(LOCALE_MAINSETTINGS_MISC, (int *)&g_settings.personalize_misc, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));

	pSTMenu->exec (NULL, "");
	pSTMenu->hide ();
	delete pSTMenu;

}

void CPersonalizeGui::ShowServiceOptions()
{
/*      Here we give the user the option to enable, disable, or PIN protect items on the Service Menu.
        We also provide a means of PIN protecting the menu itself. */

	CMenuWidget* pSMMenu = new CMenuWidget(LOCALE_MAINMENU_SERVICE,NEUTRINO_ICON_PROTECTING, width);

	pSMMenu->addItem(GenericMenuSeparator);
	pSMMenu->addItem(GenericMenuBack);
	pSMMenu->addItem(GenericMenuSeparatorLine);

	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_PERSONALIZE_SVPROTECT, (int *)&g_settings.personalize_service, PERSONALIZE_YON_OPTIONS, PERSONALIZE_YON_OPTION_COUNT, true, NULL));
	pSMMenu->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_PERSONALIZE_ACCESS));

	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_BOUQUETEDITOR_NAME, (int *)&g_settings.personalize_bouqueteditor, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_SCANTS, (int *)&g_settings.personalize_scants, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	pSMMenu->addItem(GenericMenuSeparatorLine);
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_RELOAD, (int *)&g_settings.personalize_reload, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_1));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_GETPLUGINS, (int *)&g_settings.personalize_getplugins, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_2));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_RESTART, (int *)&g_settings.personalize_restart, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_3));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_EPGRESTART, (int *)&g_settings.personalize_epgrestart, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_4));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_UCODECHECK, (int *)&g_settings.personalize_ucodecheck, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_5));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_CHAN_EPG_STAT, (int *)&g_settings.personalize_chan_epg_stat, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_6));

	pSMMenu->addItem(GenericMenuSeparatorLine);
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_IMAGEINFO, (int *)&g_settings.personalize_imageinfo, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
	pSMMenu->addItem(new CMenuOptionChooser(LOCALE_SERVICEMENU_UPDATE, (int *)&g_settings.personalize_update, PERSONALIZE_STD_OPTIONS, PERSONALIZE_STD_OPTION_COUNT, true, NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));

	pSMMenu->exec (NULL, "");
	pSMMenu->hide ();
	delete pSMMenu;
}

void CPersonalizeGui::ShowHelpPersonalize()
{
	Helpbox helpbox;
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE1));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE2));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE3));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE4));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE5));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE6));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE7));
	helpbox.addLine(g_Locale->getText(LOCALE_PERSONALIZE_HELP_LINE8));
	hide();
	helpbox.show(LOCALE_PERSONALIZE_HELP);
}

void CPersonalizeGui::SaveAndRestart()
{
	// Save the settings and restart Neutrino, if user wants to!
	CHintBox * hintBox = new CHintBox(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_MAINSETTINGS_SAVESETTINGSNOW_HINT)); // UTF-8
	hintBox->paint();
	CNeutrinoApp::getInstance()->saveSetup();
	hintBox->hide();
	delete hintBox;

	CNeutrinoApp::getInstance()->exec(NULL, "restart");
}

/*adds a personalized menue entry to menue, based upon menue widget class/structur, expands with personalizing parameters*/
int CPersonalizeGui::addItem(	CMenuWidget &item, 
				const neutrino_locale_t Text,
				bool isActiv,
				const char * const Option,
				CMenuTarget* Target,
				const char * const ActionKey,
				neutrino_msg_t DirectKey,
				const char * const IconName,
				const bool defaultselected,
				const int & personalize_mode,
				const int & personalize_protect_mode,
				const bool alwaysAsk)
{
	int ret = 1;

	if (personalize_mode == PERSONALIZE_MODE_VISIBLE && personalize_protect_mode == PROTECT_MODE_NOT_PROTECTED) 
	{
		item.addItem(new CMenuForwarder(Text, isActiv, Option, Target, ActionKey, DirectKey, IconName),  defaultselected);
	}
	else if (personalize_mode == PERSONALIZE_MODE_PIN || personalize_protect_mode == PROTECT_MODE_PIN_PROTECTED)
	{
		item.addItem(new CLockedMenuForwarder(Text, g_settings.personalize_pincode, alwaysAsk, isActiv, (char*)Option, Target, ActionKey, DirectKey, IconName), defaultselected);
	}
	else
	{
		ret = 0;
	}
	return ret;
}

// returns RC_key depends of shortcut between key number 1 to 0, 10 returns 0, >10 returns no key
// parameter alternate_rc_key allows using an alternate key, default key is RC_nokey
neutrino_msg_t CPersonalizeGui::setShortcut(const int & shortcut_num, neutrino_msg_t alternate_rc_key)
{
	if (shortcut_num < 10) 
	{
		return CRCInput::convertDigitToKey(shortcut_num);
	}
	else if (shortcut_num == 10) 
	{
		return CRCInput::RC_0;
	}
	else	
	{
		return alternate_rc_key;
	}
}


//handle/collects old int settings
void  CPersonalizeGui::handleSetting(int *setting)
{	
	settings_int_t val	= {*setting, setting};
	v_int_settings.push_back(val);
}

void  CPersonalizeGui::manageSettings()
{
	//main menu settings
	handleSetting(&g_settings.personalize_tvmode);
	handleSetting(&g_settings.personalize_radiomode);
	handleSetting(&g_settings.personalize_scartmode);
	handleSetting(&g_settings.personalize_games);
#ifdef ENABLE_AUDIOPLAYER
	handleSetting(&g_settings.personalize_audioplayer);
#ifdef ENABLE_INTERNETRADIO
	handleSetting(&g_settings.personalize_inetradio);
#endif
#endif
#ifdef ENABLE_ESD
	handleSetting(&g_settings.personalize_esound);
#endif
#ifdef ENABLE_MOVIEPLAYER
	handleSetting(&g_settings.personalize_movieplayer);
#endif
#ifdef ENABLE_PICTUREVIEWER
	handleSetting(&g_settings.personalize_pictureviewer);
#endif
#ifdef ENABLE_UPNP
	handleSetting(&g_settings.personalize_upnpbrowser);
#endif
	handleSetting(&g_settings.personalize_settings);
	handleSetting(&g_settings.personalize_service);
	handleSetting(&g_settings.personalize_sleeptimer);
	handleSetting(&g_settings.personalize_reboot);
	handleSetting(&g_settings.personalize_shutdown);
	
	//settings menu
	handleSetting(&g_settings.personalize_settings);
	handleSetting(&g_settings.personalize_video);
	handleSetting(&g_settings.personalize_audio);
	handleSetting(&g_settings.personalize_youth);
	handleSetting(&g_settings.personalize_network);
	handleSetting(&g_settings.personalize_recording);
	handleSetting(&g_settings.personalize_colors);
	handleSetting(&g_settings.personalize_lcd);
	handleSetting(&g_settings.personalize_keybinding);
#if defined(ENABLE_AUDIOPLAYER) || defined(ENABLE_PICTUREVIEWER) || defined(ENABLE_ESD) || defined(ENABLE_MOVIEPLAYER)
	handleSetting(&g_settings.personalize_mediaplayer);
#endif
	handleSetting(&g_settings.personalize_driver);
	handleSetting(&g_settings.personalize_misc);
#ifdef ENABLE_DRIVE_GUI
	handleSetting(&g_settings.personalize_drive_setup_stat);
#endif

	//service menu
	handleSetting(&g_settings.personalize_service);
	handleSetting(&g_settings.personalize_bouqueteditor);
	handleSetting(&g_settings.personalize_scants);
	handleSetting(&g_settings.personalize_reload);
	handleSetting(&g_settings.personalize_getplugins);
	handleSetting(&g_settings.personalize_restart);
	handleSetting(&g_settings.personalize_epgrestart);
	handleSetting(&g_settings.personalize_ucodecheck);
	handleSetting(&g_settings.personalize_imageinfo);
	handleSetting(&g_settings.personalize_update);
	handleSetting(&g_settings.personalize_chan_epg_stat);
	
}

//check for setup changes
bool  CPersonalizeGui::haveChangedSettings()
{
	//compare old settings with current settings
	for (uint i = 0; i < v_int_settings.size(); i++)
		if (v_int_settings[i].old_val != *v_int_settings[i].p_val)
			return true;

	return false;
}

//restore old settings
void CPersonalizeGui::restoreSettings()
{
	//restore settings with current settings
	for (uint i = 0; i < v_int_settings.size(); i++)
		*v_int_settings[i].p_val = v_int_settings[i].old_val;
}

