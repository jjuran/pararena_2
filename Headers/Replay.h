/*============================================================*//*============================================================*//*==														==*//*==					Replay Header						==*//*==														==*//*============================================================*//*============================================================*//*========================================================  Constants Defined  */#define kCameraInertia		5/*========================================================  Prototypes  */void AmassReplayData (void);void DoInstantReplay (void);void RenderInstantReplayQD1 (void);void RenderInstantReplayQDC (void);void RenderInstantReplay (void);/*========================================================  External Variables */extern playerType		thePlayer;extern playerType		theOpponent;extern ballType			theBall;extern doorType			theDoor;extern digiDispType		scoreDisplays[2];extern replayType		*replayData;extern WindowPtr		mainWndo;extern GrafPtr			offBackPtr, offWorkPtr;extern RGBColor			RGBBlackColor, RGBWhiteColor;extern RgnHandle		screenRgn;extern GrafPtr			offMaskPort;extern BitMap			offPartsBits, offWorkBits, offBackBits, offMaskMap;extern CGrafPort		offCPartsPort, offCWorkPort;extern CGrafPtr			offCPartsPtr, offCWorkPtr, offCBackPtr;extern Ptr				offBackPix;extern Rect				offPartsRect, offWorkRect, offMaskRect;extern Rect				cameraRect, replayRect, colonDest, screenRect;extern Rect				playerSrcRects[9][3], fadeMaskRects[9][3], opponentSrcRects[9][3];extern Rect				colonSrc[2];extern long				timeElapsed, baseTime, wasTime, replaySrc, replayDest;extern short			frameCounter, newFrameCount, lastFrameCount;extern short			primaryMode, displayMode, screenWide, screenHigh, arenaSize;extern short			displayHCenter, displayVCenter, workRowBytes, screenRowBytes;extern short			playerFouls, opponentFouls;extern short			playerScore, opponentScore;extern short			leftPlayerNumber, rightPlayerNumber, loopDelay;extern short			masterSendSize, slaveSendSize, loopDelay;extern short			droppedPackets, sentPackets, whosGotBall;extern Boolean			pausing, quitting, useQD, isColor, leftScoredLast, replayZooms;extern Boolean			autoTeamsDialog, leftGoalIsPlayers, madeThePickUp;extern Boolean			netGameInSession, imTheMaster, canReplay, playerJustScored;