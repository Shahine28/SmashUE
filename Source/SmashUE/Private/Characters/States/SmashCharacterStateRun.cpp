// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::OnInputMoveYFast(float InputJump)
{
	
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Enter State: Run")
	// );
	Character->InputMoveYFastEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputMoveYFast);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Cyan,
	// TEXT("Exit State: Run")
	// );
	Character->InputMoveYFastEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputMoveYFast);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 3.f,
	// FColor::Green,
	// TEXT("Tick State: Run"));
	if(Character->GetVelocity().Z < 0 && !Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);	
	}
	else if (FMath::Abs(Character->GetInputMoveY()) > CharacterSettings->InputMoveYTreshold &&
		Character->GetInputMoveY() > .1f && Character->GetCharacterMovement()->IsMovingOnGround() ||
		Character->GetVelocity().Z > .1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	else if (Character->GetInputMoveX() < CharacterSettings->InputMoveYTreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(WalkMaxSpeed);
	}
}

