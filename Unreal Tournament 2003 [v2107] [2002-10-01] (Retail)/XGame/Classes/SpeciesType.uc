class SpeciesType extends Object
	abstract
	native;

var string	MaleVoice;
var string	FemaleVoice;
var string	GibGroup;
var string	MaleRagSkelName;
var string	FemaleRagSkelName;
var string	FemaleSkeleton;
var string	MaleSkeleton;
var string	MaleSoundGroup;
var string	FemaleSoundGroup;
var string  PawnClassName;
var localized string SpeciesName;	// human readable name, for menus
var int RaceNum;

var float AirControl, GroundSpeed, WaterSpeed, JumpZ, ReceivedDamageScaling, DamageScaling, AccelRate, WalkingPct,CrouchedPct,DodgeSpeedFactor, DodgeSpeedZ;

static function LoadResources( xUtil.PlayerRecord rec, LevelInfo Level, PlayerReplicationInfo PRI, int TeamNum )
{
	local string BodySkinName, VoiceType, SkelName;
	local Material NewBodySkin, NewFaceSkin;
	local class<VoicePack> VoiceClass;
	local xPlayerReplicationInfo xPRI;
	local class<xPawnGibGroup> GibGroupClass;
	 
	if ( class'DeathMatch'.default.bForceDefaultCharacter )
		return;
		
    DynamicLoadObject(rec.MeshName,class'Mesh');
		
	if ( rec.Sex ~= "Female" )
	{
		SkelName = Default.FemaleSkeleton;
		if ( Level.bLowSoundDetail )
			DynamicLoadObject("XGame.xJuggFemaleSoundGroup", class'Class');
		else
			DynamicLoadObject(Default.FemaleSoundGroup, class'Class');
	}
	else
	{
		SkelName = Default.MaleSkeleton;
		if ( Level.bLowSoundDetail )
			DynamicLoadObject("XGame.xJuggMaleSoundGroup", class'Class');
		else
			DynamicLoadObject(Default.MaleSoundGroup, class'Class');
	}
	if ( SkelName != "" )
		DynamicLoadObject(SkelName,class'Mesh'); 

	xPRI = xPlayerReplicationInfo(PRI);

	if ( (TeamNum != 255) && ((xPRI == None) || !xPRI.bNoTeamSkins) )
	{
		BodySkinName = rec.BodySkinName$"_"$TeamNum;
		NewBodySkin = Material(DynamicLoadObject(BodySkinName, class'Material'));
		if ( NewBodySkin == None )
		{
			log("TeamSkin not found "$NewBodySkin);
			NewBodySkin = Material(DynamicLoadObject(rec.BodySkinName, class'Material'));
		}
	}
	else
		NewBodySkin = Material(DynamicLoadObject(rec.BodySkinName, class'Material'));

	NewFaceSkin = Material(DynamicLoadObject(rec.FaceSkinName, class'Material')); 
	Level.AddPrecacheMaterial(NewBodySkin);
	Level.AddPrecacheMaterial(NewFaceSkin);
	Level.AddPrecacheMaterial(rec.Portrait);
	GibGroupClass = class<xPawnGibGroup>(DynamicLoadObject(Default.GibGroup, class'Class'));
	GibGroupClass.static.PrecacheContent(Level);

	if ( !Level.bLowSoundDetail && (rec.VoiceClassName != "") )
	{
		VoiceType = rec.VoiceClassName;
		VoiceClass = class<VoicePack>(DynamicLoadObject(VoiceType,class'Class'));
	}
	if ( VoiceClass == None )
	{
		if ( rec.Sex ~= "Female" )
		{
			if ( Level.bLowSoundDetail )
				VoiceType = "XGame.JuggFemaleVoice";
			else
				VoiceType = Default.FemaleVoice;
		}
		else
		{
			if ( Level.bLowSoundDetail )
				VoiceType = "XGame.JuggMaleVoice";
			else
				VoiceType = Default.MaleVoice;
		}
		class<VoicePack>(DynamicLoadObject(VoiceType,class'Class'));
	}	
}

