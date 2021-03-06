#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "SlotObject.generated.h"

UCLASS(BlueprintType)
class XRPROJECT_API USlotObject : public UObject
{
	GENERATED_BODY()
public:
		USlotObject() {};
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetIconID();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
};
