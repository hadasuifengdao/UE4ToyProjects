// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Corner.generated.h"

UCLASS()
class RUNNER_API ACorner : public AActor
{
	GENERATED_BODY()

public:
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
		class UBoxComponent* EndTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Runner)
		class UBoxComponent* TurnArea;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		class UParticleSystem* BoomEmitter;

	FTimerHandle TimerHandle;
	
public:	
	// Sets default values for this actor's properties
	ACorner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//检测是否走完这一段路
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//走完这段路，将Destroy这段路上的所有东西
	void DestroyCorner();

	//获取这段路的AttachPoint
	FTransform GetAttachTransform();
};
