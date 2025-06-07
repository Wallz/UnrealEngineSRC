// ====================================================================
//  Class:  UnrealGame.CinematicHud
//  Parent: Engine.HUD
//
//  This is the hud used for Cinematic sequences
// ====================================================================

class CinematicHud extends HUD;

// #exec OBJ LOAD FILE=..\textures\ScopeOverlay.utx PACKAGE=ScopeOverlay

var float Delta;
var bool  bHideScope;
var float xOffsets[2];
var float xRates[2];
var float yOffsets[2];
var float yRates[2];
var bool  bInitialized;
var float Scale;

simulated function DrawHUD(canvas Canvas)
{
	// Setup Timing

	
	if (!bInitialized)
	{
		Initialize(Canvas);
	}
	

	Scale = Canvas.ClipX / 1024;
	
	Super.DrawHud(Canvas);

	
	if (!bHideScope)
		DrawScope(Canvas);


	
	// Draw any specific sequences here
}

simulated function Initialize(canvas Canvas)
{

	if (Scale == 0)
		return;
	
//	xOffsets[0] = -390.0*Scale;
//	xRates[0]   = 384.0*Scale; 	
//	xOffsets[1] = Canvas.ClipX+1;
//	xRates[1]   = 384.0*Scale;

	xOffsets[0] = -123.0*Scale;
	xRates[0]   = 512.0*Scale; 	
	xOffsets[1] = Canvas.ClipY+1;
	xRates[1]   = 512.0*Scale;
	
	
	yOffsets[0] = (Canvas.ClipY / 2) - (64.0*Scale);
	yOffsets[1] = yOffsets[0];
	yRates[0]   = -200.0*Scale;
	yRates[1]   = +256.0*Scale;
	
	bInitialized = true;

}

simulated function DrawScope(canvas Canvas) // merge_hack this whole func
{
/* 
	local float cx,cy;
	local int i;
	
	cx = Canvas.ClipX / 2;
	cy = Canvas.ClipY / 2; 

	
	// Draw the Background
	
	Canvas.SetPos(0,0);
//	Canvas.SetDrawColor(128,128,128);
	Canvas.SetDrawColor(255,255,255);
	Canvas.Style = 5;
	Canvas.DrawTile(shader'ScopeOverlay.base.SSO_Main',1024*Scale,768*Scale,0,0,1024,768);
	
	// Draw Outer/Inner rings
	
	Canvas.SetPos(cx-(128*Scale),cy-(128*Scale));
	Canvas.DrawTile(shader'ScopeOverlay.SSO_Outer',256*Scale,256*Scale,0,0,256,256);
	Canvas.SetPos(cx-(64*Scale),cy-(64*Scale));
	Canvas.DrawTile(shader'ScopeOverlay.SSO_Inner',128*Scale,128*Scale,0,0,128,128);
*/
	// Move Left Panel
/*
	if (xRates[0]!=0)
	{
		xOffsets[0]+= xRates[0] * delta;
		if (xOffsets[0] > 122.0*Scale)
		{
			xRates[0] = 0;
			xOffsets[0]=122.0*Scale;
		}
	}

	if (xRates[1]!=0)
	{
		xOffsets[1]-= xRates[1] * delta;
		if (xOffsets[1] < 511.0*Scale)
		{
			xRates[1] = 0;
			xOffsets[1]=511.0*Scale;
		}
	}
*/

	/*
	if (xRates[0]!=0)
	{
		xOffsets[0]+= xRates[0] * delta;
		if (xOffsets[0] > 262.0*Scale)
		{
			xRates[0] = 0;
			xOffsets[0]=262.0*Scale;
		}
	}

	if (xRates[1]!=0)
	{
		xOffsets[1]-= xRates[1] * delta;
		if (xOffsets[1] < 385.0*Scale)
		{
			xRates[1] = 0;
			xOffsets[1]=385.0*Scale;
		}
	}

	canvas.SetPos(122*Scale,xOffsets[0]); //,262*Scale);
	Canvas.DrawTile(shader'ScopeOverlay.base.SSO_LeftPanel',390*Scale,123*Scale,0,0,390,120);
	
	canvas.SetPos(511*Scale,xOffsets[1]); //,385*Scale);
	Canvas.DrawTile(shader'ScopeOverlay.base.SSO_RightPanel',390*Scale,123*Scale,0,0,390,120);

	// Draw the Hashes
	
	for (i=0;i<2;i++)
	{
		yOffsets[i] += yRates[i] * Delta;
		if (yOffsets[i]<64.0*Scale)
		{
			yOffsets[i]=64.0*Scale;
			yRates[i]*=-1;
		}
		if (yOffsets[i]>Canvas.ClipY-(128.0*Scale) - (64.0*Scale))
		{
			yOffsets[i]=Canvas.CLipY-(128.0*Scale) - (64.0*Scale);
			yRates[i]*=-1;
		}
	}
			
	canvas.SetPos(90*Scale,yOffsets[0]);
	canvas.DrawTile(shader'ScopeOverlay.base.SSO_LeftArrowMoving',32*Scale,128*Scale,0,0,32,128);
			
	canvas.SetPos(904*Scale,yOffsets[1]);
	canvas.DrawTile(shader'ScopeOverlay.base.SSO_RightArrowMoving',32*Scale,128*Scale,0,0,32,128);
*/
}

simulated exec function ToggleScope()
{
	bHideScope = !bHideScope;
	if (bHideScope)
		bInitialized = false;
}	
 

defaultproperties
{
	bHideScope=true
}
