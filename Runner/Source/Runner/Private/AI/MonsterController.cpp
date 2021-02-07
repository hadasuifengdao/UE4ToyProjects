// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterController.h"
#include "Engine.h"
#include "LogTest.h"
#include "RunnerCharacter.h"
#include "MonsterCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

void AMonsterController::Possess(APawn *Pawn)
{
	Super::Possess(Pawn);

	AMonsterCharacter* AICharacter = Cast<AMonsterCharacter>(Pawn);

	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
		}

		BehaviorComp->StartTree(*AICharacter->BehaviorTree);
	}
}

AMonsterController::AMonsterController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	PlayerKey = "Target";
}

void AMonsterController::SetPlayerCaught(APawn *Pawn)
{
	//MoveToLocation(Pawn->GetActorLocation());
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, Pawn);
	}
}