// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunnerController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARunnerController : public APlayerController
{
	GENERATED_BODY()

public:

	//BeginPlay
	virtual void BeginPlay() override;

	//ShowMouseCursor
	UFUNCTION(BlueprintCallable, Category = "MouseCursor")
		void ShowMouseCursor();

	//GameOver	
	UFUNCTION(BlueprintCallable, Category = "GameOver")
		void GameOver();

	//GameStart
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStart")
		bool gameStart;

	//GameStart
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStart")
		bool canRun;

	//GameOver
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOver")
		bool gameEnd;
};
