// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"
#include "InventoryComponent.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"


void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}


void UInventoryGridWidget::Init(UInventoryComponent* inventoryComponent, float tileSize)
{
	this->InventoryComponent = inventoryComponent;
	this->TileSize = tileSize;
	if(GridBorder && TileSize)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder)->SetSize(
			FVector2d(InventoryComponent->Columns * TileSize, InventoryComponent->Rows * TileSize));
		CreateLineSegments();
	}
}

void UInventoryGridWidget::CreateLineSegments()
{
	
	// Create Vertical Lines
	for (int i = 0; i < InventoryComponent->Columns; i++)
	{
		float linePosX = i * TileSize;
		float linePosY = InventoryComponent->Rows * TileSize;
		Lines.Emplace(FLine {FVector2d(linePosX, 0), FVector2d(linePosX, linePosY)});
	}
	// Create Horizontal Lines
	for (int i = 0; i < InventoryComponent->Rows; i++)
	{
		float linePosX = InventoryComponent->Columns * TileSize;
		float linePosY = i * TileSize;
		Lines.Emplace(FLine {FVector2d(0, linePosY), FVector2d(linePosX, linePosY)});
	}
}


int32 UInventoryGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context { AllottedGeometry, MyCullingRect, OutDrawElements,
		LayerId, InWidgetStyle, bParentEnabled };
	for (FLine line : Lines)
	{
		FVector2d TopLeft = USlateBlueprintLibrary::GetLocalTopLeft(GridBorder->GetCachedGeometry());
		UWidgetBlueprintLibrary::DrawLine(Context, line.Start + TopLeft, line.End + TopLeft);
	}	
	return LayerId;
}

