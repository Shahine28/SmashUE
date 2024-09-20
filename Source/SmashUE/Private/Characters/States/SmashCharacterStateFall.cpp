// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->GetCharacterMovement()->AirControl = FallAirControl;
	Character->GetCharacterMovement()->GravityScale = FallGravityScale;
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1.f;
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Green,
	// TEXT("Tick State: Fall"));
	if(Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);	
	}
	if (FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold &&
		Character->GetInputMoveY() < 0.f)
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}
	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXTreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(FallHorizontalMoveSpeed);
	}
	if (Character->GetInputMoveY() > 0.1f && FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold
		&& Character->JumpCurrentCount < CharacterSettings->JumpMaxCount && Character->CanJumpAgain)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
}
