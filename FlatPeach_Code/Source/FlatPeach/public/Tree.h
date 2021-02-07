// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

UCLASS()
class FLATPEACH_API ATree : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
			class UStaticMeshComponent* TreeMeshCom;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* TreeCapsuleCom;
public:	
	// Sets default values for this actor's properties
	ATree();

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	/* Fire a shot in the specified direction */
	void FireShot();

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

private:
	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetTreeMesh() const { return TreeMeshCom; }

public:
	//ÑªÁ¿
	UPROPERTY(BlueprintReadWrite)
		int32	MaxTreeBlood;

	UPROPERTY(BlueprintReadWrite)
		int32	CurrentTreeBlood;

	//ÄÜÁ¿
	UPROPERTY(BlueprintReadWrite)
		int32 TreeEnergyNeed;

	//Åö×²
	UPROPERTY(BlueprintReadWrite)
		TArray<TEnumAsByte<EObjectTypeQuery>> AnimalsTypes;

	//damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	bool BHasEnemy();
	void Die();
	void ReleaseCube();
};
