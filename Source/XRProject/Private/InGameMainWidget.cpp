#include "InGameMainWidget.h"
#include "WidgetTree.h"

UInGameMainWidget::UInGameMainWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsVisible = true;
}

void UInGameMainWidget::CreateDungeonResultWidget(InputStream & input)
{
	UDungeonResultWidget* ReslutWidget = Cast<UDungeonResultWidget>(CreateWidget(GetWorld(), DungeonResultWidget));
	ReslutWidget->GetReward(input);
	ReslutWidget->AddToViewport();
}

ULoadingBarWidget* UInGameMainWidget::CreateLoadingBarWidget()
{
	ULoadingBarWidget* ReslutWidget = Cast<ULoadingBarWidget>(CreateWidget(GetWorld(), LoadingBarWidget));
	ReslutWidget->AddToViewport();
	return ReslutWidget;
}
