class AnnounceAdrenaline extends Info;

var sound AnnounceSound;

function Timer()
{
	PlayerController(Owner).PlayAnnouncement(AnnounceSound,1);
	Destroy();
}

defaultproperties
{
	lifespan=+30.0
}