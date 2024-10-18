// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}


void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Purple,
	TEXT("Enter State: Jump")
	);
	if (Character->JumpMaxCount != CharacterSettings->JumpMaxCount)
	{
		Character->JumpMaxCount = CharacterSettings->JumpMaxCount;
	}
	Character->CurrentJumpCount++;
	Character->GetCharacterMovement()->JumpZVelocity = (2 * JumpMaxHeight) / JumpDuration;
	Character->GetCharacterMovement()->GravityScale = ((-2 * JumpMaxHeight)/FMath::Square(JumpDuration))/GetWorld()->GetGravityZ();
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->DoJump();
	Character->InputMoveYFastEvent.AddDynamic(this, &USmashCharacterStateJump::OnInputMoveYFast);
	if (!Character->JumpFlipFlop)
	{
		Character->bCanJump = false;
		// GEngine->AddOnScreenDebugMessage(
		// -1,
		// 3.f,
		// FColor::Cyan,
		// TEXT("Started")
		// );
	}
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Exit State: Jump")
	// );
	Character->GetCharacterMovement()->AirControl = 1.f;
	Character->GetCharacterMovement()->GravityScale = 1.f;
	Character->InputMoveYFastEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnInputMoveYFast);
	Timer = 0;
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	Timer += DeltaTime;
	// Super::StateTick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Green,
	// TEXT("Tick State: Jump"));
	if(Timer >= JumpDuration ||
		Character->GetVelocity().Z < 0 || (Character->GetInputMoveY() < 0.f &&
		FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold))
	{
		Character->StopJumping();
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);	
	}
	else if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXTreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(JumpWalkSpeed);
	}
	else if (Character->GetInputMoveY() > CharacterSettings->InputMoveYTreshold
		&& Character->CurrentJumpCount < Character->JumpMaxCount && Character->bCanJump)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
}

void USmashCharacterStateJump::OnInputMoveYFast(float InputMoveY)
{
	
}
