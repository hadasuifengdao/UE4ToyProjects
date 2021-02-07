// Fill out your copyright notice in the Description page of Project Settings.

#include "Blocker.h"
#include "Engine.h"
#include "LogTest.h"
#include "RunnerCharacter.h"


// Sets default values
ABlocker::ABlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Blocker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blocker"));
	Blocker->BodyInstance.SetCollisionProfileName("MyCollisionProfile");
	Blocker->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ABlocker::BeginPlay()
{
	Super::BeginPlay();
	Blocker->OnComponentHit.AddDynamic(this, &ABlocker::OnHitComponent);
}

// Called every frame
void ABlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlocker::OnHitComponent(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, struct FVector NormalImpulse, const FHitResult& Hit)
{
	ARunnerCharacter* runner = Cast<ARunnerCharacter>(OtherActor);
	if (runner != NULL && runner->BIsDie == false)
		runner->Death();
}
