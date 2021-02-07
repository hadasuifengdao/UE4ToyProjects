// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFortress, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogFortressWeapon, Log, All);

#if _MSC_VER
#define FUNC_NAME    TEXT(__FUNCTION__)
#else // FIXME - GCC?
#define FUNC_NAME    TEXT(__func__)
#endif

#define NETMODE_WORLD (((GEngine == nullptr) || (GetWorld() == nullptr)) ? TEXT("") \
        : (GEngine->GetNetMode(GetWorld()) == NM_Client) ? TEXT("[Client] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_ListenServer) ? TEXT("[ListenServer] ") \
        : (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? TEXT("[DedicatedServer] ") \
        : TEXT("[Standalone] "))

/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
/*#define COLLISION_WEAPON_T1							ECC_GameTraceChannel1
#define COLLISION_WEAPON_T2							ECC_GameTraceChannel2
#define COLLISION_PROJECTILE_T1						ECC_GameTraceChannel3
#define COLLISION_PROJECTILE_T2						ECC_GameTraceChannel4
#define COLLISION_PICKUP							ECC_GameTraceChannel5

static ECollisionChannel SwitchWeaponTraceChannel(int32 id = 0)
{
	switch (id)
	{
	case 0:
		return COLLISION_WEAPON_T1;
	case 1:
		return COLLISION_WEAPON_T2;
	default:
		return COLLISION_WEAPON_T1;
	}
}
static ECollisionChannel SwitchProjectileTraceChannel(int32 id = 0)
{
	switch (id)
	{
	case 0:
		return COLLISION_PROJECTILE_T1;
	case 1:
		return COLLISION_PROJECTILE_T2;
	default:
		return COLLISION_PROJECTILE_T1;
	}
}
static TArray<ECollisionChannel> GetWeaponTraceChannelList()
{
	TArray<ECollisionChannel> List;
	List.Add(COLLISION_WEAPON_T1);
	List.Add(COLLISION_WEAPON_T2);
	return List;
}
static TArray<ECollisionChannel> GetProjectileTraceChannelList()
{
	TArray<ECollisionChannel> List;
	List.Add(COLLISION_PROJECTILE_T1);
	List.Add(COLLISION_PROJECTILE_T2);
	return List;
}
template<class Type>
static void SetWeaponTraceChannel(Type Target, ECollisionResponse Res)
{
	TArray<ECollisionChannel> WeaponList = GetWeaponTraceChannelList();
	for (int32 i = 0; i < WeaponList.Num(); i++)
	{
		Target->SetCollisionResponseToChannel(WeaponList[i], Res);
	}
}
template<class Type>
static void SetProjectileTraceChannel(Type Target, ECollisionResponse Res)
{
	TArray<ECollisionChannel> ProjectileList = GetProjectileTraceChannelList();
	for (int32 i = 0; i < ProjectileList.Num(); i++)
	{
		Target->SetCollisionResponseToChannel(ProjectileList[i], Res);
	}
}

template<class Type>
static void SetTraceChannelMask(Type Target, int32 TeamID, bool Mask = true)
{
	ECollisionResponse MaskTrue = Mask ? ECR_Ignore : ECR_Block;
	ECollisionResponse MaskFalse = Mask ? ECR_Block : ECR_Ignore;

	TArray<ECollisionChannel> WeaponList = GetWeaponTraceChannelList();
	TArray<ECollisionChannel> ProjectileList = GetProjectileTraceChannelList();
	for (int32 i = 0; i < WeaponList.Num(); i++)
	{
		Target->SetCollisionResponseToChannel(WeaponList[i], (TeamID == i) ? MaskTrue : MaskFalse);
	}
	for (int32 i = 0; i < ProjectileList.Num(); i++)
	{
		Target->SetCollisionResponseToChannel(ProjectileList[i], (TeamID == i) ? MaskTrue : MaskFalse);
	}
}*/

//Ãû³Æ×Ö·û´®ÉÏÏÞ
#define MAX_PLAYER_NAME_LENGTH						16


//
#define LEFT_HANDWP_APPEND							TEXT("_L")
#define RIGHT_HANDWP_APPEND							TEXT("_R")


//string Ó³Éä
#define PLAY_MOD_CHR								TEXT("PlayMod=")
#define TEAM_NUM_CHR								TEXT("Team_Num=")
#define BOT_LEVEL_CHR								TEXT("Bot_Level=")
#define TEAM_MEMBER_CHR								TEXT("Team_Member=")


//function

#define OPTION_INT									TEXT("%s%d")
#define GET_OPTION_INT(Base,Value,DefVal)			(Base.HasOption(Value)?FCString::Atoi(Base.GetOption(Value,NULL)):DefVal)


#define DELETE_POOL(object)							if(object) {delete object;object = NULL;};




#define GETENUMSTRING(etype, evalue)				((FindObject<UEnum>(ANY_PACKAGE, TEXT(#etype), true) != nullptr) ? \
													FindObject<UEnum>(ANY_PACKAGE, TEXT(#etype), true)->GetEnumName((int32)evalue) : \
													FString("Invalid - are you sure enum uses UENUM() macro?") )




#if !UE_BUILD_SHIPPING
//obsolete
#define ADD_SCREEN_DEBUG_L(Str,Index)				GEngine->AddOnScreenDebugMessage(Index,10.0f,FColor::Red,Str)
#define ADD_SCREEN_DEBUG_S(Str,Index)				GEngine->AddOnScreenDebugMessage(Index,1.0f,FColor::Red,Str)
#define DEBUG_CONSOLE_SCREEN(Str,Index)				GEngine->AddOnScreenDebugMessage(Index,10.0f,FColor::Red,Str);GLog->Log(Str);
#define ADD_DEBUG_RTIME_TAG(Tag,Index)				GEngine->AddOnScreenDebugMessage(Index,10.0f,FColor::Red,Tag+FString::SanitizeFloat(GetWorld()->GetRealTimeSeconds()))
#define NEXT_DEBUG_LOG(Str)							GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,Str);GLog->Log(Str);
//end obsolete

#define ADD_DEBUG_LINE_P(Start,End)					DrawDebugLine(GetWorld(),Start,End,FColor::Red,true)
#define ADD_DEBUG_LINE_T(Start,End,time)			DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,time)

#define CHECK_LINE(exr)								if(!exr){UE_LOG(LogFortress, Error, TEXT("%s ,line: %d"), ANSI_TO_TCHAR(__FILE__), __LINE__);}

#define TRACE(Format, ...) \
{ \
    SET_WARN_COLOR(COLOR_CYAN);\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (Msg == "") \
    { \
        UE_LOG(LogFortress, Log, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *GetNameSafe(this));\
    } \
    else \
    { \
        UE_LOG(LogFortress, Log, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *Msg);\
    } \
    CLEAR_WARN_COLOR();\
}

#define TRACESTATIC(Format, ...) \
{ \
    SET_WARN_COLOR(COLOR_CYAN);\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogFortress, Log, TEXT("%s() : %s"), FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

#define TRACEWARN(Format, ...) \
{ \
    SET_WARN_COLOR( COLOR_YELLOW );\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogFortress, Log, TEXT("**WARNING** %s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

#define TRACEERROR(Format, ...) \
{ \
    SET_WARN_COLOR( COLOR_RED );\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    UE_LOG(LogFortress, Log, TEXT("**ERROR** %s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

#define SCREENDEBUG(Format, ...) \
{ \
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (Msg == "") \
    { \
        TCHAR StdMsg[MAX_SPRINTF] = TEXT(""); \
        FCString::Sprintf(StdMsg, TEXT("%s%s() : %s"), NETMODE_WORLD, FUNC_NAME, *GetNameSafe(this)); \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, StdMsg); \
    } \
    else \
    { \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, Msg); \
    } \
}

#else

#define ADD_SCREEN_DEBUG_L(Str,Index)
#define ADD_SCREEN_DEBUG_S(Str,Index)

#define DEBUG_CONSOLE_SCREEN(Str,Index)

#define ADD_DEBUG_LINE_P(Start,End)
#define ADD_DEBUG_LINE_T(Start,End,time)

#define ADD_DEBUG_RTIME_TAG(Tag,Index)

#define NEXT_DEBUG_LOG(Str)

#define CHECK_LINE(exr)

#define SCREENDEBUG(Format, ...)
#define TRACEERROR(Format, ...)
#define TRACEWARN(Format, ...)
#define TRACESTATIC(Format, ...)
#define TRACE(Format, ...)

#endif

#if UE_SERVER || !UE_BUILD_SHIPPING
#define CHECK_RELEASE_SERVER						1
#else
#define CHECK_RELEASE_SERVER						0
#endif