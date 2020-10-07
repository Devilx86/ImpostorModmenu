#include "utils.h"
#include "libil2cpp.h"
#if defined(__aarch64__)
struct {
    uintptr_t colorPallete     = 0x000000000175B578;
    uintptr_t voteBanAwake     = 0x0000000000776F24;

    uintptr_t getBoughtNoAds   = 0x0000000000EF67C8;
    uintptr_t getUnlockedPets  = 0x0000000000792F1C;
    uintptr_t getUnlockedHats  = 0x0000000000792FA8;
    uintptr_t getUnlockedSkins = 0x0000000000793070;
    uintptr_t getAmBanned      = 0x0000000000678404;
    uintptr_t getClientID      = 0x00000000007414CC;

    uintptr_t pcFixedUpdate    = 0x00000000006919BC;
    uintptr_t pcSetVisible     = 0x00000000006912DC;

    uintptr_t rpcSnapTo        = 0x00000000006F4F34;
    uintptr_t rpcSetInfected   = 0x000000000068DC3C;
    uintptr_t rpcSendChat      = 0x000000000068F838;
    uintptr_t rpcSetScanner    = 0x000000000068D30C;
    uintptr_t rpcRepairSystem  = 0x0000000000EEE4F0;
    uintptr_t rpcMurderPlayer  = 0x000000000068FF50;
    uintptr_t rpcCompleteTask  = 0x000000000068D830;
    uintptr_t rpcCloseDoors    = 0x0000000000F02200;
    uintptr_t rpcPlayAnimation = 0x000000000068D598;
    uintptr_t rpcSetPlayerName = 0x000000000068F1D8;
    uintptr_t cmdReportDead    = 0x000000000068FAE8;
    uintptr_t cmdAddVote       = 0x0000000000776F78;
    uintptr_t cmdCastVote      = 0x00000000006A6FC4;
    uintptr_t cmdClearVote     = 0x00000000006A7524;
    uintptr_t cmdCheckColor    = 0x000000000068F3B0;

    uintptr_t onGameJoined     = 0x00000000007237CC;
    uintptr_t introCoBegin     = 0x0000000000749E48;
    uintptr_t chatSetVisible   = 0x000000000073A0B8;

    uintptr_t meetingHudUpdate = 0x00000000006A7AF0;
    uintptr_t netFixedUpdate   = 0x000000000073EDD4;

    uintptr_t repairSystem     = 0x0000000000EF38B0;


} offsets;
#else if defined(__ARM_ARCH_7A__)
struct {
    uintptr_t colorPallete     = 0x0114E600;
    uintptr_t voteBanAwake     = 0x0049DD70;

    uintptr_t getBoughtNoAds   = 0x00D57CEC;
    uintptr_t getUnlockedPets  = 0x004BE64C;
    uintptr_t getUnlockedHats  = 0x004BE6E8;
    uintptr_t getUnlockedSkins = 0x004BE7DC;
    uintptr_t getAmBanned      = 0x003611B0;
    uintptr_t getClientID      = 0x0045A498;

    uintptr_t pcFixedUpdate    = 0x00380CC4;
    uintptr_t pcSetVisible     = 0x0038043C;

    uintptr_t rpcSnapTo        = 0x003FA67C;
    uintptr_t rpcSetInfected   = 0x0037BC6C;
    uintptr_t rpcSendChat      = 0x0037E158;
    uintptr_t rpcSetScanner    = 0x0037B050;
    uintptr_t rpcRepairSystem  = 0x00D4D464;
    uintptr_t rpcMurderPlayer  = 0x0037EAC8;
    uintptr_t rpcCompleteTask  = 0x0037B720;
    uintptr_t rpcCloseDoors    = 0x00D66364;
    uintptr_t rpcPlayAnimation = 0x0037B3C0;
    uintptr_t rpcSetPlayerName = 0x0037D914;
    uintptr_t cmdReportDead    = 0x0037E520;
    uintptr_t cmdAddVote       = 0x0049DDCC;
    uintptr_t cmdCastVote      = 0x0039B9C4;
    uintptr_t cmdClearVote     = 0x0039C0CC;
    uintptr_t cmdCheckColor    = 0x0037DBA8;

    uintptr_t onGameJoined     = 0x00430B8C;
    uintptr_t introCoBegin     = 0x0046504C;
    uintptr_t chatSetVisible   = 0x00452624;

    uintptr_t meetingHudUpdate = 0x0039C880;
    uintptr_t netFixedUpdate   = 0x004572E4;

    uintptr_t repairSystem     = 0x00D53F44;


} offsets;
#endif

