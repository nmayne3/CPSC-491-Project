// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ImbadCharacter.h"
#include "ImbadProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "Inventory/InventoryComponent.h"
#include "Widgets/WeaponUI.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent(): Ammo(6), MaxAmmo(6), Recoil(0), HSpread(5), VSpread(5), NumProjectiles(1),
                                            Hitscan(true),
                                            Damage(20),
                                            FireSound(nullptr),
                                            FireAnimation(nullptr),
                                            FireMappingContext(nullptr),
                                            FireAction(nullptr), ReloadAction(nullptr), MuzzleFlashFX(nullptr),
                                            Scale(1), Character(nullptr) 
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	LocalOffset = FVector(0, 0, 0);
	//ItemDataAsset();
}


void UTP_WeaponComponent::Fire()
{
	
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	OnShoot.Broadcast(Character);

	if(Ammo == 0)
	{
		// Have it play an animation for trying to fire
		return;
	}

	// Try and fire a projectile
	if (UWorld* const World = GetWorld())
	{
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FVector SpawnLocation = GetSocketLocation(FName(TEXT("Muzzle")));
		for(int i = 0; i < NumProjectiles; i++)
		{
			FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			SpawnRotation += { FMath::RandRange(-VSpread/2, VSpread/2), FMath::RandRange(-HSpread/2, HSpread/2), 0};
			
			if(Hitscan || ProjectileClass == nullptr)
			{
				FVector TraceEnd = SpawnLocation + SpawnRotation.Vector() * 36000;
				FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
				TraceParams.bReturnPhysicalMaterial = true;
				FHitResult HitResult;
				// Perform Raycast
				if(GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, TraceEnd, ECC_GameTraceChannel1, TraceParams))
				{
					AActor* OtherActor = HitResult.GetActor();
					OtherActor->TakeDamage(Damage, FPointDamageEvent(Damage, HitResult, HitResult.ImpactPoint,
						nullptr), nullptr, Character);
					if ((OtherActor != nullptr) && (HitResult.Component != nullptr) && HitResult.Component->IsSimulatingPhysics())
					{
						HitResult.Component->AddImpulseAtLocation(HitResult.ImpactNormal * 100.0f, HitResult.Location);
					}
					OnHit.Broadcast(HitResult);
				}
			}	// Projectile Weapon
			else
			{
				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// Assign Instigator/Owner of Bullet
				ActorSpawnParams.Owner = Character;
				
				// Spawn the projectile at the muzzle
				AImbadProjectile* Bullet = World->SpawnActor<AImbadProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if(Bullet != nullptr)
				{
					Bullet->SetOwner(Character);
					Character->MoveIgnoreActorAdd(Bullet);
				}
			}
		}
		if(CameraShake)
		{
			PlayerController->PlayerCameraManager->StartCameraShake(CameraShake, 1);
		}
		Ammo--;
		if(UWeaponUI* WeaponUI = Character->WeaponUI)
		{
			WeaponUI->UpdateAmmo();
		}
	}
	
	// Try and play the sound if specified (MOVED TO ANIMATION)
	if (FireSound != nullptr)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		if(GetAnimInstance())
		{
			GetAnimInstance()->Montage_Play(FireAnimation,1.f);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,"AnimInstanceNull??");
		}
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance && ArmsAnimation)
		{
			AnimInstance->Montage_Play(ArmsAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::Reload()
{
	// ToDo: Play Reload Animation of Some Sort
	if(UItemDataAsset* AmmoData = Character->InventoryComponent->GetItemByName(AmmoName))
	{
		uint8 RequestedAmmo = MaxAmmo - Ammo;
		Ammo = (AmmoData->CurrentStack < RequestedAmmo) ? AmmoData->CurrentStack : MaxAmmo;
		Character->InventoryComponent->Subtract(AmmoName, RequestedAmmo);
		if(UWeaponUI* WeaponUI = Character->WeaponUI)
		{
			WeaponUI->UpdateAmmo();
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AImbadCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	RegisterComponent();

	SetRelativeScale3D({Scale, Scale, Scale});
	SetRelativeLocation(LocalOffset);
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);
	
	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Reload);
		}
	}
}


void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
