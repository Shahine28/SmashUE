// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"

#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/SmashCharacterInputData.h"


// Sets default values
ASmashCharacter::ASmashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanJump = true;
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreatStateMachine();
	InitStateMachine();
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();

}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	BindInputMoveXAxisAndAction(EnhancedInputComponent);
	BindInputMoveYAndAction(EnhancedInputComponent);

}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreatStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
	
}

void ASmashCharacter::InitStateMachine()
{
	if (!StateMachine) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (!StateMachine) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::Move(float MaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	AddMovementInput(FVector::ForwardVector, OrientX);
}

void ASmashCharacter::DoJump()
{
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Cyan,
	TEXT("PlayerJump")
	);
	Jump();
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem) return;

	
	InputSystem->AddMappingContext(InputMappingContext, 0);
	
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::BindInputMoveXAxisAndAction(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!InputData) return;

	if (InputData -> InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX
		);

		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX
		);
	}

	if (InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast
			);
	}
	
}

float ASmashCharacter::GetInputMoveY() const
{
	return InputMoveY;
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		FString::Printf(TEXT("InputMoveY value = %f"), InputMoveY)
		);
	JumpFlipFlop = !JumpFlipFlop;
	if (JumpFlipFlop)
	{
		bCanJump = true;
		GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Completed Jump Input")
		);
	}
}

void ASmashCharacter::OnInputMoveYFast(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
	InputMoveYFastEvent.Broadcast(InputMoveY);
	
}

void ASmashCharacter::BindInputMoveYAndAction(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!InputData) return;
	if (InputData -> InputActionMoveY)
	{
		 EnhancedInputComponent->BindAction(
		 	InputData -> InputActionMoveY,
		 	ETriggerEvent::Started,
		 	this,
		 	&ASmashCharacter::OnInputMoveY);
		// EnhancedInputComponent->BindAction(
		// 	InputData -> InputActionMoveY,
		//    	ETriggerEvent::Triggered,
		//    	this,
		// 	&ASmashCharacter::OnInputMoveY);
		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveY,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveY);
	}
	if (InputData->InputActionMoveYFast)
	{
		EnhancedInputComponent->BindAction(
			InputData -> InputActionMoveYFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveYFast);
	}
}












