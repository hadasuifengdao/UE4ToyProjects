// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RunnerCharacter.h"
#include "RunnerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Rotator.h"
#include "LogTest.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// ARunnerCharacter

ARunnerCharacter::ARunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
	GetCharacterMovement()->AirControl = 1.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//value
	CanTurn = false;
	BIsDie = false;
	HasJump = false;
	UseMagnet = false;
	HasSeen = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunnerCharacter::MoveJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunnerCharacter::StopJump);

	//Move
	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);
	
	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARunnerCharacter::MoveCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ARunnerCharacter::EndCrouch);

	//Turn
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ARunnerCharacter::TurnLeft);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ARunnerCharacter::TurnRight);

}

///////////////////////////////////////////////////////////////////////////
// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ARunnerController* runnerController = Cast<ARunnerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (runnerController->gameStart == true)
	{
		if (!BIsDie)
		{
			this->EnableInput(Cast<APlayerController>(GetController()));
			TurnCorner(DeltaTime);
			if (Controller != NULL)
			{
				//find out which way is forward
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				//get forward vector
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				AddMovementInput(Direction, 1);

				TotalDistance += Direction.ForwardVector.Size();
				TotalGrade += Direction.ForwardVector.Size();
			}
		}
	}
	else
	{
		this->DisableInput(Cast<APlayerController>(GetController()));
	}
}

//Jump
void ARunnerCharacter::MoveJump()
{
	HasJump = true;
	Jump();
}

//StopJump
void ARunnerCharacter::StopJump()
{
	HasJump = false;
	StopJumping();
}

//MoveRight
void ARunnerCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Crouch
void ARunnerCharacter::MoveCrouch()
{
	Crouch();
}

//EndCrouch
void ARunnerCharacter::EndCrouch()
{
	UnCrouch();
}

void ARunnerCharacter::TurnLeft()
{
	if (CanTurn)
	{
		FRotator YawRotation(0, -90, 0);
		DesiredRotation = DesiredRotation + YawRotation;
		CanTurn = false;
	}
}

void ARunnerCharacter::TurnRight()
{
	if (CanTurn)
	{
		FRotator YawRotation(0, 90, 0);
		DesiredRotation = DesiredRotation + YawRotation;
		CanTurn = false;
	}
}

void ARunnerCharacter::TurnCorner(float DeltaTime)
{
	if (Controller != NULL)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		if (ControlRotation != DesiredRotation)
		{
			GetController()->SetControlRotation(FMath::RInterpConstantTo(ControlRotation, DesiredRotation, DeltaTime, 200.0f));
		}
	}
}

//AddCoins
void ARunnerCharacter::AddCoins()
{
	TotalCoins += 1;
	TotalGrade += 10;
}

//Death
void ARunnerCharacter::Death()
{
	BIsDie = true;
	this->DisableInput(Cast<ARunnerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
	//this->DisableInput(Cast<APlayerController>(GetController()));
	UGameplayStatics::SpawnEmitterAtLocation(this, BoomEmitter, GetActorLocation(), GetActorRotation(), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BoomSound, GetActorLocation());
	GetMesh()->SetVisibility(false, false);
}