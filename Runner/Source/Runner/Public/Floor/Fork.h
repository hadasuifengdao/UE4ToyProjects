// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fork.generated.h"

UCLASS()
class RUNNER_API AFork : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFork();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* FloorL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* FloorR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Wall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Wall1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Wall2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* WallL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* WallR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* AttachPointL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UArrowComponent* AttachPointR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* EndTriggerL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* EndTriggerR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* TurnArea;

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBeginL(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapBeginR(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DestroyFork();
};
