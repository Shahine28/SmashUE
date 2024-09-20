// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateIdle.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateIdle::OnInputMoveYFast(float InputMoveY)
{
	
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	/*Engine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State: Idle")
		);*/
	
	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputMoveYFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveYFast);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Exit State: Idle")
	// );

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputMoveYFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveYFast);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	// Super::StateTick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Green,
	// TEXT("Tick State: Idle"));
	if(Character->GetVelocity().Z < 0 && !Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);	
	}
	if ((FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold &&
	Character->GetInputMoveY() > 0.1f && Character->GetCharacterMovement()->IsMovingOnGround())||
	Character->GetVelocity().Z > .1f && Character->CanJumpAgain)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXTreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}
