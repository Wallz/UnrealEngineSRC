class MinigunAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Bullets"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=445,Y1=75,X2=544,Y2=149)

    PickupClass=class'MinigunAmmoPickup'
    MaxAmmo=250
    InitialAmount=100
}
