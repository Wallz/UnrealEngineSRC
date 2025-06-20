//=============================================================================
// xTeamBanner.
//=============================================================================
class xTeamBanner extends Decoration;

// todo: the white texture used for dom2 neutral state needs to be a shader so it can be 2-sided!!!

var() byte Team;
var GameReplicationInfo GRI;

simulated function PostBeginPlay()
{
    LoopAnim('AnimFlag');
    SetAnimFrame(FRand());

    Super.PostBeginPlay();	
}    

simulated function UpdateForTeam()
{
    if (Team == 0)
        Skins[0] = Shader'XGameShaders.RedBannerShader';
    else if ( Team > 1 )
		Skins[0] = Texture'XGameShaders.TeamBanner';
	else
        Skins[0] = Shader'XGameShaders.BlueBannerShader';

	if ( (GRI != None) && (Team < 2) && (GRI.TeamSymbols[Team] != None) )
		TexScaler(Combiner(Shader(Skins[0]).Diffuse).Material2).Material = GRI.TeamSymbols[Team];
}

simulated function SetGRI(GameReplicationInfo NewGRI)
{
	GRI = NewGRI;
	UpdateForTeam();
}	
	
simulated function PostNetBeginPlay()
{
	Super.PostNetBeginPlay();	
    if ( Level.Game != None )
		SetGRI(Level.Game.GameReplicationInfo);
}

simulated function Trigger( actor Other, pawn EventInstigator )
{
    local xDomPoint DPoint;

	DPoint = xDomPoint(Other);
    if ( DPoint != None )
    {        
        // is the point disabled?
        if (!DPoint.bControllable)
			Team = 254;
        else if (DPoint.ControllingTeam == None)
 			Team = 255;               
        else
			Team = DPoint.ControllingTeam.TeamIndex;
        UpdateForTeam();
    }
}

defaultproperties
{
	RemoteRole=Role_None
	bNoDelete=true
    bHidden=false
    bUnlit=true
    bStasis=false
    bStatic=false
    bCollideActors=true
    bCollideWorld=true
    bNetNotify=true
    DrawScale=1.70000
    DrawType=DT_Mesh
    Style=STY_Normal
    Mesh=Mesh'TeamBannerMesh'
    Skins(0)=Texture'XGameShaders.TeamBanner'   
    CollisionRadius=48.000000
    CollisionHeight=90.000000    
    Mass=100.000000
    Tag='DominationChange'
}
