// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerController.h"

//BeginPlay
void ARunnerController::BeginPlay()
{
	Super::BeginPlay();
	//�������ɶ�
	bShowMouseCursor = true;

	//����gameStart
	gameStart = false;

	//����gameOver
	gameEnd = false;

	//����canRun
	canRun = false;
}

//ShowMouseCursor
void ARunnerController::ShowMouseCursor()
{
	bShowMouseCursor = false;
	gameStart = true;
}

//GameMover
void ARunnerController::GameOver()
{
	bShowMouseCursor = true;
	gameEnd = true;
}


