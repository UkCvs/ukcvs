/*
  $Id: systeminfo.h, v2.3 2008/09/20 19:25:21 mohousch Exp $
*/

#ifndef __sysinfo__
#define __sysinfo__

#include <driver/framebuffer.h>
#include <gui/widget/menue.h>

#include <string>
#include <vector>

#define MAXLINES 256


typedef struct sfileline
{
	bool state;
	char* addr;
}sfileline;

typedef struct sreadline
{
	char line[256];
}sreadline;

//class CBESysInfoWidget : public CMenuTarget

class CBESysInfoWidget : public CMenuWidget
{

	private:

		CFrameBuffer *frameBuffer;

		enum
		{
			beDefault,
			beMoving
		} state;

		unsigned int selected;
		unsigned int origPosition;
		unsigned int newPosition;

		unsigned int liststart;
		unsigned int listmaxshow;
		unsigned int numwidth;
		int fheight; // Fonthoehe Bouquetlist-Inhalt
		int theight; // Fonthoehe Bouquetlist-Titel

		int  ButtonHeight;

		bool syslistChanged;
		int width;
		int height;
		int x;
		int y;
		int mode;

		void paintItem(int pos);
		void paint();
		void paintHead();
		void paintFoot();
		void hide();

		int sysinfo();
		int dmesg();
		int cpuinfo();
		int top();
		int readList(struct sfileline *inbuffer);

	public:
		CBESysInfoWidget(int m);
		int exec(CMenuTarget* parent, const std::string & actionKey);
};

#endif

