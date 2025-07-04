//=============================================================================
// SkaarjProjectile.
//=============================================================================
class SkaarjProjectile extends Projectile;

#exec TEXTURE IMPORT NAME=ExplosionPal FILE=textures\exppal.pcx GROUP=Effects
#exec OBJ LOAD FILE=textures\SkaarjP.utx PACKAGE=UnrealShare.SKEffect

#exec AUDIO IMPORT FILE="Sounds\Skaarj\skasht2.WAV" NAME="Skrjshot" GROUP="Skaarj"
#exec AUDIO IMPORT FILE="Sounds\Skaarj\skaimp3.WAV" NAME="SkrjImp" GROUP="Skaarj"

auto state Flying
{
	simulated function Timer()
	{
		Texture = Texture'Skj_a04';
	}

	simulated function ProcessTouch (Actor Other, Vector HitLocation)
	{
		local vector momentum;
	
		if ( !Other.IsA('SkaarjWarrior') )
		{
			if ( Role == ROLE_Authority )
			{
				momentum = 10000.0 * Normal(Velocity);
				Other.TakeDamage(Damage, instigator, HitLocation, momentum, 'zapped');
			}
			Destroy();
		}
	}

	function MakeSound()
	{
		PlaySound(ImpactSound);
		MakeNoise(1.0);
	}
			
	simulated function Explode(vector HitLocation, vector HitNormal)
	{
		local EnergyBurst e;

		MakeSound();
		if ( Level.NetMode != NM_DedicatedServer )
		{
			e = spawn(class 'EnergyBurst',,,HitLocation+HitNormal*9);
			e.RemoteRole = ROLE_None;
		}
		destroy();
	}

	function BeginState()
	{
		if ( ScriptedPawn(Instigator) != None )
			Speed = ScriptedPawn(Instigator).ProjectileSpeed;
		Velocity = Vector(Rotation) * speed;
		PlaySound(SpawnSound);
		SetTimer(0.20,False);
	}

Begin:
	Sleep(7.0); //self destruct after 7.0 seconds
	Explode(Location, vect(0,0,0));
}

defaultproperties
{
     speed=+00800.000000
     MaxSpeed=+01200.000000
     Damage=+00016.000000
     SpawnSound=UnrealShare.Skrjshot
     ImpactSound=UnrealShare.SkrjImp
     DrawType=DT_Sprite
     Style=STY_Translucent
     Texture=UnrealShare.SKEffect.Skj_a00
     DrawScale=+00000.700000
     bUnlit=True
     bMeshCurvy=False
     LightType=LT_Steady
     LightEffect=LE_NonIncidence
     LightBrightness=149
     LightHue=165
     LightSaturation=186
     LightRadius=4
     LifeSpan=+00007.300000
     RemoteRole=ROLE_SimulatedProxy
     NetPriority=+00002.500000
}
