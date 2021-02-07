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
	//	bool Num; //Ҫ��Ҫ�ӻ�����
	UPROPERTY(BluePrintReadWrite)
		 int32 Number1; //������

public:

	UPROPERTY(BluePrintReadWrite)
		int32 Mode = 0;

	/*����Ϸ��ʼʱ����*/
	virtual void BeginPlay() override;

	/*�Ƴ���ǰ�˵��ؼ�����������ܣ���ָ�����д����¿ؼ�*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	/*����Ϸ��ʼʱ���ǽ���Ϊ�˵�ʹ�õĿؼ���*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/*����Ϊ�˵��Ŀؼ�ʵ��*/
	UPROPERTY()
		UUserWidget* CurrentWidget;
	
	
};
