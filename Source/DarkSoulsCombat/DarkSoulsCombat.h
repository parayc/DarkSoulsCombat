// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(DarkSoulsCombat, Log, All);

#define DSLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define DSLOG_S(Verbosity) UE_LOG(DarkSoulsCombat, Verbosity, TEXT("%s"), *DSLOG_CALLINFO)

#define DSLOG(Verbosity, Format, ...) UE_LOG(DarkSoulsCombat, Verbosity, TEXT("%s %s"), *DSLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


#define DSCHECK(Expr, ...) { if(!(Expr)) { DSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;} }

//DECLARE_LOG_CATEGORY_EXTERN(DarkSoulsCombat, Log, All);