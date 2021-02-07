// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AMonsterController : public AAIController
{
	GENERATED_BODY()

	class UBehaviorTreeComponent* BehaviorComp;

	class UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName PlayerKey;

	virtual void Possess(APawn* Pawn) override;

public:
	AMonsterController();

	void SetPlayerCaught(APawn* Pawn);

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }

};
