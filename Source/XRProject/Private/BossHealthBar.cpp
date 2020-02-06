#include "BossHealthBar.h"

UBossHealthBar::UBossHealthBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UBossHealthBar::SetTargetNPC(ANonePlayerCharacter * Target)
{
	Owner = Target;
}

void UBossHealthBar::GetName(ANonePlayerCharacter * Target)
{
	Owner;
}

void UBossHealthBar::SetMaxHp(int PlayerMaxHP)
{
	TargetMaxHP = PlayerMaxHP;
	TargetCurrentHP = PlayerMaxHP;
}
