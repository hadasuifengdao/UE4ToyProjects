// Fill out your copyright notice in the Description page of Project Settings.

#include "Skid.h"
#include "Engine.h"
#include "LogTest.h"

// Sets default values
ASkid::ASkid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->AttachTo(RootComponent);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ASkid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

