// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

bool UItem::Use()
{
	return false;
}

int UItem::GetCount()
{
	return 1;
}

int UItem::GetID()
{
	return 0;
}

EItemType UItem::GetItemType()
{
	return ItemType;
}

UTexture2D* UItem::GetIcon()
{
	return nullptr;
}