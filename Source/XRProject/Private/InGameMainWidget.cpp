#include "InGameMainWidget.h"
#include "WidgetTree.h"

UInGameMainWidget::UInGameMainWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsVisible = true;
}

void UInGameMainWidget::CreateDungeonResultWidget(InputStream & input)
{
	UDungeonResultWidget* ReslutWidget = WidgetTree->ConstructWidget<UDungeonResultWidget>(DungeonResultWidget);
	ReslutWidget->GetReward(input);
	ReslutWidget->AddToViewport();
}

ULoadingBarWidget* UInGameMainWidget::CreateLoadingBarWidget()
{
	ULoadingBarWidget* ReslutWidget = WidgetTree->ConstructWidget<UDungeonResultWidget>(LoadingBarWidget);
	ReslutWidget->GetReward(input);
	ReslutWidget->AddToViewport();
	return ReslutWidget;
}
