// Used for weapons page

class SpinnyWeap extends Actor;

var() int SpinRate;

var()	bool			bPlayRandomAnims;
var()	float			AnimChangeInterval;
var()	array<name>		AnimNames;

var		float			CurrentTime;
var		float			NextAnimTime;

function Tick(float Delta)
{
	local rotator NewRot;

	NewRot = Rotation;
	NewRot.Yaw += Delta * SpinRate;
	SetRotation(NewRot);

	CurrentTime += Delta;

	// If desired, play some random animations
	if(bPlayRandomAnims && CurrentTime >= NextAnimTime)
	{
		PlayNextAnim();
	}
}

function PlayNextAnim()
{
	local name NewAnimName;

	if(Mesh == None || AnimNames.Length == 0)
		return;

	NewAnimName = AnimNames[Rand(AnimNames.Length)];
	PlayAnim(NewAnimName, 1.0, 0.25); 

	NextAnimTime = CurrentTime + AnimChangeInterval;
}

defaultproperties
{
	RemoteRole=ROLE_None
	bUnlit=true
	SpinRate=20000
	DrawType=DT_StaticMesh
	//DrawType=DT_Mesh
	bAlwaysTick=true
	DrawScale=0.5
	LODBias=100000

	AnimNames(0)=Idle_Rest
	AnimNames(1)=Crouch
	AnimNames(2)=asssmack
	AnimNames(3)=pthrust
	AnimNames(4)=throatcut
	AnimNames(5)=gesture_halt
	AnimNames(6)=gesture_point
	AnimNames(7)=gesture_beckon
	AnimChangeInterval=3.0;
}