// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cluster.generated.h"

UCLASS()
class FLATPEACH_API ACluster : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			class UStaticMeshComponent* ClusterMeshCom;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent * ClusterCapsuleCom;

private:
	bool BIsOverlaping;
	AActor* OtherActor1;
	FVector OtherActor1Loc;
	FHitResult SweepResult1;
	float BetweenDam;
	float DamTime;

public:	
	// Sets default values for this actor's properties
	ACluster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Overlaping(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void EventEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE class UStaticMeshComponent* GetClusterMesh() const { return ClusterMeshCom; }

public:
	
	//ÄÜÁ¿
	UPROPERTY(BlueprintReadWrite)
		int32 ClusterEnergyNeed;

	//¹¥»÷
	UPROPERTY(BlueprintReadWrite)
		int32 ClusterAttack;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageType> DamageType;
};
