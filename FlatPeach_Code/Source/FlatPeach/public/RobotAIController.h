// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RobotAIController.generated.h"

/**
 * 
 */
UCLASS()
class FLATPEACH_API ARobotAIController : public AAIController
{
	GENERATED_BODY()

	//class UBehaviorTreeComponent* RobotBehaviorCom;
	//class UBlackboardComponent* RobotBlackCom;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//	FName Target;

	//virtual void Possess(APawn* Pawn) override;

public:
	ARobotAIController();
	//void MOVE(FVector);
	//FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return RobotBlackCom; }
};
