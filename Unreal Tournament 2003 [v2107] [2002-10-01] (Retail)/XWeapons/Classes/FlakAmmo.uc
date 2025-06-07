class FlakAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Flak Shells"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=545,Y1=0,X2=644,Y2=74)

    PickupClass=class'FlakAmmoPickup'
    MaxAmmo=50
    InitialAmount=15
}
