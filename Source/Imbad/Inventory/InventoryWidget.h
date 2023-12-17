// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UInventoryGridWidget;
class UBackgroundBlur;
class UBorder;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class IMBAD_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta= (BindWidget));
	TObjectPtr<UInventoryGridWidget> InventoryGridWidget;


	virtual void NativeConstruct() override;
};
