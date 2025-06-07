class ShockAmmo extends Ammunition;

#EXEC OBJ LOAD FILE=InterfaceContent.utx

defaultproperties
{
    ItemName="Shock Core"
    IconMaterial=Material'InterfaceContent.Hud.SkinA'
    IconCoords=(X1=645,Y1=75,X2=744,Y2=149)

    PickupClass=class'ShockAmmoPickup'
    MaxAmmo=50
    InitialAmount=20
}
