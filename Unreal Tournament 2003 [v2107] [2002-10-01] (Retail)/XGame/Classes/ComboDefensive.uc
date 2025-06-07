class ComboDefensive extends Combo;

var xEmitter Effect;

function StartEffect(xPawn P)
{
    if (P.Role == ROLE_Authority)
        Effect = Spawn(class'RegenCrosses', P,, P.Location, P.Rotation);

    SetTimer(0.9, true);
    Timer();
}

function Timer()
{
    if (Pawn(Owner).Role == ROLE_Authority)
    {
        Pawn(Owner).GiveHealth(5, 199);
        if ( Pawn(Owner).Health == 199 )
			Pawn(Owner).AddShieldStrength(5);
    }
}

function StopEffect(xPawn P)
{
    if ( Effect != None )
        Effect.Destroy();
}

defaultproperties
{
    ExecMessage="Booster!"
    ComboAnnouncement=sound'AnnouncerMain.Booster'
    keys(0)=2
    keys(1)=2
    keys(2)=2
    keys(3)=2
}
