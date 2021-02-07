// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "RobotAIController.h"
#include "LogTest.h"
#include "FlatPeachGameModeBase.h"
#include "Tasks/AITask_MoveTo.h"
#include "GameFramework/DamageType.h"
#include "MyPlayerController.h"
#include "Animation/AnimMontage.h"
#include "MyBlueprintFunctionLibrary.h"
//三个，血量条
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"

ARobot::ARobot()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanBeDamaged = true;
	GetCapsuleComponent()->InitCapsuleSize(105.f, 156.0f);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("WarrokAndRobot");

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARobot::RobotOnHit);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RobotMeshComAssert(TEXT("/Game/Characters/Character/robot/SK_Gruntling_Scud.SK_Gruntling_Scud"));
	RobotMeshCom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotMesh0"));
	RobotMeshCom->SetSkeletalMesh(RobotMeshComAssert.Object);
	RobotMeshCom->SetupAttachment(GetCapsuleComponent());
	RobotMeshCom->SetRelativeLocation(FVector(0, 0, -180));
	RobotMeshCom->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	RobotMeshCom->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<UUserWidget> RobotBloodShow(TEXT("/Game/UMG/BP_Blood"));
	RobotBloodWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RobotBlood"));
	RobotBloodWidget->SetupAttachment(GetCapsuleComponent());
	RobotBloodWidget->SetWidgetClass(RobotBloodShow.Class);
	RobotBloodWidget->SetRelativeRotation(FRotator(90, 0, 0));

	//初始化
	RobotAddEnergyValue = 50;
	RobotAttackValue = 25;
	InitBlood();
	//TRACE("Robot   ATTACK     %d", RobotAttackValue);
	//TRACE("Robot   BLOOD     %d", MaxRobotBlood);
	CurrentRobotBlood = MaxRobotBlood;
	GetCharacterMovement()->MaxWalkSpeed = 80;
	BIsDead = false;
	BCanDamage = true;
	lasttime = 0.f;
	//GetMovementComponent()->Velocity = FVector(0, -10000, 0);
	//static ConstructorHelpers::FObjectFinder<UAnimInstance> RobotWalkInstance(TEXT("/Game/Characters/Character/robot/Animation/ABP_RobotWalk.ABP_RobotWalk"));
	//UAnimInstance *ABP_Move = RobotMeshCom->GetAnimInstance();
	//UAnimationAsset *RobotWalk = Cast<UAnimationAsset>(RobotWalkInstance);
	//RobotMeshCom->SetAnimation(RobotWalk);

	//打包成功后运行出现Fatal error
	//static ConstructorHelpers::FObjectFinder<UBlueprint> RobotWalkInstance(TEXT("AnimBlueprint'/Game/Characters/Character/robot/Animation/ABP_RobotWalk.ABP_RobotWalk'"));
	//BlueprintVar = (UClass*)RobotWalkInstance.Object->GeneratedClass;
	//RobotMeshCom->SetAnimInstanceClass(BlueprintVar); 

	//打包成功后运行出现Fatal error
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> RobotWalkInstance(TEXT("AnimBlueprint'/Game/Characters/Character/robot/Animation/ABP_RobotWalk.ABP_RobotWalk'"));
	//RobotMeshCom->SetAnimInstanceClass(RobotWalkInstance.Object->GeneratedClass);

	//无动作
	//UAnimBlueprintGeneratedClass* armAnimBPGC = Cast<UAnimBlueprintGeneratedClass>(StaticLoadObject(UAnimBlueprintGeneratedClass::StaticClass(), NULL, TEXT("AnimBlueprint'/Game/Characters/Character/robot/Animation/ABP_RobotWalk.ABP_RobotWalk'")));
	//if (armAnimBPGC)	
	//{
	//	RobotMeshCom->AnimBlueprintGeneratedClass = armAnimBPGC;
	//}

	//打包成功
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> RobotWalkInstance(TEXT("AnimBlueprint'/Game/Characters/Character/robot/Animation/ABP_RobotWalk.ABP_RobotWalk_C'"));
	check(RobotWalkInstance.Succeeded());
	RobotMeshCom->AnimClass = RobotWalkInstance.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RobotDieMontage(TEXT("AnimMontage'/Game/Characters/Character/robot/Animation/RobotDeath2.RobotDeath'"));
	RobotDie = RobotDieMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RobotAttackMontage(TEXT("AnimMontage'/Game/Characters/Character/robot/Animation/RobotAttack.RobotAttack'"));
	RobotAttack = RobotAttackMontage.Object;
	

	AutoPossessAI = EAutoPossessAI::Spawned;
	AIControllerClass = ARobotAIController::StaticClass();
}

void ARobot::InitBlood()
{
	//AFlatPeachGameModeBase *gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AFlatPeachGameModeBase* gamemode = UMyBlueprintFunctionLibrary::GetGameModeInstance();
	if (gamemode != nullptr)
	{
		int mode = gamemode->Mode;
		switch (mode)
		{
		case 0:
			MaxRobotBlood = 51;
			break;
		case 1:
			MaxRobotBlood = 81;
			break;
		case 2:
			MaxRobotBlood = 151;
			break;
		case 3:
			MaxRobotBlood = 251;
			break;
		default:
			break;
		}
	}
}

