// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TP_WeaponComponent.h"
#include "Inventory/InventoryComponent.h"
#include "ImbadCharacter.generated.h"

class UWeaponUI;
class UWidgetComponent;
class UUserWidgetBlueprint;
class UInventoryWidget;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AImbadCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Face;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Grab Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* GrabAction;

	/** Inventory Toggle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;
	
public:
	AImbadCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float CameraBobOffset;

	bool bSteppingUp;
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDistance = 2.0f;

	UFUNCTION(BlueprintCallable, Category = Dungeon)
	void TeleportToDungeonStart();
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for grabbing input */
	void Interact();

	/** Toggle Inventory */
	void ToggleInventory();

	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P sub-object **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UStaticMeshComponent* GetFace() const { return Face; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTP_WeaponComponent* EquippedWeapon;


	/** Returns FirstPersonCameraComponent sub-object **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/*
	 * UI
	 */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<UWeaponUI> WeaponWidgetComponent;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	
	UPROPERTY(EditAnywhere, Category=Weapon)
	UWeaponUI* WeaponUI;
	
    /*
     *	Inventory
     */
	// Gets class from blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	// Instance of Widget used in Code
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UFUNCTION()
	void EquipItem(UItemDataAsset* ItemData);

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Die();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Character)
	float Health;
};

