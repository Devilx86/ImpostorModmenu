#include <pthread.h>
#include <jni.h>
#include <vector>

#include "mods.h"

extern "C"
JNIEXPORT void JNICALL updateMods(JNIEnv *env, jobject instance, jint feature, jint value) {
    LOGD("Feature: = %d", feature);
    LOGD("Value: = %d", value);

    switch (feature) {
        case 1: {
            fakeImpostor = value;
            break;
        }
        case 2:
            noKillTime = value;
            break;

        case 3:
            showImpostors = value;
            break;

        case 4:
            noEmergencyCoolDown = value;
            break;

        case 5:
            // no clip
            noClipToggled = true;
            noClip = value;
            break;

        case 6:
            rainbowColor = value;
            colorStartTime = 0;
            if(rainbowColor) {
                colorIndex = 0;
                colorStartTime = get_now_ms();
            }
            break;

        case 7:
            if (value > -1 && value < 10)
                teleportToPlayer = players->m_Items[value];
            break;

        case 8:
            if (value > -1 && value < 10)
                teleportToMe = players->m_Items[value];
            break;

        case 9:
            freezePlayer = NULL;
            if (value > -1 && value < 10)
                freezePlayer = players->m_Items[value];
            freezePlayerTimer = get_now_ms();
            break;

        case 10:
            if (value > -1 && value < 10) {
                teleportToPlayer = players->m_Items[value];
                attachToPlayer = true;
            } else {
                attachToPlayer = false;
            }
            break;

        case 11:
            if (value > -1 && value < 10) {
                teleportToMe = players->m_Items[value];
                attachPlayerToMe = true;
            } else {
                attachPlayerToMe = false;
            }
            break;

        case 12:
            if (value > -1 && value < 10)
                spoofChatPlayer = players->m_Items[value];
            break;

        case 13:
            if(value > -1 && value < 10)
                voteKickPlayer = players->m_Items[value];
            break;

        case 14:
            skipVote = value;
            break;

        case 15:
            // Skip Vote
            if(value < 0) {
                castVotePlayer = players->m_Items[0];
                castVotePlayer->PlayerId = 10; // skip index
            } else if(value < 10) {
                castVotePlayer = players->m_Items[value];
            }
            break;

        case 16:
            if(value > -1 && value < 10)
                reportDeadPlayer = players->m_Items[value];
            break;

        case 17:
        // TODO:
            if (value > -1 && value < 10)
                revivePlayer = players->m_Items[value];
            break;

        case 18:
            // kill player
            if(value > -1 && value < 10)
                murderPlayer = players->m_Items[value];
            break;

        case 19:
            // kill player as me
            if(value > -1 && value < 10) {
                murderPlayer = players->m_Items[value];
                murderAsMe = true;
            }
            break;

        case 20:
            if(value > -1 && value < 10)
                completePlayerTasks = players->m_Items[value];
            break;

        case 21:
            if(value > -1 && value < 10) {
                copyPlayer = players->m_Items[value];
                copyPlayerTimer = get_now_ms();
            }
            break;

        case 22:
            lockAllDoors = value;
            lockDoorTimer = get_now_ms();
            break;

        case 23:
            sabotageLoop = -1;
            sabotageTimer = 0;
            if (value > -1 && value < 5) {
                sabotageLoop = sabotageOptions[value];
                sabotageTimer = get_now_ms();
            }
            break;

        case 24:
            sabotageAll = value;
            break;

        case 25:
            repairLoop = value;
            break;

        case 26:
            playShipAnimation = value;
            break;

        case 27:
            // Impostor Count
            impostorCount = value;
            break;

        case 28:
            forceImpostor = value;
            break;

        case 29:
            if (value > -1 && value < 10) {
                impostor1 = players->m_Items[value];
            }
            break;

        case 30:
            if (value > -1 && value < 10) {
                impostor2 = players->m_Items[value];
            }
            break;

        case 31:
            if (value > -1 && value < 10) {
                impostor3 = players->m_Items[value];
            }
            break;

        case 32:
            attachLobbyToPlayer = NULL;
            attachLobbyTime = 0;
            attachSurround = false;
            if (value > -1 && value < 10) {
                attachLobbyToPlayer = players->m_Items[value];
                attachSurround = true;
                attachLobbyTime = get_now_ms();
            }
            break;

        case 33:
            attachSurround = false;
            attachLobbyToPlayer = NULL;
            attachLobbyTime = 0;
            if (value > -1 && value < 10) {
                attachLobbyToPlayer = players->m_Items[value];
                attachLobbyTime = get_now_ms();
            }
            break;

        case 34:
            teleportAlltoMe = true;
            break;

        case 35:
            if(value > -1 && value < 10)
                teleportAllToPlayer = players->m_Items[value];
            break;

        case 36:
            freezeLobby = value;
            break;

        case 37:
            // kill crew as someone else
            murderLobby = true;
            murderLobbyAsMe = false;
            break;

        case 38:
            // kill crew as me
            murderLobby = true;
            murderLobbyAsMe = true;
            break;

        case 39:
            // kill impostors only
            murderImpostors = true;
            break;

        case 40:
            lastMurdererPlayer = NULL;
            freezeMurderer = value;
            break;

        case 41:
            teleportOnCrime = value;
            break;

        case 42:
            lobbySkinChanger = value;
            skinChangerTimer = get_now_ms();
            break;

        case 43:
            lobbyPetChanger = value;
            petChangerTimer = get_now_ms();
            break;

        case 44:
            lobbyHatChanger = value;
            hatChangerTimer = get_now_ms();
            break;

        case 45:
            lobbyColorChanger = value;
            break;

        case 46:
            playerSpeed = value;
            break;

        case 47:
            changeLightamount = value;
            break;
    }
}