typedef void CustomNetworkTransform_RpcSnapTo(CustomNetworkTransform_o *instance, UnityEngine_Vector2_o position);
typedef void PlayerControl_setVisible(PlayerControl_o *instance, bool isVisible);
typedef void PlayerControl_RpcSetScanner(PlayerControl_o *instance, bool value);
typedef void VoteBanSystem_CmdAddVote(VoteBanSystem_o *instance, int32_t clientId);
typedef void MeetingHud_RpcClearVote(MeetingHud_o *instance, int32_t clientId);
typedef void MeetingHud_CmdCastVote(MeetingHud_o *instance, uint8_t playerId, int8_t suspectIdx);
typedef int32_t InnerNetClient_GetClientIdFromCharacter(InnerNet_InnerNetClient_o *instance, InnerNet_InnerNetObject_o *character);
typedef void ShipStatus_RpcRepairSystem(ShipStatus_o *instance, uint8_t systemType, int32_t amount);
typedef void ShipStatus_RepairSystem(ShipStatus_o *instance, uint8_t systemType, PlayerControl_o* player, int32_t amount);
typedef void PlayerControl_CmdReportDeadBody(PlayerControl_o *instance, GameData_PlayerInfo_o *target);
typedef void PlayerControl_RpcMurderPlayer(PlayerControl_o *instance, PlayerControl_o *target);
typedef void PlayerControl_RpcCompleteTask(PlayerControl_o *instance, uint32_t idx);
typedef void ShipStatus_RpcCloseDoorsOfType(ShipStatus_o *instance, uint8_t type);
typedef void PlayerControl_RpcSetName(PlayerControl_o *instance, System_String_o *name);

//typedef bool PlayerControl_RpcSendChat(PlayerControl_o *instance, System_String_o *chatText);

typedef void PlayerControl_RpcPlayAnimation(PlayerControl_o *instance, uint8_t animType);
typedef void PlayerControl_CmdCheckColor(PlayerControl_o *instance, uint8_t bodyColor);

//PlayerControl_RpcSendChat             *RpcSendChat;
InnerNetClient_GetClientIdFromCharacter *getClientIdFromCharacter;
PlayerControl_setVisible                *pcSetVisible;

CustomNetworkTransform_RpcSnapTo        *RpcSnapTo;
PlayerControl_RpcSetScanner             *rpcSetScanner;
PlayerControl_RpcMurderPlayer           *rpcMurderPlayer;
PlayerControl_RpcCompleteTask           *rpcCompleteTask;
ShipStatus_RpcCloseDoorsOfType          *rpcCloseDoorsOfType;
ShipStatus_RpcRepairSystem              *rpcRepairSystem;
PlayerControl_RpcPlayAnimation          *rpcPlayAnimation;
PlayerControl_RpcSetName                *rpcSetName;

ShipStatus_RepairSystem                 *repairSystem;
//PlayerPhysics_RpcEnterVent              *rpcEnterVent;   // client side only..
//PlayerPhysics_RpcExitVent               *rpcExitVent;


PlayerControl_CmdReportDeadBody         *cmdReportDead;
VoteBanSystem_CmdAddVote                *cmdAddVote;
MeetingHud_CmdCastVote                  *cmdCastVote;
MeetingHud_RpcClearVote                 *cmdClearVote;
PlayerControl_CmdCheckColor             *cmdCheckColor;


void initFunction() {

    //RpcSendChat   = (PlayerControl_RpcSendChat *) getAbsoluteAddress(offsets.rpcSendChat);
    cmdAddVote      = (VoteBanSystem_CmdAddVote *) getAbsoluteAddress(offsets.cmdAddVote);
    cmdClearVote    = (MeetingHud_RpcClearVote*) getAbsoluteAddress(offsets.cmdClearVote);
    cmdCastVote     = (MeetingHud_CmdCastVote *) getAbsoluteAddress(offsets.cmdCastVote);
    cmdCheckColor   = (PlayerControl_CmdCheckColor*) getAbsoluteAddress(offsets.cmdCheckColor);

    pcSetVisible    = (PlayerControl_setVisible *) getAbsoluteAddress(offsets.pcSetVisible);
    getClientIdFromCharacter = (InnerNetClient_GetClientIdFromCharacter *) getAbsoluteAddress(offsets.getClientID);

    RpcSnapTo       = (CustomNetworkTransform_RpcSnapTo *) getAbsoluteAddress(offsets.rpcSnapTo);
    rpcSetName      = (PlayerControl_RpcSetName*) getAbsoluteAddress(offsets.rpcSetPlayerName);
    rpcSetScanner   = (PlayerControl_RpcSetScanner *) getAbsoluteAddress(offsets.rpcSetScanner);
    rpcRepairSystem = (ShipStatus_RpcRepairSystem *) getAbsoluteAddress(offsets.rpcRepairSystem);
    repairSystem    = (ShipStatus_RepairSystem *) getAbsoluteAddress(offsets.repairSystem);
    cmdReportDead   = (PlayerControl_CmdReportDeadBody *) getAbsoluteAddress(offsets.cmdReportDead);
    rpcMurderPlayer = (PlayerControl_RpcMurderPlayer *) getAbsoluteAddress(offsets.rpcMurderPlayer);
    rpcCompleteTask = (PlayerControl_RpcCompleteTask *) getAbsoluteAddress(offsets.rpcCompleteTask);
    rpcCloseDoorsOfType = (ShipStatus_RpcCloseDoorsOfType *) getAbsoluteAddress(offsets.rpcCloseDoors);

    rpcPlayAnimation = (PlayerControl_RpcPlayAnimation *) getAbsoluteAddress(offsets.rpcPlayAnimation);
}