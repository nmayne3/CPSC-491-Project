// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Imbad/Weapon.h"
#include "Firearm.generated.h"

/**
 *	Custom Class for firearm weapons
 */
UCLASS()
class IMBAD_API UFirearm : public UWeapon
{
	GENERATED_BODY()

public:
	//UFirearm();
	
	
protected:
	
private:
	virtual void Fire() override;
	void Reload();
	uint8 AmmoCount;
	uint8 AmmoCapacity;
};
