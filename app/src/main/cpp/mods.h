#if defined(__aarch64__)
#include "And64InlineHook/And64InlineHook.hpp"
#else if defined(__ARM_ARCH_7A__)
#include "Substrate/CydiaSubstrate.h"
#endif

#include "func.h"

bool noEmergencyCoolDown = false;
bool teleportAlltoMe = false;
bool getPlayersList = false;
bool forceImpostor = false;
bool showImpostors = false;
bool fakeImpostor = false;
bool hasGameBegan = false;
bool wasImpostor = false;
bool noKillTime = false;
bool freezeLobby = false;
bool murderAsMe = false;
bool murderImpostors = false;
bool sabotageAll = false;
bool showGhosts = true;
bool lockAllDoors = false;
bool attachToPlayer = false;
bool advertiseMenu = false;
bool attachPlayerToMe = false;

bool murderLobby = false;
bool murderLobbyAsMe = false;
bool repairLoop = false;

bool rainbowColor = false;

AmongUsClient_o *amongUsClient = NULL;
VoteBanSystem_o *vbInstance = NULL;
Palette_c *classPalette = NULL;

System_String_o *msg = new System_String_o;

PlayerControl_array *players;

PlayerControl_o *teleportToPlayer = NULL;
PlayerControl_o *teleportToMe = NULL;
PlayerControl_o *freezePlayer = NULL;
PlayerControl_o *voteKickPlayer = NULL;
PlayerControl_o *castVotePlayer = NULL;
PlayerControl_o *teleportAllToPlayer = NULL;
PlayerControl_o *medScanPlayer = NULL;
PlayerControl_o *reportDeadPlayer = NULL;
PlayerControl_o *murderPlayer = NULL;
PlayerControl_o *completePlayerTasks = NULL;
PlayerControl_o *attachLobbyToPlayer = NULL;
PlayerControl_o *replacePlayer = NULL;

PlayerControl_o *spoofChatPlayer = NULL;
std::string message;

PlayerControl_o *impostors[3] = {NULL, NULL, NULL};

int sabotageOptions[5] = {0x3, // reactor
                          0x7, // lights
                          0x8, // oxygen
                          0xE, // comms
                          0xF // Seismic
};
int sabotageLoop = -1;
int playShipAnimation = -1;

double attachLobbyTime;
double colorStartTime;
double freezeLobbyTimer;
double sabotageTimer;

uint8_t colorIndex = 0;

static double get_now_ms(void) {
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}
static double get_delta(double start, double end) {
    return end - start;
}

void (*old_ChatController_SetVisible)(void *instance, bool visible);
void ChatController_SetVisible(void *instance, bool visible) {
    old_ChatController_SetVisible(instance, true);
}

void (*old_PlayerControl_RpcSendChat)(PlayerControl_o *instance, System_String_o *chatText);
void PlayerControl_RpcSendChat(PlayerControl_o *instance, System_String_o *chatText)
{
    if(spoofChatPlayer != NULL){
        instance = spoofChatPlayer;
    }
    old_PlayerControl_RpcSendChat(instance, chatText);
}

// Function Only called when you are host
void (*old_PlayerControl_RpcSetInfected)(PlayerControl_o *instance, GameData_PlayerInfo_array *infected);
void PlayerControl_RpcSetInfected(PlayerControl_o *instance, GameData_PlayerInfo_array *infected)
{
    // Replace the first impostor with our handle
    if(forceImpostor)
        infected->m_Items[0] = instance->_cachedData;

    old_PlayerControl_RpcSetInfected(instance, infected);
}

