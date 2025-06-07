class UTJumpPad extends JumpPad
	placeable;

event PostTouch(Actor Other)
{
	local Pawn P;
	local Bot B;

	Super.PostTouch(Other);

	P = Pawn(Other);
	if ( P == None )
		return;

	B = Bot(P.Controller);	
	if ( (B != None) && (PhysicsVolume.Gravity.Z > PhysicsVolume.Default.Gravity.Z) )
		B.Focus = B.FaceActor(2);
}	

/*
defaultproperties
{
	JumpSound=sound'
}
*/