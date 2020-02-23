// Fill out your copyright notice in the Description page of Project Settings.


#include "PotalTrigger.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "XRGameInstance.h"

APotalTrigger::APotalTrigger()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &APotalTrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APotalTrigger::OnOverlapEnd);

}

void APotalTrigger::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
}

void APotalTrigger::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	int64_t TargetID = 0;
	if (OtherActor && (OtherActor != this)) 
	{	
		GetMapMgr().PotalInPlayer(OtherActor);
	}
}


void APotalTrigger::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	int64_t TargetID = 0;
	if (OtherActor && (OtherActor != this))
	{
		EndOverlapDelegate.ExecuteIfBound(OtherActor);
	}
}

void APotalTrigger::SetSize(float SphereRadius)
{
	USphereComponent* Shpere = Cast<USphereComponent>(GetCollisionComponent());
	SetActorScale3D(FVector(SphereRadius, SphereRadius, SphereRadius));
}