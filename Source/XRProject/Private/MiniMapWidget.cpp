//#include "MiniMapWidget.h"
//#include "CanvasPanelSlot.h"
//#include "WidgetTree.h"
//
//UMiniMapWidget* UMiniMapWidget::MiniMapInstance = nullptr;
//
//void UMiniMapWidget::SetMapID(int ID)
//{
//	MiniMapInstance = this;
//	ImageSize = FVector2D(0.0f, 0.0f);
//	switch (ID)
//	{
//	case 100:
//		ImageSize.X = 2200;
//		ImageSize.Y = 2200;
//		ImageRate = 5.29054;
//		break;
//	case 111:
//		ImageSize.X = 2200;
//		ImageSize.Y = 2200;
//		ImageRate = 12.4583f;
//		break;
//	case 112:
//		ImageSize.X = 2200;
//		ImageSize.Y = 2200;
//		ImageRate = 17.67647f;
//		break;
//	case 113:
//		ImageSize.X = 2200;
//		ImageSize.Y = 2200;
//		ImageRate = 26.09375f;
//		break;
//	default:
//		break;
//	}
//	SetMapImage(ID);
//}
//
//void UMiniMapWidget::ListClear()
//{
//	ZoomLevel = 1;
//
//	MyCharacter = nullptr;
//	if (MyIcon) MyIcon->RemoveFromParent(); MyIcon = nullptr;
//
//	ActorList.clear();
//	for (auto Temp : ActorIconList)
//	{
//		Temp.first->RemoveFromParent();
//	}
//	ActorIconList.clear();
//}
//
//void UMiniMapWidget::SetMyCharacter(AActor* MyActor)
//{
//	if (!MyActor) return;
//	MyCharacter = MyActor;
//	MyIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
//	Canvas->AddChild(MyIcon);
//	UCanvasPanelSlot* CSlot = Cast<UCanvasPanelSlot>(MyIcon->Slot);
//	if (CSlot)
//	{
//		CSlot->SetSize(FVector2D(26, 26));
//		CSlot->SetPosition(FVector2D(0, 0));
//		CSlot->SetAlignment(FVector2D(0.5f, 0.5f));
//		FAnchors Anchor;
//		Anchor.Minimum = FVector2D(0.5f, 0.5f);
//		Anchor.Maximum = FVector2D(0.5f, 0.5f);
//		CSlot->SetAnchors(Anchor);
//	}
//	MyIcon->SetBrush(MyIconBrush);
//}
//
//void UMiniMapWidget::AddActorList(AActor * Actor, int Type)
//{
//	if (!Actor) return;
//	ActorList.push_back(Actor);
//	UImage* ActorIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
//	Canvas->AddChild(ActorIcon);
//	switch ((EMiniMapObjectType)Type)
//	{
//	case EMiniMapObjectType::EParty:
//		ActorIcon->SetBrush(PartyIconBrush);
//		break;
//	case EMiniMapObjectType::EEnemy:
//		ActorIcon->SetBrush(EnemyIconBrush);
//		break;
//	case EMiniMapObjectType::EBoss:
//		ActorIcon->SetBrush(BossIconBrush);
//		break;
//	default:
//		break;
//	}
//	UCanvasPanelSlot* CSlot = Cast<UCanvasPanelSlot>(ActorIcon->Slot);
//	if (CSlot)
//	{
//		CSlot->SetPosition(FVector2D(0, 0));
//		CSlot->SetAlignment(FVector2D(0.5f, 0.5f));
//		FAnchors Anchor;
//		Anchor.Minimum = FVector2D(0.5f, 0.5f);
//		Anchor.Maximum = FVector2D(0.5f, 0.5f);
//		CSlot->SetAnchors(Anchor);
//		switch ((EMiniMapObjectType)Type)
//		{
//		case EMiniMapObjectType::EParty:
//			CSlot->SetSize(FVector2D(26, 26));
//			ActorIcon->SetBrush(PartyIconBrush);
//			break;
//		case EMiniMapObjectType::EEnemy:
//			CSlot->SetSize(FVector2D(16, 16));
//			ActorIcon->SetBrush(EnemyIconBrush);
//			break;
//		case EMiniMapObjectType::EBoss:
//			CSlot->SetSize(FVector2D(22, 22));
//			ActorIcon->SetBrush(BossIconBrush);
//			break;
//		default:
//			break;
//		}
//	}
//	ActorIconList.insert(std::make_pair(ActorIcon, Actor));
//}
//
//void UMiniMapWidget::Update()
//{
//	if (!MyCharacter) return;
//	FVector Center = MyCharacter->GetActorLocation();
//	float ZoomRate = 10 * ZoomLevel;
//	for (auto ITER = ActorIconList.begin(); ITER != ActorIconList.end(); ITER++)
//	{
//		if ((*ITER).second)
//		{
//			UCanvasPanelSlot* CSlot = Cast<UCanvasPanelSlot>((*ITER).first->Slot);
//			if (CSlot)
//			{
//				FVector Position = Center - (*ITER).second->GetActorLocation();
//				CSlot->SetPosition(FVector2D(-Position / ZoomRate));
//			}
//		}
//		else
//		{
//			auto temp = ITER;
//			ITER--;
//			ActorIconList.erase((*temp).first);
//		}
//	}
//	if (ImageBorder)
//	{
//		ImageBorder->SetRenderScale(FVector2D(ImageSize / 300 * ImageRate / ZoomRate));
//		ImageBorder->SetRenderTranslation(FVector2D(-Center.X / ZoomRate, -Center.Y / ZoomRate));
//	}
//}
//
//
//
//
