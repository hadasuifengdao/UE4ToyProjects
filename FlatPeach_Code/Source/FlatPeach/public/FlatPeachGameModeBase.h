// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlatPeachGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FLATPEACH_API AFlatPeachGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFlatPeachGameModeBase();

	//UPROPERTY(BluePrintReadWrite)
	//	bool Num; //要不要加击败数
	UPROPERTY(BluePrintReadWrite)
		 int32 Number1; //击败数

public:

	UPROPERTY(BluePrintReadWrite)
		int32 Mode = 0;

	/*在游戏开始时调用*/
	virtual void BeginPlay() override;

	/*移除当前菜单控件并且如果可能，从指定类中创建新控件*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	/*在游戏开始时我们将作为菜单使用的控件类*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/*用作为菜单的控件实例*/
	UPROPERTY()
		UUserWidget* CurrentWidget;
	
	
};
