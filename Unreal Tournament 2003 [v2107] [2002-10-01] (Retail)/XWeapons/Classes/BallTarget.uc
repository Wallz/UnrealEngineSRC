class BallTarget extends WeaponFire;

var() float AimCone;
var() float MaxDist;

simulated function bool AllowFire()
{
    return true;
}

function DoFireEffect()
{
    local Pawn p;
    local Pawn bestP;
    local float bestWt;
    local vector toPawn;
    local vector eyeDir;
    local float cosdot;

    local float curwt;
    local float curdist;
    local float curdot;    

    if ( (Instigator.Role < ROLE_Authority) || (Instigator.PlayerReplicationInfo == None) || (Instigator.PlayerReplicationInfo.Team == None) )
        return;

    bestP = None;
    cosdot = cos(AimCone*PI/180);
    eyeDir = Vector(Instigator.Controller.Rotation);
    bestWt = 0.0;

    foreach Instigator.VisibleActors(class'Pawn', p, MaxDist )
    {
        if ( (p == Instigator) || (p.PlayerReplicationInfo == None) || (P.PlayerReplicationInfo.Team != Instigator.PlayerReplicationInfo.Team) )
            continue;
        
        toPawn = p.Location - Instigator.Location;
        curdot = Normal(toPawn) dot eyeDir;
        curdist = VSize(toPawn);

        if( curdot < cosdot )
            continue;
        if( curdist > MaxDist )
            continue;       

        curwt = 1.0 - (curDist / MaxDist);
        curwt *= curdot;
        if( curwt > bestWt )
        {
            bestWt = curwt;
            bestP = p;
        }
    }

    BallLauncher(Weapon).SetPassTarget(bestP);
}

defaultproperties
{
    AmmoClass=class'BallAmmo'
    AmmoPerFire=0

    FireRate=0.25
    bFireOnRelease=true
    AimCone=20.0
    MaxDist=4000.0
}
