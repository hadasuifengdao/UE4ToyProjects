 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackLeaves.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class FLATPEACH_API AAttackLeaves : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* LeavesCapsuleCom;

public:	
	// Sets default values for this actor's properties
	AAttackLeaves();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
		void LeavesOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:

	UPROPERTY(BlueprintReadWrite)
		int32 LeavesAttack;		//��Ҷ������

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageType> DamageType;
};
