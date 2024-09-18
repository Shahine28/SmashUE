// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Cyan,
	TEXT("Enter State: Run")
	);
	
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Cyan,
	TEXT("Exit State: Run")
	);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Green,
	TEXT("Tick State: Run"));

	Character->Move(WalkMaxSpeed);
}

