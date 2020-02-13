#include "MainMenuWidget.h"
#include "Engine.h" 
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"

TArray<FVector2D> UMainMenuWidget::GetSizeList()
{
	TArray<FVector2D> Result;
	FVector2D OldSize;
	GEngine->GameViewport->GetViewportSize(OldSize);
	GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	FVector2D viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);
	Result.Add(viewportSize);
	int32 X = FGenericPlatformMath::FloorToInt(viewportSize.X);
	int32 Y = FGenericPlatformMath::FloorToInt(viewportSize.Y);
	auto asd2 = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(X, Y));
	//auto OldView = GEngine->GameViewport->Viewport;
	GEngine->GameViewport->Viewport->MoveWindow(OldSize.X / 2, OldSize.Y / 2, OldSize.X, OldSize.Y);
	GEngine->GameViewport->Viewport->MoveWindow(250, 250, 500, 500);
	return Result;
}