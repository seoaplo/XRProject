#pragma once

#include <map>
#include <list>

#include "CoreMinimal.h"
#include "Image.h"
#include "Border.h"
#include "CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

enum class EMiniMapObjectType
{
	//EPlayer = 0,
	EParty = 1,
	EEnemy = 2,
	EBoss = 3,
};

UCLASS()
class XRPROJECT_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	AActor* MyCharacter;
	std::list<AActor*> ActorList;
private:
	UImage* MyIcon;
	std::map<UImage*, AActor*> ActorIconList;
	static UMiniMapWidget* MiniMapInstance;
public:
	static UMiniMapWidget* GetInstance() { return MiniMapInstance; };
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		int ZoomLevel;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		int MapID;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FVector2D ImageSize;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		float ImageRate;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FVector2D MapImageSize; // 이미지의 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FVector2D MapImageWorldSize; // 이미지가 표현하는 영역의 월드상 크디
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UCanvasPanel* Canvas;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UBorder* ImageBorder;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FSlateBrush MyIconBrush;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FSlateBrush PartyIconBrush;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FSlateBrush EnemyIconBrush;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		FSlateBrush BossIconBrush;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Widget")
		void SetMapID(int ID);
	UFUNCTION(BlueprintCallable, Category = "C_Widget")
		void ListClear();
	UFUNCTION(BlueprintCallable, Category = "C_Widget")
		void SetMyCharacter(AActor* MyActor);
	UFUNCTION(BlueprintCallable, Category = "C_Widget")
		void AddActorList(AActor* Actor, int Type);
public:
	UFUNCTION(BlueprintCallable, Category = "C_Widget")
		void Update();
	UFUNCTION(BlueprintImplementableEvent)
		void SetMapImage(int ID);

};









