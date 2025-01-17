#include "z3D/z3D.h"
#include "settings.h"

void SaveFile_Init() {
#ifdef ENABLE_DEBUG
    gSaveContext.equipment  |= 0x0FFF;  //Swords, shields, tunics, boots
    gSaveContext.bgsFlag     = 1;
    gSaveContext.bgsHitsLeft = 1;
    gSaveContext.upgrades   |= 0x109;   //bomb bag, quiver, strength
    gSaveContext.questItems |= 0x3FFC0; //songs
    gSaveContext.items[SLOT_OCARINA]  = ITEM_OCARINA_FAIRY;
    gSaveContext.items[SLOT_HOOKSHOT] = ITEM_LONGSHOT;
    gSaveContext.items[SLOT_HAMMER]   = ITEM_HAMMER;
    gSaveContext.items[SLOT_FARORES_WIND] = ITEM_FARORES_WIND;
    gSaveContext.items[SLOT_DINS_FIRE] = ITEM_DINS_FIRE;
    gSaveContext.items[SLOT_BOMB] = ITEM_BOMB;
    gSaveContext.items[SLOT_BOW] = ITEM_BOW;
    gSaveContext.items[SLOT_ARROW_FIRE] = ITEM_ARROW_FIRE;
    gSaveContext.items[SLOT_HOVER_BOOTS] = ITEM_BOOTS_HOVER;
    gSaveContext.items[SLOT_ARROW_LIGHT] = ITEM_ARROW_LIGHT;
    gSaveContext.magicAcquired = 1;
    gSaveContext.magicLevel = 2;
    gSaveContext.magic = 48;
    gSaveContext.dungeonKeys[6] = 8;
    gSaveContext.dungeonKeys[7] = 5;
    gSaveContext.ammo[2] = 20; //bombs
    gSaveContext.ammo[3] = 20; //arrows
#endif

    /*-----------------------------------
    |       THINGS TO ALWAYS SET        |
    -----------------------------------*/
    gSaveContext.cutsceneIndex = 0;          //no intro cutscene
    gSaveContext.infTable   [0x0] |= 0x01;   //greeted by Saria
    gSaveContext.infTable  [0x11] |= 0x0400; //Met Darunia in Fire Temple
    gSaveContext.infTable  [0x14] |= 0x000E; //Ruto in Jabu can be escorted immediately
    gSaveContext.eventChkInf[0x3] |= 0x0800; //began Nabooru Battle
    gSaveContext.eventChkInf[0x7] |= 0x01DF; //began boss battles (except Twinrova and Ganondorf)
    gSaveContext.eventChkInf[0x9] |= 0x0010; //Spoke to Nabooru as child
    gSaveContext.eventChkInf[0xA] |= 0x017B; //entrance cutscenes (minus temple of time)
    gSaveContext.eventChkInf[0xB] |= 0x07FF; //more entrance cutscenes
    gSaveContext.eventChkInf[0xC] |= 0x0001; //Nabooru ordered to fight by Twinrova
    gSaveContext.eventChkInf[0xC] |= 0x8000; //Forest Temple entrance cutscene (3ds only)

    gSaveContext.sceneFlags[5].swch |= 0x00010000; //remove Ruto cutscene in Water Temple

    //navi text triggers
    gSaveContext.sceneFlags[DUNGEON_DEKU_TREE]      .swch |= 0x80080400; //deku tree vines and door and rolling spike
    gSaveContext.sceneFlags[DUNGEON_DODONGOS_CAVERN].swch |= 0x00004900; //dodongo entrance text and spike trap text
    gSaveContext.sceneFlags[DUNGEON_JABUJABUS_BELLY].swch |= 0x0F010000; //jabu jabu
    gSaveContext.sceneFlags[DUNGEON_FOREST_TEMPLE]  .swch |= 0x01C00300; //forest temple
    gSaveContext.sceneFlags[DUNGEON_FIRE_TEMPLE]    .swch |= 0x00080000; //fire temple
    gSaveContext.sceneFlags[DUNGEON_WATER_TEMPLE]   .swch |= 0x00000080; //water temple
    gSaveContext.sceneFlags[DUNGEON_ICE_CAVERN]     .swch |= 0x00000020; //ice cavern TODO: doesn't work
    gSaveContext.sceneFlags[86].swch |= 0x00004000; //sacred forest meadow

    //open lowest Fire Temple locked door (to prevent key logic lockouts)
    gSaveContext.sceneFlags[DUNGEON_FIRE_TEMPLE].swch |= 0x00800000;

    //open middle locked door in Water Temple (to prevent key logic lockouts)
    gSaveContext.sceneFlags[DUNGEON_WATER_TEMPLE].swch |= 0x00200000;

    /*-----------------------------------
    |THINGS TO SET DEPENDING ON SETTINGS|
    -----------------------------------*/

    if (gSettingsContext.rainbowBridge == RAINBOWBRIDGE_OPEN) {
        gSaveContext.eventChkInf[0x4] |= 0x2000;
    }

    if (gSettingsContext.startingAge == AGE_ADULT) {
        gSaveContext.linkAge       = AGE_ADULT;  //age is adult
        gSaveContext.entranceIndex = 0xF4050000; //spawn at temple of time
        gSaveContext.sceneIndex    = 0x6100;     //^
        gSaveContext.childEquips.equipment = 0x1100; //Child equips Kokiri Tunic and Kokiri Boots, no sword or shield
        gSaveContext.adultEquips.equipment = 0x1120; //Adult equips Kokiri Tunic, Kokiri Boots, and Master Sword
        gSaveContext.infTable[29]  = 0x00; //Unset swordless flag
    }

    //set master quest flag for mirror world
    if (gSettingsContext.mirrorWorld == ON) {
        gSaveContext.masterQuestFlag = 1;
    }

    if (gSettingsContext.startingTime == STARTINGTIME_NIGHT) {
        gSaveContext.dayTime = 0x1400; //Set night time
    }

    //give Link the starting stone or medallion
    gSaveContext.questItems |= gSettingsContext.dungeonRewardBitMask;

    if (gSettingsContext.openDoorOfTime) {
        gSaveContext.eventChkInf[0x4] |= 0x0800; //Open Door of Time
    }

    if (gSettingsContext.gerudoFortress == GERUDOFORTRESS_FAST) {
        gSaveContext.eventChkInf[0x9] |= 0x000E; //Free 3 carpenters
        gSaveContext.sceneFlags[DUNGEON_GERUDO_FORTRESS].swch    |= 0x000D01DC; //heard yells/unlocked doors
        gSaveContext.sceneFlags[DUNGEON_GERUDO_FORTRESS].collect |= 0x0000C400; //picked up keys
    } else if (gSettingsContext.gerudoFortress == GERUDOFORTRESS_OPEN) {
        gSaveContext.eventChkInf[0x9] |= 0x000F; //Free all carpenters
        gSaveContext.sceneFlags[DUNGEON_GERUDO_FORTRESS].swch    |= 0x000F01FE; //heard yells/unlocked doors
        gSaveContext.sceneFlags[DUNGEON_GERUDO_FORTRESS].collect |= 0x0000D400; //picked up keys
    }

    if (gSettingsContext.zorasFountain == ZORASFOUNTAIN_OPEN) {
        gSaveContext.eventChkInf[0x3] |= 0x0008; //King Zora Moved Aside
    }

    //Set Skipped Trials
    gSaveContext.eventChkInf[0xB] |= (gSettingsContext.forestTrialSkip) ? 0x0800 : 0;
    gSaveContext.eventChkInf[0xB] |= (gSettingsContext.fireTrialSkip)   ? 0x4000 : 0;
    gSaveContext.eventChkInf[0xB] |= (gSettingsContext.waterTrialSkip)  ? 0x1000 : 0;
    gSaveContext.eventChkInf[0xA] |= (gSettingsContext.spiritTrialSkip) ? 0x2000 : 0;
    gSaveContext.eventChkInf[0xB] |= (gSettingsContext.shadowTrialSkip) ? 0x2000 : 0;
    gSaveContext.eventChkInf[0xB] |= (gSettingsContext.lightTrialSkip)  ? 0x8000 : 0;

    if (gSettingsContext.forestTrialSkip && gSettingsContext.fireTrialSkip && gSettingsContext.waterTrialSkip &&
        gSettingsContext.spiritTrialSkip && gSettingsContext.shadowTrialSkip && gSettingsContext.lightTrialSkip) {
          gSaveContext.eventChkInf[0xC] |= 0x0008; //dispel Ganon's Tower Barrier
    }

    if (gSettingsContext.fourPoesCutscene == SKIP) {
        gSaveContext.sceneFlags[3].swch |= 0x08000000; //Remove Poe cutscene in Forest Temple
    }

    if (gSettingsContext.templeOfTimeIntro == SKIP) {
        gSaveContext.eventChkInf[0xA] |= 0x0080; //Remove Temple of Time intro cutscene
    }

    //give maps and compasses
    if (gSettingsContext.mapsAndCompasses == MAPSANDCOMPASSES_START_WITH) {
        for (u8 i = 0; i < 0xA; i++) {
            gSaveContext.dungeonItems[i] |= 0x6;
        }
    }

    //give small keys
    if (gSettingsContext.keysanity == KEYSANITY_START_WITH) {
      gSaveContext.dungeonKeys[DUNGEON_FOREST_TEMPLE] = 5;
      gSaveContext.dungeonKeys[DUNGEON_FIRE_TEMPLE] = 8;
      gSaveContext.dungeonKeys[DUNGEON_WATER_TEMPLE] = 6;
      gSaveContext.dungeonKeys[DUNGEON_SPIRIT_TEMPLE] = 5;
      gSaveContext.dungeonKeys[DUNGEON_SHADOW_TEMPLE] = 5;
      gSaveContext.dungeonKeys[DUNGEON_BOTTOM_OF_THE_WELL] = 3;
      gSaveContext.dungeonKeys[DUNGEON_GERUDO_TRAINING_GROUNDS] = 9;
      gSaveContext.dungeonKeys[DUNGEON_GANONS_CASTLE_FIRST_PART] = 2;
    }

    //give boss keys
    if (gSettingsContext.bossKeysanity == BOSSKEYSANITY_START_WITH) {
      for (u8 i = 3; i < 8; i++) {
        gSaveContext.dungeonItems[i] |= 0x1;
      }
    }

    //give Ganon's Castle Boss Key
    if (gSettingsContext.ganonsBossKey == GANONSBOSSKEY_START_WITH) {
      gSaveContext.dungeonItems[DUNGEON_GANONS_CASTLE_SECOND_PART] |= 0x1;
    }

    //give the Gerudo Token if Gerudo Fortress is Open and Shuffle Gerudo Card is off
    if (gSettingsContext.gerudoFortress == GERUDOFORTRESS_OPEN && gSettingsContext.shuffleGerudoToken == OFF) {
      gSaveContext.questItems |= 0x00400000;
    }

    gSaveContext.eventChkInf[0x0] |= 0x14;   //spoke to mido and moved him
    gSaveContext.eventChkInf[0x0] |= 0x1020; //met deku tree and opened mouth

    gSaveContext.eventChkInf[0x4] |= 0x8020; //entered MS chamber, Pulled MS from pedestal

    gSaveContext.eventChkInf[0xC] |= 0x0020; //Sheik Spawned at MS pedestal as Adult
}

void SaveFile_SaveChildBButton(void) {
    gSaveContext.infTable[29] &= 0x00; //Unset the swordless flag when going adult
}

u16 SaveFile_RestoreChildEquips(void) {
    //if Kokiri Sword is not on child B button
    if (gSaveContext.childEquips.buttonItems[0] != ITEM_SWORD_KOKIRI) {
      gSaveContext.infTable[29] |= 0x1; //set swordless flag
    }
    return (gSaveContext.childEquips.equipment & 0xFFF0) | (gSaveContext.equipment & 0x1);
}

u32 SaveFile_CheckGerudoToken(void) {
    return ((gSaveContext.questItems & 0x400000) != 0) ? 1 : 0;
}
