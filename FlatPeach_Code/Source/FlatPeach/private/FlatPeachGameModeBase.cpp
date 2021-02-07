// Fill out your copyright notice in the Description page of Project Settings.

#include "FlatPeachGameModeBase.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MyBlueprintFunctionLibrary.h"

AFlatPeachGameModeBase::AFlatPeachGameModeBase() 
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	Number1 = 0;
}

void AFlatPeachGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UMyBlueprintFunctionLibrary::SetGameModeInstance(this);
	ChangeMenuWidget(StartingWidgetClass);
}

//void AFlatPeachGameModeBase::Tick(float DeltaTime)
//{
//	if (Number1>30)	//Ê¤Àû
//	{
//
//	}
//}

void AFlatPeachGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

}
