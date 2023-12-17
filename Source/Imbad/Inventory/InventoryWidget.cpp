// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryGridWidget.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//FTimerHandle TimerHandle;
	InventoryGridWidget->Init(InventoryComponent, TileSize);

}
