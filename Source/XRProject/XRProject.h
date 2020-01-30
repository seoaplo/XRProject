// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(XRProject, Log, All);

#define XRLOG_CALLINFO	(FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__) + TEXT(")"))
#define XRLOG_S(Verbosity) UE_LOG(XRProject,Verbosity,TEXT("%s"),*XRLOG_CALLINFO)
#define XRLOG(Verbosity,Format,...) UE_LOG(XRProject,Verbosity,TEXT("%s %s"),*XRLOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
#define XRCHECK(Expr,...){if(!(Expr)) {ABLOG(Error,TEXT("ASSERTION:%s"),TEXT("'"#Expr"'")); return __VA_ARGS__; } }

