// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "Characters/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);
}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (!ArenaPlayerStartActor) continue;
		ResultActors.Add(ArenaPlayerStartActor);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	switch (InputType)
	{
	case EAutoReceiveInput::Player0:
		return SmashCharacterClassP0;
	case EAutoReceiveInput::Player1:
		return SmashCharacterClassP1;
	case EAutoReceiveInput::Player2:
		return SmashCharacterClassP2;
	case EAutoReceiveInput::Player3:
		return SmashCharacterClassP3;
	default :
		return nullptr;
	}
	
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (!SmashCharacterClass) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
		SmashCharacterClass,
		SpawnPoint->GetTransform()
		);

		if (!NewCharacter) continue;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(NewCharacter);
		
	}
}

