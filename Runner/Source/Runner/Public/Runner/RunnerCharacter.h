// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UCLASS(config=Game)
class ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Rotation
	UPROPERTY(EditAnywhere, Category = Runner)
		FRotator DesiredRotation;

	//CanTurn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		bool CanTurn;

	//BIsDie
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		bool BIsDie;

	//CanTurn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		bool HasJump;

	//grade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		int TotalGrade;

	//TotalCoins
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		int TotalCoins;

	//Distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Runner)
		int TotalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
		bool UseMagnet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
		bool HasSeen;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		class UParticleSystem* BoomEmitter;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
		class USoundBase* BoomSound;

public:
	ARunnerCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Tick
	virtual void Tick(float DeltaTime) override;

	//Jump
	void MoveJump();

	//StopJump
	void StopJump();

	//Move
	void MoveRight(float Value);

	//Crouch
	void MoveCrouch();

	//EndCrouch
	void EndCrouch();

	//TurnLeft
	void TurnLeft();

	//TurnRight
	void TurnRight();

	//TurnCorner
	void TurnCorner(float DeltaTime);

	//AddCoins
	void AddCoins();

	//Death
	UFUNCTION(BlueprintCallable, Category = Runner)
		void Death();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

