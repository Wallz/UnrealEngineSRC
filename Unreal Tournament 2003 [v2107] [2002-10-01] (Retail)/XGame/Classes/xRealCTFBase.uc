class xRealCTFBase extends CTFBase
	abstract;

var GameReplicationInfo GRI;
var() vector BaseOffset;

simulated function UpdateForTeam()
{
	if ( (GRI != None) && (GRI.TeamSymbols[DefenderTeamIndex] != None) )
	    TexScaler(Combiner(Shader(FinalBlend(Skins[0]).Material).Diffuse).Material2).Material = GRI.TeamSymbols[DefenderTeamIndex];
}

simulated function SetGRI(GameReplicationInfo NewGRI)
{
	GRI = NewGRI;
	UpdateForTeam();
}	

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();	
    
    if ( Level.NetMode != NM_DedicatedServer )
        LoopAnim('flag',0.8);
    if ( Level.Game != None )
		SetGRI(Level.Game.GameReplicationInfo);
}

defaultproperties
{
    TakenSound=Sound'GameSounds.CTFAlarm'
    bHidden=false
    bNetNotify=true
    CollisionHeight=80.00000
    Mesh=Mesh'XGame_rc.FlagMesh'
    DrawScale=1.2 
    BaseOffset=(x=2.0,y=0.0,z=-18.0)   
}