// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorTile.h"

#include "Coin.h"
#include "Stone.h"
#include "Magnet.h"
#include "Blocker.h"

#include "Engine.h"
#include "LogTest.h"
#include "TimerManager.h"

#include "RunnerGameMode.h"
#include "RunnerCharacter.h"
#include "MonsterCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->AttachTo(RootComponent);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	Wall->AttachTo(RootComponent);

	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Wall2->AttachTo(RootComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->AttachTo(RootComponent);

	AttachPoint1 = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint1"));
	AttachPoint1->AttachTo(RootComponent);

	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTrigger"));
	EndTrigger->AttachTo(RootComponent);
	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnOverlapBegin);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->AttachTo(RootComponent);

	SpawnPointL = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointL"));
	SpawnPointL->AttachTo(RootComponent);

	SpawnPointR = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointR"));
	SpawnPointR->AttachTo(RootComponent);

	SpawnPointI = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointI"));
	SpawnPointI->AttachTo(RootComponent);

	SpawnPointH = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointH"));
	SpawnPointH->AttachTo(RootComponent);

	CoinArea = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinArea"));
	CoinArea->AttachTo(RootComponent);
	//
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker(TEXT("/Game/Item/BP_Blocker.BP_Blocker"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker1(TEXT("/Game/Item/BP_Stone.BP_Stone"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker2(TEXT("/Game/Item/BP_Coin.BP_Coin"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker3(TEXT("/Game/Item/BP_Bridge.BP_Bridge"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker4(TEXT("/Game/Item/BP_Self.BP_Self"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>blocker5(TEXT("/Game/Item/BP_Magnet.BP_Magnet"));
	//
	//BlueprintVar = (UClass*)blocker.Object->GeneratedClass;
	//BlueprintVar1 = (UClass*)blocker1.Object->GeneratedClass;
	//BlueprintVar2 = (UClass*)blocker2.Object->GeneratedClass;
	//BlueprintVar3 = (UClass*)blocker3.Object->GeneratedClass;
	//BlueprintVar4 = (UClass*)blocker4.Object->GeneratedClass;
	//BlueprintVar5 = (UClass*)blocker5.Object->GeneratedClass;

	static ConstructorHelpers::FClassFinder<ABlocker>blocker(TEXT("Blueprint'/Game/Item/BP_Blocker.BP_Blocker_C'"));
	static ConstructorHelpers::FClassFinder<AStone>blocker1(TEXT("Blueprint'/Game/Item/BP_Stone.BP_Stone_C'"));
	static ConstructorHelpers::FClassFinder<ACoin>blocker2(TEXT("Blueprint'/Game/Item/BP_Coin.BP_Coin_C'"));
	static ConstructorHelpers::FClassFinder<ABlocker>blocker3(TEXT("Blueprint'/Game/Item/BP_Bridge.BP_Bridge_C'"));
	static ConstructorHelpers::FClassFinder<ABlocker>blocker4(TEXT("Blueprint'/Game/Item/BP_Self.BP_Self_C'"));
	static ConstructorHelpers::FClassFinder<AMagnet>blocker5(TEXT("Blueprint'/Game/Item/BP_Magnet.BP_Magnet_C'"));
	
	BlueprintVar = blocker.Class;
	BlueprintVar1 = blocker1.Class;
	BlueprintVar2 = blocker2.Class;
	BlueprintVar3 = blocker3.Class;
	BlueprintVar4 = blocker4.Class;
	BlueprintVar5 = blocker5.Class;

	coin.Empty();
	bridge.Empty();
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	SetSpawnPoints();

	int num = FMath::RandHelper(7);

	if (num == 0 || num == 1 || num == 2)
		SpawnBlocker();

	if (num == 3 || num == 4 || num == 5)
		SpawnCoins();

	if (num == 6)
		SpawnMagnet();
}

// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorTile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool fork = false;
	ARunnerCharacter* Runner = Cast<ARunnerCharacter>(OtherActor);
	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
	if (Runner != NULL)
	{
		ARunnerGameMode* gameMode = Cast<ARunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (gameMode->ForkIndex != 0)
			gameMode->AddFloorTile();
	}
	if (Monster != NULL)
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AFloorTile::DestroyFloor, 2.0f, true);
}

void AFloorTile::DestroyFloor()
{
	int num = 0;
	if(stone != NULL)
		stone->Destroy();

	if(block != NULL)
		block->Destroy();

	if (coin.Num() != 0)
	{
		for (int i = 0; i < coin.Num(); i++)
		{
			coin[i]->Destroy();
		}
	}

	if (bridge.Num() != 0)
	{
		for (int i = 0;i < 2;i++)
			bridge[i]->Destroy();
		bridge.Empty();
	}

	if (shelf != NULL)
		shelf->Destroy();

	if (magnet != NULL)
		magnet->Destroy();

	this->Destroy();
}

//SetSpawnPoints
void AFloorTile::SetSpawnPoints()
{
	SpawnPoints.Add(SpawnPointR->GetComponentTransform());
	SpawnPoints.Add(SpawnPointL->GetComponentTransform());
	SpawnPoints.Add(SpawnPoint->GetComponentTransform());
}

//SpawnBlocker
void AFloorTile::SpawnBlocker()
{
	int index = FMath::RandHelper(SpawnPoints.Num());
	int num = FMath::RandHelper(4);
	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if(num == 0)
		block = World->SpawnActor<ABlocker>(BlueprintVar, SpawnPoints[index], SpawnParams);

	if(num == 1)
		stone = World->SpawnActor<AStone>(BlueprintVar1, SpawnPointI->GetComponentTransform(), SpawnParams);

	if (num == 2)
	{
		bridge.Add(World->SpawnActor<ABlocker>(BlueprintVar3, SpawnPoints[0], SpawnParams));
		bridge.Add(World->SpawnActor<ABlocker>(BlueprintVar3, SpawnPoints[1], SpawnParams));
	}

	if(num == 3)
		shelf = World->SpawnActor<ABlocker>(BlueprintVar4, SpawnPoints[2], SpawnParams);
}

//SpawnCoins
void AFloorTile::SpawnCoins()
{
	for (int i = 0;i < 9;i++)
	{
		//UKismetMathLibrary::RandomPointInBoundingBox(CoinArea->GetComponentTransform().GetLocation(), CoinArea->GetCollisionShape().GetExtent());CoinArea->GetCollisionShape().GetExtent())
		FTransform transform = (FTransform)(UKismetMathLibrary::RandomPointInBoundingBox(CoinArea->Bounds.Origin,CoinArea->Bounds.BoxExtent));

		UWorld* const World = GetWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		coin.Add(World->SpawnActor<ACoin>(BlueprintVar2, transform, SpawnParams));
	}
}

//SpawnMagnet
void AFloorTile::SpawnMagnet()
{
	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	magnet = World->SpawnActor<AMagnet>(BlueprintVar5, SpawnPointH->GetComponentTransform(), SpawnParams);
}

//GetAttachTransform
FTransform AFloorTile::GetAttachTransform()
{
	FTransform attachPoint = AttachPoint->GetComponentTransform();
	return attachPoint;
}