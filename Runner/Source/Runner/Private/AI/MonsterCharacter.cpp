// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterCharacter.h"
#include "Engine.h"
#include "RunnerCharacter.h"
#include "RunnerController.h"
#include "MonsterController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->MaxWalkSpeed = 1540.f;
	GetCharacterMovement()->AirControl = 0.2f;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.0f);
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComp)
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonsterCharacter::OnPlayerCaught);
}

void AMonsterCharacter::OnPlayerCaught(APawn *pawn)
{
	ARunnerController* runnerController = Cast<ARunnerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (runnerController->gameStart == true)
	{
		AMonsterController* AIController = Cast<AMonsterController>(GetController());
		if (AIController)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You have beenCaught!"));
			ARunnerCharacter* Runner = Cast<ARunnerCharacter>(pawn);
			if(Runner != NULL)
				Runner->HasSeen = true;
			if (Runner->HasJump == true)
				Jump();
			AIController->SetPlayerCaught(pawn);
		}
	}
}
