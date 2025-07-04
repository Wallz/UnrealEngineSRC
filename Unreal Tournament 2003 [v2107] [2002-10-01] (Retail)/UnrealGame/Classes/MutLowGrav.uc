class MutLowGrav extends Mutator;

var float GravityZ;

function PostBeginPlay()
{
	Super.PostBeginPlay();
	Level.DefaultGravity = GravityZ;
}

function bool MutatorIsAllowed()
{
	return true;
}

function bool CheckReplacement(Actor Other, out byte bSuperRelevant)
{
    local PhysicsVolume PV;
    local vector XYDir;
    local float ZDiff,Time;
    local JumpPad J;
    
    PV = PhysicsVolume(Other);
    
	if ( PV != None )
		PV.Gravity.Z = FMax(PV.Gravity.Z,GravityZ);

	J = JumpPad(Other);
	if ( J != None )
	{
		XYDir = J.JumpTarget.Location - J.Location;
		ZDiff = XYDir.Z;
		Time = 2.5f * J.JumpZModifier * Sqrt(Abs(ZDiff/GravityZ));
		J.JumpVelocity = XYDir/Time; 
		J.JumpVelocity.Z = ZDiff/Time - 0.5f * GravityZ * Time;
	}
	return true;
}

defaultproperties
{
	GravityZ=-300.0

    IconMaterialName="MutatorArt.nosym"
    ConfigMenuClassName=""
    GroupName="Gravity"
    FriendlyName="LowGrav"
    Description="Low gravity."
}