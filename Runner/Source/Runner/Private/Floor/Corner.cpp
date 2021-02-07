// Fill out your copyright notice in the Description page of Project Settings.

#include "Corner.h"
#include "Engine.h"
#include "LogTest.h"
#include "TimerManager.h"
#include "RunnerGameMode.h"
#include "RunnerCharacter.h"
#include "MonsterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACorner::ACorner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->AttachTo(RootComponent);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	Wall->AttachTo(RootComponent);
	Wall->OnComponentHit.AddDynamic(this, &ACorner::OnHit);

	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Wall2->AttachTo(RootComponent);
	Wall2->OnComponentHit.AddDynamic(this, &ACorner::OnHit);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->AttachTo(RootComponent);

	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTrigger"));
	EndTrigger->AttachTo(RootComponent);
	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACorner::OnOverlapBegin);

	TurnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnArea"));
	TurnArea->AttachTo(RootComponent);
	TurnArea->OnComponentBeginOverlap.AddDynamic(this, &ACorner::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACorner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACorner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//OnHit
void ACorner::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float near;
	ARunnerCharacter* runner = Cast<ARunnerCharacter>(OtherActor);
	if (runner != NULL)
	{
		near = Dot3(Hit.Normal, runner->GetActorForwardVector());
		if (near <= 1)
			runner->Death();
	}
}

//OnOverlapBegin
void ACorner::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunnerCharacter* Runner = Cast<ARunnerCharacter>(OtherActor);
	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(OtherActor);
	if (Runner != NULL)
	{
		if (OverlappedComponent->GetName().Equals("TurnArea"))
			Runner->CanTurn = true;

		else
		{
			ARunnerGameMode* gameMode = Cast<ARunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (gameMode->ForkIndex != 0)
				gameMode->AddFloorTile();
		}
	}
	if(Monster != NULL)
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACorner::DestroyCorner, 2.0f, false);
}

//Destroy
void ACorner::DestroyCorner()
{
	Destroy();
}

//GetAttachTransform
FTransform ACorner::GetAttachTransform()
{
	FTransform attachPoint = AttachPoint->GetComponentTransform();
	return attachPoint;
}