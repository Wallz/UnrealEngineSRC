class BioAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Bio-Rifle Goop"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=545,Y1=75,X2=644,Y2=149)

    PickupClass=class'BioAmmoPickup'
    MaxAmmo=50
    InitialAmount=20
}
