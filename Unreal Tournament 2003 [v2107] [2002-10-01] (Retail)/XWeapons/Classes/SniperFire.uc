class SniperFire extends InstantFire;

var() class<xEmitter> HitEmitterClass;
var() class<xEmitter> SecHitEmitterClass;
var() int NumArcs;
var() float SecDamageMult;
var() float SecTraceDist;
var() float HeadShotDamageMult;
var() float HeadShotRadius;
var() class<DamageType> DamageTypeHeadShot;

function DoTrace(Vector Start, Rotator Dir)
{
    local Vector X,Y,Z, End, HitLocation, HitNormal, RefNormal;
    local Actor Other, mainArcHitTarget;
    local int Damage, ReflectNum, arcsRemaining;
    local bool bDoReflect;
    local xEmitter hitEmitter;
    local class<Actor> tmpHitEmitClass;
    local float tmpTraceRange, dist;
    local vector arcEnd, mainArcHit;

    Weapon.GetViewAxes(X, Y, Z);
    arcEnd = (Instigator.Location + 
        Instigator.CalcDrawOffset(Weapon) + 
        Weapon.EffectOffset.X * X + 
        Weapon.EffectOffset.Y * Y + 
        Weapon.EffectOffset.Z * Z); 

    arcsRemaining = NumArcs;

    tmpHitEmitClass = HitEmitterClass;
    tmpTraceRange = TraceRange;
    
    ReflectNum = 0;
    while (true)
    {
        bDoReflect = false;
        X = Vector(Dir);
        End = Start + tmpTraceRange * X;
        Other = Trace(HitLocation, HitNormal, End, Start, true);

        if ( Other != None && (Other != Instigator || ReflectNum > 0) )
        {
            if (bReflective && Other.IsA('xPawn') && xPawn(Other).CheckReflect(HitLocation, RefNormal, DamageMin*0.25))
            {
                bDoReflect = true;
            }
            else if ( Other != mainArcHitTarget )
            {
                if ( !Other.bWorldGeometry )
                {
                    Damage = (DamageMin + Rand(DamageMax - DamageMin)) * DamageAtten;
					if ( (Pawn(Other) != None) && (arcsRemaining == NumArcs) 
						&& Other.GetClosestBone( HitLocation, X, dist, 'head', HeadShotRadius ) == 'head' )
                        Other.TakeDamage(Damage * HeadShotDamageMult, Instigator, HitLocation, Momentum*X, DamageTypeHeadShot);
                    else
                    {
						if ( arcsRemaining < NumArcs )
							Damage *= SecDamageMult;
                        Other.TakeDamage(Damage, Instigator, HitLocation, Momentum*X, DamageType);
					}
                }
                else
					HitLocation = HitLocation + 2.0 * HitNormal;
            }
        }
        else
        {
            HitLocation = End;
            HitNormal = Normal(Start - End);
        }
        hitEmitter = xEmitter(Spawn(tmpHitEmitClass,,, HitLocation, Rotator(HitNormal)));
        if ( hitEmitter != None )
			hitEmitter.mSpawnVecA = arcEnd;

        if( arcsRemaining == NumArcs )
        {
            mainArcHit = HitLocation + (HitNormal * 2.0);
            if ( Other != None && !Other.bWorldGeometry )
                mainArcHitTarget = Other;
        }
        
        if (bDoReflect && ++ReflectNum < 4)
        {
            //Log("reflecting off"@Other@Start@HitLocation);
            Start = HitLocation;
            Dir = Rotator( X - 2.0*RefNormal*(X dot RefNormal) );
        }
        else if ( arcsRemaining > 0 )
        {
            arcsRemaining--;

            // done parent arc, now move trace point to arc trace hit location and try child arcs from there
            Start = mainArcHit;
            Dir = Rotator(VRand());
            tmpHitEmitClass = SecHitEmitterClass;
            tmpTraceRange = SecTraceDist;
            arcEnd = mainArcHit;
        }
        else
        {
            break;
        }
    }

    if (Weapon.Ammo[ThisModeNum].AmmoAmount > 0)
    {
        Weapon.PlaySound(Sound'WeaponSounds.LightningGunChargeUp', SLOT_Misc,,,,,false);
    }
}

defaultproperties
{
    AmmoClass=class'SniperAmmo'
    AmmoPerFire=1
    DamageType=class'DamTypeSniperShot'
    DamageTypeHeadShot=class'DamTypeSniperHeadShot'
    DamageMin=70
    DamageMax=70
    HitEmitterClass=class'LightningBolt'
    SecHitEmitterClass=class'ChildLightningBolt'
    FireSound=Sound'WeaponSounds.BLightningGunFire'
    FireForce="LightningGunFire"  // jdf
    TraceRange=17000
    FireRate=1.8
    NumArcs=4
    SecDamageMult=0.5
    SecTraceDist=300.0
	FireAnimRate=1.11
	
    BotRefireRate=0.4
    AimError=850

    HeadShotDamageMult=2.0
    HeadShotRadius=8.0

    ShakeOffsetMag=(X=-15.0,Y=0.0,Z=10.0)
    ShakeOffsetRate=(X=-4000.0,Y=0.0,Z=4000.0)
    ShakeOffsetTime=1.6
    ShakeRotMag=(X=0.0,Y=0.0,Z=0.0)
    ShakeRotRate=(X=0.0,Y=0.0,Z=0.0)
    ShakeRotTime=2
}
