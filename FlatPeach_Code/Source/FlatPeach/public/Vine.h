// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vine.generated.h"

UCLASS()
class FLATPEACH_API AVine : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* VineMeshCom;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* VineCapsuleCom;
	
public:	
	// Sets default values for this actor's properties
	AVine();

private:
	FTimerHandle TimerHandle_ShotTimerExpired;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetVineMesh() const { return VineMeshCom; }

public:

	//ÑªÁ¿
	UPROPERTY(BlueprintReadWrite)
		int32	MaxVineBlood;

	UPROPERTY(BlueprintReadWrite)
		int32	CurrentVineBlood;

	//ÄÜÁ¿
	UPROPERTY(BlueprintReadWrite)
		int32 VineEnergyNeed;

	//damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Die();

	void ReleaseCube();
};
