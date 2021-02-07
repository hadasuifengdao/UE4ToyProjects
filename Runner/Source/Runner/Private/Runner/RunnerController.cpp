// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerController.h"

//BeginPlay
void ARunnerController::BeginPlay()
{
	Super::BeginPlay();
	//设置鼠标可动
	bShowMouseCursor = true;

	//设置gameStart
	gameStart = false;

	//设置gameOver
	gameEnd = false;

	//设置canRun
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


