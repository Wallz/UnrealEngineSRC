//=============================================================================
// SuperHealthPack
//=============================================================================
class SuperHealthPack extends TournamentHealth
	notplaceable;

defaultproperties
{
    PickupMessage="You picked up a Big Keg O' Health +"
    MaxDesireability=2.0
    RespawnTime=60.000000
    bSuperHeal=true
    Physics=PHYS_Rotating
	RotationRate=(Yaw=2000)
    DrawScale=0.5
    PickupSound=sound'PickupSounds.LargeHealthPickup'
    PickupForce="LargeHealthPickup"  // jdf
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'E_Pickups.SuperKeg'
    CollisionRadius=42.0
    HealingAmount=100
    Style=STY_AlphaZ
    ScaleGlow=0.6
    bPredictRespawns=true
	bUnlit=true
    bAmbientGlow=false
    AmbientGlow=64
}
