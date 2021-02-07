// Fill out your copyright notice in the Description page of Project Settings.

#include "Stone.h"
#include "Engine.h"
#include "LogTest.h"
#include "RunnerCharacter.h"

// Sets default values
AStone::AStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stone"));
	//Stone->BodyInstance.SetCollisionProfileName("MyCollisionProfile");
	RootComponent = Stone;
}

// Called when the game starts or when spawned
void AStone::BeginPlay()
{
	Super::BeginPlay();
	Stone->OnComponentHit.AddDynamic(this, &AStone::OnHitComponent);
}

// Called every frame
void AStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStone::OnHitComponent(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, struct FVector NormalImpulse, const FHitResult& Hit)
{
	runner = Cast<ARunnerCharacter>(OtherActor);
	if (runner != NULL)
	{
		runner->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AStone::ChangeSpeed, 2.0f, false);
	}
}

void AStone::ChangeSpeed()
{
	runner->GetCharacterMovement()->MaxWalkSpeed = 1500.f;
}