void ModFixedUpdate(PlayerControl_o *instance) {

    PlayerControl_o *localPlayer = instance->klass->static_fields->LocalPlayer;

    if (getPlayersList) {
        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        if (pArray != NULL) {
            players = pArray;
        }
        getPlayersList = false;
    }

    if(noEmergencyCoolDown) {
        instance->klass->static_fields->GameOptions->EmergencyCooldown = 0;
        instance->klass->static_fields->GameOptions->NumEmergencyMeetings = 6666;
    }

    // Game has started
    if (hasGameBegan) {
        if (noKillTime) {
            if (localPlayer->killTimer > 0)
                localPlayer->killTimer = 0;
        }

        localPlayer->_cachedData->IsImpostor = wasImpostor;

        if (fakeImpostor) {
            localPlayer->_cachedData->IsImpostor = true;
        }

        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        if (pArray != NULL) {
            for (int i = 0; i < pArraySize; i++) {
                // Show impostor
                if(pArray->m_Items[i]->_cachedData->IsImpostor) {
                    if (impostors[0] == NULL) {
                        impostors[0] = pArray->m_Items[i];
                    }
                    else if (impostors[1] == NULL) {
                        impostors[1] = pArray->m_Items[i];
                    }
                    else if (impostors[2] == NULL) {
                        impostors[2] = pArray->m_Items[i];
                    }

                    if (!wasImpostor) {
                        pArray->m_Items[i]->nameText->Color = classPalette->static_fields->White;
                        if (showImpostors) {
                            pArray->m_Items[i]->nameText->Color = classPalette->static_fields->ImpostorRed;
                            localPlayer->nameText;

                        }
                    }
                }
                // Show Ghosts
                if (showGhosts && pArray->m_Items[i]->_cachedData->IsDead) {
                    pcSetVisible(pArray->m_Items[i], true);
                }

                if (murderLobby) {
                    // me Killing every crewmate
                    if (murderLobbyAsMe && wasImpostor) {
                        if (pArray->m_Items[i] != localPlayer)
                            if (!pArray->m_Items[i]->_cachedData->IsImpostor)
                                rpcMurderPlayer(localPlayer,
                                                pArray->m_Items[i]);
                    } else if(!murderAsMe) {
                        // An impostor killing every crew mate including me
                        // so that I can call him a hacker
                        for (int j = 0; j < 3; j++) {
                            if (impostors[j] != NULL &&
                                impostors[j] != localPlayer &&
                                impostors[j]->_cachedData->IsDead == false)
                                if (pArray->m_Items[i] != impostors[j]) {
                                    std::string killer = readUint162Str(&impostors[j]->_cachedData->PlayerName->m_firstChar,
                                                                        impostors[j]->_cachedData->PlayerName->m_stringLength);


                                    std::string victim = readUint162Str(&pArray->m_Items[i]->_cachedData->PlayerName->m_firstChar,
                                                                        impostors[j]->_cachedData->PlayerName->m_stringLength);
                                    LOGD("%s Killing %s", killer.data(), victim.data());
                                    rpcMurderPlayer(impostors[j], pArray->m_Items[i]);
                                    break;
                                }
                        }
                    }
                }
            }
            murderLobby = false;
            murderLobbyAsMe = false;
        }

        if (murderImpostors) {
            // Make the impostors kill themselves
            for (int j = 0; j < 3; j++) {
                if (impostors[j] != NULL)
                    rpcMurderPlayer(impostors[j], impostors[j]);
            }
            murderImpostors = false;
        }

        if(sabotageAll) {
            ShipStatus_o *shipInstance = amongUsClient->ShipPrefabs->_items->m_Items[0]->klass->static_fields->Instance;
            for(int i = 0; i < 5; i++)
                repairSystem(shipInstance, sabotageOptions[i], localPlayer, 0x80);

            rpcRepairSystem(shipInstance, 0x7, 0); // toggle first switch
            rpcRepairSystem(shipInstance, 0x7, 1); // toggle second switch
            rpcRepairSystem(shipInstance, 0x7, 2); // toggle third switch
            rpcRepairSystem(shipInstance, 0x7, 3); // toggle fourth switch
            rpcRepairSystem(shipInstance, 0x7, 4); // toggle fifth switch

            sabotageAll = false;
        }

        if(sabotageLoop != -1 && sabotageTimer != 0) {
           if (get_delta(sabotageTimer, get_now_ms()) > 700) {
                ShipStatus_o *shipInstance = amongUsClient->ShipPrefabs->_items->m_Items[0]->klass->static_fields->Instance;
               rpcRepairSystem(shipInstance, 0x11, sabotageLoop);
                sabotageLoop = -1;
            }
            sabotageTimer = get_now_ms();
        }

        if(repairLoop) {
            ShipStatus_o *shipInstance = amongUsClient->ShipPrefabs->_items->m_Items[0]->klass->static_fields->Instance;

            // Fix Oxygen
            rpcRepairSystem(shipInstance, 0x8, 64);
            rpcRepairSystem(shipInstance, 0x8, 65);

            /*
           // to toggle light switches
           rpcRepairSystem(shipInstance, 0x7, 0); // toggle first switch
           rpcRepairSystem(shipInstance, 0x7, 1); // toggle second switch
           rpcRepairSystem(shipInstance, 0x7, 2); // toggle third switch
           rpcRepairSystem(shipInstance, 0x7, 3); // toggle fourth switch
           rpcRepairSystem(shipInstance, 0x7, 4); // toggle fifth switch
           */

            // Simulate top side
            rpcRepairSystem(shipInstance, 0x3, 64);
            rpcRepairSystem(shipInstance, 0x3, 32);
            // Simulate bottom side
            rpcRepairSystem(shipInstance, 0x3, 65);
            rpcRepairSystem(shipInstance, 0x3, 33);

            // sometimes when a player is holding one of the sides
            // try flipping
            rpcRepairSystem(shipInstance, 0x3, 65);
            rpcRepairSystem(shipInstance, 0x3, 33);

            rpcRepairSystem(shipInstance, 0x3, 64);
            rpcRepairSystem(shipInstance, 0x3, 32);

            // Communication broken.. will look into it
            rpcRepairSystem(shipInstance, 0xE, 0);

            repairLoop = false;
        }

        if(completePlayerTasks != NULL) {
            int taskCount = completePlayerTasks->myTasks->_size;

            for(int i = 0 ; i < taskCount; i++) {
                uint32_t taskID = completePlayerTasks->myTasks->_items->m_Items[i]->_Id_k__BackingField;
                rpcCompleteTask(completePlayerTasks, taskID);
            }
            completePlayerTasks = NULL;
        }

        if(lockAllDoors) {
            ShipStatus_o *shipInstance = amongUsClient->ShipPrefabs->_items->m_Items[0]->klass->static_fields->Instance;

            for(int i = 0; i < shipInstance->AllDoors->max_length && shipInstance->AllDoors->m_Items[i] != NULL; i++)
                if(shipInstance->AllDoors->m_Items[i]->Open)
                    rpcCloseDoorsOfType(shipInstance, shipInstance->AllDoors->m_Items[i]->Room);
        }

        if(playShipAnimation != -1) {
            rpcPlayAnimation(localPlayer, playShipAnimation);
            playShipAnimation = -1;
        }

        if(reportDeadPlayer != NULL) {
            cmdReportDead(reportDeadPlayer, reportDeadPlayer->_cachedData);
            reportDeadPlayer = NULL;
        }

        if(murderPlayer != NULL && impostors[0] != NULL) {
            if(murderAsMe && wasImpostor) {
                rpcMurderPlayer(localPlayer, murderPlayer);
                murderAsMe = false;
            } else if(!murderAsMe) {
                for(int i = 0 ; i < 3; i++) {
                    if(impostors[i]->PlayerId != localPlayer->PlayerId
                       && impostors[i]->_cachedData->IsDead == false) {
                        rpcMurderPlayer(impostors[i], murderPlayer);
                        break;
                    }
                }
            }
            murderPlayer = NULL;
            murderAsMe = false;
        }
    }

    /* being invisble through this function kills player, will look later
            if(instance->klass->static_fields->LocalPlayer != NULL) {
                LOGD("Setting visibility");
                pcSetVisible(instance->klass->static_fields->LocalPlayer, isVisble);
            }
    */

    if(rainbowColor && colorStartTime != 0) {
        if( get_delta(colorStartTime, get_now_ms()) > 500 ) {
            cmdCheckColor(localPlayer, colorIndex);
            colorIndex++;
            if(colorIndex > 12)
                colorIndex = 0;
            colorStartTime = get_now_ms();
        }
    }

    if (advertiseMenu && localPlayer != NULL) {
        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;

        for (int i = 0; i < pArraySize && i < 10; i++) {
            if(pArray->m_Items[i] != NULL) {
                msg->m_stringLength = message.length();
                readStr2Uint16(message.data(), &msg->m_firstChar, message.length());
                msg->klass = pArray->m_Items[i]->_cachedData->PlayerName->klass;
                msg->monitor = pArray->m_Items[i]->_cachedData->PlayerName->monitor;

                old_PlayerControl_RpcSendChat(pArray->m_Items[i]->_cachedData->_object, msg);
            }
        }
        advertiseMenu = false;
    }

    if (teleportToPlayer != NULL) {
        UnityEngine_Vector2_o pos;
        pos.x = teleportToPlayer->NetTransform->targetSyncPosition.x;
        pos.y = teleportToPlayer->NetTransform->targetSyncPosition.y + 0.2f;
        RpcSnapTo(localPlayer->NetTransform, pos);

        if(!attachToPlayer)
            teleportToPlayer = NULL;
    }

    if (teleportToMe != NULL) {
        UnityEngine_Vector2_o pos;
        pos.x = localPlayer->NetTransform->prevPosSent.x;
        pos.y = localPlayer->NetTransform->prevPosSent.y + 0.2f;
        RpcSnapTo(teleportToMe->NetTransform, pos);

        if(!attachPlayerToMe)
            teleportToMe = NULL;
    }

    if (freezePlayer != NULL) {
        UnityEngine_Vector2_o pos;
        pos.x = freezePlayer->NetTransform->targetSyncPosition.x;
        pos.y = freezePlayer->NetTransform->targetSyncPosition.y;
        RpcSnapTo(freezePlayer->NetTransform, pos);
    }

    /* Fake medscan on player - only works client side but maybe there is a way..
    if(medScanPlayer != NULL) {
        rpcSetScanner(medScanPlayer, medScanPlayerToggled);
        if(medScanPlayerToggled == false) {
            medScanPlayer = NULL;
        }
    }
    */

    if (teleportAlltoMe) {
        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (localPlayer != targetPlayer && targetPlayer != NULL) {
                UnityEngine_Vector2_o pos;
                pos.x = localPlayer->NetTransform->prevPosSent.x + 0.5f;
                pos.y = localPlayer->NetTransform->prevPosSent.y;
                RpcSnapTo(targetPlayer->NetTransform, pos);
            }
        }
        teleportAlltoMe = false;
    }

    if(teleportAllToPlayer != NULL) {
        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;

        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (targetPlayer != localPlayer && targetPlayer != NULL) {
                UnityEngine_Vector2_o pos;
                pos.x = teleportAllToPlayer->NetTransform->prevPosSent.x + 0.5f;
                pos.y = teleportAllToPlayer->NetTransform->prevPosSent.y;
                RpcSnapTo(targetPlayer->NetTransform, pos);
            }
        }
        teleportAllToPlayer = NULL;
    }

    if(attachLobbyToPlayer != NULL && attachLobbyTime != 0) {
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        PlayerControl_o * prev = localPlayer;

        if( get_delta(attachLobbyTime, get_now_ms()) > 800 ) {
            for (int i = 0; i < pArraySize; i++) {
                PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];
                if (attachLobbyToPlayer != targetPlayer && targetPlayer != NULL && targetPlayer != localPlayer) {
                    UnityEngine_Vector2_o pos;
                    pos.x = prev->NetTransform->prevPosSent.x + 0.7f;
                    pos.y = prev->NetTransform->prevPosSent.y;
                    RpcSnapTo(targetPlayer->NetTransform, pos);
                }
                prev = targetPlayer;
            }
            attachLobbyTime = get_now_ms();
        }
    }

    if (freezeLobby) {
        if( get_delta(freezeLobbyTimer, get_now_ms()) > 700 ) {
            PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
            int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;

            for (int i = 0; i < pArraySize; i++) {
                PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

                if (targetPlayer != NULL && targetPlayer != localPlayer) {
                    UnityEngine_Vector2_o pos;
                    pos.x = targetPlayer->NetTransform->targetSyncPosition.x;
                    pos.y = targetPlayer->NetTransform->targetSyncPosition.y;
                    RpcSnapTo(targetPlayer->NetTransform, pos);
                }
            }
            freezeLobbyTimer = get_now_ms();
        }
    }
}

