// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DefaultAttack.h"
#include "AIController.h"
#include "Imbad/Characters/Enemy.h"

UBTT_DefaultAttack::UBTT_DefaultAttack()
{
	NodeName = TEXT("Default Attack");	
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Get AI Pawn
	const AAIController* AIController {OwnerComp.GetAIOwner()};
	APawn* AIPawn {AIController->GetPawn()};

	// Cast Pawn to Enemy
	AEnemy* Enemy;
	Enemy = Cast<AEnemy>(AIPawn);

	Enemy->Attack();

	// Signal to BehaviorTreeComponent Task Success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTT_DefaultAttack::GetStaticDescription() const
{
	return FString::Printf(TEXT("Smack that bitch"));
}
