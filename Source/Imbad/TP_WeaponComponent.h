// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory/Item.h"
#include "Inventory/ItemDataAsset.h"
#include "TP_WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShoot, AImbadCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, HitResult);


class AImbadCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMBAD_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category = "Events")
	FOnShoot OnShoot;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category = "Events")
	FOnHit OnHit;
	
	// Local Ammunition count for single weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	uint8 Ammo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	uint8 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	float HSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	float VSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	float NumProjectiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	bool Hitscan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	float Damage;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Type of Projectile Spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AImbadProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<USoundBase> FireSound;

	/** Animation Played when fired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> FireAnimation;

	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<UAnimMontage> ArmsAnimation;

	/** Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	class UInputAction* ReloadAction;
	
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AImbadCharacter* TargetCharacter);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UItemDataAsset* ItemDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
	FName AmmoName;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlashFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=VFX)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UFUNCTION(Category="Transform")
	float GetScale() const { return Scale; }

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transform")
	float Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Transform")
	FVector LocalOffset;
private:
	/** The Character holding this weapon*/
	UPROPERTY()
	AImbadCharacter* Character;
	
};
