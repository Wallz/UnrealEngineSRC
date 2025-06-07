class RocketAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Rockets"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=744,Y1=0,X2=645,Y2=74)

    PickupClass=class'RocketAmmoPickup'
    MaxAmmo=48
    InitialAmount=12
}
