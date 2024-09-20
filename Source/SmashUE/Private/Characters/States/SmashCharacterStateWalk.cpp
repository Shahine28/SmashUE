// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateWalk.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateWalk::OnInputMoveYFast(float InputJump)
{
	
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Enter State: Walk")
	// );

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputMoveYFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveYFast);
	
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Exit State: Walk")
	// );

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputMoveYFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveYFast);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Green,
	// TEXT("Tick State: Walk"));
	if(Character->GetVelocity().Z < 0 && !Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);	
	}
	else if ((FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold &&
	Character->GetInputMoveY() > .1f && Character->GetCharacterMovement()->IsMovingOnGround())||
	Character->GetVelocity().Z > .1f && Character->CanJumpAgain)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	else if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXTreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(WalkMaxSpeed);
	}
}
