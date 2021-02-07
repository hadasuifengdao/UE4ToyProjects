// Fill out your copyright notice in the Description page of Project Settings.

#include "Fork.h"
#include "Engine.h"
#include "LogTest.h"
#include "TimerManager.h"
#include "RunnerGameMode.h"
#include "RunnerCharacter.h"
#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"


// Sets default values
AFork::AFork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->AttachTo(RootComponent);

	FloorL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorL"));
	FloorL->AttachTo(RootComponent);

	FloorR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorR"));
	FloorR->AttachTo(RootComponent);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	Wall->AttachTo(RootComponent);

	Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall1"));
	Wall1->AttachTo(RootComponent);

	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Wall2->AttachTo(RootComponent);

	WallL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallL"));
	WallL->AttachTo(RootComponent);

	WallR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallR"));
	WallR->AttachTo(RootComponent);

	AttachPointL = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPointL"));
	AttachPointL->AttachTo(RootComponent);

	AttachPointR = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPointR"));
	AttachPointR->AttachTo(RootComponent);

	EndTriggerL = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTriggerL"));
	EndTriggerL->AttachTo(RootComponent);

	EndTriggerR = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTriggerR"));
	EndTriggerR->AttachTo(RootComponent);

	TurnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnArea"));
	TurnArea->AttachTo(RootComponent);

	EndTriggerL->OnComponentBeginOverlap.AddDynamic(this, &AFork::OnOverlapBeginL);

	EndTriggerR->OnComponentBeginOverlap.AddDynamic(this, &AFork::OnOverlapBeginR);
}

// Called when the game starts or when spawned
void AFork::BeginPlay()
{
	Super::BeginPlay();

	TurnArea->OnComponentBeginOverlap.AddDynamic(this, &AFork::OnOverlapBegin);
}

// Called every frame
void AFork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFork::OnOverlapBeginL(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunnerCharacter* Runner = Cast<ARunnerCharacter>(OtherActor);
	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
	if (Runner != NULL)
	{
		ARunnerGameMode* gameMode = Cast<ARunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		gameMode->NextSpawnPoint = AttachPointL->GetComponentTransform();

		for (int i = 0;i < 10;i++)
			gameMode->AddFloorTile();
	}
	if(Monster != NULL)
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AFork::DestroyFork, 2.0f, false);
}

void AFork::OnOverlapBeginR(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunnerCharacter* Runner = Cast<ARunnerCharacter>(OtherActor);
	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
	if (Runner != NULL)
	{
		ARunnerGameMode* gameMode = Cast<ARunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		gameMode->NextSpawnPoint = AttachPointR->GetComponentTransform();

		for (int i = 0;i < 10;i++)
			gameMode->AddFloorTile();
	}
	if (Monster != NULL)
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AFork::DestroyFork, 2.0f, false);
}

void AFork::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunnerCharacter* Runner = Cast<ARunnerCharacter>(OtherActor);
	if (Runner != NULL)
		Runner->CanTurn = true;
}

void AFork::DestroyFork()
{
	Destroy();
}