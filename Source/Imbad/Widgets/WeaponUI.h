// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponUI.generated.h"

class UTextBlock;
class AImbadCharacter;
/**
 * 
 */
UCLASS()
class IMBAD_API UWeaponUI : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TObjectPtr<AImbadCharacter> Character;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalAmmoLabel;

public:
	UFUNCTION()
	void UpdateAmmo();

	void SetOwner(AImbadCharacter* Owner); 
};
