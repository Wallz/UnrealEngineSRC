//=============================
// Jumppad - bounces players/bots up
// not directly placeable.  Make a subclass with appropriate sound effect etc.
//
class JumpPad extends NavigationPoint
	native;

var vector JumpVelocity;
var Actor JumpTarget;
var() float JumpZModifier;	// for tweaking Jump, if needed
var() sound JumpSound;

cpptext
{
	void addReachSpecs(APawn * Scout, UBOOL bOnlyChanged);
	void RenderEditorSelected(FLevelSceneNode* SceneNode,FRenderInterface* RI, FDynamicActor* FDA);
}

event Touch(Actor Other)
{
	if ( Pawn(Other) == None )
		return;

	PendingTouch = Other.PendingTouch;
	Other.PendingTouch = self;
}

event PostTouch(Actor Other)
{
	local Pawn P;

	P = Pawn(Other);
	if ( P == None )
		return;

	if ( AIController(P.Controller) != None )
	{
		P.Controller.Movetarget = JumpTarget;
		P.Controller.Focus = JumpTarget;
		P.Controller.MoveTimer = 2.0;
		P.DestinationOffset = JumpTarget.CollisionRadius;
	}
	if ( P.Physics == PHYS_Walking )
		P.SetPhysics(PHYS_Falling);
	P.Velocity =  JumpVelocity;
	P.Acceleration = vect(0,0,0);
	if ( JumpSound != None )
		P.PlaySound(JumpSound);
}

defaultproperties
{
	bDestinationOnly=true
	bCollideActors=true
	JumpVelocity=(x=0.0,y=0.0,z=1200.0)
	JumpZModifier=+1.0
}