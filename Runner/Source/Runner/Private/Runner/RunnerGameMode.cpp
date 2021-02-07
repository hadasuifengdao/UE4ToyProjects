// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RunnerGameMode.h"
#include "Fork.h"
#include "Engine.h"
#include "Corner.h"
#include "LogTest.h"
#include "FloorTile.h"
#include "RunnerCharacter.h"
#include "RunnerController.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ARunnerGameMode::ARunnerGameMode()
{
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile1(TEXT("Blueprint'/Game/Item/BP_FloorTile.BP_FloorTile'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile2(TEXT("Blueprint'/Game/Item/BP_FloorTile_Jump.BP_FloorTile_Jump'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile3(TEXT("Blueprint'/Game/Item/BP_FloorTile_RampDown.BP_FloorTile_RampDown'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile4(TEXT("Blueprint'/Game/Item/BP_FloorTile_RampUp.BP_FloorTile_RampUp'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile5(TEXT("Blueprint'/Game/Item/BP_FloorTile_Half.BP_FloorTile_Half'"));

	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile6(TEXT("Blueprint'/Game/Item/BP_CornerL.BP_CornerL'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>floorTile7(TEXT("Blueprint'/Game/Item/BP_CornerR.BP_CornerR'"));

	//static ConstructorHelpers::FObjectFinder<UBlueprint>forkRoad(TEXT("Blueprint'/Game/Item/BP_Fork.BP_Fork'"));
	////static ConstructorHelpers::FObjectFinder<UBlueprint>skid(TEXT("Blueprint'/Game/Item/BP_Skid.BP_Skid'"));

	//BlueprintVar1 = (UClass*)floorTile1.Object->GeneratedClass;
	//BlueprintVar2 = (UClass*)floorTile2.Object->GeneratedClass;
	//BlueprintVar3 = (UClass*)floorTile3.Object->GeneratedClass;
	//BlueprintVar4 = (UClass*)floorTile4.Object->GeneratedClass;
	//BlueprintVar5 = (UClass*)floorTile5.Object->GeneratedClass;
	//BlueprintVar6 = (UClass*)floorTile6.Object->GeneratedClass;
	//BlueprintVar7 = (UClass*)floorTile7.Object->GeneratedClass;
	//BlueprintVar8 = (UClass*)forkRoad.Object->GeneratedClass;
	////BlueprintVar9 = (UClass*)skid.Object->GeneratedClass;

	static ConstructorHelpers::FClassFinder<AFloorTile>floorTile1(TEXT("Blueprint'/Game/Item/BP_FloorTile.BP_FloorTile_C'"));
	static ConstructorHelpers::FClassFinder<AFloorTile>floorTile2(TEXT("Blueprint'/Game/Item/BP_FloorTile_Jump.BP_FloorTile_Jump_C'"));
	static ConstructorHelpers::FClassFinder<AFloorTile>floorTile3(TEXT("Blueprint'/Game/Item/BP_FloorTile_RampDown.BP_FloorTile_RampDown_C'"));
	static ConstructorHelpers::FClassFinder<AFloorTile>floorTile4(TEXT("Blueprint'/Game/Item/BP_FloorTile_RampUp.BP_FloorTile_RampUp_C'"));
	static ConstructorHelpers::FClassFinder<AFloorTile>floorTile5(TEXT("Blueprint'/Game/Item/BP_FloorTile_Half.BP_FloorTile_Half_C'"));

	static ConstructorHelpers::FClassFinder<ACorner>floorTile6(TEXT("Blueprint'/Game/Item/BP_CornerL.BP_CornerL_C'"));
	static ConstructorHelpers::FClassFinder<ACorner>floorTile7(TEXT("Blueprint'/Game/Item/BP_CornerR.BP_CornerR_C'"));

	static ConstructorHelpers::FClassFinder<AFork>forkRoad(TEXT("Blueprint'/Game/Item/BP_Fork.BP_Fork_C'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint>skid(TEXT("Blueprint'/Game/Item/BP_Skid.BP_Skid'"));

	BlueprintVar1 = floorTile1.Class;
	BlueprintVar2 = floorTile2.Class;
	BlueprintVar3 = floorTile3.Class;
	BlueprintVar4 = floorTile4.Class;
	BlueprintVar5 = floorTile5.Class;
	BlueprintVar6 = floorTile6.Class;
	BlueprintVar7 = floorTile7.Class;
	BlueprintVar8 = forkRoad.Class;
	//BlueprintVar9 = (UClass*)skid.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void ARunnerGameMode::BeginPlay()
{
	Super::BeginPlay();

	//加载游戏菜单
	ChangeMenuWidget(StartingWidgetClass);

	//添加地板
	for (int i = 0;i < 10;i++)
		AddFloorTile();
}


void ARunnerGameMode::AddFloorTile()
{
	ForkIndex += 1;

	if (ForkIndex > 20)
		AddForkRoad();

	else if (CornerIndex > 9)
		AddFloorCorner();

	else
	{
		int index = FMath::RandHelper(5);
		if (index == 0)
			BlueprintVar = BlueprintVar1;
		else if (index == 1)
			BlueprintVar = BlueprintVar2;
		else if (index == 2)
			BlueprintVar = BlueprintVar3;
		else if (index == 3)
			BlueprintVar = BlueprintVar4;
		else if (index == 4)
			BlueprintVar = BlueprintVar5;

		UWorld* const World = GetWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		AFloorTile* floor = World->SpawnActor<AFloorTile>(BlueprintVar, NextSpawnPoint, SpawnParams);
		NextSpawnPoint = floor->GetAttachTransform();

		CornerIndex += 1;
	}
}

void ARunnerGameMode::AddFloorCorner()
{
	int index = FMath::RandHelper(2);

	if (index == 0)
		BlueprintVar_Corner = BlueprintVar6;
	else if (index == 1)
		BlueprintVar_Corner = BlueprintVar7;

	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	ACorner* corner = World->SpawnActor<ACorner>(BlueprintVar_Corner, NextSpawnPoint, SpawnParams);
	NextSpawnPoint = corner->GetAttachTransform();

	CornerIndex = 0;
}

void ARunnerGameMode::AddForkRoad()
{
	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	AFork* fork = World->SpawnActor<AFork>(BlueprintVar8, NextSpawnPoint, SpawnParams);

	ForkIndex = 0;
}

void ARunnerGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget>NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
