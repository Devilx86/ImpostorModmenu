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
    uintptr_t getAmHost        = 0x000000000073F184;

    uintptr_t pcFixedUpdate    = 0x00000000006919BC;
    uintptr_t pcSetVisible     = 0x00000000006912DC;
    uintptr_t pcSetPlayerName  = 0x000000000068F298;
    uintptr_t pcGetCanMove     = 0x0000000000689914;
    uintptr_t pcMurderPlayer   = 0x0000000000690020;

    uintptr_t pPhyFixedUpdate  = 0x00000000006962EC;

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
    uintptr_t rpcSetSkin       = 0x000000000068EC44;
    uintptr_t rpcSetHat        = 0x000000000068EDC4;
    uintptr_t rpcSetPet        = 0x000000000068A764;
    uintptr_t rpcSetColor      = 0x000000000068F5EC;

    uintptr_t cmdReportDead    = 0x000000000068FAE8;
    uintptr_t cmdAddVote       = 0x0000000000776F78;
    uintptr_t cmdCastVote      = 0x00000000006A6FC4;
    uintptr_t cmdClearVote     = 0x00000000006A7524;
    uintptr_t cmdCheckColor    = 0x000000000068F3B0;

    uintptr_t onGameJoined     = 0x00000000007237CC;
    uintptr_t introCoBegin     = 0x0000000000749E48;

    uintptr_t chatSetVisible   = 0x000000000073A0B8;
    uintptr_t chatCharAllowed  = 0x00000000006858B4;
    uintptr_t textBoxUpdate    = 0x0000000000685480;
    uintptr_t lightSourceUpdate = 0x000000000069F0D4;

    uintptr_t meetingHudUpdate = 0x00000000006A7AF0;
    uintptr_t netFixedUpdate   = 0x000000000073EDD4;
    uintptr_t shipFixedUpdate  = 0x0000000000F041E0;

    uintptr_t repairSystem     = 0x0000000000EF38B0;

    uintptr_t unityCompGetGameObj = 0x0000000000AEE5D8;
    uintptr_t unityNameToLayer    = 0x0000000000AF630C;
    uintptr_t unitySetLayer       = 0x0000000000AF5874;
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
    uintptr_t getAmHost        = 0x004577B4;

    uintptr_t pcFixedUpdate    = 0x00380CC4;
    uintptr_t pcSetVisible     = 0x0038043C;
    uintptr_t pcSetPlayerName  = 0x0037DA24;
    uintptr_t pcGetCanMove     = 0x00376508;
    uintptr_t pcMurderPlayer   = 0x0037EBD4;

    uintptr_t pPhyFixedUpdate  = 0x00386D94;

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
    uintptr_t rpcSetSkin       = 0x0037D1C4;
    uintptr_t rpcSetPet        = 0x003777A8;
    uintptr_t rpcSetHat        = 0x0037D3C0;
    uintptr_t rpcSetColor      = 0x0037DE7C;

    uintptr_t cmdReportDead    = 0x0037E520;
    uintptr_t cmdAddVote       = 0x0049DDCC;
    uintptr_t cmdCastVote      = 0x0039B9C4;
    uintptr_t cmdClearVote     = 0x0039C0CC;
    uintptr_t cmdCheckColor    = 0x0037DBA8;

    uintptr_t onGameJoined     = 0x00430B8C;
    uintptr_t introCoBegin     = 0x0046504C;

    uintptr_t chatSetVisible   = 0x00452624;
    uintptr_t chatCharAllowed  = 0x00372414;
    uintptr_t textBoxUpdate    = 0x00371EEC;
    uintptr_t lightSourceUpdate = 0x00391160;

    uintptr_t meetingHudUpdate = 0x0039C880;
    uintptr_t netFixedUpdate   = 0x004572E4;
    uintptr_t shipFixedUpdate  = 0x00D68BB4;

    uintptr_t repairSystem     = 0x00D53F44;

    uintptr_t unityCompGetGameObj = 0x008A948C;
    uintptr_t unityNameToLayer    = 0x008B2D9C;
    uintptr_t unitySetLayer       = 0x008B218C;
} offsets;
#endif

