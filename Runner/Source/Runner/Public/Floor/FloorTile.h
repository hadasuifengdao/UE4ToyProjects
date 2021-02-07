// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

UCLASS()
class RUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Wall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Wall2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* AttachPoint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* EndTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* SpawnPointL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* SpawnPointR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* SpawnPointI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* SpawnPointH;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* CoinArea;

	UPROPERTY(EditAnywhere)
		TArray<FTransform> SpawnPoints;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABlocker> BlueprintVar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AStone> BlueprintVar1;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACoin> BlueprintVar2;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABlocker> BlueprintVar3;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABlocker> BlueprintVar4;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMagnet> BlueprintVar5;

	class ABlocker* block;

	class ABlocker* shelf;

	class AStone* stone;

	TArray<class ABlocker*> bridge;

	TArray<class ACoin*> coin;

	class AMagnet* magnet;

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//����Ƿ�������һ��·
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�������·����Destroy���·�ϵ����ж���
	void DestroyFloor();

	//��ȡBlocker�����λ��
	void SetSpawnPoints();

	//���Blocker
	void SpawnBlocker();

	//���Coin
	void SpawnCoins();

	//��Ӵ���
	void SpawnMagnet();

	//��ȡ���·��AttachPoint
	FTransform GetAttachTransform();
};
