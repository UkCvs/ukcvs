/*
  $Id: camdmenu.cpp, v2.3 2008/09/20 19:25:21 mohousch Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
#include "widget/lcdcontroler.h"
#include "widget/keychooser.h"
#include "widget/stringinput.h"
#include "widget/stringinput_ext.h"

#include "color.h"
#include "camdmenu.h"
#include "systeminfo.h"

#define OPTIONS_OFF_ON_OPTION_COUNT 2
const CMenuOptionChooser::keyval OPTIONS_OFF_ON_OPTIONS[OPTIONS_OFF_ON_OPTION_COUNT] =
{
	{ 0, LOCALE_OPTIONS_OFF  },
	{ 1, LOCALE_OPTIONS_ON }
};

CamdAuswahl::CamdAuswahl()
{
	frameBuffer = CFrameBuffer::getInstance();
	width = 600;
	hheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	mheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
	height = hheight+13*mheight+ 10;

	x = getScreenStartX (width);
	y = getScreenStartY (height);
}

int CamdAuswahl::exec(CMenuTarget* parent, const std::string & actionKey)
{
	int   res = menu_return::RETURN_REPAINT;
	
	if(actionKey == "camdreset") 
	{
		this->CamdReset();
		return res;
	}
	
	if (parent)
	{
		parent->hide();
	}

	paint();

	Settings();
	
	return res;
}

void CamdAuswahl::hide()
{
	frameBuffer->paintBackgroundBoxRel(x,y, width,height);
}

void CamdAuswahl::paint()
{
	printf("[neutrino-rebuild] softcam emulators menu\n");
}

void CamdAuswahl::Settings()
{
	//camd2
	int camd2 = 0;

	FILE* fdcamd2 = fopen("/var/etc/.camd2", "r");
	if(fdcamd2)
	{
		camd2=1;
		fclose(fdcamd2);
	}

	//camd3
	int camd3 = 0;

	FILE* fdcamd3 = fopen("/var/etc/.camd3", "r");
	if(fdcamd3)
	{
		camd3=1;
		fclose(fdcamd3);
	}

	//mgcamd
	int mgcamd = 0;

	FILE* fdmgcamd = fopen("/var/etc/.mgcamd", "r");
	if(fdmgcamd)
	{
		mgcamd=1;
		fclose(fdmgcamd);
	}

	//evocamd
	int evocamd = 0;

	FILE* fdevocamd = fopen("/var/etc/.evocamd", "r");
	if(fdevocamd)
	{
		evocamd=1;
		fclose(fdevocamd);
	}

	//newcamd
	int newcamd = 0;

	FILE* fdnewcamd = fopen("/var/etc/.newcamd", "r");
	if(fdnewcamd)
	{
		newcamd=1;
		fclose(fdnewcamd);
	}

	//cccam
	int cccam = 0;

	FILE* fdcccam = fopen("/var/etc/.cccam", "r");
	if(fdcccam)
	{
		cccam=1;
		fclose(fdcccam);
	}
	
	//scam
	int scam = 0;

	FILE* fdscam = fopen("/var/etc/.scam", "r");
	if(fdscam)
	{
		scam=1;
		fclose(fdscam);
	}

	//camx
	int camx = 0;

	FILE* fdcamx = fopen("/var/etc/.camx", "r");
	if(fdcamx)
	{
		camx=1;
		fclose(fdcamx);
	}

	//gbox
	int gbox = 0;

	FILE* fdgbox = fopen("/var/etc/.gbox", "r");
	if(fdgbox)
	{
		gbox=1;
		fclose(fdgbox);
	}

	//cardserver
	//betad
	int betad = 0;

	FILE* fdbetad = fopen("/var/etc/.betad", "r");
	if (fdbetad)
	{
		betad=1;
		fclose(fdbetad);
	}
	//cardserver
	int cardserver = 0;

	FILE* fdcardserver = fopen("/var/etc/.cardserver", "r");
	if (fdcardserver)
	{
		cardserver=1;
		fclose(fdcardserver);
	}
	//radegast
	int radegast = 0;

	FILE* fdradegast = fopen("/var/etc/.radegast", "r");
	if (fdradegast)
	{
		radegast=1;
		fclose(fdradegast);
	}
	//newcs
	int newcs = 0;

	FILE* fdnewcs = fopen("/var/etc/.newcs", "r");
	if (fdnewcs)
	{
		newcs=1;
		fclose(fdnewcs);
	}
	//oscam
	int oscam = 0;

	FILE* fdoscam = fopen("/var/etc/.oscam", "r");
	if (fdoscam)
	{
		oscam=1;
		fclose(fdoscam);
	}
	//dynamic nnumeric
	int shortcut = 1;

	//MENU AUFBAUEN
	CMenuWidget* scSettings = new CMenuWidget(LOCALE_MAINMENU_EXTRASMENU, "lock.raw");
	scSettings->addItem( new CMenuSeparator(CMenuSeparator::ALIGN_LEFT | CMenuSeparator::SUB_HEAD | CMenuSeparator::STRING, LOCALE_CAMDMENU_CAMDMENU));
	scSettings->addItem(GenericMenuSeparator);
	scSettings->addItem(GenericMenuBack);
	scSettings->addItem(GenericMenuSeparatorLine);

	//rezap
	scSettings->addItem(new CMenuForwarder(LOCALE_CAMDMENU_CAMDRESET, true, "", this, "camdreset", CRCInput::RC_red, NEUTRINO_ICON_BUTTON_RED));

	//cam info
 	scSettings->addItem(new CMenuForwarder(LOCALE_CAMDMENU_INFO, true, NULL, new CBESysInfoWidget(5), NULL, CRCInput::RC_blue, NEUTRINO_ICON_BUTTON_BLUE));

	//camd auswahl
	FILE* fdcamd2_installed = fopen("/var/bin/emu/camd2", "r");
	FILE* fdcamd3_installed = fopen("/var/bin/emu/camd3", "r");
	FILE* fdmgcamd_installed = fopen("/var/bin/emu/mgcamd", "r");
	FILE* fdevocamd_installed = fopen("/var/bin/emu/evocamd", "r");
	FILE* fdnewcamd_installed = fopen("/var/bin/emu/newcamd", "r");
	FILE* fdcccam_installed = fopen("/var/bin/emu/CCcam", "r");
	FILE* fdscam_installed = fopen("/var/bin/emu/scam", "r");
	FILE* fdcamx_installed = fopen("/var/bin/emu/camx", "r");
	FILE* fdgbox_installed = fopen("/var/bin/emu/gbox", "r");
	FILE* fdoscam_installed = fopen("/var/bin/emu/oscam", "r");

	CHintBox * CamdDetectionBox = new CHintBox(LOCALE_CAMDMENU_CAMDMENU, "Detecting SoftCams...");
	CamdDetectionBox->paint();

	//show camd bar title if one camd is installed
	if((fdcamd2_installed) || (fdcamd3_installed) || (fdmgcamd_installed) || (fdevocamd_installed) || (fdnewcamd_installed) || (fdcccam_installed) || (fdscam_installed) || (fdcamx_installed) || (fdgbox_installed)  || (fdoscam_installed))
	{
		system("echo 'Installed Softcam Versions' >/tmp/emu_versions.txt");
		scSettings->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_CAMDMENU_CAMDMENU));
	}
	//camd2
	if(fdcamd2_installed)
	{
		CCamd2DestChangeNotifier	*Camd2DestinationChanger = new CCamd2DestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_CAMD2, &camd2, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, Camd2DestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdcamd2_installed);
	}
	//camd3
	if(fdcamd3_installed)
	{
		CCamd3DestChangeNotifier	*Camd3DestinationChanger = new CCamd3DestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_CAMD3, &camd3, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, Camd3DestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdcamd3_installed);
	}
	//mgcamd
	if(fdmgcamd_installed)
	{
		system("strings /var/bin/emu/mgcamd|grep 'v[0-9]..[0-9]' >>/tmp/emu_versions.txt");
		CMgCamdDestChangeNotifier	*MgCamdDestinationChanger = new CMgCamdDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_MGCAMD, &mgcamd, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, MgCamdDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdmgcamd_installed);
	}
	//evocamd
	if(fdevocamd_installed)
	{
		CEvoCamdDestChangeNotifier	*EvoCamdDestinationChanger = new CEvoCamdDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_EVOCAMD, &evocamd, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, EvoCamdDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdevocamd_installed);
	}
	//newcamd
	if(fdnewcamd_installed)
	{
		CNewCamdDestChangeNotifier	*NewCamdDestinationChanger = new CNewCamdDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_NEWCAMD, &newcamd, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, NewCamdDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdnewcamd_installed);
	}
	//cccam
	if(fdcccam_installed)
	{
		system("strings /var/bin/emu/CCcam|grep 'CCcam [0-9].[0-9].[0-9]' >>/tmp/emu_versions.txt");
		CCCcamDestChangeNotifier	*CCcamDestinationChanger = new CCCcamDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_CCCAM, &cccam, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, CCcamDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdcccam_installed);
	}
	//scam
	if(fdscam_installed)
	{
		CScamDestChangeNotifier	 	*ScamDestinationChanger = new CScamDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_SCAM, &scam, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, ScamDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdscam_installed);
	}
	//camx
	if(fdcamx_installed)
	{
		CCamXDestChangeNotifier		*CamXDestinationChanger = new CCamXDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_CAMX, &camx, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, CamXDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdcamx_installed);
	}
	//gbox
	if(fdgbox_installed)
	{
		CGboxDestChangeNotifier	 	*GboxDestinationChanger = new CGboxDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_GBOX, &gbox, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, GboxDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdgbox_installed);
	}
	//oscam
	if(fdoscam_installed)
	{
		COscamDestChangeNotifier	 *OscamDestinationChanger = new COscamDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CAMDMENU_OSCAM, &oscam, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, OscamDestinationChanger, CRCInput::convertDigitToKey(shortcut++)));
		fclose(fdoscam_installed);
	}
	//CardServer
	FILE* fdbetad_installed = fopen("/var/bin/emu/betad", "r");
	FILE* fdcardserver_installed = fopen("/var/bin/emu/cardserver", "r");
	FILE* fdradegast_installed = fopen("/var/bin/emu/radegast", "r");
	FILE* fdnewcs_installed = fopen("/var/bin/emu/newcs", "r");

	//show cardserver bar if one of cardserver is installed
	if((fdbetad_installed) || (fdcardserver_installed) || (fdradegast_installed) || (fdnewcs_installed))
	{
		scSettings->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_CAMDMENU_CARDSERVERMENU));
	}
	//betad
	if(fdbetad_installed)
	{
		CBetadDestChangeNotifier	*BetadDestinationChanger = new CBetadDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CARDSERVERMENU_BETAD, &betad, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, BetadDestinationChanger));
		fclose(fdbetad_installed);
	}
	//cardserver
	if(fdcardserver_installed)
	{
		CCardserverDestChangeNotifier	*CardserverDestinationChanger = new CCardserverDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CARDSERVERMENU_CARDSERVER, &cardserver, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, CardserverDestinationChanger));
		fclose(fdcardserver_installed);
	}
	//radegast
	if(fdradegast_installed)
	{
		CRadegastDestChangeNotifier	*RadegastDestinationChanger = new CRadegastDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CARDSERVERMENU_RADEGAST, &radegast, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, RadegastDestinationChanger));
		fclose(fdradegast_installed);
	}
	//newcs
	if(fdnewcs_installed)
	{
		CNewCSDestChangeNotifier	*NewCSDestinationChanger = new CNewCSDestChangeNotifier;
		scSettings->addItem(new CMenuOptionChooser(LOCALE_CARDSERVERMENU_NEWCS, &newcs, OPTIONS_OFF_ON_OPTIONS, OPTIONS_OFF_ON_OPTION_COUNT, true, NewCSDestinationChanger));
		fclose(fdnewcs_installed);
	}

	CamdDetectionBox->hide();
	delete CamdDetectionBox;

	//emu version
	//voraus die emu version werden manuell in "/var/etc/emu_versions.txt eingetragen
	//camd2
	char camd2version[20] = "Prem-Support";
	char camd3version[7]= "N/A";
	char mgcamdversion[7] = "N/A";
	char evocamdversion[7] = "N/A"; 
	char newcamdversion[7] = "N/A";
	char cccamversion[7]= "N/A"; 
	char scamversion[7]= "N/A"; 
	char camxversion[7]= "N/A"; 
	char gboxversion[7] = "N/A";
	char oscamversion[7] = "N/A";
	char betadversion[7] = "N/A";
	char cardserverversion[7] = "N/A";
	char radegastversion[7] = "N/A";
	char newcsversion[7] = "N/A";

	FILE* fdcamd2_detected = fopen("/var/bin/emu/camd2", "r");
	FILE *fdcamd3_detected = fopen("/var/bin/emu/camd3", "r");
	FILE *fdmgcamd_detected = fopen("/var/bin/emu/mgcamd", "r");
	FILE *fdevocamd_detected = fopen("/var/bin/emu/evocamd", "r");
	FILE *fdnewcamd_detected = fopen("/var/bin/emu/newcamd", "r");
	FILE *fdcccam_detected = fopen("/var/bin/emu/CCcam", "r");
	FILE *fdscam_detected = fopen("/var/bin/emu/scam", "r");
	FILE *fdcamx_detected = fopen("/var/bin/emu/camx", "r");
	FILE *fdgbox_detected = fopen("/var/bin/emu/gbox", "r");
	FILE *fdbetad_detected = fopen("/var/bin/emu/betad", "r");
	FILE *fdcardserver_detected = fopen("/var/bin/emu/cardserver", "r");
	FILE *fdradegast_detected = fopen("/var/bin/emu/radegast", "r");
	FILE *fdnewcs_detected = fopen("/var/bin/emu/newcs", "r");
	FILE *fdoscam_detected = fopen("/var/bin/emu/oscam", "r");

	//show emu version if one emu is present
	if((fdcamd2_detected) || (fdcamd3_detected) || (fdmgcamd_detected) || (fdevocamd_detected) || (fdnewcamd_detected) || (fdcccam_detected) || (fdscam_detected) || (fdcamx_detected) || (fdgbox_detected) || (fdbetad_detected) || (fdcardserver_detected) || (fdradegast_detected) || (fdnewcs_detected) || (fdoscam_detected))
	{
		scSettings->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_CAMDMENU_CAMDVERSION));
	}

	if(fdcamd2_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Camd2 Version", false, camd2version));
		fclose(fdcamd2_detected);
	}
	
	//get auto detected emu version
	FILE *tmp_emu_versions = fopen("/tmp/emu_versions.txt", "r");
	if (tmp_emu_versions) 
	{
		char buffer[120];
		while(fgets(buffer, 120, tmp_emu_versions)!=NULL)
			{	
				sscanf(buffer, "v%7s", mgcamdversion);
				sscanf(buffer, "CCcam %7s", cccamversion);
			}
		fclose(tmp_emu_versions);
	}

	//get manual emu version, overriding auto detected version
	FILE *emu_versions = fopen("/var/etc/emu_versions.txt", "r");
	if (emu_versions) 
	{
		char buffer[120];
		while(fgets(buffer, 120, emu_versions)!=NULL)
			{	
				sscanf(buffer, "camd3=%7s", camd3version);
				sscanf(buffer, "mgcamd=%7s", mgcamdversion);
				sscanf(buffer, "evocamd=%7s", evocamdversion);
				sscanf(buffer, "newcamd=%7s", newcamdversion);
				sscanf(buffer, "cccam=%7s", cccamversion);
				sscanf(buffer, "scam=%7s", scamversion);
				sscanf(buffer, "camx=%7s", camxversion);
				sscanf(buffer, "gbox=%7s", gboxversion);
				sscanf(buffer, "betad=%7s", betadversion);	
				sscanf(buffer, "cardserver=%7s", cardserverversion);
				sscanf(buffer, "radegast=%7s", radegastversion);
				sscanf(buffer, "newcs=%7s", newcsversion);
				sscanf(buffer, "oscam=%7s", oscamversion);
			}
		fclose(emu_versions);
	}

	if (fdcamd3_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Camd3 Version", false, camd3version));
		fclose(fdcamd3_detected);
	}

	if (fdmgcamd_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("MgCamd Version", false, mgcamdversion));
		fclose(fdmgcamd_detected);
	}

	if (fdevocamd_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("EvoCamd Version", false, evocamdversion));
		fclose(fdevocamd_detected);
	}

	if (fdnewcamd_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("NewCamd Version", false, newcamdversion));
		fclose(fdnewcamd_detected);
	}

	if (fdcccam_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("CCcam Version", false, cccamversion));
		fclose(fdcccam_detected);
	}

	if (fdscam_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Scam Version", false, scamversion));
		fclose(fdscam_detected);
	}

	if (fdcamx_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Camx Version", false, camxversion));
		fclose(fdcamx_detected);
	}

	if (fdgbox_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Gbox Version", false, gboxversion));
		fclose(fdgbox_detected);
	}

	if (fdbetad_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Betad Version", false, betadversion));
		fclose(fdbetad_detected);
	}

	if (fdcardserver_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("CardServer Version", false, cardserverversion));
		fclose(fdcardserver_detected);
	}

	if (fdradegast_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("Radegast Version", false, radegastversion));
		fclose(fdradegast_detected);
	}

	if (fdnewcs_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("NewCS Version", false, newcsversion));
		fclose(fdnewcs_detected);
	}
	if (fdoscam_detected)
	{
		scSettings->addItem(new CMenuForwarderNonLocalized("OScam Version", false, oscamversion));
		fclose(fdoscam_detected);
	}

	scSettings->exec (NULL, "");
	scSettings->hide ();
	delete scSettings;
}

bool CamdAuswahl::CamdReset()
{
	//camd2
	int camd2 = 0;

	FILE* fdcamd2 = fopen("/var/etc/.camd2", "r");
	if(fdcamd2)
	{
		camd2=1;
		fclose(fdcamd2);
	}

	//camd3
	int camd3 = 0;

	FILE* fdcamd3 = fopen("/var/etc/.camd3", "r");
	if(fdcamd3)
	{
		camd3=1;
		fclose(fdcamd3);
	}

	//mgcamd
	int mgcamd = 0;

	FILE* fdmgcamd = fopen("/var/etc/.mgcamd", "r");
	if(fdmgcamd)
	{
		mgcamd=1;
		fclose(fdmgcamd);
	}

	//evocamd
	int evocamd = 0;

	FILE* fdevocamd = fopen("/var/etc/.evocamd", "r");
	if(fdevocamd)
	{
		evocamd=1;
		fclose(fdevocamd);
	}

	//newcamd
	int newcamd = 0;

	FILE* fdnewcamd = fopen("/var/etc/.newcamd", "r");
	if(fdnewcamd)
	{
		newcamd=1;
		fclose(fdnewcamd);
	}

	//cccam
	int cccam = 0;

	FILE* fdcccam = fopen("/var/etc/.cccam", "r");
	if(fdcccam)
	{
		cccam=1;
		fclose(fdcccam);
	}
	
	//scam
	int scam = 0;

	FILE* fdscam = fopen("/var/etc/.scam", "r");
	if(fdscam)
	{
		scam=1;
		fclose(fdscam);
	}

	//camx
	int camx = 0;

	FILE* fdcamx = fopen("/var/etc/.camx", "r");
	if(fdcamx)
	{
		camx=1;
		fclose(fdcamx);
	}

	//gbox
	int gbox = 0;

	FILE* fdgbox = fopen("/var/etc/.gbox", "r");
	if(fdgbox)
	{
		gbox=1;
		fclose(fdgbox);
	}
		//oscam
	int oscam = 0;

	FILE* fdoscam = fopen("/var/etc/.oscam", "r");
	if(fdoscam)
	{
		oscam=1;
		fclose(fdoscam);
	}

	CHintBox * CamdResetBox = new CHintBox(LOCALE_CAMDMENU_CAMDRESET, "Please Wait, SoftCamd is Restarting");
	CamdResetBox->paint();
	//karte(camd2)
	if (camd2 == 1)
	{
		//kill camd2
		system("killall -9 camd2");
		system("sleep 2");
		//reactivate camd2
		system("camd2");
		system("sleep 1");
	}

	//camd3
	//kill mgcamd if mgcamd aktiv
	if (camd3 == 1)
	{	
		//kill camd3
		system("pcamd -kill");
		system("killall -9 camd3");
		system("rmmod multicam");
		system("sleep 2");
		//activate camd3
		system("insmod -f /var/modules/multicam.o");
		system("camd3");
		system("sleep 1");	
	}

	//mgcamd
	if (mgcamd == 1)
	{
		//kill mgcamd
		system("kill $( cat /tmp/mgcamd.pid )");
		system("killall -9 camd2");
		system("killall -9 mgcamd");
		system("sleep 2");
		//ativate mgcamd
		system("camd2");
		system("sleep 1");
		system("mgcamd");
		system("sleep 1");
	}

	//evocamd
	if (evocamd == 1)
	{
		//kill evocamd
		system("kill $( pidof evocamd )");
		system("killall -9 evocamd");
		system("sleep 2");
		//activate evocamd
		system("evocamd");
		system("sleep 1");
	}

	//newcamd
	if (newcamd == 1)
	{
		//kill newcamd
		system("kill $( cat /tmp/newcamd.pid )");
		system("killall -9 newcamd");
		system("sleep 2");
		//activate newcamd
		system("newcamd");
		system("sleep 1");	
	}

	//cccam
	if (cccam == 1)
	{
		//kill cccam
		system("killall -9 CCcam");
		system("killall -9 epg-restart");
		system("sleep 2");
		//activate cccam
		system("epg-restart &");
		system("sleep 1");
		system("CCcam &");
		system("sleep 1");
	}

	//scam
	if (scam == 1)
	{
		//kill scam
		system("killall -9 scam");
		system("sleep 2");
		//activate scam
		system("scam");
		system("sleep 1");
	}

	//camx
	if (camx == 1)
	{
		//kill camx
		system("killall -9 camx");
		system("sleep 2");
		//activate camx
		system("camx &");
		system("sleep 1");
	}

	//gbox
	if (gbox == 1)
	{
		//kill gbox
		system("touch /tmp/gbox.kill");
		system("killall -9 gbox");
		system("sleep 2");
		//activate gbox
		system("gbox &");
		system("sleep 1");
	}
	//oscam
	if (oscam == 1)
	{
		//kill oscam
		system("touch /tmp/oscam.kill");
		system("killall -9 oscam");
		system("sleep 2");
		//activate oscam
		system("oscam &");
		system("sleep 1");
	}
	//rezap and delete msgbox
	CChannelList  *channelList;
	channelList = CNeutrinoApp::getInstance()->channelList;
	CNeutrinoApp::getInstance()->channelList->ReZap();
		
	CamdResetBox->hide();
	delete CamdResetBox;
}

bool CCamd2DestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Camd2
	int camd2 = *(int *)Data;
	
	if (camd2 == 1)
	{
		system("touch /var/etc/.camd2");
		system("chmod 755 /var/bin/emu/camd2");
		system("camd2");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMD2START), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 camd2");
		system("rm /var/etc/.camd2");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMD2STOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CCamd3DestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Camd3
	int camd3 = *(int *)Data;
	
	if (camd3 == 1)
	{
		system("touch /var/etc/.camd3");
		system("insmod -f /var/modules/multicam.o");
		system("chmod 755 /var/bin/emu/camd3");
		system("camd3");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMD3START), 450, 2); // UTF-8("")
	}
	else
	{
		system("pcamd -kill");
		system("killall -9 camd3");
		system("rmmod multicam");
		system("rm /var/etc/.camd3");
		system("rm /tmp/ecm.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMD3STOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CMgCamdDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//MgCamd
	int mgcamd = *(int *)Data;
	
	if (mgcamd == 1)
	{
		system("touch /var/etc/.mgcamd");
		system("chmod 755 /var/bin/emu/mgcamd");
		system("camd2");
		system("sleep 1");
		system("mgcamd");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_MGCAMDSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("kill $( cat /tmp/mgcamd.pid )");
		system("killall -9 mgcamd");
		system("killall -9 camd2");
		system("rm /var/etc/.mgcamd");
		system("rm /tmp/ecm.info");
		system("rm /tmp/mgshare.info");
		system("rm /tmp/mgstat.info");
		system("rm /tmp/pid.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_MGCAMDSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CEvoCamdDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//EvoCamd
	int evocamd = *(int *)Data;
	
	if (evocamd == 1)
	{
		system("touch /var/etc/.evocamd");
		system("chmod 755 /var/bin/emu/evocamd");
		system("evocamd");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_EVOCAMDSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("kill $( pidof evocamd )");
		system("killall -9 evocamd");
		system("rm /var/etc/.evocamd");
		system("rm /tmp/ecm.info");
		system("rm /tmp/pid.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_EVOCAMDSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CNewCamdDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//NewCamd
	int newcamd = *(int *)Data;
	
	if (newcamd == 1)
	{
		system("touch /var/etc/.newcamd");
		system("chmod 755 /var/bin/emu/newcamd");
		system("newcamd");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_NEWCAMDSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("kill $( cat /tmp/newcamd.pid )");
		system("killall -9 newcamd");
		system("rm /var/etc/.newcamd");
		system("rm /tmp/cainfo.txt");
		system("rm /tmp/pid.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_NEWCAMDSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CCCcamDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//CCcam
	int cccam = *(int *)Data;
	
	if (cccam == 1)
	{
		system("touch /var/etc/.cccam");
		system("epg-restart &");
		system("sleep 1");
		system("chmod 755 /var/bin/emu/CCcam");
		system("CCcam &");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CCCAMSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 CCcam");
		system("killall -9 epg-restart");
		system("rm /var/etc/.cccam");
		system("rm /tmp/ecm.info");
		system("rm /tmp/ecm0.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CCCAMSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CScamDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Scam
	int scam = *(int *)Data;
	
	if (scam == 1)
	{
		system("touch /var/etc/.scam");
		system("chmod 755 /var/bin/emu/scam");
		system("scam");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_SCAMSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 scam");
		system("rm /var/etc/.scam");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_SCAMSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CCamXDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//CamX
	int camx = *(int *)Data;
	
	if (camx == 1)
	{
		system("touch /var/etc/.camx");
		system("chmod 755 /var/bin/emu/camx");
		system("camx &");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMXSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 camx");
		system("rm /var/etc/.camx");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_CAMXSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CGboxDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Gbox
	int gbox = *(int *)Data;
	
	if (gbox == 1)
	{
		system("touch /var/etc/.gbox");
		system("chmod 755 /var/bin/emu/gbox");
		system("gbox &");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_GBOXSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("touch /tmp/gbox.kill");
		system("killall -9 gbox");
		system("rm /var/etc/.gbox");
		system("rm /tmp/ecm.info");
		system("rm /tmp/gbox.ver");
		system("rm /tmp/pid.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_GBOXSTOP), 450, 2); // UTF-8("")
	}
	return true;
}
bool COscamDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//oscam
	int oscam = *(int *)Data;
	
	if (oscam == 1)
	{
		system("touch /var/etc/.oscam");
		system("chmod 755 /var/bin/emu/oscam");
		system("oscam &");
		system("sleep 1");
		CChannelList  *channelList;
		channelList = CNeutrinoApp::getInstance()->channelList;
		CNeutrinoApp::getInstance()->channelList->ReZap();
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_OSCAMSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("touch /tmp/oscam.kill");
		system("killall -9 oscam");
		system("rm /var/etc/.oscam");
		system("rm /tmp/ecm.info");
		system("rm /tmp/oscam.ver");
		system("rm /tmp/pid.info");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CAMDMENU_OSCAMSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CBetadDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Betad
	int betad = *(int *)Data;
	
	if (betad == 1)
	{
		FILE* fdcardserver = fopen("/var/etc/.cardserver","r");
		if (fdcardserver)
		{
			system("killall -9 cardserver");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdcardserver);
		}

		FILE* fdradegast = fopen("/var/etc/.radegast","r");
		if (fdradegast)
		{
			system("killall -9 radegast");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.radegast");
			fclose(fdradegast);
		}

		FILE* fdnewcs = fopen("/var/etc/.newcs","r");
		if (fdnewcs)
		{
			system("killall -9 newcs");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdnewcs);
		}

		system("touch /var/etc/.betad");
		system("insmod -f /var/modules/camd-cardserver.o");
		system("chmod 755 /var/bin/emu/betad");
		system("betad");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_BETADSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 betad");
		system("rmmod camd-cardserver");
		system("rm /var/etc/.betad");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_BETADSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CCardserverDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Cardserver
	int cardserver = *(int *)Data;
	
	if (cardserver == 1)
	{
		FILE* fdbetad = fopen("/var/etc/.betad","r");
		if (fdbetad)
		{
			system("killall -9 betad");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.betad");
			fclose(fdbetad);
		}

		FILE* fdradegast = fopen("/var/etc/.radegast","r");
		if (fdradegast)
		{
			system("killall -9 radegast");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.radegast");
			fclose(fdradegast);
		}

		FILE* fdnewcs = fopen("/var/etc/.newcs","r");
		if (fdnewcs)
		{
			system("killall -9 newcs");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdnewcs);
		}
		
		system("touch /var/etc/.cardserver");
		system("insmod -f /var/modules/camd-cardserver.o");
		system("chmod 755 /var/bin/emu/cardserver");
		system("cardserver");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_CARDSERVERSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 cardserver");
		system("rmmod camd-cardserver");
		system("rm /var/etc/.cardserver");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_CARDSERVERSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CRadegastDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//Radegast
	int radegast = *(int *)Data;
	
	if (radegast == 1)
	{
		FILE* fdbetad = fopen("/var/etc/.betad","r");
		if (fdbetad)
		{
			system("killall -9 betad");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.betad");
			fclose(fdbetad);
		}

		FILE* fdcardserver = fopen("/var/etc/.cardserver","r");
		if (fdcardserver)
		{
			system("killall -9 cardserver");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdcardserver);
		}

		FILE* fdnewcs = fopen("/var/etc/.newcs","r");
		if (fdnewcs)
		{
			system("killall -9 newcs");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdnewcs);
		}

		system("touch /var/etc/.radegast");
		system("insmod -f /var/modules/camd-cardserver.o");
		system("chmod 755 /var/bin/emu/radegast");
		system("radegast");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_RADEGASTSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 radegast");
		system("rmmod camd-cardserver");
		system("rm /var/etc/.cardserver");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_RADEGASTSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

bool CNewCSDestChangeNotifier::changeNotify(const neutrino_locale_t, void * Data)
{
	//NewCS
	int newcs = *(int *)Data;
	
	if (newcs == 1)
	{
		FILE* fdbetad = fopen("/var/etc/.betad","r");
		if (fdbetad)
		{
			system("killall -9 betad");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.betad");
			fclose(fdbetad);
		}

		FILE* fdcardserver = fopen("/var/etc/.cardserver","r");
		if (fdcardserver)
		{
			system("killall -9 cardserver");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.cardserver");
			fclose(fdcardserver);
		}

		FILE* fdradegast = fopen("/var/etc/.radegast","r");
		if (fdradegast)
		{
			system("killall -9 radegast");
			system("rmmod camd-cardserver");
			system("rm /var/etc/.radegast");
			fclose(fdradegast);
		}

		system("touch /var/etc/.newcs");
		system("insmod -f /var/modules/camd-cardserver.o");
		system("chmod 755 /var/bin/emu/newcs");
		system("newcs");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_NEWCSSTART), 450, 2); // UTF-8("")
	}
	else
	{
		system("killall -9 newcs");
		system("rmmod camd-cardserver");
		system("rm /var/etc/.newcs");
		ShowHintUTF(LOCALE_MESSAGEBOX_INFO, g_Locale->getText(LOCALE_CARDSERVERMENU_NEWCSSTOP), 450, 2); // UTF-8("")
	}
	return true;
}