void (*old_PlayerControl_FixedUpdate)(PlayerControl_o *instance);
void PlayerControl_FixedUpdate(PlayerControl_o *instance)
{
    ModFixedUpdate(instance);
    old_PlayerControl_FixedUpdate(instance);
}

void (*old_AmongUsClient_OnGameJoined)(AmongUsClient_o *instance, System_String_o *gameIdString, InnerNet_ClientData_o *data);
void AmongUsClient_OnGameJoined (AmongUsClient_o *instance, System_String_o *gameIdString, InnerNet_ClientData_o *data) {
    LOGD("AmongUsClient_OnGameJoined");
    old_AmongUsClient_OnGameJoined(instance, gameIdString, data);

    if(classPalette == NULL) {
        memRead(getAbsoluteAddress(offsets.colorPallete), &classPalette, sizeof(classPalette));
    }
    amongUsClient = instance;

    // Joined a new Lobby, reset variables
    hasGameBegan = false;
    wasImpostor = false;
    murderLobbyAsMe = false;
    murderLobby = false;
    murderAsMe = false;
    murderImpostors = false;

    impostors[0] = NULL;
    impostors[1] = NULL;
    impostors[2] = NULL;

    playShipAnimation = -1;
    repairLoop = false;

    teleportToPlayer = NULL;
    teleportToMe = NULL;
    teleportAllToPlayer = NULL;
    replacePlayer = NULL;

    attachLobbyToPlayer = NULL;
    attachToPlayer = false;
    attachPlayerToMe = false;

    spoofChatPlayer = NULL;

    voteKickPlayer = NULL;
    freezePlayer = NULL;
    castVotePlayer = NULL;
    medScanPlayer = NULL;
    murderPlayer = NULL;
    completePlayerTasks = NULL;

    getPlayersList = false;
    teleportAlltoMe = false;
    advertiseMenu = false;
}

