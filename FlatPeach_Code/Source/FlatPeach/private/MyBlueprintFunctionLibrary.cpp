// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "FlatPeach.h"
#include "FlatPeachGameModeBase.h"

AFlatPeachGameModeBase* UMyBlueprintFunctionLibrary::GameModePtr = NULL;

AFlatPeachGameModeBase * UMyBlueprintFunctionLibrary::GetGameModeInstance()
{
	return GameModePtr;
}

void UMyBlueprintFunctionLibrary::SetGameModeInstance(const AFlatPeachGameModeBase* pGameMode)
{
	GameModePtr = (AFlatPeachGameModeBase*)pGameMode;
}