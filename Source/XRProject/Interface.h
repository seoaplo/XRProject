// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OutputStream;
class InputStream;

class NETWORKMODULE_API SerializeInterface
{
public:
    SerializeInterface();
	virtual ~SerializeInterface();

    virtual void Write(OutputStream& stream) const = 0;
    virtual void Read(InputStream& stream) = 0;
};