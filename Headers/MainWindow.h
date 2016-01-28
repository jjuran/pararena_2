/*============================================================*//*============================================================*//*==														==*//*==					Main Window Header					==*//*==														==*//*============================================================*//*============================================================*//*========================================================  Defines  */#define 	rMainWndoID				128		/* 'wind' Resource ID Constant */#define		rCLUTID					128		/* 'clut' Resource ID Constant */#define 	rSplashPICT1			5000	/* 'PICT' ID for B&W splash screen */#define		rSplashPICT4			5001	/* 'PICT' ID for 16 color splash screen *//*========================================================  Routines  */void EraseMenuBarRect (void);void FixVisRegion (void);void LoadBackgroundPict (void);void UpdateGoalPicts (Boolean);void RefreshMainWindow (void);void OpenMainWindow (void);void DoSplashScreen (void);void CloseMainWindow (void);/*========================================================  External Variables  */extern playerType		thePlayer;extern WindowPtr		mainWndo;extern RGBColor			RGBBlackColor, RGBWhiteColor;extern BitMap			offBackBits, offWorkBits;extern GrafPtr			offBackPtr;extern CGrafPtr			offCBackPtr, offCWorkPtr;extern Rect				dragRect, offWorkRect;extern short			screenHigh, screenWide, arenaSize, displayMode, wasMenuBarHeight;extern short			horizontal, vertical, primaryMode;extern short			leftGoalLeague, rightGoalLeague;extern Boolean			isColor, leftGoalIsPlayers, useQD, splashIsUp;