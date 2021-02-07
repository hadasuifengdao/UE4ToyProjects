// Fill out your copyright notice in the Description page of Project Settings.

#include "Warrok.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "WarrokAIController.h"
#include "FlatPeachGameModeBase.h"
#include "LogTest.h"
#include "MyPlayerController.h"
#include "MyBlueprintFunctionLibrary.h"
//三个，血量条
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"

// Sets default values
AWarrok::AWarrok()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;

	GetCapsuleComponent()->InitCapsuleSize(105.f, 156.0f);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("WarrokAndRobot");

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AWarrok::WarrokOnHit);

	//ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WarrokSkeletalMeshAssert(TEXT("/Game/Characters/Character/Warrok/Warrok_W_Kurniawan.Warrok_W_Kurniawan"));
	WarrokMeshCom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WarrokMeshCom0"));
	WarrokMeshCom->SetSkeletalMesh(WarrokSkeletalMeshAssert.Object);
	WarrokMeshCom->SetupAttachment(GetCapsuleComponent());
	WarrokMeshCom->SetRelativeLocation(FVector(0, 0, -180));
	WarrokMeshCom->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	WarrokMeshCom->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<UUserWidget> RobotBloodShow(TEXT("/Game/UMG/BP_Blood"));
	RobotBloodWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RobotBlood"));
	RobotBloodWidget->SetupAttachment(GetCapsuleComponent());
	RobotBloodWidget->SetWidgetClass(RobotBloodShow.Class);
	RobotBloodWidget->SetRelativeRotation(FRotator(90, 0, 0));
	RobotBloodWidget->SetRelativeLocation(FVector(0, 0, 130));

	//初始化
	GetCharacterMovement()->MaxWalkSpeed = 50;
	WarrokAddEnergyValue = 60;
	WarrokAttackValue = 5;
	InitBlood();
	CurrentWarrokBlood = MaxWarrokBlood;
	BIsDead = false;
	BCanDam = true;
	lasttime = 0.f;
	//TRACE("Warrok  ATTACK     %d", WarrokAttackValue);
	//TRACE("Warrok  BLOOD     %d", MaxWarrokBlood);
	
	//static ConstructorHelpers::FObjectFinder<UBlueprint> WarrokWalkInstance(TEXT("/Game/Characters/Character/Warrok/Animation/ABP_WarrokWalk.ABP_WarrokWalk"));
	//BlueprintVar = (UClass*)WarrokWalkInstance.Object->GeneratedClass;
	//WarrokMeshCom->SetAnimInstanceClass(BlueprintVar);

	//打包成功
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> WarrokWalkInstance(TEXT("/Game/Characters/Character/Warrok/Animation/ABP_WarrokWalk.ABP_WarrokWalk_C"));
	check(WarrokWalkInstance.Succeeded());
	WarrokMeshCom->AnimClass = WarrokWalkInstance.Object;

	AutoPossessAI = EAutoPossessAI::Spawned;
	AIControllerClass = AWarrokAIController::StaticClass();
	//GetMovementComponent()->Velocity = FVector(0, -10000, 0);
	//GetCharacterMovement()->MoveTo
}

void AWarrok::InitBlood()
{
	//AFlatPeachGameModeBase *gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AFlatPeachGameModeBase* gamemode = UMyBlueprintFunctionLibrary::GetGameModeInstance();
	if (gamemode != nullptr)
	{
		int mode = gamemode->Mode;
		switch (mode)
		{
		case 0:
			MaxWarrokBlood = 100;
			break;
		case 1:
			MaxWarrokBlood = 200;
			break;
		case 2:
			MaxWarrokBlood = 250;
			break;
		case 3:
			MaxWarrokBlood = 350;
			break;
		default:
			break;
		}
	}
}

void AWarrok::BeginPlay()
{
	Super::BeginPlay();
	FVector pos = GetActorLocation();
	//TRACE("BEGIN loc x		%f", pos.X);
	//TRACE("BEGIN loc y		%f", pos.Y);
	//TRACE("BEGIN loc z		%f", pos.Z);
	pos.Y -= 3100;
	WarrokTarget = pos;
	AWarrokAIController * WarrokCon = Cast<AWarrokAIController>(GetController());
	WarrokCon->MoveToLocation(WarrokTarget);
	//TRACE("target loc x		%f", pos.X);
	//TRACE("target loc y		%f", pos.Y);
	//TRACE("target loc z		%f", pos.Z);
}

void AWarrok::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUserWidget * CurrentWidget = RobotBloodWidget->GetUserWidgetObject();

	if (CurrentWidget)
	{
		RobotBloodProgressBar = Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("ProgressBar_24")));
		if (RobotBloodProgressBar)
		{
			RobotBloodProgressBar->SetPercent(1.f);
		}
	}
}

// Called every frame
void AWarrok::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetGameTimeSinceCreation() - lasttime > 0.1)
	{
		GetCharacterMovement()->MaxWalkSpeed = 50;
		AWarrokAIController * WarrokCon = Cast<AWarrokAIController>(GetController());
		WarrokCon->MoveToLocation(WarrokTarget);
		lasttime = GetGameTimeSinceCreation();
	}
	/*if (!BWarrokHit)
	{
		FVector pos = GetActorLocation();
		pos.Y -= DeltaTime * 100;
		SetActorLocation(pos);
	}*/
	if (!BIsDead && CurrentWarrokBlood <= 0)
	{
		Die();
	}
	if (CurrentWarrokBlood >= 0)
	{
		RobotBloodProgressBar->SetPercent((float)((float)CurrentWarrokBlood / (float)MaxWarrokBlood));//更新血条长度
	}
}

void AWarrok::WarrokOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComp!=nullptr && OtherActor!=nullptr && OtherComp!=nullptr)
	{
		//TRACE("Warrok  Hit");
		//GetCharacterMovement()->StopMovementKeepPathing();
		GetCharacterMovement()->MaxWalkSpeed = 0;
		if (BCanDam)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, WarrokAttackValue, OtherActor->GetActorLocation(), Hit, nullptr, this, DamageType);
			//攻击动画
			/* */
			GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &AWarrok::CanDam, 2.f, false);
			BCanDam = false;
		}
	}
}

float AWarrok::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (CurrentWarrokBlood > 0)
	{
		//FHitResult hitRes;
		//FVector impulseDir;
		//DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentWarrokBlood -= ActualDamage;
		//TRACE("Warrok blood    %d", CurrentWarrokBlood);
		return ActualDamage;
	}
	return 0;
}

void AWarrok::Die()
{
	BIsDead = true;
	float DeathDuration = 0.2f;
	//死亡动画
	GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &AWarrok::Des, DeathDuration, false);

	AFlatPeachGameModeBase *gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gamemode->Number1++;
	
	AMyPlayerController *gameCon = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	gameCon->Energy += WarrokAddEnergyValue;
}

void AWarrok::Des()
{
	Destroy();
}

void AWarrok::CanDam()
{
	BCanDam = true;
}