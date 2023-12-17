// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Weapon.generated.h"

class AImbadCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMBAD_API UWeapon : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeapon();

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AImbadProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AImbadCharacter* TargetCharacter);

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	AImbadCharacter* Character;
		
};