void (*old_InnerNetClient_FixedUpdate)(InnerNet_InnerNetClient_o *instance);
void InnerNetClient_FixedUpdate(InnerNet_InnerNetClient_o *instance) {
    old_InnerNetClient_FixedUpdate(instance);

    if(voteKickPlayer != NULL && vbInstance != NULL) {
        int32_t targetClientId = getClientIdFromCharacter(instance, (InnerNet_InnerNetObject_o*)voteKickPlayer);
        int32_t myClientId = instance->ClientId;

        LOGD("HostID: %d", instance->HostId);
        if(targetClientId != instance->HostId) {
            for (int i = 0; i < instance->allClients->_size; i++) {
                PlayerControl_o *p = instance->allClients->_items->m_Items[i]->Character;
                instance->ClientId = getClientIdFromCharacter(instance,
                                                              (InnerNet_InnerNetObject_o *) p->_cachedData->_object);
                cmdAddVote(vbInstance, targetClientId);
            }
            instance->ClientId = myClientId;
        }
        voteKickPlayer = NULL;
    }

}

void (*old_VoteBanSystem_Awake)(VoteBanSystem_o *instance);
void VoteBanSystem_Awake(VoteBanSystem_o *instance) {
    vbInstance = instance;
    old_VoteBanSystem_Awake(instance);
}

