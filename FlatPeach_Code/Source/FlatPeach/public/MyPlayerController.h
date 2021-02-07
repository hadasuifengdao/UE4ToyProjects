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
		int32 index1;	//ֲ��

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool BPlant; //�Ƿ�����ֲ��

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 index2; //����

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool BUseShovel; //�����Ƿ�����ֲ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LocToBP;

	int i = 0;	//ʱ��ƴ�
	bool BDestroy;		//�Ƿ��Ƴ�����

	FTimerHandle DelayTime;

	UPROPERTY(BlueprintReadWrite)
		int PlanterNum;	

	UPROPERTY(BlueprintReadWrite)
		bool BPauseClick = true;

	UPROPERTY(BlueprintReadWrite)
		bool BClickPlanter;		//������
	UPROPERTY(BlueprintReadWrite)
		bool BReleasePlanter;		//�ͷ����

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UImage* PlanterPicture;//�϶���ͼƬ

	UPROPERTY()
		UUserWidget* EnergyNotEnough;	//���������������

	//����ص��
	UPROPERTY(BlueprintReadWrite)
		bool BClickTree;		//�������ť
	UPROPERTY(BlueprintReadWrite)
		bool BHoverTree;		//�϶����̲��ͷ����
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseTree;	//�ͷ����

	//��������ص��
	UPROPERTY(BlueprintReadWrite)
		bool BClickCluster;		//��������ư�ť
	UPROPERTY(BlueprintReadWrite)
		bool BHoverCluster;		//�϶����̲��ͷ����
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseCluster;	//�ͷ����

	//������ص��
	UPROPERTY(BlueprintReadWrite)
		bool BClickVine;		//���������ť
	UPROPERTY(BlueprintReadWrite)
		bool BHoverVinee;		//�϶����̲��ͷ����
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseVine;	//�ͷ����

	//������ص��
	UPROPERTY(BlueprintReadWrite)
		bool BClickShovels;		//������°�ť
	UPROPERTY(BlueprintReadWrite)
		bool BHoverShovels;		//�϶����̲��ͷ����
	UPROPERTY(BlueprintReadWrite)
		bool BCRealeaseShovels;	//�ͷ����

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyEnergyWidget();	//�Ƴ�����

	void ShovelsFun();//������ع���

	void FindCubeAndPlanter(FVector pos); //�ҵ�cube��Ȼ�������û�б�ռ����ֲ��,����

	void ShovelDestroyPlanter(FVector pos);	//�����Ƴ�ֲ��

	bool BEnergyEnoughAndDestroy(int Planter, UUserWidget* EnergyNot);	//�ж������Ƿ����������ֲ���Ƴ�ֲ��

	FVector FindMousePosition();	//���λ��

	void UseEnergy(int Planter,FVector);

	void CreateAnimals();  //�������

	UFUNCTION(BlueprintCallable)
		int Change();

	UFUNCTION(BlueprintCallable)
		int ChangeShovels();

	//�������cubeλ�ã������ռ�죬���أ�0��0��0����δ��ռ����ռ��λ��
	FVector FindNearestCubeLocation(FVector pos);

	UFUNCTION(BlueprintCallable)	//ͼƬ��������ƶ�
		void PictureFollowMouse(bool BClickPlanterRef, UImage* PlanterPictureRef);

	UFUNCTION(BlueprintCallable)	//ͼƬ��ԭλ�ã�����
		void PictureBack(bool BClickPlanterRef, UImage* PlanterPictureRef);

	UFUNCTION(BlueprintCallable)	//��ֲ��
		void PlantAndDestroy(bool BReleasePlanterRef, int Planter,UUserWidget* EnergyNot);

	/*��ʾ��������˵�*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void EnergynotEnoughWidget(UUserWidget* WidgetClass);

};
