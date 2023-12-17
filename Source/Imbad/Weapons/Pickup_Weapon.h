// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Imbad/TP_PickUpComponent.h"
#include "ImBad/Interfaces/InteractInterface.h"
#include "Pickup_Weapon.generated.h"

UCLASS()
class IMBAD_API APickup_Weapon : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup_Weapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTP_PickUpComponent> TP_PickUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMeshComponent> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TArray<UItemDataAsset*> ItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UStaticMeshComponent> Mesh;

	virtual void Interact(APawn& Caller) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
