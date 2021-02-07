// Fill out your copyright notice in the Description page of Project Settings.

#include "Magnet.h"
#include "Engine.h"
#include "LogTest.h"
#include "TimerManager.h"
#include "RunnerCharacter.h"

// Sets default values
AMagnet::AMagnet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Magnet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magnet"));
	Magnet->AttachTo(RootComponent);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->AttachTo(RootComponent);

	RotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

// Called when the game starts or when spawned
void AMagnet::BeginPlay()
{
	Super::BeginPlay();
	Magnet->OnComponentBeginOverlap.AddDynamic(this, &AMagnet::OnOverlapBegin);
}

// Called every frame
void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagnet::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Runner = Cast<ARunnerCharacter>(OtherActor);
	if (Runner != NULL)
	{
		Runner->UseMagnet = true;
		this->Destroy();
		//this->SetActorHiddenInGame(true);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMagnet::MagnetDestroy, 10.0f, true);
	}
}

//UseMagnet
void AMagnet::MagnetDestroy()
{
	Runner->UseMagnet = false;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), RunnerClass, RunnerArray);
	//ARunnerCharacter* runner = Cast<ARunnerCharacter>(RunnerArray[0]);
	//if (runner != NULL)
	//	runner->UseMagnet = false;
}