static function int ModifyReceivedDamage( int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
{
	return Damage * Default.ReceivedDamageScaling;
}

static function int ModifyImpartedDamage( int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
{
	return Damage * Default.DamageScaling;
}

static function ModifyPawn(Pawn P)
{
	P.AirControl = P.Default.AirControl * Default.AirControl;
	P.GroundSpeed = P.Default.GroundSpeed * Default.GroundSpeed;
	P.WaterSpeed = P.Default.WaterSpeed * Default.WaterSpeed;
	P.JumpZ = P.Default.JumpZ * Default.JumpZ;
	P.AccelRate = P.Default.AccelRate * Default.AccelRate;
	P.WalkingPct = P.Default.WalkingPct * Default.WalkingPct;
	P.CrouchedPct = P.Default.CrouchedPct * Default.CrouchedPct;
	P.DodgeSpeedFactor = P.Default.DodgeSpeedFactor * Default.DodgeSpeedFactor;
	P.DodgeSpeedZ = P.Default.DodgeSpeedZ * Default.DodgeSpeedZ;
}

static function string GetRagSkelName(string MeshName)
{
	if(InStr(MeshName, "Female") >= 0)
		return Default.FemaleRagSkelName;
	else if(InStr(MeshName, "Male") >= 0)
		return Default.MaleRagSkelName;
	return "";
}

static function bool Setup(xPawn P, xUtil.PlayerRecord rec)
{
	local mesh NewMesh;
	local string BodySkinName, VoiceType, SkelName;
	local Material NewBodySkin;
	local class<VoicePack> VoiceClass;
	local xPlayerReplicationInfo xPRI;

    NewMesh = Mesh(DynamicLoadObject(rec.MeshName,class'Mesh'));
    if ( NewMesh == None )
		return false;
		
	P.LinkMesh(NewMesh);
	P.AssignInitialPose();
	
	P.bIsFemale = ( rec.Sex ~= "Female" );
	P.PlayerReplicationInfo.bIsFemale = P.bIsFemale;
	if ( P.bIsFemale )
	{
		SkelName = Default.FemaleSkeleton;
		if ( P.Level.bLowSoundDetail )
			P.SoundGroupClass = class<xPawnSoundGroup>(DynamicLoadObject("XGame.xJuggFemaleSoundGroup", class'Class'));
		else
			P.SoundGroupClass = class<xPawnSoundGroup>(DynamicLoadObject(Default.FemaleSoundGroup, class'Class'));
	}
	else
	{
		SkelName = Default.MaleSkeleton;
		if ( P.Level.bLowSoundDetail )
			P.SoundGroupClass = class<xPawnSoundGroup>(DynamicLoadObject("XGame.xJuggMaleSoundGroup", class'Class'));
		else
			P.SoundGroupClass = class<xPawnSoundGroup>(DynamicLoadObject(Default.MaleSoundGroup, class'Class'));
	}
	if ( SkelName != "" )
		P.SkeletonMesh = mesh(DynamicLoadObject(SkelName,class'Mesh')); 

	xPRI = xPlayerReplicationInfo(P.PlayerReplicationInfo);

	if ( (P.PlayerReplicationInfo != None) && (P.PlayerReplicationInfo.Team != None) && !(xPRI != None && xPRI.bNoTeamSkins) )
	{
		BodySkinName = rec.BodySkinName$"_"$P.PlayerReplicationInfo.Team.TeamIndex;
		NewBodySkin = Material(DynamicLoadObject(BodySkinName, class'Material'));
		if ( NewBodySkin == None )
		{
			log("TeamSkin not found "$NewBodySkin);
			NewBodySkin = Material(DynamicLoadObject(rec.BodySkinName, class'Material'));
		}
		P.Skins[0] = NewBodySkin;
	}
	else
		P.Skins[0] = Material(DynamicLoadObject(rec.BodySkinName, class'Material'));
	P.Skins[1] = Material(DynamicLoadObject(rec.FaceSkinName, class'Material')); 
	P.GibGroupClass = class<xPawnGibGroup>(DynamicLoadObject(Default.GibGroup, class'Class'));

	if ( !P.Level.bLowSoundDetail && (rec.VoiceClassName != "") )
	{
		VoiceType = rec.VoiceClassName;
		VoiceClass = class<VoicePack>(DynamicLoadObject(VoiceType,class'Class'));
	}
	if ( VoiceClass == None )
	{
		if ( P.bIsFemale )
		{
			if ( P.Level.bLowSoundDetail )
				VoiceType = "XGame.JuggFemaleVoice";
			else
				VoiceType = Default.FemaleVoice;
		}
		else
		{
			if ( P.Level.bLowSoundDetail )
				VoiceType = "XGame.JuggMaleVoice";
			else
				VoiceType = Default.MaleVoice;
		}
		VoiceClass = class<VoicePack>(DynamicLoadObject(VoiceType,class'Class'));
	}
	P.VoiceType = VoiceType;
	P.PlayerReplicationInfo.VoiceType = VoiceClass;
	P.VoiceClass = class<TeamVoicePack>(VoiceClass);
	return true;
}

defaultproperties
{
	AirControl=+1.0
	GroundSpeed=+1.0
	WaterSpeed=+1.0
	JumpZ=+1.0
	ReceivedDamageScaling=+1.0
	DamageScaling=+1.0
	AccelRate=+1.0
	WalkingPct=+1.0
	CrouchedPct=+1.0
	DodgeSpeedFactor=+1.0
	DodgeSpeedZ=+1.0
	PawnClassName="xGame.xPawn"
	SpeciesName="Human"
}