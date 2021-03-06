/*
  $Id: camdmenu.h, v2.3 2008/09/20 19:25:21 mohousch Exp $
*/

#ifndef __camdmenu__
#define __camdmenu__

#include <string>
#include <vector>

#include <driver/framebuffer.h>
#include <system/setting_helpers.h>

using namespace std;

class CCamd2DestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CCamd3DestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CMgCamdDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CEvoCamdDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CNewCamdDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CCCcamDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CScamDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CCamXDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CGboxDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class COscamDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CBetadDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CCardserverDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CRadegastDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CNewCSDestChangeNotifier : public CChangeObserver
{
	public:
		bool changeNotify(const neutrino_locale_t, void * Data);
};

class CamdAuswahl : public CMenuTarget, CChangeObserver
{
	private:

		CFrameBuffer	*frameBuffer;
		int x;
		int y;
		int width;
		int height;
		int hheight,mheight; // head/menu font height

	public:

		CamdAuswahl();
		bool CamdReset();
		void paint();
		void hide();
		int exec(CMenuTarget* parent, const std::string & actionKey);
		void Settings();
};

#endif //__camdmenu__

