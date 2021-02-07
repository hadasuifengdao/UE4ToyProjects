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
	//ͳ��ֱ�еذ������-��ӹ���
	UPROPERTY(EditAnywhere)
		int CornerIndex;

	//ͳ��ֱ�еذ������-��Ӳ�·
	UPROPERTY(EditAnywhere)
		int ForkIndex;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//��һ��ƴ�ӽ��
	UPROPERTY(EditAnywhere)
		FTransform NextSpawnPoint;

	//�ذ�����
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

	//���췽��
	ARunnerGameMode();

	//���ֱ�еذ�
	void AddFloorTile();

	//��ӹ���ذ�
	void AddFloorCorner();

	//��Ӳ�·
	void AddForkRoad();

	//�Ƴ���ǰ�˵��ؼ�����������ܣ���ָ�����д����¿ؼ�
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget>NewWidgetClass);

protected:

	//����Ϸ��ʼʱ���ǽ���Ϊ�˵�ʹ�õĿؼ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget>StartingWidgetClass;

	//����Ϊ�˵��Ŀؼ�ʵ��
	UPROPERTY()
		UUserWidget* CurrentWidget;
};