typedef void CustomNetworkTransform_RpcSnapTo(CustomNetworkTransform_o *instance, UnityEngine_Vector2_o position);
typedef void PlayerControl_setVisible(PlayerControl_o *instance, bool isVisible);
typedef void ShipStatus_ReviveEveryone();
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
typedef void PlayerControl_SetName(PlayerControl_o *instance, System_String_o *name);
typedef void PlayerControl_RpcSetName(PlayerControl_o *instance, System_String_o *name);
typedef void PlayerControl_RpcSetSkin(PlayerControl_o *instance, uint32_t skinId);
typedef void PlayerControl_RpcSetPet(PlayerControl_o *instance, uint32_t petID);
typedef void PlayerControl_RpcSetHat(PlayerControl_o *instance, uint32_t hatID);
typedef void PlayerControl_RpcSetColor(PlayerControl_o *instance, uint32_t colorID);
typedef bool InnerNetClient_getAmHost(InnerNet_InnerNetClient_o *instance);

typedef void ShipStatus_FixedUpdate(ShipStatus_o *instance);

typedef void PlayerControl_RpcPlayAnimation(PlayerControl_o *instance, uint8_t animType);
typedef void PlayerControl_CmdCheckColor(PlayerControl_o *instance, uint8_t bodyColor);
typedef void PlayerControl_RpcSetColor(PlayerControl_o *instance, uint32_t colorID);

typedef UnityEngine_GameObject_o *UnityEngine_Component_getGameObject(UnityEngine_Component_o *instance);
typedef int32_t UnityEngine_LayerMask_NameToLayer(System_String_o *layerName);
typedef void UnityEngine_SetLayerRecursively(UnityEngine_GameObject_o *go, int32_t layer);

typedef System_Text_StringBuilder_o *StringBuilder_AppendLine(System_Text_StringBuilder_o *instance, System_String_o *value);


//PlayerControl_RpcSendChat             *RpcSendChat;
InnerNetClient_GetClientIdFromCharacter *getClientIdFromCharacter;
InnerNetClient_getAmHost                *getAmHost;
PlayerControl_setVisible                *pcSetVisible;

CustomNetworkTransform_RpcSnapTo        *RpcSnapTo;
PlayerControl_RpcSetScanner             *rpcSetScanner;
PlayerControl_RpcMurderPlayer           *rpcMurderPlayer;
PlayerControl_RpcCompleteTask           *rpcCompleteTask;
ShipStatus_RpcCloseDoorsOfType          *rpcCloseDoorsOfType;
ShipStatus_RpcRepairSystem              *rpcRepairSystem;
PlayerControl_RpcPlayAnimation          *rpcPlayAnimation;

PlayerControl_SetName                   *pcSetName;
PlayerControl_RpcSetName                *rpcSetName;
ShipStatus_ReviveEveryone               *reviveAll;
PlayerControl_RpcSetSkin                *rpcSetSkin;
PlayerControl_RpcSetPet                 *rpcSetPet;
PlayerControl_RpcSetHat                 *rpcSetHat;
PlayerControl_RpcSetColor               *rpcSetColor;

ShipStatus_RepairSystem                   *repairSystem;
//PlayerPhysics_RpcEnterVent              *rpcEnterVent;   // client side only..
//PlayerPhysics_RpcExitVent               *rpcExitVent;

PlayerControl_CmdReportDeadBody         *cmdReportDead;
VoteBanSystem_CmdAddVote                *cmdAddVote;
MeetingHud_CmdCastVote                  *cmdCastVote;
MeetingHud_RpcClearVote                 *cmdClearVote;
PlayerControl_CmdCheckColor             *cmdCheckColor;
ShipStatus_FixedUpdate                  *shipFixedUpdate;

