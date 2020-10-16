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
bool freezeMurderer = false;
bool teleportOnCrime = false;
bool noClip = false;
bool noClipToggled = false;
bool attachSurround = false;

bool murderLobby = false;
bool murderLobbyAsMe = false;
bool repairLoop = false;

bool rainbowColor = false;

bool skipVote = false;
bool lobbySkinChanger = false;
bool lobbyPetChanger = false;
bool lobbyHatChanger = false;
bool lobbyColorChanger = false;

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
PlayerControl_o *revivePlayer = NULL;
PlayerControl_o *medScanPlayer = NULL;
PlayerControl_o *reportDeadPlayer = NULL;
PlayerControl_o *murderPlayer = NULL;
PlayerControl_o *completePlayerTasks = NULL;
PlayerControl_o *copyPlayer = NULL;
PlayerControl_o *attachLobbyToPlayer = NULL;
PlayerControl_o *impostor1 = NULL;
PlayerControl_o *impostor2 = NULL;
PlayerControl_o *impostor3 = NULL;
PlayerControl_o *lastMurdererPlayer = NULL;

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
int impostorCount = -1;
int changeLightamount = -1;

float playerSpeed = 0.0;

double attachLobbyTime;
double colorStartTime;
double freezeLobbyTimer;
double sabotageTimer;

double skinChangerTimer;
double petChangerTimer;
double hatChangerTimer;
double freezePlayerTimer;
double copyPlayerTimer;
double lobbyCopyMeTimer;
double lockDoorTimer;

uint8_t colorIndex = 0;

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

// Function invoked when you are the host to set impostors
void (*old_PlayerControl_RpcSetInfected)(PlayerControl_o *instance, GameData_PlayerInfo_array *infected);
void PlayerControl_RpcSetInfected(PlayerControl_o *instance, GameData_PlayerInfo_array *infected)
{
    if(impostor1 != NULL) {
        infected->m_Items[0] = impostor1->_cachedData;
        impostor1 = NULL;
    }

    if(impostor2 != NULL) {
        infected->m_Items[1] = impostor2->_cachedData;
        impostor2 = NULL;
    }

    if(impostor3 != NULL) {
        infected->m_Items[2] = impostor3->_cachedData;
        impostor3 = NULL;
    }

    if(forceImpostor)
        infected->m_Items[0] = instance->_cachedData;

    old_PlayerControl_RpcSetInfected(instance, infected);
}

void (*old_LightSource_Update)(LightSource_o *instance);
void LightSource_Update(LightSource_o *instance) {
    old_LightSource_Update(instance);
    if (changeLightamount != -1 && instance != NULL) {
        instance->LightRadius = changeLightamount;
    }
}

void (*old_PlayerPhysics_FixedUpdate)(PlayerPhysics_o *instance);
void PlayerPhysics_FixedUpdate(PlayerPhysics_o *instance) {
    old_PlayerPhysics_FixedUpdate(instance);
    if(playerSpeed != 0.0 && instance != NULL) {
        instance->Speed = playerSpeed;
        instance->GhostSpeed = playerSpeed;
    }
}
void (*old_TextBox_Update)(TextBox_o *instance);
void TextBox_Update(TextBox_o *instance) {
    old_TextBox_Update(instance);
    if (instance != NULL) {
        instance->AllowSymbols = true;
        instance->AllowEmail = true;
        instance->AllowPaste = true;
        instance->pipeBlinkTimer = 0;
        instance->characterLimit = 0x7CFFFFFF;
    }
}

