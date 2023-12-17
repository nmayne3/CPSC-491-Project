// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUI.h"

#include "Components/TextBlock.h"
#include "Imbad/ImbadCharacter.h"

void UWeaponUI::SetOwner(AImbadCharacter* Owner)
{
	Character = Owner;
}

void UWeaponUI::UpdateAmmo()
{
	if(UTP_WeaponComponent* Weapon = Character->EquippedWeapon)
	{
		uint8 CurrentAmmo;
		uint8 WeaponAmmo = Weapon->Ammo;
		FSlateColor Color = FLinearColor(1,1,1,1);
		WeaponAmmoLabel->SetText(FText::AsNumber(WeaponAmmo));
		if(WeaponAmmo == 0)
		{
			Color = FLinearColor(1, 0, 0, 1);
		}
		WeaponAmmoLabel->SetColorAndOpacity(Color);
		if (UItemDataAsset* AmmoData = Character->InventoryComponent->GetItemByName(Weapon->AmmoName))
		{
			CurrentAmmo = AmmoData->CurrentStack;
		}
		else
		{
			CurrentAmmo = 0;
			Color = FLinearColor(1,0,0,1);
		}
		TotalAmmoLabel->SetText(FText::AsNumber(CurrentAmmo));
		TotalAmmoLabel->SetColorAndOpacity(Color);
	}
}
