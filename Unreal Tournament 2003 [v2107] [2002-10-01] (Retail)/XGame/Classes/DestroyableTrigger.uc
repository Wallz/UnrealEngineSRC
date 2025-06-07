//=============================================================================
// for tutorial
//=============================================================================
class DestroyableTrigger extends Decoration;

var() Name DamageTypeName;
var() bool bActive;

function Destroyed()
{
    SpawnEffects();
    Super.Destroyed();
}

function SpawnEffects()
{
    Spawn(class'RocketExplosion',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'RocketExplosion',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'WallSparks',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'WallSparks',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'WallSparks',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'WallSparks',,,Location+VRand()*Vect(50,50,50));
    Spawn(class'ExplosionCrap',,, Location, Rotation);
}

function DoHitEffect()
{
    Spawn(class'WallSparks',,,Location+VRand()*Vect(50,50,50));
}

function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, 
						Vector momentum, class<DamageType> damageType)
{
    if( !bActive || (DamageTypeName != 'None' && DamageTypeName != damageType.Name) )
        return;

    if( Health <= 0 )
        return;

    Health -= Damage;

	if ( (Health <= 0) )
	{
		// Broadcast the Trigger message to all matching actors.
		TriggerEvent(Event, self, instigatedBy);
        Destroy();
	}
    else
    {
        DoHitEffect();
    }
}

defaultproperties
{
    bUnlit=false
    bHidden=false
    bStasis=false
    bStatic=false
    bCollideActors=true
    bCollideWorld=true    
    bNetNotify=true
    bBlockKarma=true
    bBlockActors=true
    bBlockPlayers=true
    bProjTarget=true
    bBlockZeroExtentTraces=true
    bBlockNonZeroExtentTraces=true
    bUseCylinderCollision=false    

    bActive=false
    Health=30
}
