// Fill out your copyright notice in the Description page of Project Settings.

#include "Tree.h"
#include "AttackLeaves.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "LogTest.h"
#include "MyPlayerController.h"
#include "Cube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Eagle.h"
#include "Robot.h"
#include "Warrok.h"
// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;

	TreeCapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TreeCapsule"));
	TreeCapsuleCom->InitCapsuleSize(75.f, 206.0f);
	RootComponent = TreeCapsuleCom;
	TreeCapsuleCom->BodyInstance.SetCollisionProfileName("TreeAndVine");
	TreeCapsuleCom->CanCharacterStepUpOn = ECB_No;

	// Create the mesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TreeMesh(TEXT("/Game/Characters/Character/Tree/Meshes/plamtree.plamtree"));
	TreeMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
	TreeMeshCom->BodyInstance.SetCollisionProfileName("TreeAndVine");
	TreeMeshCom->SetupAttachment(RootComponent);
	TreeMeshCom->SetStaticMesh(TreeMesh.Object);
	TreeMeshCom->SetRelativeLocation(FVector(0, 0, -200));
	TreeMeshCom->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));
	
	MaxTreeBlood = 100;
	CurrentTreeBlood = MaxTreeBlood;
	TreeEnergyNeed = 200;

	FireRate = 1.5f;
	bCanFire = true;
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Try and fire a shot
	if (CurrentTreeBlood<=0)
	{
		//TRACE("tree die blood  %d", CurrentTreeBlood);
		Die();
		return;
	}
	if (BHasEnemy())
	{
		FireShot();
	}
}

float ATree::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentTreeBlood > 0)
	{
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentTreeBlood -= ActualDamage;
		//TRACE("tree damage blood    %d", CurrentTreeBlood);
		return ActualDamage;
	}
	return 0;
}

bool ATree::BHasEnemy()
{
	//设置ObjectType数组
	AnimalsTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));			//Eagle
	//AnimalsTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));			//Cluster
	AnimalsTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3));			//WarrokAndRobot
	//AnimalsTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));		//treeAndVine

	FVector pos = GetActorLocation();
	pos.Z += 150;
	FVector pos1 = pos;
	pos1.Y += 2700;

	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	//UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), pos, pos1, AnimalsTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, 
		Hit, true, FLinearColor::Red, FLinearColor::Green, 2.f))
	{//如果检测到了碰撞
		//TRACE("block charactor %s", *Hit.GetActor()->GetName());
		return true;
	}
	else
	{
		//TRACE("block nothing");
		return false;
	}
}

void ATree::Die()
{
	ReleaseCube();
	Destroy();
}

void ATree::FireShot()
{
	// If we it's ok to fire again
	if (bCanFire)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 300;
		UWorld* World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile
			World->SpawnActor<AAttackLeaves>(SpawnLocation, FRotator(0,0,0));
		}
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ATree::ShotTimerExpired, FireRate);
		bCanFire = false;
	}
}

void ATree::ShotTimerExpired()
{
	bCanFire = true;
}

void ATree::ReleaseCube()
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
	//TRACE("rec   %d", rec);
	AMyPlayerController *gameCon = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	gameCon->index2 = rec;		//利用蓝图解除占用
	gameCon->BUseShovel = true;
}