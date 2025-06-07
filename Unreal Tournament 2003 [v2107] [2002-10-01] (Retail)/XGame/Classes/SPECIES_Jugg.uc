class SPECIES_Jugg extends SpeciesType
	abstract;

static function string GetRagSkelName(String MeshName)
{
	return "Jugg2";
}

defaultproperties
{
	SpeciesName="Juggernaut"
	RaceNum=3
	MaleVoice="XGame.JuggMaleVoice"
	FemaleVoice="XGame.JuggFemaleVoice"
	MaleSoundGroup="XGame.xJuggMaleSoundGroup"
	FemaleSoundGroup="XGame.xJuggFemaleSoundGroup"
	MaleSkeleton="Jugg.SkeletonJugg"
	FemaleSkeleton="Jugg.SkeletonJugg"
	GibGroup="xGame.xJuggGibGroup"
	AirControl=+0.7
	GroundSpeed=+1.0
	WaterSpeed=+1.0
	JumpZ=+1.0
	ReceivedDamageScaling=+0.7
	DamageScaling=+1.0
	AccelRate=+0.7
	WalkingPct=+0.8
	CrouchedPct=+0.8
	DodgeSpeedFactor=+0.9
	DodgeSpeedZ=+0.9
}