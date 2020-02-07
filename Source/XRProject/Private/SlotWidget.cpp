#include "SlotWidget.h"
#include "..\Public\SlotWidget.h"
#include "XRGameInstance.h"
#include "Inventory.h"

USlotWidget::USlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlotObject = nullptr;
}

int USlotWidget::GetCount()
{
	if (!IsEmpty())
	{
		return SlotObject->GetCount();
	}
	return 0;
}

bool USlotWidget::IsEmpty()
{
	return !SlotObject;
}

void USlotWidget::SetSlotObject()
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	if (Index < 0 || Inventory::GetInstance().GetInventorySize() <= Index) return;
	if (IsEquipment)
	{
		SlotObject = GetMapMgr().GetPlayer()->GetEquippedItem((EEquipmentsType)Index);
		if (!IsEmpty()) XRGI->ItemManager->GetIcon(this, SlotObject->GetIconID());
	}
	else
	{
		SlotObject = Inventory::GetInstance().GetItem(Index);
		if (!IsEmpty()) XRGI->ItemManager->GetIcon(this, SlotObject->GetIconID());
	}
	Update();
}

void USlotWidget::DropIn(UUserWidget * SlotWidget)
{
	USlotWidget* Target = Cast<USlotWidget>(SlotWidget);
	if (Target)
	{
		if (Target->IsEquipment && IsEquipment) return;
		if (Target->IsEquipment || IsEquipment)
		{
			UItemEquipment* Equipment1 = Cast<UItemEquipment>(SlotObject);
			if (!Equipment1) return;
			UItemEquipment* Equipment2 = Cast<UItemEquipment>(Target->SlotObject);
			if (!Equipment2) return;
			if (Equipment1->DefaultInfo.Type != Equipment2->DefaultInfo.Type) return;

		}
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kInventoryUpdateRequest);
		out << (bool)IsEquipment;
		out << (int32_t)Index;
		out << (bool)Target->IsEquipment;
		out << (int32_t)Target->Index;
		out.CompletePacketBuild();
		UNetworkManager::GetInstance().SendPacket(out);
	}
}
