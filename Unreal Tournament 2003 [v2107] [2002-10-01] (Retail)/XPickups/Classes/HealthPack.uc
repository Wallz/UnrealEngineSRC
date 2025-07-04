//=============================================================================
// HealthPack
//=============================================================================
class HealthPack extends TournamentHealth;

#exec OBJ LOAD FILE=PickupSounds.uax
#exec OBJ LOAD FILE=E_Pickups.usx

defaultproperties
{
    Physics=PHYS_Rotating
	RotationRate=(Yaw=24000)
    DrawScale=0.45
    PickupSound=sound'PickupSounds.HealthPack'
    PickupForce="HealthPack"  // jdf
    DrawType=DT_StaticMesh
    StaticMesh=StaticMesh'E_Pickups.MidHealth'    
    CollisionRadius=32.0
    HealingAmount=25
    Style=STY_AlphaZ
    ScaleGlow=0.6
}
