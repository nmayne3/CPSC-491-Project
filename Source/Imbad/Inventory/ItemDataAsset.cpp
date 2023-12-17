// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset.h"

UItemDataAsset::UItemDataAsset()
{
}

UItemDataAsset::UItemDataAsset(FName Name, UMeshComponent* MeshComponent, FString Description,
                               uint8 Stack)
{
	ItemName = Name;
	ItemMeshComponent = MeshComponent;
	ItemDescription = Description;
	MaxStack = Stack;
}
