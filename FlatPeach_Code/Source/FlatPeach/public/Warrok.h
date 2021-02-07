// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Warrok.generated.h"

UCLASS()
class FLATPEACH_API AWarrok : public ACharacter//: public AMyAI
{
	GENERATED_BODY()
	
private:
	FVector WarrokTarget;
	bool BCanDam;
	bool BIsDead;
	float lasttime;

public:
	//die
	UPROPERTY()
		FTimerHandle TimerHandle_Death;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageType> DamageType;

public:
	// Sets default values for this character's properties
	AWarrok();

	void InitBlood();

public:	
	virtual void BeginPlay() override;

	//血量条
	virtual void PostInitializeComponents() override;
	UPROPERTY()
		class UProgressBar *RobotBloodProgressBar;

	UPROPERTY(VisibleAnyWhere, BlueprintReadonly, Category = Mesh)
		class UWidgetComponent* RobotBloodWidget;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USkeletalMeshComponent* GetWarrokMesh() const { return WarrokMeshCom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class USkeletalMeshComponent* WarrokMeshCom;

	UFUNCTION()
		void WarrokOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	UPROPERTY(BlueprintReadWrite)
		int32 MaxWarrokBlood;		//血量属性

	UPROPERTY(BlueprintReadWrite)
		int32 CurrentWarrokBlood;		//当前血量属性

	UPROPERTY(BlueprintReadWrite)
		int32 WarrokAttackValue;		//攻击力

	UPROPERTY(BlueprintReadWrite)
		int32 WarrokAddEnergyValue;		//增加能量

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAnimInstance> BlueprintVar;

	//damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Die();

	void Des();

	void CanDam();
};
