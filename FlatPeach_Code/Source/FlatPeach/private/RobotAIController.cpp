// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotAIController.h"
#include "Robot.h"
#include "Engine.h"
//#include "BehaviorTree/BehaviorTree.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"

//void ARobotAIController::Possess(APawn * Pawn)
//{
//	Super::Possess(Pawn);
//
//	ARobot* Robot = Cast<ARobot>(Pawn);
//
//	if (Robot)
//	{
//		if (Robot->BehaviorTree->BlackboardAsset)
//		{
//			RobotBlackCom->InitializeBlackboard(*(Robot->BehaviorTree->BlackboardAsset));
//		}
//
//		RobotBehaviorCom->StartTree(*Robot->BehaviorTree);
//	}
//}

ARobotAIController::ARobotAIController()
{
	//RobotBehaviorCom = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("RobotBehaviorCom"));
	//RobotBlackCom = CreateDefaultSubobject<UBlackboardComponent>(TEXT("RobotBlackboardCom"));
	//Target = "Target";
}
//
//void ARobotAIController::SetBlackboard(FVector pos)
//{
//	//RobotBlackCom->SetValueAsVector(Target, pos);
//	
//}

