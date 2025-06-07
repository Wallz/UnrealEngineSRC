class SniperAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Lightning Charges"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=445,Y1=150,X2=544,Y2=224)

    bTryHeadShot=true
    PickupClass=class'SniperAmmoPickup'
    MaxAmmo=40
    InitialAmount=15
}
