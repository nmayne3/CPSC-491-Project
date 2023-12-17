// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class IMBAD_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UItemDataAsset();
	UItemDataAsset(FName Name, UMeshComponent* MeshComponent = nullptr, FString Description = TEXT(""),
	               uint8 Stack = 1);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 CurrentStack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshComponent* ItemMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ItemMesh;

	UFUNCTION()
	bool IsFull() { return CurrentStack >= MaxStack; }
};
