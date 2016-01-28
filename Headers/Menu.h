/*============================================================*//*============================================================*//*==														==*//*==					Menu Header							==*//*==														==*//*============================================================*//*============================================================*//*========================================================  Defines  */											/* Menu Constants */#define 	iNoItem					0#define 	rAppleMenuID			128		/* Apple */#define 	iAboutItem				1#define 	rFileMenuID				129		/* File */#define		iNewGame				1#define		iResumeGame				2#define		iEndGame				3#define		iQuit					5#define 	rGameMenuID				130		/* Game */#define		iTeamsSetUp				1#define		iChooseLeague			2#define		iPracticeBoardin		4#define		iPracticeScoring		5#define		iStandardGame			6#define		iTournament				7#define		rOptionsMenuID			131		/* Options */#define 	iInstantReplay			1#define 	iSound					2#define 	iNames					3#define 	iPreferences			4#define 	iPlayersStats			6#define		iRecords				7#define		iMemory					9#define		iConvert				10#define		iHelp					11/*========================================================  Routines  */void CheckGameMenu (void);void SetMBarToPlaying (void);void SetMBarToIdle (void);void ValidateMenuBar (void);void DoAppleMenu (short);void DoFileMenu (short);void DoGameMenu (short);void DoOptionsMenu (short);void DoMenuChoice (long);void MenuBarInit (void);/*========================================================  Globals  */extern macEnvironment	thisMac;extern short			primaryMode, whichGame, leftPlayerNumber, rightPlayerNumber;extern short			system7HelpItem;extern Boolean			quitting, menuBarIsHidden, pausing, netOnly;