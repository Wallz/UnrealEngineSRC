class LinkAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Link Ammo"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=545,Y1=150,X2=644,Y2=224)

    PickupClass=class'LinkAmmoPickup'
    MaxAmmo=120
    InitialAmount=40
}
