#include "ChatingWidget.h"



UChatingWidget* UChatingWidget::ChatingWidgetInstance = nullptr;

UChatingWidget::UChatingWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	this->bIsFocusable = true;
}

FLinearColor UChatingWidget::GetColorFormType(TextType Type)
{
	switch (Type)
	{
	case TextType::SystemMessage:
	{
		return FLinearColor(0.0f, 0.8f, 0.7f);
		break;
	}
	case TextType::NormalMessage:
	{
		return FLinearColor(0.95f, 0.95f, 0.95f);
		break;
	}
	default:
		break;
	}
	return FLinearColor(0.8f, 0.15f, 0.21f);
}

void UChatingWidget::ReciveChat(TextType Type, std::wstring Chat, bool IsEraseLastText)
{
	FString ChatString(Chat.c_str());
	FText ChatText = FText::FromString(ChatString);
	AddText(GetColorFormType(Type), ChatText);
	if (IsEraseLastText) EraseLastText();
}


void UChatingWidget::SendChat(FText Chat)
{
	if (Chat.IsEmpty()) return;
	std::wstring Str(*Chat.ToString());
	ChatingManager::GetInstance().SendChat(Str);
}

void UChatingWidget::Setup()
{
	UChatingWidget::ChatingWidgetInstance = this;
	for (auto Temp : ChatingManager::GetInstance().ChatList)
	{
		FText TextStr;
		TextType Type = Temp.Type;
		TextStr.FromString(Temp.Data.c_str());
		AddText(GetColorFormType(Type), TextStr);
	}
}