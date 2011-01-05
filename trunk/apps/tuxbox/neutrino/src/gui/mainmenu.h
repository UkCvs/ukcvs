#ifndef __mainmenu__
#define __mainmenu__

#include <string>
#include <vector>
#include <configfile.h>

#include <driver/framebuffer.h>
#include <system/setting_helpers.h>

using namespace std;

class MainMenu : public CMenuTarget, CChangeObserver
{
	private:

		CFrameBuffer	*frameBuffer;
		int x;
		int y;
		int width;
		int height;
		int hheight,mheight; // head/menu font height
	
	public:
		MainMenu();
		void hide();
		int exec(CMenuTarget* parent, const std::string & actionKey);
		void Settings();
};

#endif //__mainmenu__

