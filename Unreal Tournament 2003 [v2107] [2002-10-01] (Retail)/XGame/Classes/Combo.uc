class Combo extends Info;

#exec OBJ LOAD FILE=GameSounds.uax

var localized string ExecMessage;
var float AdrenalineCost;
var float Duration;
var Sound ActivateSound;
var Material Icon;
var class<xEmitter> ActivationEffectClass;
var sound ComboAnnouncement;
var int keys[4];
var class<SpeciesType> species;

// CK_Up      = 1;
// CK_Down    = 2;
// CK_Left    = 4;
// CK_Right   = 8;

function PostBeginPlay()
{
    local xPawn P;

    P = xPawn(Owner);
    if (P == None)
    {
        Destroy();
        return;
    }

    if (ActivateSound != None)
        PlaySound(ActivateSound, SLOT_None, 2*TransientSoundVolume);

    if (ActivationEffectClass != None)
        Spawn(ActivationEffectClass, P,, P.Location, P.Rotation); // it's responsible for killing itself

    StartEffect(P);
}

// called when Adrenaline has been drained empty
function AdrenalineEmpty()
{
    Destroy();
}

function Destroyed()
{
    local xPawn P;
    P = xPawn(Owner);

    if (P != None)
    {
        StopEffect(P);

        if (P.CurrentCombo == self)
            P.CurrentCombo = None;
    }
}

function StartEffect(xPawn P);
function StopEffect(xPawn P);

simulated function Tick(float DeltaTime)
{
    local Pawn P;
    
    P = Pawn(Owner);

    if ( (P == None) || (P.Controller == None) )
	{
        Destroy();
        return;
    }
    P.Controller.Adrenaline -= AdrenalineCost*DeltaTime/Duration;
    if (P.Controller.Adrenaline <= 0.0)
    {
        P.Controller.Adrenaline = 0.0;
        Destroy();
    }
}

defaultproperties
{
	Duration=30
	AdrenalineCost=100
    ActivateSound=Sound'GameSounds.ComboActivated'
    ActivationEffectClass=class'xEffects.ComboActivation'
}
