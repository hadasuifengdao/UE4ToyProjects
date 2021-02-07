// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunnerGameMode.generated.h"

UCLASS(minimalapi)
class ARunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//统计直行地板的数量-添加拐弯
	UPROPERTY(EditAnywhere)
		int CornerIndex;

	//统计直行地板的数量-添加岔路
	UPROPERTY(EditAnywhere)
		int ForkIndex;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//下一个拼接结点
	UPROPERTY(EditAnywhere)
		FTransform NextSpawnPoint;

	//地板种类
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AFloorTile> BlueprintVar1;
		TSubclassOf<class AFloorTile> BlueprintVar2;
		TSubclassOf<class AFloorTile> BlueprintVar3;
		TSubclassOf<class AFloorTile> BlueprintVar4;
		TSubclassOf<class AFloorTile> BlueprintVar5;

		TSubclassOf<class ACorner> BlueprintVar6;
		TSubclassOf<class ACorner> BlueprintVar7;

		TSubclassOf<class AFork> BlueprintVar8;
		//TSubclassOf<class ASkid> BlueprintVar9;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AFloorTile> BlueprintVar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACorner> BlueprintVar_Corner;

public:

	//Tick
	//virtual void Tick(float DeltaTime) override;

	//构造方法
	ARunnerGameMode();

	//添加直行地板
	void AddFloorTile();

	//添加拐弯地板
	void AddFloorCorner();

	//添加岔路
	void AddForkRoad();

	//移除当前菜单控件并且如果可能，从指定类中创建新控件
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget>NewWidgetClass);

protected:

	//在游戏开始时我们将作为菜单使用的控件类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget>StartingWidgetClass;

	//用作为菜单的控件实例
	UPROPERTY()
		UUserWidget* CurrentWidget;
};