UnityEngine_Component_getGameObject     *getGameObject;
UnityEngine_LayerMask_NameToLayer       *getLayerWithName;
UnityEngine_SetLayerRecursively         *setLayer;

StringBuilder_AppendLine                *appendLine;

void initFunction() {

    //RpcSendChat   = (PlayerControl_RpcSendChat *) getAbsoluteAddress(offsets.rpcSendChat);
    cmdAddVote      = (VoteBanSystem_CmdAddVote *) getAbsoluteAddress(offsets.cmdAddVote);
    cmdClearVote    = (MeetingHud_RpcClearVote *) getAbsoluteAddress(offsets.cmdClearVote);
    cmdCastVote     = (MeetingHud_CmdCastVote *) getAbsoluteAddress(offsets.cmdCastVote);
    cmdCheckColor   = (PlayerControl_CmdCheckColor *) getAbsoluteAddress(offsets.cmdCheckColor);

    getClientIdFromCharacter = (InnerNetClient_GetClientIdFromCharacter *) getAbsoluteAddress(offsets.getClientID);
    pcSetVisible    = (PlayerControl_setVisible *) getAbsoluteAddress(offsets.pcSetVisible);
    getAmHost       = (InnerNetClient_getAmHost *) getAbsoluteAddress(offsets.getAmHost);

    RpcSnapTo       = (CustomNetworkTransform_RpcSnapTo *) getAbsoluteAddress(offsets.rpcSnapTo);
    pcSetName       = (PlayerControl_SetName*) getAbsoluteAddress(offsets.pcSetPlayerName);
    rpcSetName      = (PlayerControl_RpcSetName*) getAbsoluteAddress(offsets.rpcSetPlayerName);
    rpcSetSkin      = (PlayerControl_RpcSetSkin*) getAbsoluteAddress(offsets.rpcSetSkin);
    rpcSetPet       = (PlayerControl_RpcSetPet*) getAbsoluteAddress(offsets.rpcSetPet);
    rpcSetHat       = (PlayerControl_RpcSetPet*) getAbsoluteAddress(offsets.rpcSetHat);
    rpcSetColor       = (PlayerControl_RpcSetColor*) getAbsoluteAddress(offsets.rpcSetColor);
    rpcSetScanner   = (PlayerControl_RpcSetScanner *) getAbsoluteAddress(offsets.rpcSetScanner);
    rpcRepairSystem = (ShipStatus_RpcRepairSystem *) getAbsoluteAddress(offsets.rpcRepairSystem);
    repairSystem    = (ShipStatus_RepairSystem *) getAbsoluteAddress(offsets.repairSystem);
    cmdReportDead   = (PlayerControl_CmdReportDeadBody *) getAbsoluteAddress(offsets.cmdReportDead);
    rpcMurderPlayer = (PlayerControl_RpcMurderPlayer *) getAbsoluteAddress(offsets.rpcMurderPlayer);
    rpcCompleteTask = (PlayerControl_RpcCompleteTask *) getAbsoluteAddress(offsets.rpcCompleteTask);
    rpcCloseDoorsOfType = (ShipStatus_RpcCloseDoorsOfType *) getAbsoluteAddress(offsets.rpcCloseDoors);

    rpcPlayAnimation = (PlayerControl_RpcPlayAnimation *) getAbsoluteAddress(offsets.rpcPlayAnimation);

    getGameObject    = (UnityEngine_Component_getGameObject *) getAbsoluteAddress(offsets.unityCompGetGameObj);
    getLayerWithName = (UnityEngine_LayerMask_NameToLayer *) getAbsoluteAddress(offsets.unityNameToLayer);
    setLayer         = (UnityEngine_SetLayerRecursively *) getAbsoluteAddress(offsets.unitySetLayer);
}