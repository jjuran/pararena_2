/*============================================================*//*============================================================*//*==														==*//*==			Rendering Directing Routines				==*//*==														==*//*============================================================*//*============================================================*//*========================================================  Includes  */#include "Globals.h"#include "UnivUtilities.h"#include "Render.h"#include "RenderAsm1.h"#include "RenderAsm4.h"#include "RenderQD.h"/*========================================================  Functions  *//*========================================================  FillInTheSrcAndMaskRect  */void FillInTheSrcAndMaskRect (playerType *who){	if (who->selector == kPlayerSelector)	{		if (leftGoalIsPlayers)			who->srcRect = playerSrcRects[who->dirFlagSrc][who->postFlagSrc];		else			who->srcRect = opponentSrcRects[who->dirFlagSrc][who->postFlagSrc];	}	else	{		if (leftGoalIsPlayers)			who->srcRect = opponentSrcRects[who->dirFlagSrc][who->postFlagSrc];		else			who->srcRect = playerSrcRects[who->dirFlagSrc][who->postFlagSrc];	}		if (who->arrayFlagMask == kMask)	{		who->maskRect = playerSrcRects[who->dirFlagMask][who->postFlagMask];	}	else	{		who->maskRect = fadeMaskRects[who->dirFlagMask][who->postFlagMask];	}}/*========================================================  FillInTheSrcAndMaskAddr  */void FillInTheSrcAndMaskAddr (playerType *who){	if (who->selector == kPlayerSelector)	{		if (leftGoalIsPlayers)			who->srcAddr = playerSrcAddrs[who->dirFlagSrc][who->postFlagSrc];		else			who->srcAddr = opponentSrcAddrs[who->dirFlagSrc][who->postFlagSrc];	}	else	{		if (leftGoalIsPlayers)			who->srcAddr = opponentSrcAddrs[who->dirFlagSrc][who->postFlagSrc];		else			who->srcAddr = playerSrcAddrs[who->dirFlagSrc][who->postFlagSrc];	}		if (who->arrayFlagMask == kMask)		who->maskAddr = maskAddrs[who->dirFlagMask][who->postFlagMask];	else		who->maskAddr = fadeMaskAddrs[who->dirFlagMask][who->postFlagMask];}/*========================================================  DetermineFrameRate  */void DetermineFrameRate (void){	if (doSkipFrames)	{		if ((Ticks % kFramesToSkip) == 0)			drawThisFrame = TRUE;		else			drawThisFrame = FALSE;	}	else		drawThisFrame = TRUE;}/*========================================================  HandlePreGraphics  */void HandlePreGraphics (void){	if (netGameInSession)	{		if (imTheMaster)		{			playerInBack = (thePlayer.zPos > theOpponent.zPos);		}		else if (!madeThePickUp)		{			return;		}	}	else	{		playerInBack = (thePlayer.zPos > theOpponent.zPos);	}		if (useQD)	{		FillInTheSrcAndMaskRect(&thePlayer);		FillInTheSrcAndMaskRect(&theOpponent);	}	else	{		FillInTheSrcAndMaskAddr(&thePlayer);		FillInTheSrcAndMaskAddr(&theOpponent);	}}/*========================================================  HandlePostGraphics  */void HandlePostGraphics (void){	if (netGameInSession)	{		if ((!imTheMaster) && (!madeThePickUp))			return;	}		if (drawThisFrame)	{		theBall.eraser = theBall.isRect;		thePlayer.wasRect = thePlayer.isRect;		theOpponent.wasRect = theOpponent.isRect;		theBall.wasRect = theBall.isRect;		boardCursor.wasRect = boardCursor.isRect;	}}/*========================================================  RenderScene  */void RenderScene (void){	if (netGameInSession)	{		if ((!imTheMaster) && (!madeThePickUp))			return;	}		if (useQD)	{		if (isColor)			RenderSceneQDC();		else			RenderSceneQD1();	}	else	{		switch (isDepth)		{			case kDisplay1Bit:				RenderSceneAsm1();				break;			case kDisplay4Bit:				RenderSceneAsm4();				break;			case kDisplay8Bit:				break;		}	}}/*========================================================  DisplayScoreDigit  */void DisplayScoreDigit (short place, short digit, short who){	if (isColor)		DisplayScoreDigitQDC(place, digit, who);	else		DisplayScoreDigitQD1(place, digit, who);}/*========================================================  DisplayPlayerFouls  */void DisplayPlayerFouls (short fouls){	if (isColor)		DisplayPlayerFoulsQDC(fouls);	else		DisplayPlayerFoulsQD1(fouls);}/*========================================================  DisplayOpponentFouls  */void DisplayOpponentFouls (short fouls){	if (isColor)		DisplayOpponentFoulsQDC(fouls);	else		DisplayOpponentFoulsQD1(fouls);}/*========================================================  TwinkleAStar  */void TwinkleAStar (void){	short		starNumber, colorNumber;		starNumber = RandomInt(kNumberOfStars);	colorNumber = RandomInt(kNumberOfStarColors);		if (splashIsUp)	{		starNumber = 0;		colorNumber = 0;	}		if (isColor)		TwinkleAStarQDC(starNumber, colorNumber);	else		TwinkleAStarQD1(starNumber);}/*========================================================  DrawAllStars  */void DrawAllStars (void){	short		i, colorNumber;		for (i = 0; i < kNumberOfStars; i++)	{		colorNumber = RandomInt(kNumberOfStarColors);				if (isColor)			DrawOffscreenAStarQDC(i, colorNumber);		else			DrawOffscreenAStarQD1(i);	}}/*========================================================  UpdateArrows  */void UpdateArrows (void){	if (isColor)		UpdateArrowsQDC();	else		UpdateArrowsQD1();}/*========================================================  UpdateBallTimers  */void UpdateBallTimers (playerType *who){	Rect			tempRect;		if (who->selector == kPlayerSelector)	{		if (leftGoalIsPlayers)			tempRect = leftTimingDest;		else			tempRect = rightTimingDest;	}	else	{		if (leftGoalIsPlayers)			tempRect = rightTimingDest;		else			tempRect = leftTimingDest;	}		tempRect.bottom = tempRect.top + ((kLoopLimitOnHeldBall - who->loopsBallHeld) / 			kTimingScale);		if (isColor)		UpdateBallTimersQDC(&tempRect);	else		UpdateBallTimersQD1(&tempRect);}