// TODO: Fix Custom messages not visibile to players
JNIEXPORT void JNICALL sendLobbyMessage(JNIEnv *env, jclass klass, jstring text)
{
    if(!advertiseMenu) {
        char *msg = (char *) env->GetStringUTFChars(text, NULL);
        message = std::string(msg);
        LOGD("Message: %s", message.data());
        advertiseMenu = true;
    }
}

JNIEXPORT jobjectArray JNICALL getPlayerNames(JNIEnv *env, jclass klass)
{
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray playerNames = env->NewObjectArray(10, stringClass, 0);

    players = NULL;
    int tries = 0;

    if(amongUsClient == NULL || amongUsClient->GameState == 0 || amongUsClient->GameState == 3) {
        for(int i = 0; i < 10; i++) {
            env->SetObjectArrayElement(playerNames, i, env->NewStringUTF("-"));
        }
    } else {
        getPlayersList = true;

        do {
            sleep(1);
            tries++;
        } while (tries < 10 && getPlayersList == true);

        if(tries > 10) {
            for(int i = 0; i < 10; i++) {
                env->SetObjectArrayElement(playerNames, i, env->NewStringUTF("-"));
            }
        } else if (tries < 10 && players != NULL) {
            for (int i = 0; i < players->max_length && i < 10; i++) {
                PlayerControl_o *player = players->m_Items[i];
                std::string pName = "-";

                if (player != NULL) {
                    pName = "[" + readUint162Str(&player->_cachedData->PlayerName->m_firstChar,
                                                 player->_cachedData->PlayerName->m_stringLength) +
                            "]";

                    if (classPalette != NULL) {
                        std::string color = readUint162Str(
                                &classPalette->static_fields->ShortColorNames->m_Items[player->_cachedData->ColorId]->m_firstChar,
                                classPalette->static_fields->ShortColorNames->m_Items[player->_cachedData->ColorId]->m_stringLength);

                        pName = pName + " - " + color;
                    }
                }
                LOGD("Name: %s", pName.data());
                env->SetObjectArrayElement(playerNames, i, env->NewStringUTF(pName.c_str()));
            }
        }
    }

    getPlayersList = false;

    return playerNames;
}

void* hack_thread(void*) {
    basePtr = 0;
    int tries = 0;

    uintptr_t tempPtr = 0;

    // wrong basePtr, maybe cos library has not loaded yet?
/*
    do {
        tempPtr = get_lib_start("libil2cpp.so");
        if(tries++ > 100) { LOGD("[-] Exiting: tempPtr not found"); exit(0); }
    } while(tempPtr == 0);

    // Get the right basePtr
    do {
        basePtr = get_lib_start("libil2cpp.so");
        if(tries++ > 100) { LOGD("[-] Exiting: basePtr not found"); exit(0); }
    } while(basePtr == tempPtr || basePtr == 0x0);
*/
    do {
        tempPtr = get_lib_start("libil2cpp.so");
        if(tries++ > 200) { LOGD("[-] Exiting: tempPtr not found"); exit(0);}
    } while(tempPtr == 0);

    do {
        sleep(2);
        tempPtr = get_lib_start("libil2cpp.so");
        if (tries++ > 15) {
            if (tempPtr == get_lib_start("libil2cpp.so")) {
                basePtr = tempPtr;
            } else {
                LOGD("[-] Exiting: basePtr not found");
                exit(0);
            }
        }
    } while(basePtr == 0x0);

    applyHooks();

    return NULL;
}

// https://developer.android.com/training/articles/perf-jni#native-libraries
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/devilx86/modmenu/MenuService");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    // Signatures: javap -s MenuService*.class | grep updateMods - A 4
    static const JNINativeMethod methods[] = {
            {"updateMods", "(II)V", reinterpret_cast<void*>(updateMods)},
            {"getPlayerNames", "()[Ljava/lang/String;", reinterpret_cast<void*>(getPlayerNames)},
            {"sendLobbyMessage", "(Ljava/lang/String;)V", reinterpret_cast<void*>(sendLobbyMessage)},
    };

    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);

    return JNI_VERSION_1_6;
}