void ModFixedUpdate(PlayerControl_o *instance) {
    PlayerControl_o* localPlayer = instance->klass->static_fields->LocalPlayer;

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

    if(revivePlayer!= NULL) {
        revivePlayer == NULL;
    }

    if(impostorCount != -1) {
        instance->klass->static_fields->GameOptions->NumImpostors = impostorCount;
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
            // Skeld only
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

            // communication
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


        if(lockAllDoors && lockDoorTimer != 0) {
            if (get_delta(lockDoorTimer, get_now_ms()) > 500) {
                ShipStatus_o *shipInstance = amongUsClient->ShipPrefabs->_items->m_Items[0]->klass->static_fields->Instance;

                for (int i = 0; i < shipInstance->AllDoors->max_length &&
                                shipInstance->AllDoors->m_Items[i] != NULL; i++)
                    if (shipInstance->AllDoors->m_Items[i]->Open)
                        rpcCloseDoorsOfType(shipInstance, shipInstance->AllDoors->m_Items[i]->Room);

                lockDoorTimer = get_now_ms();
            }
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



    if(localPlayer != NULL && copyPlayer != NULL && copyPlayerTimer != 0) {
        if(localPlayer->_cachedData->PetId == copyPlayer->_cachedData->PetId
           && localPlayer->_cachedData->HatId == copyPlayer->_cachedData->HatId
           && localPlayer->_cachedData->SkinId == copyPlayer->_cachedData->SkinId) {

            LOGD("Completed!");
            copyPlayer = NULL;
            copyPlayerTimer = 0;

        } else if (get_delta(copyPlayerTimer, get_now_ms()) > 600) {
            LOGD("Setting Copy Player");
            rpcSetHat(localPlayer, copyPlayer->_cachedData->HatId);
            rpcSetPet(localPlayer, copyPlayer->_cachedData->PetId);
            rpcSetSkin(localPlayer, copyPlayer->_cachedData->SkinId);
            // rpcSetName(localPlayer, copyPlayer->_cachedData->PlayerName); client side only
            copyPlayerTimer = get_now_ms();
        }
    }

    // translated from 0x00000000006935DC
    if(noClipToggled && localPlayer != NULL) {
        // There are two LayerMasks with the names 'Players' and 'Ghost'
        if(noClip) {
            LOGD("Ghost");
            msg->m_stringLength = 5;
            readStr2Uint16("Ghost", &msg->m_firstChar, 5);
        } else {
            LOGD("Players!");
            msg->m_stringLength = 7;
            readStr2Uint16("Players", &msg->m_firstChar, 7);
        }

        msg->klass = localPlayer->_cachedData->PlayerName->klass;
        msg->monitor = localPlayer->_cachedData->PlayerName->monitor;

        UnityEngine_Component_o *playerComponent = (UnityEngine_Component_o *)getGameObject((UnityEngine_Component_o*) localPlayer);
        setLayer((UnityEngine_GameObject_o *)playerComponent, getLayerWithName(msg));

        noClipToggled = false;
    }

    if(rainbowColor && colorStartTime != 0) {
        if( get_delta(colorStartTime, get_now_ms()) > 500 ) {
            cmdCheckColor(localPlayer, colorIndex);
            colorIndex++;
            if(colorIndex > 12)
                colorIndex = 0;
            colorStartTime = get_now_ms();
        }
    }

    //TODO: fix people say only localPlayer sees it sometimes
    if (advertiseMenu && localPlayer != NULL) {
        PlayerControl_array *pArray = instance->klass->static_fields->AllPlayerControls->_items;
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;

        for (int i = 0; i < pArraySize && i < 10; i++) {
            if(pArray->m_Items[i] != NULL) {
                msg->m_stringLength = message.length();
                readStr2Uint16(message.data(), &msg->m_firstChar, message.length());
                msg->monitor = pArray->m_Items[i]->monitor;
                msg->klass   = pArray->m_Items[i]->_cachedData->PlayerName->klass;
                old_PlayerControl_RpcSendChat(pArray->m_Items[i], msg);
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

    if (freezePlayer != NULL && get_delta(freezePlayerTimer, get_now_ms()) > 700) {
        UnityEngine_Vector2_o pos;
        pos.x = freezePlayer->NetTransform->targetSyncPosition.x;
        pos.y = freezePlayer->NetTransform->targetSyncPosition.y;
        RpcSnapTo(freezePlayer->NetTransform, pos);
        freezePlayerTimer = get_now_ms();
    }

    if(freezeMurderer && lastMurdererPlayer != NULL) {

        LOGD("Inside FreezeMurderer");
        if (lastMurdererPlayer != localPlayer) {
            UnityEngine_Vector2_o pos;
            pos.x = lastMurdererPlayer->NetTransform->prevPosSent.x;
            pos.y = lastMurdererPlayer->NetTransform->prevPosSent.y;
            RpcSnapTo(lastMurdererPlayer->NetTransform, pos);

            LOGD("Done!");
        }
    }

    if (teleportAlltoMe) {
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
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;

        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (targetPlayer != NULL) {
                UnityEngine_Vector2_o pos;
                pos.x = teleportAllToPlayer->NetTransform->targetSyncPosition.x + 1.0f;
                pos.y = teleportAllToPlayer->NetTransform->targetSyncPosition.y;
                RpcSnapTo(targetPlayer->NetTransform, pos);
            }
        }
        teleportAllToPlayer = NULL;
    }

    if(!attachSurround && attachLobbyToPlayer != NULL && attachLobbyTime != 0) {
        LOGD("Attaching lobby");
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        PlayerControl_o * prev = attachLobbyToPlayer;

        if( get_delta(attachLobbyTime, get_now_ms()) > 800 ) {
            for (int i = 0; i < pArraySize; i++) {
                PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];
                if (attachLobbyToPlayer != targetPlayer && targetPlayer != NULL && targetPlayer != localPlayer) {
                    UnityEngine_Vector2_o pos;
                    pos.x = prev->NetTransform->targetSyncPosition.x + 0.7f;
                    pos.y = prev->NetTransform->targetSyncPosition.y;
                    pos.x = prev->NetTransform->prevPosSent.x + 0.7f;
                    pos.y = prev->NetTransform->prevPosSent.y;

                    RpcSnapTo(targetPlayer->NetTransform, pos);
                }
                prev = targetPlayer;
            }
            attachLobbyTime = get_now_ms();
        }
    }

    if(attachSurround && attachLobbyToPlayer != NULL && attachLobbyTime != 0) {
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        if( get_delta(attachLobbyTime, get_now_ms()) > 800 ) {

            float distance = 0.8f;
            UnityEngine_Vector2_o pos;

            bool last_add_y, last_sub_x, last_sub_y = false;
            for (int i = 0; i < pArraySize; i++) {
                PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

                if (attachLobbyToPlayer != targetPlayer && targetPlayer != NULL && targetPlayer != localPlayer) {
                    if(targetPlayer != NULL && targetPlayer != localPlayer) {
                        if(!last_add_y) {
                            pos.x = attachLobbyToPlayer->NetTransform->targetSyncPosition.x;
                            pos.y = attachLobbyToPlayer->NetTransform->targetSyncPosition.y + distance;
                            pos.x = attachLobbyToPlayer->NetTransform->prevPosSent.x;
                            pos.y = attachLobbyToPlayer->NetTransform->prevPosSent.y + distance;
                            last_add_y = true;
                        } else if(!last_sub_x) {
                            pos.x = attachLobbyToPlayer->NetTransform->targetSyncPosition.x - distance;
                            pos.y = attachLobbyToPlayer->NetTransform->targetSyncPosition.y;
                            pos.x = attachLobbyToPlayer->NetTransform->prevPosSent.x - distance;
                            pos.y = attachLobbyToPlayer->NetTransform->prevPosSent.y;
                            last_sub_x = true;
                        } else if(!last_sub_y) {
                            pos.x = attachLobbyToPlayer->NetTransform->targetSyncPosition.x;
                            pos.y = attachLobbyToPlayer->NetTransform->targetSyncPosition.y - distance;
                            pos.x = attachLobbyToPlayer->NetTransform->prevPosSent.x;
                            pos.y = attachLobbyToPlayer->NetTransform->prevPosSent.y - distance;
                            last_sub_y = true;
                        } else {
                            pos.x = attachLobbyToPlayer->NetTransform->targetSyncPosition.x + distance;
                            pos.y = attachLobbyToPlayer->NetTransform->targetSyncPosition.y;
                            pos.x = attachLobbyToPlayer->NetTransform->prevPosSent.x + distance;
                            pos.y = attachLobbyToPlayer->NetTransform->prevPosSent.y;
                            distance = 2 * distance;
                            last_add_y = false;
                            last_sub_x = false;
                            last_sub_y = false;
                        }
                        RpcSnapTo(targetPlayer->NetTransform, pos);
                    }
                }
            }
            attachLobbyTime = get_now_ms();
        }
    }

    if (freezeLobby) {
        if( get_delta(freezeLobbyTimer, get_now_ms()) > 700 ) {
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

    if (lobbySkinChanger && (get_delta(skinChangerTimer, get_now_ms()) > 700)) {
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (targetPlayer != NULL) {
                rpcSetSkin(targetPlayer, rand() % 40);
            }
        }
        skinChangerTimer = get_now_ms();
    }

    if (lobbyPetChanger && (get_delta(petChangerTimer, get_now_ms()) > 900)) {
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (targetPlayer != NULL) {
                rpcSetPet(targetPlayer, rand() % 10);
            }
        }
        petChangerTimer = get_now_ms();
    }

    if (lobbyHatChanger && (get_delta(hatChangerTimer, get_now_ms()) > 700)) {
        int pArraySize = instance->klass->static_fields->AllPlayerControls->_size;
        for (int i = 0; i < pArraySize; i++) {
            PlayerControl_o *targetPlayer = instance->klass->static_fields->AllPlayerControls->_items->m_Items[i];

            if (targetPlayer != NULL) {
                rpcSetHat(targetPlayer, rand() % 40);
            }
        }
        hatChangerTimer = get_now_ms();
    }

    if (lobbyColorChanger && instance != NULL) {
        rpcSetColor(instance, rand() % 12);
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

    attachLobbyToPlayer = NULL;
    attachToPlayer = false;
    attachPlayerToMe = false;
    lastMurdererPlayer = NULL;

    spoofChatPlayer = NULL;

    voteKickPlayer = NULL;
    freezePlayer = NULL;
    castVotePlayer = NULL;
    medScanPlayer = NULL;
    murderPlayer = NULL;
    completePlayerTasks = NULL;
    copyPlayer = NULL;
    lastMurdererPlayer = NULL;

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

MeetingHud_o *meetingHud = NULL;
void(*old_MeetingHud_Update)(MeetingHud_o *instance);
void MeetingHud_Update(MeetingHud_o *instance) {
    meetingHud = instance;
    lastMurdererPlayer = NULL;
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
        if(skipVote) {
            LOGD("SkipVote");
            cmdCastVote(instance, instance->playerStates->m_Items[i]->TargetPlayerId, 10);
        } else if(castVotePlayer != NULL) {
            cmdCastVote(instance, instance->playerStates->m_Items[i]->TargetPlayerId, castVotePlayer->PlayerId);
        }
    }
    castVotePlayer = NULL;
    old_MeetingHud_Update(instance);
}

bool (*old_PlayerControl_getCanMove) (PlayerControl_o *instance);
bool PlayerControl_getCanMove(PlayerControl_o *instance) {
    if(meetingHud != NULL && meetingHud->state >= 0 && meetingHud->state < 4) {
        return true;
    }
    return old_PlayerControl_getCanMove(instance);
}

void (*old_PlayerControl_MurderPlayer) (PlayerControl_o *murderer, PlayerControl_o *target);
void PlayerControl_MurderPlayer(PlayerControl_o *murderer, PlayerControl_o *target) {
    old_PlayerControl_MurderPlayer(murderer, target);
    if(freezeMurderer) {
        lastMurdererPlayer = murderer;
    }

    if(teleportOnCrime) {
        teleportAllToPlayer = murderer;
    }
    LOGD("End PlayerControl_MurderPlayer");
}

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
                 && memWrite(getAbsoluteAddress(offsets.getUnlockedSkins), sc_ret_1, sizeof(sc_ret_1) - 1)
                 && memWrite(getAbsoluteAddress(offsets.chatCharAllowed), sc_ret_1, sizeof(sc_ret_1)- 1);
        LOGD("isPatched: %d", isDone);
    } while(!isDone);
    LOGD("Patches Applied!");

    do {
        isDone = true;
        if (*old_ChatController_SetVisible == NULL) {
            LOGD("Failed to hook ChatController_SetVisible");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.chatSetVisible),
                         (void *) &ChatController_SetVisible,
                         (void **) &old_ChatController_SetVisible);
        }
        if (*old_PlayerControl_RpcSetInfected == NULL) {
            LOGD("Failed to hook RpcSetInfected");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.rpcSetInfected),
                         (void *) &PlayerControl_RpcSetInfected,
                         (void **) &old_PlayerControl_RpcSetInfected);
        }

        if (*old_IntroCutscene_CoBegin == NULL) {
            LOGD("Failed to hook IntroCutscene");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.introCoBegin),
                         (void *) &IntroCutscene_CoBegin,
                         (void **) &old_IntroCutscene_CoBegin);
        }

        if (*old_PlayerControl_FixedUpdate == NULL) {
            LOGD("Failed to hook PlayerControl_FixedUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.pcFixedUpdate),
                         (void *) &PlayerControl_FixedUpdate,
                         (void **) &old_PlayerControl_FixedUpdate);
        }

        if(*old_TextBox_Update == NULL) {
            LOGD("Failed to hook playerControl_getData");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.textBoxUpdate),
                         (void *) &TextBox_Update,
                         (void **) &old_TextBox_Update);
        }

        if (*old_StatsManager_getAmBanned == NULL) {
            LOGD("Failed to hook getAmBanned");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.getAmBanned),
                         (void *) &StatsManager_getAmBanned,
                         (void **) &old_StatsManager_getAmBanned);
        }

        if (*old_InnerNetClient_FixedUpdate == NULL) {
            LOGD("Failed to hook innerNetFixedUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.netFixedUpdate),
                         (void *) &InnerNetClient_FixedUpdate,
                         (void **) &old_InnerNetClient_FixedUpdate);
        }

        if (*old_MeetingHud_Update == NULL) {
            LOGD("Failed to hook meetingHudUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.meetingHudUpdate),
                         (void *) &MeetingHud_Update,
                         (void **) &old_MeetingHud_Update);
        }

        if (*old_PlayerControl_RpcSendChat == NULL) {
            LOGD("Failed to hook PlayerControl_RpcSendChat");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.rpcSendChat),
                         (void *) &PlayerControl_RpcSendChat,
                         (void **) &old_PlayerControl_RpcSendChat);
        }

        if (*old_AmongUsClient_OnGameJoined == NULL) {
            LOGD("Failed to hook AmongUsClient_OnGameJoined");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.onGameJoined),
                         (void *) AmongUsClient_OnGameJoined,
                         (void **) &old_AmongUsClient_OnGameJoined);
        }

        if (*old_VoteBanSystem_Awake == NULL) {
            LOGD("Failed to hook VoteBanSystem_Awake");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.voteBanAwake),
                         (void *) VoteBanSystem_Awake,
                         (void **) &old_VoteBanSystem_Awake);
        }

        if(*old_LightSource_Update == NULL) {
            LOGD("Failed to hook LightSource_Update");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.lightSourceUpdate),
                         (void*) LightSource_Update,
                         (void**) &old_LightSource_Update);
        }

        if(*old_PlayerPhysics_FixedUpdate == NULL) {
            LOGD("Failed to hook PlayerPhysics_FixedUpdate");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.pPhyFixedUpdate),
                         (void*) PlayerPhysics_FixedUpdate,
                         (void**) &old_PlayerPhysics_FixedUpdate);
        }

        if(*old_PlayerControl_getCanMove == NULL) {
            LOGD("Failed to hook PlayerControl_getCanMove");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.pcGetCanMove),
                         (void*) PlayerControl_getCanMove,
                         (void**) &old_PlayerControl_getCanMove);
        }

        if (*old_PlayerControl_MurderPlayer == NULL) {
            LOGD("Failed to hook old_MurderPlayer");
            isDone = false;
            HookFunction(getAbsoluteAddress(offsets.pcMurderPlayer),
                         (void *) PlayerControl_MurderPlayer,
                         (void **) &old_PlayerControl_MurderPlayer);
        }
    } while(!isDone);

    initFunction();

    LOGD("All Hooks Applied!");
}