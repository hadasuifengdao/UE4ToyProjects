// Fill out your copyright notice in the Description page of Project Settings.

#include "Cluster.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "LogTest.h"

// Sets default values
ACluster::ACluster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;

	ClusterCapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ClusterCapsule"));
	ClusterCapsuleCom->InitCapsuleSize(125.f, 86.0f);
	RootComponent = ClusterCapsuleCom;
	ClusterCapsuleCom->BodyInstance.SetCollisionProfileName("Cluster");

	ClusterCapsuleCom->OnComponentBeginOverlap.AddDynamic(this, &ACluster::Overlaping);
	ClusterCapsuleCom->OnComponentEndOverlap.AddDynamic(this, &ACluster::EventEndOverlap);

	// Create the mesh component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ClusterMesh(TEXT("/Game/Characters/Character/Cluster/Meshes/SM_Bushes_Cluster_A.SM_Bushes_Cluster_A"));
	ClusterMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClusterMesh"));
	ClusterMeshCom->SetupAttachment(RootComponent);
	ClusterMeshCom->BodyInstance.SetCollisionProfileName("Cluster");
	ClusterMeshCom->SetStaticMesh(ClusterMesh.Object);

	ClusterMeshCom->SetRelativeScale3D(FVector(0.8, 0.8, 0.4));
	ClusterAttack = 10;
	ClusterEnergyNeed = 150;
	BIsOverlaping = false;
	BetweenDam = 2;
	DamTime = 0;
}

// Called when the game starts or when spawned
void ACluster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACluster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BIsOverlaping)
	{
		if (GetGameTimeSinceCreation() - DamTime > BetweenDam)
		{
			DamTime = GetGameTimeSinceCreation();
			UGameplayStatics::ApplyPointDamage(OtherActor1, ClusterAttack, OtherActor1Loc, SweepResult1, nullptr, this, DamageType);
		}
	}
}

void ACluster::Overlaping(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	BIsOverlaping = true;
	//TRACE("BEGIN");
	OtherActor1 = OtherActor;
	OtherActor1Loc = OtherActor->GetActorLocation();
	SweepResult1 = SweepResult;
}

void ACluster::EventEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	BIsOverlaping = false;
	//TRACE("END");
}

