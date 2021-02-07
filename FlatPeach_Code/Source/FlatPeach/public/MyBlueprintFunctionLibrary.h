// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

class AFlatPeachGameModeBase;

/**
 * 
 */
UCLASS()
class FLATPEACH_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void SetGameModeInstance(const AFlatPeachGameModeBase* pGameMode);

	static AFlatPeachGameModeBase* GetGameModeInstance();

private:
	static AFlatPeachGameModeBase* GameModePtr;
};
