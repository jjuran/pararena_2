//============================================================//============================================================//==														==//==					AppleTalkDDP Unit					==//==														==//============================================================//============================================================//========================================================  Includes  #include "Globals.h"#include "UnivUtilities.h"#include "AppleTalkDDP.h"#include "AnimCursor.h"//========================================================  Local Variables  ATDDPRecHandle	inABRecordDDP, outABRecordDDP;EntityName		thisEntity;EntityName		theLookupEntity;MPPParamBlock	theLookUpBlock, theNameBlock;NamesTableEntry	theNameTable;AddrBlock		opponentsAddress;netUserArray	knownNetUserArray;Str255			thisName, opponentsName;Ptr				sendBuffer, receiveBuffer;OSErr			netErr;short			thisSocket, knownNetUsers, netUserNowSelected, lastByteCount;short			firstPackageShort;char			theLookupBuffer[kLookupBufferSize];Boolean			newAppleTalkCalls, nameIsRegistered, imTheMaster, requestNoted;//========================================================  Functions  //========================================================  InitializeAppleTalk  short InitializeAppleTalk (void){	#define			kNCVersion	48	SysEnvRec		thisWorld;	Boolean			callWorked;		thisSocket = 0;	nameIsRegistered = FALSE;	requestNoted = FALSE;		netErr = SysEnvirons(2, &thisWorld);		// get machine environment 	if (netErr != noErr)		return(kNetErrBadHardware);		if (thisWorld.machineType < env512KE)		// abort on less than Mac 512KE 		return(kNetErrBadHardware);												// do we have the new calls? 	newAppleTalkCalls = (thisWorld.atDrvrVersNum >= kNCVersion);		netErr = OpenAppleTalkDriver();	if (netErr != noErr)		return (kNetErrDriverFailed);		sendBuffer = NewPtr(kBufferSize);	if (sendBuffer == kNilPointer)		return (kNetErrBuffersFailed);		receiveBuffer = NewPtr(kBufferSize);	if (receiveBuffer == kNilPointer)		return (kNetErrBuffersFailed);		inABRecordDDP = (ATDDPRecHandle)NewHandle(ddpSize);	if (inABRecordDDP == kNilPointer)		return (kNetErrBuffersFailed);	MoveHHi((Handle)inABRecordDDP);	HLock((Handle)inABRecordDDP);		outABRecordDDP = (ATDDPRecHandle)NewHandle(ddpSize);	if (outABRecordDDP == kNilPointer)		return (kNetErrBuffersFailed);	MoveHHi((Handle)outABRecordDDP);	HLock((Handle)outABRecordDDP);		GetDefaultName();	if (thisName[0] == 0)		GetIndString(thisName, rNetStringID, 2);		netErr = OpenSocketDDP();			// open an ATP socket 	if (netErr != noErr)		return(kNetErrSocketFailed);		return (kNetErrNoErr);}//========================================================  RegisterNameOnNet  OSErr RegisterNameOnNet (void){	#define			kDoVerify	0xFF	MPPParamBlock	theNameBlock;	OSErr			theErr;	Boolean			itWorked;		theErr = noErr;		if (nameIsRegistered)		return (theErr);		PasStringCopy((StringPtr)theNames[whichHumanNumber - 1], (StringPtr)thisName);	NBPSetEntity((Ptr)&thisEntity, (Ptr)thisName, (Ptr)kThisType, (Ptr)kAnyZone);		NBPSetNTE((Ptr)&theNameTable, (Ptr)thisName, (Ptr)kThisType, (Ptr)kAnyZone, 			thisSocket);		theNameBlock.MPPioCompletion = kNilPointer;	theNameBlock.NBPinterval = 3;	theNameBlock.NBPcount = 3;	theNameBlock.NBPentityPtr = (Ptr)&theNameTable;	theNameBlock.NBPverifyFlag = kDoVerify;		theErr = PRegisterName(&theNameBlock, kSynch);	if (theErr == noErr)		nameIsRegistered = TRUE;		return (theErr);}//========================================================  NamesFilter  pascal Boolean NamesFilter (DialogPtr theDialog, EventRecord *theEvent, short *itemHit){	switch (theEvent->what)	{		case keyDown:			switch ((theEvent->message) & charCodeMask)			{				case kEnterKeyASCII:				case kReturnKeyASCII:					if (netUserNowSelected != 0)					{						FlashDialogButton(theDialog, kButtNetOkay);						*itemHit = kButtNetOkay;						return(TRUE);					}					break;				default:					return(FALSE);					break;			}			break;		default:			while (IsThereAnotherName(knownNetUsers))			{				knownNetUsers++;				ProcessAName(knownNetUserArray, knownNetUsers);				if (netUserNowSelected == 0)					netUserNowSelected = 1;				UpdateNetOpponents(theDialog, knownNetUserArray, knownNetUsers, 					netUserNowSelected);			}						if (IsLookUpFinished())			{				netErr = LookUpNamesAsynch();			}			return(FALSE);			break;	}}//========================================================  SelectNetOpponentAsynch  Boolean SelectNetOpponentAsynch (void){	#define			kBalloon1PictID		2000	#define			kBalloon4PictID		2001	#define			kNetTitleLeft		6	#define			kNetTitleTop		16	#define			kLookingIndex		8		DialogPtr		netSelectAsynchDialog;	PicHandle		thePict;	Handle			itemHandle;	Rect			iRect;	Str255			newTitle, lookingString;	short			itemHit, iType;	Boolean			leaveDialog, userOkayed;		knownNetUsers = 0;	netUserNowSelected = 0;		CenterDialog(rNetSelectDialogID);	netSelectAsynchDialog = GetNewDialog(rNetSelectDialogID, kNilPointer, kPutInFront);		SetPort((GrafPtr)netSelectAsynchDialog);	ShowWindow((GrafPtr)netSelectAsynchDialog);		GetDItem(netSelectAsynchDialog, kUserNetFrame, &iType, &itemHandle, &iRect);	ForeColor(cyanColor);	FrameRect(&iRect);	ForeColor(blackColor);		GetDItem(netSelectAsynchDialog, kNetPowBalloonItem, &iType, &itemHandle, &iRect);	if (isColor)		thePict = GetPicture(kBalloon4PictID);	else		thePict = GetPicture(kBalloon1PictID);	if (thePict != kNilPointer)	{		DrawPicture(thePict, &iRect);		ReleaseResource((Handle)thePict);	}		GetIndString(newTitle, rNetStringID, 3);	if (newTitle[0] == 0)		PasStringCopy((StringPtr)"\pChoose Opponent", (StringPtr)newTitle);	MoveTo(kNetTitleLeft, kNetTitleTop);	DrawString(newTitle);		GetIndString(lookingString, rMiscStrings, kLookingIndex);		UpdateNetOpponents(netSelectAsynchDialog, knownNetUserArray, knownNetUsers, 			netUserNowSelected);	GetDItem(netSelectAsynchDialog, kTextNetWho, &iType, &itemHandle, &iRect);	ForeColor(blueColor);	SetIText(itemHandle, lookingString);	ForeColor(blackColor);	netErr = LookUpNamesAsynch();		leaveDialog = FALSE;		while (leaveDialog == FALSE)	{		ModalDialog(NamesFilter, &itemHit);				switch (itemHit)		{			case kButtNetOkay:				PasStringCopy((StringPtr)knownNetUserArray[netUserNowSelected - 1].name, 						(StringPtr)opponentsName);				opponentsAddress = knownNetUserArray[netUserNowSelected - 1].address;				leaveDialog = TRUE;				userOkayed = TRUE;				break;			case kButtNetCancel:				userOkayed = FALSE;				leaveDialog = TRUE;				break;			case kButtNetNext:				netUserNowSelected++;				if (netUserNowSelected > knownNetUsers)					netUserNowSelected = 1; 				GetDItem(netSelectAsynchDialog, kTextNetWho, &iType, &itemHandle, &iRect);				ForeColor(blueColor);				SetIText(itemHandle, knownNetUserArray[netUserNowSelected - 1].name);				ForeColor(blackColor);				break;			case kButtNetPrev:				netUserNowSelected--;				if (netUserNowSelected < 1)					netUserNowSelected = knownNetUsers; 				GetDItem(netSelectAsynchDialog, kTextNetWho, &iType, &itemHandle, &iRect);				ForeColor(blueColor);				SetIText(itemHandle, knownNetUserArray[netUserNowSelected - 1].name);				ForeColor(blackColor);				break;			default:				break;		}	}		DisposDialog(netSelectAsynchDialog);	return (userOkayed);}//========================================================  RequestReceiveDDP  OSErr RequestReceiveDDP (void){	#define			kChecksumReq	TRUE	OSErr			readErr;		if (requestNoted)		return;		requestNoted = TRUE;		(*inABRecordDDP)->abOpcode = tDDPRead;	(*inABRecordDDP)->abResult = 0;	(*inABRecordDDP)->abUserReference = 0L;	(*inABRecordDDP)->ddpType = 5;	(*inABRecordDDP)->ddpSocket = thisSocket;	(*inABRecordDDP)->ddpReqCount = kBufferSize;	(*inABRecordDDP)->ddpDataPtr = receiveBuffer;		readErr = DDPRead(inABRecordDDP, kChecksumReq, kAsynch);		return (readErr);}//========================================================  PollIncomingDDP  OSErr PollIncomingDDP (long theData, short maxBytes, short maxTickWait){	long			tickAbort;	OSErr			requestErr;	short			bytesGot;		tickAbort = Ticks + (long)maxTickWait;		while (((*inABRecordDDP)->abResult > 0) && (Ticks < tickAbort));		netErr = (*inABRecordDDP)->abResult;	bytesGot = (*inABRecordDDP)->ddpActCount;		if (netErr == noErr)		{		lastByteCount = bytesGot;		requestNoted = FALSE;		if ((bytesGot > 0) && 				((*inABRecordDDP)->ddpAddress.aNode == opponentsAddress.aNode))		{			if (bytesGot > maxBytes)			{				bytesGot = maxBytes;			}			BlockMove(receiveBuffer, (Ptr)theData, (long)bytesGot);		}		requestErr = RequestReceiveDDP();	}		return (netErr);}//========================================================  PollSpecificDDP  OSErr PollSpecificDDP (long theData, short exactBytes, short maxTickWait){	long			tickAbort;	OSErr			requestErr;	short			bytesGot;		tickAbort = Ticks + (long)maxTickWait;		while (((*inABRecordDDP)->abResult > 0) && (Ticks < tickAbort));		netErr = (*inABRecordDDP)->abResult;	bytesGot = (*inABRecordDDP)->ddpActCount;		if (netErr == noErr)		{		lastByteCount = bytesGot;		requestNoted = FALSE;		if ((bytesGot > 0) && 				((*inABRecordDDP)->ddpAddress.aNode == opponentsAddress.aNode))		{			if (bytesGot != exactBytes)			{				BlockMove(receiveBuffer, (Ptr)&firstPackageShort, 2L);				netErr = kNetErrWrongSize;			}			else			{				BlockMove(receiveBuffer, (Ptr)theData, (long)bytesGot);			}		}		requestErr = RequestReceiveDDP();	}		return (netErr);}//========================================================  SendOutgoingDDP  OSErr SendOutgoingDDP (long theData, short dataSize){	#define			kChecksumSend	FALSE		BlockMove((Ptr)theData, sendBuffer, (long)dataSize);		(*outABRecordDDP)->abOpcode = tDDPWrite;	(*outABRecordDDP)->abUserReference = 0L;	(*outABRecordDDP)->ddpType = 5;	(*outABRecordDDP)->ddpSocket = thisSocket;	(*outABRecordDDP)->ddpAddress = opponentsAddress;	(*outABRecordDDP)->ddpReqCount = dataSize;	(*outABRecordDDP)->ddpDataPtr = sendBuffer;		netErr = DDPWrite(outABRecordDDP, kChecksumSend, kSynch);		return (netErr);}//========================================================  CloseDownAppleTalk  short CloseDownAppleTalk (void){	Boolean			callWorked;		netErr = DeRegisterName();	if (netErr != noErr)		return (netErr);		netErr = CloseSocketDDP();	if (netErr != noErr)		return (netErr);		if (outABRecordDDP != kNilPointer)	{		HUnlock((Handle)outABRecordDDP);		DisposHandle((Handle)outABRecordDDP);	}			if (inABRecordDDP != kNilPointer)	{		HUnlock((Handle)inABRecordDDP);		DisposHandle((Handle)inABRecordDDP);	}		if (sendBuffer != kNilPointer)		DisposPtr(sendBuffer);		if (receiveBuffer != kNilPointer)		DisposPtr(receiveBuffer);		return (kNetErrNoErr);}//========================================================  OpenAppleTalkDriver  OSErr OpenAppleTalkDriver (void){	short		refNum;		netErr = OpenDriver("\p.MPP", &refNum);		return (netErr);}//========================================================  GetDefaultName  void GetDefaultName (void){	#define		kChooserStringID	-16096	Handle		theNameHandle;		theNameHandle = (Handle)GetString(kChooserStringID);	if (theNameHandle != kNilPointer)		PasStringCopy((StringPtr)*theNameHandle, (StringPtr)thisName);	else		thisName[0] = 0;		if (thisName[0] == 0)		GetIndString(thisName, rNetStringID, 2);		if (thisName[0] == 0)		PasStringCopy((StringPtr)"\pNo Name", (StringPtr)thisName);}//========================================================  OpenSocketDDP  OSErr OpenSocketDDP (void){	netErr = DDPOpenSocket(&thisSocket, kNilPointer);		return (netErr);}//========================================================  LookUpNames  OSErr LookUpNamesAsynch (void){	SpinCursor(12);		NBPSetEntity((Ptr)&theLookupEntity, (Ptr)kLookUpSelector, (Ptr)kThisType, 			(Ptr)kAnyZone);		theLookUpBlock.NBPinterval = 6;	theLookUpBlock.NBPcount = 5;	theLookUpBlock.NBPentityPtr = (Ptr)&theLookupEntity;	theLookUpBlock.NBPretBuffPtr = (Ptr)&theLookupBuffer;	theLookUpBlock.NBPretBuffSize = kLookupBufferSize;	theLookUpBlock.NBPmaxToGet = kMaxLookUpNumber;		netErr = PLookupName(&theLookUpBlock, kAsynch);		InitCursor();	return (netErr);}//========================================================  IsThereAnotherName  Boolean IsThereAnotherName (short youThink){	short			howMany;		howMany = theLookUpBlock.NBPnumGotten;	if (howMany > youThink)		return(TRUE);	else		return(FALSE);}//========================================================  IsLookUpFinished  Boolean IsLookUpFinished (void){	short			howMany, transmitsLeft;		howMany = theLookUpBlock.NBPnumGotten;	transmitsLeft = theLookUpBlock.NBPcount;	if ((howMany == kMaxLookUpNumber) || (transmitsLeft == 0))		return(TRUE);	else		return(FALSE);}//========================================================  ProcessAName  OSErr ProcessAName (netUserArray netOpponents, short nameNumber){	EntityName		lookupEntity;	AddrBlock		theAddrBlock;	short			howMany;		netErr = noErr;	howMany = theLookUpBlock.NBPnumGotten;		if ((howMany == 0) || (nameNumber == 0))	{		return (netErr);	}		if (howMany > kMaxLookUpNumber)		howMany = kMaxLookUpNumber;		if (nameNumber > kMaxLookUpNumber)		nameNumber = kMaxLookUpNumber;		if (howMany < nameNumber)	{		return (netErr);	}		netErr = NBPExtract((Ptr)&theLookupBuffer, howMany, nameNumber, &lookupEntity, 			&theAddrBlock);	if (netErr == noErr)	{		netOpponents[nameNumber - 1].address = theAddrBlock;		PasStringCopy((StringPtr)lookupEntity.objStr, 				(StringPtr)netOpponents[nameNumber - 1].name);	}		return (netErr);}//========================================================  UpdateNetOpponents  void UpdateNetOpponents (DialogPtr theDialog, netUserArray netOpponents, 		short number, short who){	#define			kNoOpponentIndex	9	Handle			itemHandle;	Rect			iRect;	Str255			numberString, noOpponentString, newTitle;	short			iType;		DrawDialog((GrafPtr)theDialog);	if (number == 0)	{		GetDItem(theDialog, kButtNetOkay, &iType, &itemHandle, &iRect);		HiliteControl((ControlHandle)itemHandle, 255);	}	else	{		GetDItem(theDialog, kButtNetOkay, &iType, &itemHandle, &iRect);		HiliteControl((ControlHandle)itemHandle, 0);		DrawDefaultButton(theDialog, kButtNetOkay);	}		NumToString((long)number, numberString);	GetDItem(theDialog, kTextNetNumber, &iType, &itemHandle, &iRect);	SetIText(itemHandle, numberString);		GetIndString(noOpponentString, rMiscStrings, kNoOpponentIndex);		if (number == 0)	{		GetIndString(newTitle, rNetStringID, 1);		GetDItem(theDialog, kTextNetWho, &iType, &itemHandle, &iRect);		if (newTitle[0] == 0)			PasStringCopy((StringPtr)noOpponentString, (StringPtr)newTitle);		ForeColor(blueColor);		SetIText(itemHandle, newTitle);		ForeColor(blackColor);		GetDItem(theDialog, kButtNetNext, &iType, &itemHandle, &iRect);		HiliteControl((ControlHandle)itemHandle, 255);		GetDItem(theDialog, kButtNetPrev, &iType, &itemHandle, &iRect);		HiliteControl((ControlHandle)itemHandle, 255);	}	else	{		GetDItem(theDialog, kTextNetWho, &iType, &itemHandle, &iRect);		ForeColor(blueColor);		SetIText(itemHandle, netOpponents[who - 1].name);		ForeColor(blackColor);		if (number <= 1)		{			GetDItem(theDialog, kButtNetNext, &iType, &itemHandle, &iRect);			HiliteControl((ControlHandle)itemHandle, 255);			GetDItem(theDialog, kButtNetPrev, &iType, &itemHandle, &iRect);			HiliteControl((ControlHandle)itemHandle, 255);		}		else		{			GetDItem(theDialog, kButtNetNext, &iType, &itemHandle, &iRect);			HiliteControl((ControlHandle)itemHandle, 0);			GetDItem(theDialog, kButtNetPrev, &iType, &itemHandle, &iRect);			HiliteControl((ControlHandle)itemHandle, 0);		}	}}//========================================================  DetermineMasterSlave  Boolean DetermineMasterSlave (void){	short			i;	Boolean			mediated;		mediated = FALSE;		if (speedFlag > theirSpeed)	{		imTheMaster = TRUE;		mediated = TRUE;	}	else if (speedFlag < theirSpeed)	{		imTheMaster = FALSE;		mediated = TRUE;	}	else if (thisName[0] > opponentsName[0])	{		imTheMaster = TRUE;		mediated = TRUE;	}	else if (thisName[0] < opponentsName[0])	{		imTheMaster = FALSE;		mediated = TRUE;	}	else	{		for (i = 1; i < thisName[0]; i++)		{			if (thisName[i] > opponentsName[i])			{				imTheMaster = TRUE;				mediated = TRUE;				break;			}			else if (thisName[i] < opponentsName[i])			{				imTheMaster = FALSE;				mediated = TRUE;				break;			}		}		if (!mediated)		{			if (thisSocket > (short)opponentsAddress.aSocket)			{				imTheMaster = TRUE;				mediated = TRUE;			}			else if (thisSocket < (short)opponentsAddress.aSocket)			{				imTheMaster = FALSE;				mediated = TRUE;			}		}	}		return (mediated);}//========================================================  DeRegisterName  OSErr DeRegisterName (void){	netErr = noErr;		if (nameIsRegistered)	{		theNameBlock.NBPentityPtr = (Ptr) &thisEntity;		netErr = PRemoveName(&theNameBlock, kSynch);		if (netErr == noErr)			nameIsRegistered = FALSE;	}		return (netErr);}//========================================================  CloseSocketDDP  OSErr CloseSocketDDP (void){	netErr = noErr;		if (thisSocket != 0)		netErr = DDPCloseSocket((Byte)thisSocket);		thisSocket = 0;		return (netErr);}