// Fill out your copyright notice in the Description page of Project Settings.


#include "PotalTrigger.h"

APotalTrigger::APotalTrigger()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &APotalTrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APotalTrigger::OnOverlapEnd);
}

void APotalTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void APotalTrigger::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	int64_t TargetID;
	if (OtherActor && (OtherActor != this)) 
	{	
		BeginOverlapDelegate.ExecuteIfBound(ObjectID, OtherActor);
	}
}
void APotalTrigger::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	int64_t TargetID;
	if (OtherActor && (OtherActor != this))
	{
		EndOverlapDelegate.ExecuteIfBound(ObjectID, OtherActor);
	}
}