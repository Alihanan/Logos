// Copyright Epic Games, Inc. All Rights Reserved.

#include "LogosCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/KismetMathLibrary.h>
#include <SphereIcosaMeshGenerator.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ALogosCharacter::ALogosCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	RootCamera = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
	RootCamera->SetupAttachment(RootComponent);

	MoveRefRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Move Ref Root"));
	MoveRefRoot->SetupAttachment(RootCamera);

	SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForwardLookDirection"));
	SphereComponent->SetStaticMesh(nullptr);
	SphereComponent->SetupAttachment(MoveRefRoot);
	SphereComponent->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f));


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootCamera);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;



	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	this->GetCharacterMovement()->bIgnoreBaseRotation = true; // IDK, but tutorial had this

	GetMesh()->bEnableUpdateRateOptimizations = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

bool ALogosCharacter::HasChangedPosition()
{
	FVector loc = this->GetActorLocation();

	if (loc.Equals(this->previousPosition))
	{
		return false;
	}

	this->previousPosition = loc;
	return true;
}

bool ALogosCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	CA_SUPPRESS(6011);

	
	
	if (const APlayerController* Player = Cast<APlayerController>(RealViewer))
	{

		FVector playerLoc = ViewTarget->GetActorLocation();
		auto point = FIcosaPointCoord::convertPositionToIcosaPoint(playerLoc);

		auto pointMe = FIcosaPointCoord::convertPositionToIcosaPoint(this->GetActorLocation());
		

		if (point.IsNeighbouring(pointMe)) 
		{
			return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
		}
	}
	return false;
}


void ALogosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALogosCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ALogosCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALogosCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALogosCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ALogosCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ALogosCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector

		const FVector actorLoc = this->GetActorLocation();
		const FVector moveRefLoc = this->SphereComponent->GetComponentLocation();

		const FVector ForwardDirection = UKismetMathLibrary::GetDirectionUnitVector(actorLoc, moveRefLoc);
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = this->FollowCamera->GetRightVector();

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ALogosCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ALogosCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ALogosCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
