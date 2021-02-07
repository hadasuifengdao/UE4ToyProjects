// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FLATPEACH_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
		AMyPlayerController();

public:

	UPROPERTY(BluePrintReadWrite)
		float GameTime;

	float LastEnergyProduceTime;

	int TickTime = 0;

	UPROPERTY(EditAnywhere,BluePrintReadWrite)
		int32 Energy;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 index1;	//植物

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool BPlant; //是否种了植物

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 index2; //铁锹

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool BUseShovel; //铁锹是否挖了植物

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LocToBP;

	int i = 0;	//时间计次
	bool BDestroy;		//是否移除界面

	FTimerHandle DelayTime;

	UPROPERTY(BlueprintReadWrite)
		int PlanterNum;	

	UPROPERTY(BlueprintReadWrite)
		bool BPauseClick = true;

	UPROPERTY(BlueprintReadWrite)
		bool BClickPlanter;		//点击鼠标
	UPROPERTY(BlueprintReadWrite)
		bool BReleasePlanter;		//释放鼠标

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UImage* PlanterPicture;//拖动的图片

	UPROPERTY()
		UUserWidget* EnergyNotEnough;	//保存能量不足界面

	//树相关点击
	UPROPERTY(BlueprintReadWrite)
		bool BClickTree;		//点击树按钮
	UPROPERTY(BlueprintReadWrite)
		bool BHoverTree;		//拖动过程不释放鼠标
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseTree;	//释放鼠标

	//仙人掌相关点击
	UPROPERTY(BlueprintReadWrite)
		bool BClickCluster;		//点击仙人掌按钮
	UPROPERTY(BlueprintReadWrite)
		bool BHoverCluster;		//拖动过程不释放鼠标
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseCluster;	//释放鼠标

	//藤蔓相关点击
	UPROPERTY(BlueprintReadWrite)
		bool BClickVine;		//点击藤蔓按钮
	UPROPERTY(BlueprintReadWrite)
		bool BHoverVinee;		//拖动过程不释放鼠标
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseVine;	//释放鼠标

	//铁锹相关点击
	UPROPERTY(BlueprintReadWrite)
		bool BClickShovels;		//点击铁锹按钮
	UPROPERTY(BlueprintReadWrite)
		bool BHoverShovels;		//拖动过程不释放鼠标
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseShovels;	//释放鼠标

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyEnergyWidget();	//移除窗口

	void ShovelsFun();//铁锹相关功能

	void FindCubeAndPlanter(FVector pos); //找到cube，然后根据有没有被占领找植物,铁锹

	void ShovelDestroyPlanter(FVector pos);	//铁锹移除植物

	bool BEnergyEnoughAndDestroy(int Planter, UUserWidget* EnergyNot);	//判断能量是否满足可以种植，移除植物

	FVector FindMousePosition();	//鼠标位置

	void UseEnergy(int Planter,FVector);

	void CreateAnimals();  //动物产生

	UFUNCTION(BlueprintCallable)
		int Change();

	UFUNCTION(BlueprintCallable)
		int ChangeShovels();

	//找最近的cube位置，如果被占领，返回（0，0，0），未被占领则占领位置
	FVector FindNearestCubeLocation(FVector pos);

	UFUNCTION(BlueprintCallable)	//图片跟随鼠标移动
		void PictureFollowMouse(bool BClickPlanterRef, UImage* PlanterPictureRef);

	UFUNCTION(BlueprintCallable)	//图片回原位置，隐藏
		void PictureBack(bool BClickPlanterRef, UImage* PlanterPictureRef);

	UFUNCTION(BlueprintCallable)	//种植物
		void PlantAndDestroy(bool BReleasePlanterRef, int Planter,UUserWidget* EnergyNot);

	/*提示能量不足菜单*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void EnergynotEnoughWidget(UUserWidget* WidgetClass);

};
