// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackLeaves.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "LogTest.h"

// Sets default values
AAttackLeaves::AAttackLeaves()
{
	LeavesCapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeavesCapsule"));
	RootComponent = LeavesCapsuleCom;
	LeavesCapsuleCom->InitCapsuleSize(30.f, 30.0f);
	LeavesCapsuleCom->BodyInstance.SetCollisionProfileName("Leaves");
	//LeavesCapsuleCom->BodyInstance.SetObjectType(ECC_WorldStatic);
	// Create mesh component for the projectile sphere
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/Characters/Character/AttackLeaves/Mesh/TwinStickProjectile.TwinStickProjectile"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetRelativeScale3D(FVector(2, 2, 2));
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Leaves");

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AAttackLeaves::LeavesOnHit);		// set up a notification for when this component hits something Î¯ÍÐ ¶¯Ì¬¼àÊÓ

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Velocity = (FVector(0, 10000, 0));
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->UpdatedComponent = ProjectileMesh;

	// Die after 3 seconds by default
	InitialLifeSpan = 2.8f;

	LeavesAttack = 8;		
}

void AAttackLeaves::LeavesOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TRACE("hit");
	//FString name = OtherActor->GetName();
	//TRACE("otherActor's name: %s", *name);
	Destroy();
	UGameplayStatics::ApplyPointDamage(OtherActor, LeavesAttack, OtherActor->GetActorLocation(), Hit, nullptr, this, DamageType);
}

