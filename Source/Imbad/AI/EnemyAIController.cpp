// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Door.h"
#include "RoomData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	
	// Configure Sight Perception
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Sight"));
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerception->ConfigureSense(*AISenseConfigSight);
	
	// Configure Hearing Perception
	AISenseConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Sense Hearing"));
	AISenseConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	AIPerception->ConfigureSense(*AISenseConfigHearing);
	AIPerception->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//GetAIPerceptionComponent()->
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run Behavior Tree
	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
	}

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated_Delegate);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0: // Sight
		// Seeks door if no target is found
		if(ADoor* Door = Cast<ADoor>(Actor))
		{
			if (!BlackboardComponent->GetValueAsObject("TargetActor"))
			{
				BlackboardComponent->SetValueAsVector("TargetLocation", Stimulus.StimulusLocation);
			}
		} // Sets TargetActor Key in Blackboard to Actor which triggered Stimulus
		else
		{
			BlackboardComponent->SetValueAsObject("TargetActor", Actor);
		}
	case 1: // Hearing
		// Sets TargetLocation Key in Blackboard to Source of Noise
		BlackboardComponent->SetValueAsVector("TargetLocation", Stimulus.StimulusLocation);
	default:
		return;
	}
}