void ARobot::BeginPlay()
{
	Super::BeginPlay();
	FVector pos = GetActorLocation();
	//TRACE("BEGIN loc x		%f", pos.X);
	//TRACE("BEGIN loc y		%f", pos.Y);
	//TRACE("BEGIN loc z		%f", pos.Z);
	pos.Y -= 3100;
	TargetLocation = pos;
	ARobotAIController * RobotCon = Cast<ARobotAIController>(GetController());
	RobotCon->MoveToLocation(TargetLocation);
	//TRACE("target loc x		%f", pos.X);
	//TRACE("target loc y		%f", pos.Y);
	//TRACE("target loc z		%f", pos.Z);
	//UAITask_MoveTo::AIMoveTo(RobotCon, pos);
	//FVector pos = GetCharacterMovement()->GetActorLocation();
	//TRACE("1");
	//TRACE("x          %f", pos.X);
	//TRACE("y          %f", pos.Y);
	//TRACE("z          %f", pos.Z);
	//pos.X += -10000;
	//ARobotAIController* RobotCon = Cast<ARobotAIController>(GetController());
	//if (RobotCon != nullptr)
	//{
	//	RobotCon->MOVE(pos);
	//	TRACE("5");
	//	TRACE("x          %f", GetActorLocation().X);
	//	TRACE("y          %f", GetActorLocation().Y);
	//	TRACE("z          %f", GetActorLocation().Z);
	//}
	//else
	//{
	//	TRACE("2");
	//	TRACE("x          %f", pos.X);
	//	TRACE("y          %f", pos.Y);
	//	TRACE("z          %f", pos.Z);
	//}
}

void ARobot::PostInitializeComponents()
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
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetGameTimeSinceCreation() - lasttime > 0.1)
	{
		GetCharacterMovement()->MaxWalkSpeed = 80;
		ARobotAIController * RobotCon = Cast<ARobotAIController>(GetController());
		RobotCon->MoveToLocation(TargetLocation);
		lasttime = GetGameTimeSinceCreation();
	}
	/*FVector pos = GetActorLocation();
	pos.Y += -DeltaTime * 100;
	SetActorLocation(pos);*/
	if (!BIsDead && CurrentRobotBlood <= 0)
	{
		//TRACE("blood    %d", CurrentRobotBlood);
		Die();
	}
	if (CurrentRobotBlood>0)
	{
		RobotBloodProgressBar->SetPercent((float)((float)CurrentRobotBlood / (float)MaxRobotBlood));//更新血条长度
	}
}

void ARobot::RobotOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		//GetCharacterMovement()->StopMovementKeepPathing();
		GetCharacterMovement()->MaxWalkSpeed = 0;
		if (BCanDamage)
		{
			//TRACE("Robot  Attack");
			UGameplayStatics::ApplyPointDamage(OtherActor, RobotAttackValue, OtherActor->GetActorLocation(), Hit, nullptr, this, DamageType);
			//攻击动画
			 if (RobotAttack!=NULL)
			{
				UAnimInstance* AnimInstan = RobotMeshCom->GetAnimInstance();
				if (AnimInstan!=NULL)
				{
					AnimInstan->Montage_Play(RobotAttack, 1.f);
				}
			}
			GetWorldTimerManager().SetTimer(TimerHandle_BeginDeath, this, &ARobot::CanDam, 1.5f, false);
			BCanDamage = false;
		}
	}
}

float ARobot::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (CurrentRobotBlood > 0)
	{
		//FHitResult hitRes;
		//FVector impulseDir;
		//DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentRobotBlood -= ActualDamage;
		//TRACE("Robot blood    %d", CurrentRobotBlood);
		return ActualDamage;
	}
	return 0;
}

void ARobot::Die()
{
	BIsDead = true;
/*
	float AMyCharacter::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName)
	{
		float Duration = 0.0f;


		if (Animation)
		{
			Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
		return Duration;
	}
*/
	//死亡动画
	if (RobotDie != NULL)
	{
		//TRACE( " Die name %s",*RobotDie->GetName());
		UAnimInstance* AnimInstance = RobotMeshCom->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			//TRACE("wertttfsdfdf");
			AnimInstance->Montage_Play(RobotDie, 1.f);
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_BeginDeath, this, &ARobot::Des, 0.5f, false);

	AFlatPeachGameModeBase *gamemode = Cast<AFlatPeachGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gamemode->Number1++;
	//TRACE("num  %d", gamemode->Number1);

	AMyPlayerController *gameCon = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	gameCon->Energy += RobotAddEnergyValue;
}

void ARobot::Des()
{
	Destroy();
}

void ARobot::CanDam()
{
	BCanDamage = true;
}
