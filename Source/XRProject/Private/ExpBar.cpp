// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpBar.h"

void UExpBar::SetMaxExp(int Exp)
{
	TargetMaxExp = Exp;
	Update();
}

void UExpBar::SetCurrentExp(int Exp)
{
	TargetCurrentExp = Exp;
	Update();
}
