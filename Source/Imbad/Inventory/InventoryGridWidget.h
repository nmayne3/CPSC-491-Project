// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridWidget.generated.h"

class UInventoryComponent;
class UBorder;
class UCanvasPanel;
struct FLine;
/**
 * 
 */
UCLASS()
class IMBAD_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Init(UInventoryComponent* inventoryComponent, float tileSize);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GridCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UBorder> GridBorder;

	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TileSize;

	UFUNCTION()
	void CreateLineSegments();

	TArray<FLine> Lines;


	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	
};

struct FLine
{
	FVector2d Start;
	FVector2d End;
};

struct FTile
{
	int32 X;
	int32 Y;
};