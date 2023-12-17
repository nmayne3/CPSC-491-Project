// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImbadCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/InteractInterface.h"
#include "Inventory/InventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WeaponUI.h"


//////////////////////////////////////////////////////////////////////////
// AImbadCharacter

AImbadCharacter::AImbadCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	bSteppingUp = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// God please god please
	EquippedWeapon = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("Equppied Weapon"));
	EquippedWeapon->SetupAttachment(Mesh1P, FName(TEXT("GripPoint")));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

void AImbadCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	
	//Add Input Mapping Context
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if(InventoryWidgetClass != nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("InventoryWidgetClass not set in blueprint."));
	}
	if(WeaponWidgetComponent)
	{
		WeaponUI = CreateWidget<UWeaponUI>(PlayerController, WeaponWidgetComponent);
		WeaponUI->SetOwner(this);
		check(WeaponUI);
		WeaponUI->AddToPlayerScreen();
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AImbadCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AImbadCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AImbadCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AImbadCharacter::Interact);

		// Toggle Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AImbadCharacter::ToggleInventory);
	}
}

// Equips a duplicate of the Weapon Component stored in ItemData
// to prevent accidentally overwriting weapon when equipping new item
void AImbadCharacter::EquipItem(UItemDataAsset* ItemData)
{
	if(UItemDataAsset* WeaponData = InventoryComponent->GetItemByName(ItemData->ItemName))
	{
		UTP_WeaponComponent* WeaponComponent = Cast<UTP_WeaponComponent>(WeaponData->ItemMeshComponent);
		if(WeaponComponent)
		{
			EquippedWeapon = DuplicateObject(WeaponComponent, this, "Equipped Weapon");
			EquippedWeapon->AttachWeapon(this);
			if(WeaponUI != nullptr)
				WeaponUI->UpdateAmmo();
			
			if(EquippedWeapon)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, WeaponData->ItemDescription);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Weapon still null"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Failed to Equip Item"));
		}
	}
}

float AImbadCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	if(Health <= 0)
	{
		// DIE
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
		Die();
	}
	return DamageAmount;
	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AImbadCharacter::TeleportToDungeonStart()
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const FTransform SpawnLocation = GameMode->FindPlayerStart(PlayerController, "Dungeon Start")->GetTransform();
	SetActorLocation(SpawnLocation.GetLocation());
	SetActorRotation(SpawnLocation.GetRotation());
	//SetActorTransform(SpawnLocation);
}

void AImbadCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AImbadCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AImbadCharacter::Interact()
{
	// Look for grab-able item within grab distance
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();

	// Setup Trace variables
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraForward * InteractDistance);
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner()); 

	// Perform Raycast
	FHitResult HitResult;
	
	// Check if we hit something
	if(GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel2, TraceParams)) 
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Well uhh we hit something"));
		IInteractInterface* Interface = Cast<IInteractInterface>(HitResult.GetActor()) ? Cast<IInteractInterface>(HitResult.GetActor()) : Cast<IInteractInterface>(HitResult.GetComponent());
		if(Interface)
		{
			Interface->Interact(*this);
		}
	}
}

void AImbadCharacter::ToggleInventory()
{
	// Toggle Inventory
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(InventoryWidget->IsInViewport())
	{
		// Closes Inventory
		UE_LOG(LogClass, Warning, TEXT("Inventory Closed"));
		InventoryWidget->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
	}
	else
	{
		// Opens Inventory
		UE_LOG(LogClass, Warning, TEXT("Inventory Opened"));
		InventoryWidget->AddToViewport();
		// Configure Inventory GameMode
		FInputModeGameAndUI InventoryMode;
		InventoryMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InventoryMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InventoryMode);
		PlayerController->SetShowMouseCursor(true);
	}
}




void AImbadCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AImbadCharacter::GetHasRifle()
{
	return bHasRifle;
}
