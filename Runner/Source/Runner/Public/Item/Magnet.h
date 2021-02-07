// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magnet.generated.h"

UCLASS()
class RUNNER_API AMagnet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagnet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnet)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnet)
		class UStaticMeshComponent* Magnet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magnet)
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Coin)
		class URotatingMovementComponent* RotationMovement;

	UPROPERTY(EditAnywhere)
		class ARunnerCharacter* Runner;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARunnerCharacter> RunnerClass;

	TArray<AActor*> RunnerArray;

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MagnetDestroy();
};
