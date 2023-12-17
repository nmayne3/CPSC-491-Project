// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "ImbadCharacter.h"
#include "ImbadProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeapon::UWeapon(): FireSound(nullptr), Character(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWeapon::Attack()
{
	if(Character == nullptr) 
	{
		return;
	}
	// Call Fire Function
	Fire();
	// Play fire sound
	if(FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
}


void UWeapon::Fire()
{
	// Try to fire projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			
			const FVector SpawnLocation = Character->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AImbadProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}
}


void UWeapon::AttachWeapon(AImbadCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
		return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(Character->GetFirstPersonCameraComponent(), AttachmentRules, FName(TEXT("GripPoint"))
}
