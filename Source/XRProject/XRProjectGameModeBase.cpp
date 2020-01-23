#include "XRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"


AXRProjectGameModeBase::AXRProjectGameModeBase()
{
}

void AXRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AXRProjectGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AXRProjectGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

