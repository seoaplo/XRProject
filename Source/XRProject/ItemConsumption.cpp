#include "ItemConsumption.h"
#include "XRGameInstance.h"

bool UItemConsumption::Use()
{
	return false;
}

int UItemConsumption::GetCount()
{
	return AddInfo.Count;
}

int UItemConsumption::GetID()
{
	return DefaultInfo.ID;
}

void UItemConsumption::SetCount(int Count)
{
	AddInfo.Count = Count;
}

UTexture2D* UItemConsumption::GetIcon()
{
	UTexture2D* result = nullptr;
	auto GI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	GI->ItemManager->GetIcon(result, DefaultInfo.IconResourceID);
	return result;
}