void(*old_MeetingHud_Update)(MeetingHud_o *instance);
void MeetingHud_Update(MeetingHud_o *instance) {
    for(int i = 0; i < instance->playerStates->max_length && i < 10; i++) {

        if(!wasImpostor) {
            instance->playerStates->m_Items[i]->NameText->Color = classPalette->static_fields->White;

            if (showImpostors) {
                if (instance->playerStates->m_Items[i]->TargetPlayerId == impostors[0]->PlayerId
                    || instance->playerStates->m_Items[i]->TargetPlayerId == impostors[1]->PlayerId
                    ||
                    instance->playerStates->m_Items[i]->TargetPlayerId == impostors[2]->PlayerId) {

                    instance->playerStates->m_Items[i]->NameText->Color = classPalette->static_fields->ImpostorRed;
                }
            }
        }
        if(castVotePlayer != NULL) {
            cmdCastVote(instance, instance->playerStates->m_Items[i]->TargetPlayerId, castVotePlayer->PlayerId);
        }
    }
    castVotePlayer = NULL;
    old_MeetingHud_Update(instance);
}

// called at the beginning of the game only
void (*old_IntroCutscene_CoBegin)(IntroCutscene_o *instance, System_Collections_Generic_List_PlayerControl__o *yourTeam, bool isImpostor);
void IntroCutscene_CoBegin(IntroCutscene_o *instance, System_Collections_Generic_List_PlayerControl__o *yourTeam, bool isImpostor)
{
    LOGD("IntroCutscene_CoBegin");
    wasImpostor = isImpostor;
    hasGameBegan = true;

    if(isImpostor) {
        fakeImpostor = false;
    }

    old_IntroCutscene_CoBegin(instance, yourTeam, isImpostor);
}

/* 10 min ban antiban */
bool (*old_StatsManager_getAmBanned)(StatsManager_o *instance);
bool StatsManager_getAmBanned(StatsManager_o *instance) {
    bool AmBan = old_StatsManager_getAmBanned(instance);

    if(AmBan) {
        instance->lastGameStarted = 0;
        instance->banPoints = 0.0;

        // Save Stats
        ((void (__fastcall *)(StatsManager_o *,
                              void *)) instance->klass->vtable._5_SaveStats.methodPtr)(instance, instance->klass->vtable._5_SaveStats.method);
    }

    AmBan = old_StatsManager_getAmBanned(instance);
    LOGD("AmBanned: %d", AmBan);

    return AmBan;
}

