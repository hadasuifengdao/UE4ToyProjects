// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skid.generated.h"

UCLASS()
class RUNNER_API ASkid : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASkid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Coin)
		class UPointLightComponent* PointLight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
