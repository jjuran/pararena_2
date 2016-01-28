/*============================================================*//*============================================================*//*==														==*//*==					NetOpponent Header					==*//*==														==*//*============================================================*//*============================================================*//*========================================================  Defines  */#define kNetworkVersion		1#define	kHelloIndicator		4321#define kRequestSize		6#define kEnviroMessageSize	16#define kRequestEnvironment	3735#define kGotYourEnvironment	842/*========================================================  Type Definintions  */typedef struct{	long		speed;				/* 4 bytes */	short		version;			/* 2 bytes */	short		arena;				/* 2 bytes */	short		goal;				/* 2 bytes */	short		spotPts;			/* 2 bytes */	short		league;				/* 2 bytes */	Boolean		scoredLast;			/* 1 byte */	Boolean		limited;			/* 1 byte */} environMessage;					/* total size = 16 bytes */typedef struct{	short		requested;			/* 2 bytes */	short		firstArgument;		/* 2 bytes */	short		secondArgument;		/* 2 bytes */} requestMessage;					/* total size = 6 bytes *//*========================================================  Prototypes  */OSErr SendSynchPulse (void);OSErr SendThisEnvironment (void);OSErr BegForSomething (short);Boolean WaitForSynch (short);Boolean ConfirmEnvironmentMatch (void);Boolean WhatsTheGamesOutcome (void);void PrepareStandardMessage (void);void SendMessage (void);Boolean SendModalMessage (short);void ConfirmModalMessage (void);void ReceiveMessage (void);void TranslateStandardMessage (void);void InitializeMessages (void);Boolean SpotPoints (void);/*========================================================  External Variables */extern playerType		thePlayer;extern playerType		theOpponent;extern ballType			theBall;extern doorType			theDoor;extern masterSendType	masterMessage;extern slaveSendType	slaveMessage;extern long				speedFlag, theirSpeed;extern short			frameCounter, newFrameCount, lastFrameCount;extern short			playerTotalFouls, opponentTotalFouls, playerTotalGoals;extern short			playerTotalCrits, opponentTotalCrits, opponentTotalGoals;extern short			playerFouls, opponentFouls, screenWide, wasLastSound;extern short			playerScore, opponentScore, whosGotBall, primaryMode;extern short			masterSendSize, slaveSendSize, loopDelay, isLeague, theirLeague;extern short			droppedPackets, sentPackets, lengthOfApplause, lengthOfMob;extern short			arenaSize, whichGame, spotPoints, spottedPoints, lastByteCount;extern short			firstPackageShort;extern short			antiFacing[9];extern short			antiWhosBall[4];extern char				playerWonTheGame;extern Boolean			imTheMaster, netMirroring, leftScoredLast, playerInBack;extern Boolean			useQD, leftGoalIsPlayers, madeThePickUp, theyAreMajor;extern Boolean			gameIsOver, doZooms, netOnly;