void HookFunction(void *const symbol, void *const replace, void **result) {
#if defined(__aarch64__)
    A64HookFunction(symbol, replace, result);
#else if defined(__ARM_ARCH_7A__)
    MSHookFunction(symbol, replace, result);
#endif
}

void applyHooks() {
#if defined(__aarch64__)
    // shellcodes for arm64
    char sc_ret_1[] = "\x20\x00\x80\xD2\xC0\x03\x5F\xD6";
    LOGD("Arm64-v8a version");
#else if defined(__ARM_ARCH_7A__)
    // shellcode for armeabi-v7a
    char sc_ret_1[] = "\x01\x00\xA0\xE3\x1E\xFF\x2F\xE1";
    LOGD("Arm-V7a version");
#endif

    bool isDone = false;
    /* Patching certain functions initially */
    do {
        isDone = memWrite(getAbsoluteAddress(offsets.getBoughtNoAds, true), sc_ret_1, sizeof(sc_ret_1) - 1)
                 && memWrite(getAbsoluteAddress(offsets.getUnlockedPets), sc_ret_1, sizeof(sc_ret_1) - 1)
                 && memWrite(getAbsoluteAddress(offsets.getUnlockedHats), sc_ret_1, sizeof(sc_ret_1) - 1)
                 && memWrite(getAbsoluteAddress(offsets.getUnlockedSkins), sc_ret_1, sizeof(sc_ret_1) - 1);
        LOGD("isPatched: %d", isDone);
    } while(!isDone);
    LOGD("Patches Applied!");

    do {
        isDone = true;
        if(*old_ChatController_SetVisible == NULL) {
            LOGD("Failed to hook ChatController_SetVisible");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.chatSetVisible),
                            (void *) &ChatController_SetVisible,
                            (void **) &old_ChatController_SetVisible);
        }
        if(*old_PlayerControl_RpcSetInfected == NULL) {
            LOGD("Failed to hook RpcSetInfected");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.rpcSetInfected),
                            (void *) &PlayerControl_RpcSetInfected,
                            (void **) &old_PlayerControl_RpcSetInfected);
        }
        if(*old_IntroCutscene_CoBegin == NULL) {
            LOGD("Failed to hook IntroCutscene");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.introCoBegin),
                            (void *) &IntroCutscene_CoBegin,
                            (void **) &old_IntroCutscene_CoBegin);
        }
        if(*old_PlayerControl_FixedUpdate == NULL) {
            LOGD("Failed to hook playerControl_getData");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.pcFixedUpdate),
                            (void *) &PlayerControl_FixedUpdate,
                            (void **) &old_PlayerControl_FixedUpdate);
        }
        if(*old_StatsManager_getAmBanned == NULL) {
            LOGD("Failed to hook getAmBanned");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.getAmBanned),
                            (void *) &StatsManager_getAmBanned,
                            (void **) &old_StatsManager_getAmBanned);
        }
        if(*old_InnerNetClient_FixedUpdate == NULL) {
            LOGD("Failed to hook innerNetFixedUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.netFixedUpdate),
                            (void *) &InnerNetClient_FixedUpdate,
                            (void **) &old_InnerNetClient_FixedUpdate);
        }
        if(*old_MeetingHud_Update == NULL) {
            LOGD("Failed to hook meetingHudUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.meetingHudUpdate),
                            (void *) &MeetingHud_Update,
                            (void **) &old_MeetingHud_Update);
        }
        if(*old_PlayerControl_RpcSendChat == NULL) {
            LOGD("Failed to hook PlayerControl_RpcSendChat");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.rpcSendChat),
                            (void *) &PlayerControl_RpcSendChat,
                            (void **) &old_PlayerControl_RpcSendChat);
        }
        if(*old_AmongUsClient_OnGameJoined == NULL) {
            LOGD("Failed to hook AmongUsClient_OnGameJoined");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.onGameJoined),
                            (void*) AmongUsClient_OnGameJoined,
                            (void**) &old_AmongUsClient_OnGameJoined);
        }

        if(*old_VoteBanSystem_Awake == NULL) {
            LOGD("Failed to hook VoteBanSystem_Awake");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.voteBanAwake),
                            (void*) VoteBanSystem_Awake,
                            (void**) &old_VoteBanSystem_Awake);
        }

    } while(!isDone);

    initFunction();

    LOGD("All Hooks Applied!");
}