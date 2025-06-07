class SuperShockBeamEffect extends ShockBeamEffect;

var class<ShockBeamCoil> CoilClass;

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
            Spawn(class'ShockMuzFlashB',,, Location);
        }
        else
        {
            Attachment = xPawn(Instigator).WeaponAttachment;
            if (Attachment != None && (Level.TimeSeconds - Attachment.LastRenderTime) < 1)
                SetLocation(Attachment.GetTipLocation());
            else
                SetLocation(Instigator.Location + Instigator.EyeHeight*Vect(0,0,1) + Normal(mSpawnVecA - Instigator.Location) * 25.0); 
            Spawn(class'ShockMuzFlashB3rd',,, Location);
        }
    }

    if ( EffectIsRelevant(EffectLoc,false) )
	{
		if (HitNormal != Vect(0,0,0))
		{
			HitRot = Rotator(HitNormal);
			EffectLoc = mSpawnVecA + HitNormal*8;
			Spawn(class'ShockImpactFlareB',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactRingB',,, EffectLoc, HitRot);
			Spawn(class'ShockImpactScorch',,, EffectLoc, Rotator(-HitNormal));
			Spawn(class'ShockExplosionCoreB',,, EffectLoc, HitRot);
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
	CoilClass=class'ShockBeamCoilB'
    Skins(0)=Texture'ShockBeamRedTex'
    LightHue=0
}
