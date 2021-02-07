// Fill out your copyright notice in the Description page of Project Settings.

#include "Eagle.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "EagleAIController.h"
#include "FlatPeachGameModeBase.h"
#include "LogTest.h"
#include "Tasks/AITask_MoveTo.h"
#include "MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "MyBlueprintFunctionLibrary.h"

// Sets default values
AEagle::AEagle()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;

	GetCapsuleComponent()->InitCapsuleSize(125.f, 176.0f);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Eagle");
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEagle::EagleOnHit);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EagleSkeletalMeshAssert(TEXT("/Game/Characters/Character/Eagle/Meshes/Skeletal_Meshes/Eagle_Skel_mesh.Eagle_Skel_mesh"));
	EagleMeshCom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EagleMeshCom0"));
	EagleMeshCom->SetSkeletalMesh(EagleSkeletalMeshAssert.Object);
	EagleMeshCom->SetupAttachment(GetCapsuleComponent());
	EagleMeshCom->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	EagleMeshCom->SetRelativeLocation(FVector(0, 0, -200));

	//血量条
	// 初始化EagleBloodWidget
	static ConstructorHelpers::FClassFinder<UUserWidget> EagleBloodShow(TEXT("/Game/UMG/BP_Blood"));
	EagleBloodWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EagleBlood"));
	EagleBloodWidget->SetupAttachment(GetCapsuleComponent());
	EagleBloodWidget->SetWidgetClass(EagleBloodShow.Class);
	EagleBloodWidget->SetRelativeRotation(FRotator(90, 0, 0));

	//初始化
	EagleAddEnergyValue = 40;
	EagleAttackValue = 10;
	InitBlood();
	CurrentEagleBlood = MaxEagleBlood;
	BIsDead = false;
	BCanDam = true;
	lasttime = 0.f;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanFly = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = false;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanWalk = false;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanSwim = false;

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 120;
	//GetMovementComponent()->Velocity = FVector(0, -10000, 0);

	AutoPossessAI = EAutoPossessAI::Spawned;
	AIControllerClass = AEagleAIController::StaticClass();

	//static ConstructorHelpers::FObjectFinder<UBlueprint> EagleFlyIns(TEXT("AnimBlueprint'/Game/Characters/Character/Eagle/Animations/Eagle_anims/ABP_EagleFly.ABP_EagleFly'"));
	//EagleFlyBlueprint = (UClass*)EagleFlyIns.Object->GeneratedClass;
	//EagleMeshCom->SetAnimInstanceClass(EagleFlyBlueprint);

	//打包成功
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> EagleFlyIns(TEXT("AnimBlueprint'/Game/Characters/Character/Eagle/Animations/Eagle_anims/ABP_EagleFly.ABP_EagleFly_C'"));
	check(EagleFlyIns.Succeeded());
	EagleMeshCom->AnimClass = EagleFlyIns.Object;
}

void AEagle::BeginPlay()
{
	Super::BeginPlay();
	FVector pos = GetActorLocation();
	//TRACE("BEGIN loc x		%f", pos.X);
	//TRACE("BEGIN loc y		%f", pos.Y);
	//TRACE("BEGIN loc z		%f", pos.Z);
	pos.Y -= 3100;
	EagleTargetLoc = pos;
	AEagleAIController * EagleCon = Cast<AEagleAIController>(GetController());
	EagleCon->MoveToLocation(EagleTargetLoc);
/*
	TRACE("target loc x		%f", pos.X);
	TRACE("target loc y		%f", pos.Y);
	TRACE("target loc z		%f", pos.Z);*/
}

void AEagle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUserWidget* CurrentWidget = EagleBloodWidget->GetUserWidgetObject();

	if (NULL != CurrentWidget)
	{
		HPBarProgress = Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("ProgressBar_24")));
		if (NULL != HPBarProgress)
		{
			HPBarProgress->SetPercent(1.f);
		}
	}
}

void AEagle::InitBlood()
{
	//AFlatPeachGameModeBase* gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AFlatPeachGameModeBase* gamemode = UMyBlueprintFunctionLibrary::GetGameModeInstance();
	//check(gamemode);
	if (gamemode != NULL && gamemode !=nullptr)
	{
		int mode = gamemode->Mode;
		switch (mode)
		{
		case 0:
			MaxEagleBlood = 30;
			break;
		case 1:
			MaxEagleBlood = 70;
			break;
		case 2:
			MaxEagleBlood = 120;
			break;
		case 3:
			MaxEagleBlood = 200;
			break;
		default:
			break;
		}
	}
}

// Called every frame
void AEagle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetGameTimeSinceCreation() - lasttime > 0.1)
	{
		GetCharacterMovement()->MaxFlySpeed = 120;
		AEagleAIController * EagleCon = Cast<AEagleAIController>(GetController());
		EagleCon->MoveToLocation(EagleTargetLoc);
		lasttime = GetGameTimeSinceCreation();
	}
	/*FVector pos = GetActorLocation();
	pos.Y += -DeltaTime * 100;
	SetActorLocation(pos);*/
	if (!BIsDead && CurrentEagleBlood <= 0)
	{
		Die();
		return;
	}
	if (CurrentEagleBlood>=0)
	{
		HPBarProgress->SetPercent((float)((float)CurrentEagleBlood / (float)MaxEagleBlood));//更新血条长度
	}
}

void AEagle::EagleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComp != nullptr && OtherActor != nullptr && OtherComp != nullptr)
	{
		FString nam = OtherActor->GetName();
		//TRACE("Eagle Hit   %s",*nam);
		//GetCharacterMovement()->StopMovementKeepPathing();
		GetCharacterMovement()->MaxFlySpeed = 0;
		if (BCanDam)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, EagleAttackValue, OtherActor->GetActorLocation(), Hit, nullptr, this, DamageType);
			//攻击动画
			/* */
			GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &AEagle::CanDam, 2.f, false);
			BCanDam = false;
		}
	}
	/*else if (OtherActor == nullptr)
	{
		GetCharacterMovement()->MaxFlySpeed = 120;
		AEagleAIController * EagleCon = Cast<AEagleAIController>(GetController());
		EagleCon->MoveToLocation(EagleTargetLoc);
	}*/
}

float AEagle::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (CurrentEagleBlood > 0)
	{
		//FHitResult hitRes;
		//FVector impulseDir;
		//DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentEagleBlood -= ActualDamage;
		//TRACE("damage blood    %d", CurrentRobotBlood);
		return ActualDamage;
	}
	return 0;
}

void AEagle::Die()
{
	BIsDead = true;
	float DeathDuration = 0.1f;
	//死亡动画
	GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &AEagle::Des, DeathDuration, false);

	AFlatPeachGameModeBase *gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gamemode->Number1++;
	//TRACE("num  %d", gamemode->Number1);

	AMyPlayerController *gameCon = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	gameCon->Energy += EagleAddEnergyValue;
}

void AEagle::Des()
{
	Destroy();
}

void AEagle::CanDam()
{
	BCanDam = true;
}
