// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Eagle.generated.h"

UCLASS()
class FLATPEACH_API AEagle : public ACharacter
{
	GENERATED_BODY()

private:
	FVector EagleTargetLoc;
	bool BIsDead;
	bool BCanDam;
	float lasttime;

public:
	//die
	UPROPERTY()
		FTimerHandle TimerHandle_Death;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageType> DamageType;

public:
	// Sets default values for this character's properties
	AEagle();
	void InitBlood();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USkeletalMeshComponent* GetEagleMesh() const { return EagleMeshCom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class USkeletalMeshComponent* EagleMeshCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class UWidgetComponent* EagleBloodWidget;

	UFUNCTION()
		void EagleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void BeginPlay() override;

	//血量条
	virtual void PostInitializeComponents() override;
	UPROPERTY()
		class UProgressBar *HPBarProgress;

	UPROPERTY(BlueprintReadWrite)
		int32 MaxEagleBlood;		//血量属性

	UPROPERTY(BlueprintReadWrite)
		int32 CurrentEagleBlood;		//当前血量属性

	UPROPERTY(BlueprintReadWrite)
		int32 EagleAttackValue;		//攻击力

	UPROPERTY(BlueprintReadWrite)
		int32 EagleAddEnergyValue;		//增加能量

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAnimInstance> EagleFlyBlueprint;

	//damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Die();

	void Des();

	void CanDam();

};
