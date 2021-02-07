// Fill out your copyright notice in the Description page of Project Settings.

#include "Vine.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "LogTest.h"
#include "Cube.h"
#include "MyPlayerController.h"

// Sets default values
AVine::AVine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;

	VineCapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VineCapsule"));
	VineCapsuleCom->InitCapsuleSize(135.f, 206.0f);
	RootComponent = VineCapsuleCom;
	VineCapsuleCom->BodyInstance.SetCollisionProfileName("TreeAndVine");
	VineCapsuleCom->CanCharacterStepUpOn = ECB_No;

	// Create the mesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VineMesh(TEXT("/Game/Characters/Character/Vine/Meshes/SM_Tree_Type_B_Leaves.SM_Tree_Type_B_Leaves"));
	VineMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VineMesh"));
	VineMeshCom->SetupAttachment(RootComponent);
	VineMeshCom->BodyInstance.SetCollisionProfileName("TreeAndVine");
	VineMeshCom->SetStaticMesh(VineMesh.Object);
	VineMeshCom->SetRelativeLocation(FVector(0, 0, -500));
	VineMeshCom->SetRelativeScale3D(FVector(1, 1, 1.5));

	MaxVineBlood = 800;
	VineEnergyNeed = 100;
	CurrentVineBlood = MaxVineBlood;
}

// Called every frame
void AVine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentVineBlood <= 0)
	{
		//TRACE("Vine die blood  %d", CurrentVineBlood);
		Die();
		return;
	}
}

float AVine::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentVineBlood > 0)
	{
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentVineBlood -= ActualDamage;
		//TRACE("Vine damage blood    %d", CurrentVineBlood);
		return ActualDamage;
	}
	return 0;
}

void AVine::Die()
{
	ReleaseCube();
	Destroy();
}

void AVine::ReleaseCube()
{
	float Length = 10000, Vectorlen;
	int i;
	static int rec;
	FVector actorloc, vec;

	TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACube::StaticClass(), arr);

	for (i = 0; i < 40; i++) //找到最近位置的Cube，记录index
	{
		//TRACE("    %d", i);
		ACube *Cube = Cast<ACube>(arr[i]);
		actorloc = Cube->GetActorLocation();//Cube位置
		vec = actorloc - GetActorLocation();		//两位置求向量
		Vectorlen = vec.Size();		//向量长度
		if (Vectorlen<Length)
		{
			Length = Vectorlen;
			rec = i;
		}
	}
	AMyPlayerController *gameCon = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	gameCon->index2 = rec;		//利用蓝图解除占用
	gameCon->BUseShovel = true;
}
