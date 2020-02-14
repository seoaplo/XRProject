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

void USlotWidget::GetIcon()
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	XRGI->ItemManager->GetIcon(this, SlotObject->GetIconID());
}

void USlotWidget::SetSlotObject()
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	if (Index < 0 || Inventory::GetInstance().GetInventorySize() <= Index) return;
	if (IsEquipment)
	{
		if (GetMapMgr().GetPlayer())	SlotObject = GetMapMgr().GetPlayer()->GetEquippedItem((EEquipmentsType)Index);
		if (!IsEmpty())
		{
			XRGI->ItemManager->GetIcon(this, SlotObject->GetIconID());
		}
	}
	else
	{
		SlotObject = Inventory::GetInstance().GetItem(Index);
		if (!IsEmpty())
		{
			XRGI->ItemManager->GetIcon(this, SlotObject->GetIconID());
		}
	}
	Update();
}

void USlotWidget::DropIn(UUserWidget * SlotWidget)
{
	if (SlotWidget == this) return;
	USlotWidget* Target = Cast<USlotWidget>(SlotWidget);
	if (Target)
	{
		if (Target->IsEquipment && IsEquipment) return;
		if (IsEquipment && !Target->IsEquipment)
		{
			// ¿Â∫Ò ¿Â¬¯
			UItemEquipment* Equipment = Cast<UItemEquipment>(Target->SlotObject);
			if (!Equipment) return;
			if (Index != Equipment->DefaultInfo.Type) return;
		}
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kInventoryUpdateRequest);
		out << (int8)2;
		out << (int8)IsEquipment;
		out << (int32_t)Index;
		out << (int8)Target->IsEquipment;
		out << (int32_t)Target->Index;
		out.CompletePacketBuild();
		UNetworkManager::GetInstance().SendPacket(out);
	}
}
