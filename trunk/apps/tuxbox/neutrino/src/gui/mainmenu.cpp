/* mainmenu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include "libnet.h"
#include <sys/wait.h>

#include <global.h>
#include <neutrino.h>

#include <driver/screen_max.h>
#include <driver/fontrenderer.h>
#include <driver/rcinput.h>
#include <daemonc/remotecontrol.h>

#include "widget/menue.h"
#include "widget/messagebox.h"
#include "widget/hintbox.h"
#include "widget/colorchooser.h"
#include "widget/lcdcontroler.h"
#include "widget/keychooser.h"
#include "widget/stringinput.h"
#include "widget/stringinput_ext.h"

#include "bouquetlist.h"
#include "color.h"
#include "eventlist.h"
#include "infoviewer.h"
#include "systeminfo.h"
#include "camdmenu.h"
#include "mainmenu.h"

MainMenu::MainMenu()
{
	frameBuffer = CFrameBuffer::getInstance();
	width = 600;
	hheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	mheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
	height = hheight+13*mheight+ 10;

	x = getScreenStartX (width);
	y = getScreenStartY (height);
}

int MainMenu::exec(CMenuTarget* parent, const std::string & actionKey)
{
	int   res = menu_return::RETURN_REPAINT;

	if (parent)
	{
		parent->hide();
	}

	Settings();

	return res;
}

void MainMenu::hide()
{
	frameBuffer->paintBackgroundBoxRel(x,y, width,height);
}

void MainMenu::Settings()
{
	// Dynamic renumbering
	int shortcut = 1;

	//MENU
	CMenuWidget* scSettings = new CMenuWidget(LOCALE_INFOVIEWER_STREAMINFO, "settings.raw");
	scSettings->addItem( new CMenuSeparator(CMenuSeparator::ALIGN_LEFT | CMenuSeparator::SUB_HEAD | CMenuSeparator::STRING, LOCALE_MAINMENU_EXTRASMENU));
	scSettings->addItem(GenericMenuSeparator);
	scSettings->addItem(GenericMenuBack);
	scSettings->addItem(GenericMenuSeparatorLine);

	//CamdMenu
	scSettings->addItem(new CMenuForwarder(LOCALE_CAMDMENU_CAMDMENU, true, NULL, new CamdAuswahl(), NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));

	//SysInfo
	CMenuWidget* scSysInfo = new CMenuWidget(LOCALE_MAINMENU_EXTRASMENU, "settings.raw");
	scSettings->addItem(new CMenuForwarder(LOCALE_SYSTEMINFO_MENU, true, NULL, scSysInfo, NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
	scSysInfo->addItem( new CMenuSeparator(CMenuSeparator::ALIGN_LEFT | CMenuSeparator::SUB_HEAD | CMenuSeparator::STRING, LOCALE_SYSTEMINFO_MENU));
	scSysInfo->addItem(GenericMenuSeparator);
	scSysInfo->addItem(GenericMenuBack);
	scSysInfo->addItem(GenericMenuSeparatorLine);
 	scSysInfo->addItem(new CMenuForwarder(LOCALE_SYSTEMINFO_PERFORMANCE, true, NULL, new CBESysInfoWidget(1), NULL, CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));
 	scSysInfo->addItem(new CMenuForwarder(LOCALE_SYSTEMINFO_MESSAGES, true, NULL, new CBESysInfoWidget(2), NULL, CRCInput::RC_green, NEUTRINO_ICON_BUTTON_GREEN));
 	scSysInfo->addItem(new CMenuForwarder(LOCALE_SYSTEMINFO_CPU, true, NULL, new CBESysInfoWidget(3), NULL, CRCInput::RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));
 	scSysInfo->addItem(new CMenuForwarder(LOCALE_SYSTEMINFO_PROCESS, true, NULL, new CBESysInfoWidget(4), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));

	scSettings->exec (NULL, "");
	scSettings->hide ();
	delete scSettings;
}

