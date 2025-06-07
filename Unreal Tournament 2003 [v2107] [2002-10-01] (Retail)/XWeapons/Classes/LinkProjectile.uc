//=============================================================================
// LinkProjectile.
//=============================================================================
class LinkProjectile extends Projectile;

#exec OBJ LOAD FILE=XEffectMat.utx

var xEmitter Trail;
var int Links;

replication
{
    unreliable if (bNetInitial && Role == ROLE_Authority)
        Links;
}

simulated function Destroyed()
{
    if (Trail != None)
    {
        Trail.Destroy();
    }
	Super.Destroyed();
}

simulated function PostBeginPlay()
{
    local Rotator R;

	Super.PostBeginPlay();

    if ( EffectIsRelevant(vect(0,0,0),false) )
		Trail = Spawn(class'LinkProjEffect',self); 

	Velocity = Vector(Rotation);
    Acceleration = Velocity * 3000.0;
    Velocity *= Speed;

    R = Rotation;
    R.Roll = Rand(65536);
    SetRotation(R);
    
    if ( Level.bDropDetail )
	{
		bDynamicLight = false;
		LightType = LT_None;
	}
} 

simulated function LinkAdjust()
{
    local float ls;

    if (Links > 0)
    {
        ls = class'LinkFire'.default.LinkScale[Min(Links,5)];

        SetDrawScale3D(default.DrawScale3D * (ls + 1));

    	if ( Trail != None )
        {
            Trail.mLifeRange[0] *= (ls + 1);
            Trail.mLifeRange[1] = Trail.mLifeRange[0];

            Trail.mSizeRange[0] *= (ls + 1);
            Trail.mSizeRange[1] = Trail.mSizeRange[0];

            Trail.mPosDev.Y *= (ls + 1);
            Trail.mPosDev.Z = Trail.mPosDev.Y;

            Trail.Skins[0] = Texture'XEffectMat.link_muz_yellow';
        }

        Speed = default.Speed + 200*Links;
        MaxSpeed = default.MaxSpeed + 350*Links;
	    Velocity = Speed * Vector(Rotation);

        SetCollisionSize(ls*10, ls*10);

        Skins[0] = FinalBlend'XEffectMat.LinkProjYellowFB';
        LightHue = 40;
    }
}

simulated function PostNetBeginPlay()
{
    if (Role < ROLE_Authority)
        LinkAdjust();
	if ( Level.bDropDetail || Level.DetailMode == DM_Low )
		LightType = LT_None;
}

simulated function Explode(vector HitLocation, vector HitNormal)
{
    if ( EffectIsRelevant(Location,false) )
	{
        if (Links == 0)
            Spawn(class'LinkProjSparks',,, HitLocation, rotator(HitNormal));
        else
            Spawn(class'LinkProjSparksYellow',,, HitLocation, rotator(HitNormal));
	}
    PlaySound(Sound'WeaponSounds.BioRifle.BioRifleGoo2');
	Destroy();
}

simulated function ProcessTouch (Actor Other, vector HitLocation)
{
    local Vector X, RefNormal, RefDir;

	if (Other == Instigator) return;
    if (Other == Owner) return;
	
    if (Other.IsA('xPawn') && xPawn(Other).CheckReflect(HitLocation, RefNormal, Damage*0.25))
    {
        if (Role == ROLE_Authority)
        {
            X = Normal(Velocity);
            RefDir = X - 2.0*RefNormal*(X dot RefNormal);
            //Log("reflecting off"@Other@X@RefDir);
            Spawn(Class, Other,, HitLocation+RefDir*20, Rotator(RefDir));
        }
        Destroy();
    }
    else if ( !Other.IsA('Projectile') || Other.bProjTarget )
	{
		if ( Role == ROLE_Authority )
			Other.TakeDamage(Damage * (1.0 + float(Links)),Instigator,HitLocation,MomentumTransfer * (1.0 + float(Links)*0.25) * Normal(Velocity),MyDamageType);
		Explode(HitLocation, vect(0,0,1));
	}
}

defaultproperties
{ 
    ExplosionDecal=class'LinkBoltScorch'
    Damage=30
    MyDamageType=class'DamTypeLinkPlasma'
    Speed=1000
    MaxSpeed=4000
    MomentumTransfer=25000
    ExploWallOut=0
    LifeSpan=4
    AmbientGlow=217
    bDynamicLight=true
    LightType=LT_Steady
    LightEffect=LE_QuadraticNonIncidence
    LightRadius=3
    LightBrightness=100
    LightHue=100
    LightSaturation=100
    bFixedRotationDir=True
    RotationRate=(Roll=80000)
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'WeaponStaticMesh.LinkProjectile'
    DrawScale3D=(X=2.55,Y=1.7,Z=1.7)
    Style=STY_Additive
    AmbientSound=Sound'WeaponSounds.ShockRifle.LinkGunProjectile'
    SoundRadius=50
    SoundVolume=255
    ForceType=FT_Constant
    ForceScale=5.0
    ForceRadius=30.0
    PrePivot=(X=10)
}
