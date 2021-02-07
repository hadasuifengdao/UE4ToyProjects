// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Robot.generated.h"

UCLASS()
class FLATPEACH_API ARobot : public ACharacter
{
	GENERATED_BODY()

public:
	
	ARobot();
	void InitBlood();

protected:
	bool BIsDead;
	bool BCanDamage;
	FVector TargetLocation;
	float lasttime;

public:	
	virtual void BeginPlay() override;

	//Ѫ����
	virtual void PostInitializeComponents() override;
	UPROPERTY()
		class UProgressBar *RobotBloodProgressBar;

	UPROPERTY(VisibleAnyWhere, BlueprintReadonly, Category = Mesh)
		class UWidgetComponent* RobotBloodWidget;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadonly, Category = Mesh)
		class USkeletalMeshComponent *RobotMeshCom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadonly, Category = Mesh)
		class UAnimMontage *RobotDie;
		
	UPROPERTY(VisibleAnyWhere, BlueprintReadonly, Category = Mesh)
		class UAnimMontage *RobotAttack;

	UFUNCTION()
		void RobotOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//die
	UPROPERTY()
		FTimerHandle TimerHandle_BeginDeath;

public:

	UPROPERTY(BlueprintReadWrite)
		int32 MaxRobotBlood;		//Ѫ������

	UPROPERTY(BlueprintReadWrite)
		int32 CurrentRobotBlood;		//��ǰѪ������

	UPROPERTY(BlueprintReadWrite)
		int32 RobotAttackValue;		//������

	UPROPERTY(BlueprintReadWrite)
		int32 RobotAddEnergyValue;		//��������

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAnimInstance> BlueprintVar;

	//damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Die();

	void Des();

	void CanDam();

};
