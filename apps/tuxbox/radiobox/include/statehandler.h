#ifndef __STATEHANDLER_H__
#define __STATEHANDLER_H__

#include <radiobox.h>
#include <mounter.h>



/*********************************************************/

class CStateHandler
{
protected:

	CStateHandler*		subhandler;
	bool				remove;

public:

						CStateHandler() : subhandler( 0 ), remove( false ) 
						{
							
						};

	virtual void		Show() = 0;
	virtual void		HandleKeys( CRadioBox::KEYS _key, bool _pressed ) = 0;

	CStateHandler*		GetSubHandler() { CStateHandler* tmp = subhandler; subhandler = 0; return tmp; }
	bool				HasToBeRemoved() { return remove; }

};


/*********************************************************/

class CMenu
{
protected:

	unsigned int menu_selected;
	int menu_top; //first element shown in menu

	void ResetMenu(); 

	CMenu() { ResetMenu(); }
};


/*********************************************************/

class CSelectPlayList : public CStateHandler, CMenu
{
private:

	strarray playlists;
	
	void LoadPlaylists( std::string _dirname );	

public:
	CSelectPlayList();
	virtual ~CSelectPlayList() { };
	void	Show();
	void	HandleKeys( CRadioBox::KEYS _key, bool _pressed );

};

/*********************************************************/

class CSelectLocation : public CStateHandler, CMenu
{
private:

	strarray locations;
	
	void LoadLocations();
	static CPlayList* GetPlayList( std::string _location );

public:
	CSelectLocation();
	virtual ~CSelectLocation() {};
	void	Show();
	void	HandleKeys( CRadioBox::KEYS _key, bool _pressed );

};

/*********************************************************/

class CPlayListEntries : public CStateHandler, CMenu
{
public:
	CPlayListEntries( CPlayList* _playlist );

	void				Show();
	void				HandleKeys( CRadioBox::KEYS _key, bool _pressed );


private:

	CPlayList* playlist;
	
};

/*********************************************************/

class CPlayLocation : public CStateHandler
{
public:
	CPlayLocation( CPlayList* _playlist, size_t _idx );

	void				Show();
	void				HandleKeys( CRadioBox::KEYS _key, bool _pressed );


private:
	unsigned long frame;
	CPlayList*	playlist;
	size_t 		idx;
	
};

/*********************************************************/

class CPlayPLRandom : public CStateHandler
{
public:
	CPlayPLRandom( CPlayList* _playlist );
	virtual ~CPlayPLRandom( ) { delete playlist; };

	void				Show();
	void				HandleKeys( CRadioBox::KEYS _key, bool _pressed );


private:
	unsigned long frame;
	CPlayList*	playlist;
};

/*********************************************************/

class CStaticMenu : public CStateHandler, CMenu
{
protected:

	strarray entries;

	bool			CanBeRemoved;
	std::string 	title;

	int				sel;

	virtual void DoAction( std::string _action ) = 0;

public:
	CStaticMenu() { CanBeRemoved = true; };
	virtual ~CStaticMenu() { };

	void	Show();
	void	HandleKeys( CRadioBox::KEYS _key, bool _pressed );
};

/*********************************************************/

class CSetupMenu : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	CSetupMenu();
	virtual ~CSetupMenu() { };
};

/*********************************************************/

class CMainMenu : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	CMainMenu();
	virtual ~CMainMenu() { };
};

/*********************************************************/

class CSetupPlayOrder : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	CSetupPlayOrder();
	virtual ~CSetupPlayOrder() { };
};

/*********************************************************/

class CSetupSoftware : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	CSetupSoftware();
	virtual ~CSetupSoftware() { };
};



/*********************************************************/

class CSetupReadFlash : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	CSetupReadFlash( );
	virtual ~CSetupReadFlash() { };
};


/*********************************************************/

class CSetupWriteFlash : public CStaticMenu
{
private:
	void DoAction( std::string _action );
public:
	bool	flag;
	std::string	file;
	CSetupWriteFlash( );
	virtual ~CSetupWriteFlash() { };
};


/*********************************************************/

class CPlayListOptions : public CStaticMenu
{
private:
	std::string	location;
	void DoAction( std::string _action );
public:
	CPlayListOptions( std::string _location );
	virtual ~CPlayListOptions() { };
};

/*********************************************************/

class CMountSetup : public CStaticMenu, CMounter
{
private:
	void DoAction( std::string _action );
public:
	CMountSetup( );
	virtual ~CMountSetup() { };
};

/*********************************************************/

class CMountEdit : public CStateHandler
{
private:
	int		idx;
	int 	cursor;

	enum	entry
	{
		me_ip,
		me_dir,
		me_ldir,
		me_username,
		me_password,
		me_opts1,
		me_opts2
	} current;

	void	MoveCursor( int _d );
	void	ChangeLetter( int _d );
	void	Next( int _d );
	
public:
	CMountEdit( int _idx ) : idx( _idx ), cursor(1), current(me_ip) {};
	
	void		Show();
	void		HandleKeys( CRadioBox::KEYS _key, bool _pressed );

};


/*********************************************************/

class CMessageBox : public CStateHandler
{
protected:
	time_t endtime;
	unsigned long frame;
	std::string msg;
	int type;
public:
	CMessageBox( std::string _msg, int _type = 0, int _timeout = 18 );
	virtual ~CMessageBox() { };

	void	Show();
	void	HandleKeys( CRadioBox::KEYS _key, bool _pressed );
};

/*********************************************************/

#endif //__STATEHANDLER_H__
