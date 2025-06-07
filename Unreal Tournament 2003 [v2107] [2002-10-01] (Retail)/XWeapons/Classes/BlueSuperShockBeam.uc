class BlueSuperShockBeam extends SuperShockBeamEffect;

simulated function SpawnEffects()
{
    local Rotator HitRot;
    local Vector EffectLoc;
    local ShockBeamCoil Coil;
    local xWeaponAttachment Attachment;

    if (Instigator != None)
    {
        if ( Instigator.IsFirstPerson() && Instigator.Weapon != None )
        {
            SetLocation(Instigator.Weapon.GetEffectStart());
            Spawn(class'ShockMuzFlash',,, Location);
        }
        else
        {
            Attachment = xPawn(Instigator).WeaponAttachment;
            if (Attachment != None && (Level.TimeSeconds - Attachment.LastRenderTime) < 1)
                SetLocation(Attachment.GetTipLocation());
            else
                SetLocation(Instigator.Location + Instigator.EyeHeight*Vect(0,0,1) + Normal(mSpawnVecA - Instigator.Location) * 25.0); 
            Spawn(class'ShockMuzFlash3rd',,, Location);
        }
    }

    if ( EffectIsRelevant(EffectLoc,false) )
	{
		if (HitNormal != Vect(0,0,0))
		{
			HitRot = Rotator(HitNormal);
			EffectLoc = mSpawnVecA + HitNormal*8;
			Spawn(class'ShockImpactFlare',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactRing',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactScorch',,, EffectLoc, Rotator(-HitNormal));
			Spawn(class'ShockExplosionCore',,, EffectLoc, HitRot);
		}
	}

	Coil = Spawn(CoilClass,,, Location, Rotation);
	if (Coil != None)
	{
		Coil.mSpawnVecA = mSpawnVecA;
	}
}
defaultproperties
{
    Skins(0)=Texture'XGameShaders.ShockBeamBlueTex'
    LightHue=230
	CoilClass=class'ShockBeamCoilBlue'
}