#include "common.h"

#include "cutscene.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "mainproc.h"

// forward declarations
void updateCutsceneSpriteAnimation(u16);                               
void endCutsceneSpriteAnimationSequence(u16 index);
void updateCutsceneEntityMovement(u16);                               
void updateCutsceneMapViewPosition(u16);        
void cutsceneHandlerAdjustAnimationDataPtr(u16 index);
void cutsceneHandlerSetAnimationDataPtrWithFlag(u16);
void cutsceneHandlerSetAnimationDataPtr(u16);
void cutsceneHandlerSetCoordinates(u16);
void cutsceneHandlerSetFrameDelta(u16);
void cutsceneHandlerSetWaitFrames(u16);
void cutsceneHandlerDeactivateSelf(u16);
void cutsceneHandlerExecuteSubroutine(u16);
void cutsceneHandlerReturnFromSubroutine(u16);
void cutsceneHandlerBranchOnCurrentButton(u16);
void cutsceneHandlerBranchOnButtonPressed(u16);
void cutsceneHandlerBranchOnButtonRepeat(u16);
void cutsceneHandlerSpawnExecutor(u16);
void cutsceneHandlerSetOtherExecutorBytecodePtr(u16);
void cutsceneHandlerDeactivateExecutor(u16);
void cutsceneHandlerDMASprite(u16);
void cutsceneHandlerSetEntityAnimations(u16);
void cutsceneHandlerDoDMA(u16);
void cutsceneHandlerSetU8Value(u16);
void cutsceneHandlerSetU16Value(u16);
void cutsceneHandlerSetU32Value(u16);
void cutsceneHandlerBranchU8VarInRange(u16);
void cutsceneHandlerBranchU16VarInRange(u16);
void cutsceneHandlerBranchU32VarInRange(u16);
void cutsceneHandlerSetSpecialBit(u16);
void cutsceneHandlerClearSpecialBit(u16);
void cutsceneHandlerBranchOnSpecialBit(u16);
void cutsceneHandlerSetAssetRotation(u16);
void cutsceneHandlerSetupMapAsset(u16);
void cutsceneHandlerEntityMoveAndAnimate(u16);
void cutsceneHandlerSetMapRotation(u16);
void cutsceneHandlerSetBehaviorFlags(u16);
void cutsceneHandlerSetMovementInformation(u16);
void cutsceneHandlerInitializeMessageBoxType1(u16);
void cutsceneHandlerWaitMessageBoxClosed(u16);
void cutsceneHandlerSetMessageBoxViewSpacePosition(u16);
void cutsceneHandlerResetMessageBoxAvatar(u16);
void cutsceneHandlerEntityMove(u16);
void cutsceneHandlerSetEntityAnimation(u16);
void cutsceneHandlerSetEntityAnimationWithDirectionChange(u16);
void cutsceneHandlerSetCallbackBytecodePtr(u16);
void cutsceneHandlerPauseEntity(u16);
void cutsceneHandlerTogglePauseEntity(u16);
void cutsceneHandlerFlipEntityDirection(u16);
void cutsceneHandlerPauseEntities(u16);
void cutsceneHandlerTogglePauseEntities(u16);
void cutsceneHandlerFlipEntityAnimation(u16);
void cutsceneHandlerSetEntityNonCollidable(u16);
void cutsceneHandlerSetupEntity(u16);
void cutsceneHandlerSetEntityMapSpaceIndependentFlag(u16);
void cutsceneHandlerLoadMap(u16);
void cutsceneHandlerSetEntityMapSpaceIndependent(u16);
void cutsceneHandlerSetRGBA(u16);
void cutsceneHandlerUpdateRGBA(u16);
void cutsceneHandlerUpdateU8Value(u16);
void cutsceneHandlerUpdateU16Value(u16);
void cutsceneHandlerUpdateU32Value(u16);
void cutsceneHandlerDeactivateMapObjects(u16);
void cutsceneHandlerUpdateGlobalRGBA(u16);
void cutsceneHandlerDeactivateSprites(u16);
void cutsceneHandlerDeactivateMapControllers(u16);
void cutsceneHandlerWaitRgbaFinished(u16);
void cutsceneHandlerCheckEntityCollision(u16);
void cutsceneHandlerInitializeDialogueSession(u16);
void cutsceneHandlerWaitForDialogueInput(u16);
void cutsceneHandlerBranchOnDialogue(u16);
void cutsceneHandlerWaitEntityAnimation(u16);
void cutsceneHandlerSetMessageBoxAssetIndices(u16);
void cutsceneHandlerSetEntityTrackingTarget(u16);
void cutsceneHandlerSetAltAnimationFlag(u16);
void cutsceneHandlerWaitMapLoad(u16);
void cutsceneHandlerBranchOnEntityDirection(u16);
void cutsceneHandlerSetEntityPhysicsFlags(u16);
void cutsceneHandlerSetEntityPalette(u16);
void cutsceneHandlerSetEntitySpriteDimensions(u16);
void cutsceneHandlerSetShadowFlags(u16);
void cutsceneHandlerSetSpriteScale(u16);
void cutsceneHandlerSetSpriteRenderngLayer(u16);
void cutsceneHandlerInitializeMessageBoxType2(u16);
void cutsceneHandlerInitMapAddition(u16);
void cutsceneHandlerBranchOnRandom(u16);
void cutsceneHandlerBranchIfU16PtrInRange(u16);
void cutsceneHandlerPauseExecutor(u16);
void cutsceneHandlerTogglePauseExecutor(u16);
void cutsceneHandlerPauseAllChildExecutors(u16);
void cutsceneHandlerTogglePauseAllChildExecutors(u16);
void cutsceneHandlerSetSpritePalette(u16);
void cutsceneHandlerBranchIfU8PtrInRange(u16);
void cutsceneHandlerSetAudioSequence(u16);
void cutsceneHandlerStopAudioSequenceWithFadeOut(u16);
void cutsceneHandlerSetAudioSequenceVolume(u16);
void cutsceneHandlerSetSfx(u16);
void cutsceneHandlerIdleWhileAudioSequencePlaying(u16);
void cutsceneHandlerUpdateMessageBoxRGBA(u16);
void cutsceneHandlerWaitMessageBoxReady(u16);
void cutsceneHandlerSetSpriteBilinearFiltering(u16);
void cutsceneHandlerSetMapAddition(u16);
void cutsceneHandlerSetMapGroundObject(u16);
void cutsceneHandlerSetMessageInterpolation(u16);

// data
void (*cutsceneCommandHandlers[])(u16) = {
    cutsceneHandlerSetAnimationDataPtrWithFlag,
    cutsceneHandlerSetAnimationDataPtr,
    cutsceneHandlerSetCoordinates,
    cutsceneHandlerSetFrameDelta,
    cutsceneHandlerSetWaitFrames,
    cutsceneHandlerDeactivateSelf,
    cutsceneHandlerExecuteSubroutine,
    cutsceneHandlerReturnFromSubroutine,
    cutsceneHandlerBranchOnCurrentButton,
    cutsceneHandlerBranchOnButtonPressed,
    cutsceneHandlerBranchOnButtonRepeat,
    cutsceneHandlerSpawnExecutor,
    cutsceneHandlerSetOtherExecutorBytecodePtr,
    cutsceneHandlerDeactivateExecutor,
    cutsceneHandlerDMASprite,
    cutsceneHandlerSetEntityAnimations,
    cutsceneHandlerDoDMA,
    cutsceneHandlerSetU8Value,
    cutsceneHandlerSetU16Value,
    cutsceneHandlerSetU32Value,
    cutsceneHandlerBranchU8VarInRange,
    cutsceneHandlerBranchU16VarInRange,
    cutsceneHandlerBranchU32VarInRange,
    cutsceneHandlerSetSpecialBit,
    cutsceneHandlerClearSpecialBit,
    cutsceneHandlerBranchOnSpecialBit,
    cutsceneHandlerSetAssetRotation,
    cutsceneHandlerSetupMapAsset,
    cutsceneHandlerEntityMoveAndAnimate,
    cutsceneHandlerSetMapRotation,
    cutsceneHandlerSetBehaviorFlags,
    cutsceneHandlerSetMovementInformation,
    cutsceneHandlerInitializeMessageBoxType1,
    cutsceneHandlerWaitMessageBoxClosed,
    cutsceneHandlerSetMessageBoxViewSpacePosition,
    cutsceneHandlerResetMessageBoxAvatar,
    cutsceneHandlerEntityMove,
    cutsceneHandlerSetEntityAnimation,
    cutsceneHandlerSetEntityAnimationWithDirectionChange,
    cutsceneHandlerSetCallbackBytecodePtr,
    cutsceneHandlerPauseEntity,
    cutsceneHandlerTogglePauseEntity,
    cutsceneHandlerFlipEntityDirection,
    cutsceneHandlerPauseEntities,
    cutsceneHandlerTogglePauseEntities,
    cutsceneHandlerFlipEntityAnimation,
    cutsceneHandlerSetEntityNonCollidable,
    cutsceneHandlerSetupEntity,
    cutsceneHandlerSetEntityMapSpaceIndependentFlag,
    cutsceneHandlerLoadMap,
    cutsceneHandlerSetEntityMapSpaceIndependent,
    cutsceneHandlerSetRGBA,
    cutsceneHandlerUpdateRGBA,
    cutsceneHandlerUpdateU8Value,
    cutsceneHandlerUpdateU16Value,
    cutsceneHandlerUpdateU32Value,
    cutsceneHandlerDeactivateMapObjects,
    cutsceneHandlerUpdateGlobalRGBA,
    cutsceneHandlerDeactivateSprites,
    cutsceneHandlerDeactivateMapControllers,
    cutsceneHandlerWaitRgbaFinished,
    cutsceneHandlerCheckEntityCollision,
    cutsceneHandlerInitializeDialogueSession,
    cutsceneHandlerWaitForDialogueInput,
    cutsceneHandlerBranchOnDialogue,
    cutsceneHandlerWaitEntityAnimation,
    cutsceneHandlerSetMessageBoxAssetIndices,
    cutsceneHandlerSetEntityTrackingTarget,
    cutsceneHandlerSetAltAnimationFlag,
    cutsceneHandlerWaitMapLoad,
    cutsceneHandlerBranchOnEntityDirection,
    cutsceneHandlerSetEntityPhysicsFlags,
    cutsceneHandlerSetEntityPalette,
    cutsceneHandlerSetEntitySpriteDimensions,
    cutsceneHandlerSetShadowFlags,
    cutsceneHandlerSetSpriteScale,
    cutsceneHandlerSetSpriteRenderngLayer,
    cutsceneHandlerInitializeMessageBoxType2,
    cutsceneHandlerInitMapAddition,
    cutsceneHandlerBranchOnRandom,
    cutsceneHandlerBranchIfU16PtrInRange,
    cutsceneHandlerPauseExecutor,
    cutsceneHandlerTogglePauseExecutor,
    cutsceneHandlerPauseAllChildExecutors,
    cutsceneHandlerTogglePauseAllChildExecutors,
    cutsceneHandlerSetSpritePalette,
    cutsceneHandlerBranchIfU8PtrInRange,
    cutsceneHandlerSetAudioSequence,
    cutsceneHandlerStopAudioSequenceWithFadeOut,
    cutsceneHandlerSetAudioSequenceVolume,
    cutsceneHandlerSetSfx,
    cutsceneHandlerIdleWhileAudioSequencePlaying,
    cutsceneHandlerUpdateMessageBoxRGBA,
    cutsceneHandlerWaitMessageBoxReady,
    cutsceneHandlerSetSpriteBilinearFiltering,
    cutsceneHandlerSetMapAddition,
    cutsceneHandlerSetMapGroundObject,
    cutsceneHandlerSetMessageInterpolation
};

// bss
CutsceneExecutor cutsceneExecutors[MAX_BYTECODE_EXECUTORS];

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", initializeCutsceneExecutors);

void initializeCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        
        cutsceneExecutors[i].assetIndex = 0;
        cutsceneExecutors[i].waitFrames = 0;
        cutsceneExecutors[i].animationFrameCounter = 0;
        cutsceneExecutors[i].entityDirectionOrMapRotation = 0;
        cutsceneExecutors[i].movementDistance = 0;

        cutsceneExecutors[i].frameDelta.x = 0;
        cutsceneExecutors[i].frameDelta.y = 0;
        cutsceneExecutors[i].frameDelta.z = 0;

        cutsceneExecutors[i].movementVector.x = 0;
        cutsceneExecutors[i].movementVector.y = 0;
        cutsceneExecutors[i].movementVector.z = 0;

        cutsceneExecutors[i].flags = 0;

        cutsceneExecutors[i].animationIndex1 = 0;
        cutsceneExecutors[i].animationIndex2 = 0;
        cutsceneExecutors[i].animationIndex3 = 0;
        cutsceneExecutors[i].animationIndex4 = 0;
        cutsceneExecutors[i].animationIndex5 = 0;
        cutsceneExecutors[i].animationIndex6 = 0;

        cutsceneExecutors[i].entityCollidedWithIndex = 0xFFFF;

        cutsceneExecutors[i].targetPosition.x = 0;
        cutsceneExecutors[i].targetPosition.y = 0;
        cutsceneExecutors[i].targetPosition.z = 0;

        cutsceneExecutors[i].behaviorFlags = 0;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", spawnCutsceneExecutor);

bool spawnCutsceneExecutor(u16 index, void *bytecodePtr) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS) {

        if (!(cutsceneExecutors[index].flags & CUTSCENE_ASSET_ACTIVE)) {
            
            cutsceneExecutors[index].bytecodePtr = bytecodePtr;
            
            cutsceneExecutors[index].waitFrames = 0;
            
            cutsceneExecutors[index].frameDelta.x = 0;
            cutsceneExecutors[index].frameDelta.y = 0;
            cutsceneExecutors[index].frameDelta.z = 0;

            cutsceneExecutors[index].movementVector.x = 0;
            cutsceneExecutors[index].movementVector.y = 0;
            cutsceneExecutors[index].movementVector.z = 0;

            cutsceneExecutors[index].flags = CUTSCENE_ASSET_ACTIVE;
            
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046A58);

// unused
// bool func_80046A58(u16 index, void* bytecodePtr) {
    
//     bool result = FALSE;

//     if (index < MAX_BYTECODE_EXECUTORS) {
        
//         if (cutsceneExecutors[index].flags & CUTSCENE_ASSET_ACTIVE) {
            
//             cutsceneExecutors[index].waitFrames = 0;
//             cutsceneExecutors[index].bytecodePtr = bytecodePtr;
            
//             result = TRUE;

//         }
//     }

//     return result;

// }

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", deactivateCutsceneExecutor);

bool deactivateCutsceneExecutor(u16 index) {

    bool result = FALSE;

    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & CUTSCENE_ASSET_ACTIVE)) {

        if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
            deactivateSprite(cutsceneExecutors[index].assetIndex);
        }

        if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
            deactivateEntity(cutsceneExecutors[index].assetIndex);
        }

        if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
            unloadMapAssets(cutsceneExecutors[index].assetIndex);
        }
        
        result = TRUE;

    }

    cutsceneExecutors[index].entityCollidedWithIndex = 0xFFFF;
    cutsceneExecutors[index].flags = 0;

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", deactivateCutsceneExecutors);

void deactivateCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        deactivateCutsceneExecutor(i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046BF8);

bool func_80046BF8(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[index].flags |= CUTSCENE_PAUSE_EXECUTION;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046C48);

bool func_80046C48(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[index].flags &= ~CUTSCENE_PAUSE_EXECUTION;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", pauseAllCutsceneExecutors);

void pauseAllCutsceneExecutors(void) {

    u16 i = 0;
    u16 j;
    
    // ??
    for (; ;) {
        j = i;
        if (j < MAX_BYTECODE_EXECUTORS) {
            if (cutsceneExecutors[j].flags & CUTSCENE_ASSET_ACTIVE) {
                cutsceneExecutors[j].flags |= CUTSCENE_PAUSE_EXECUTION;
            }
        }
        i++;
        if (i > (MAX_BYTECODE_EXECUTORS - 1)) break;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", resumeCutsceneExecutors);

void resumeCutsceneExecutors(void) {

    u16 i = 0;
    u16 j;
    
    for (; ;) {
        j = i;
        if (j < MAX_BYTECODE_EXECUTORS) {
            if (cutsceneExecutors[j].flags & CUTSCENE_ASSET_ACTIVE) {
                cutsceneExecutors[j].flags &= ~CUTSCENE_PAUSE_EXECUTION;
            }
        }
        i++;
        if (i > (MAX_BYTECODE_EXECUTORS - 1)) break;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneUpdateValue);

// same as adjustValue
inline s32 cutsceneUpdateValue(s32 initial, s32 value, s32 max) {
    
    s32 temp;
    s32 adjusted;

    adjusted = value;
    temp = initial + adjusted;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;

}
 
//INCLUDE_ASM("asm/nonmatchings/system/cutscene", updateCutsceneExecutors);

void updateCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        
        if (cutsceneExecutors[i].flags & CUTSCENE_ASSET_ACTIVE && !(cutsceneExecutors[i].flags & CUTSCENE_PAUSE_EXECUTION)) {
            
            if (cutsceneExecutors[i].flags & CUTSCENE_ENTITY_ASSET) {
                
                cutsceneExecutors[i].coordinates.x = entities[cutsceneExecutors[i].assetIndex].coordinates.x;
                cutsceneExecutors[i].coordinates.y = entities[cutsceneExecutors[i].assetIndex].coordinates.y;
                cutsceneExecutors[i].coordinates.z = entities[cutsceneExecutors[i].assetIndex].coordinates.z;
                
                if (cutsceneExecutors[i].entityCollidedWithIndex != 0xFFFF && entities[cutsceneExecutors[i].assetIndex].entityCollidedWithIndex == cutsceneExecutors[i].entityCollidedWithIndex && cutsceneExecutors[i].targetButtonMask == entities[cutsceneExecutors[i].assetIndex].buttonPressed) {
                    cutsceneExecutors[i].entityCollidedWithIndex = 0xFFFF;
                    cutsceneExecutors[i].waitFrames = 0;
                    cutsceneExecutors[i].bytecodePtr = cutsceneExecutors[i].callbackBytecodePtr;
                }

            }
            
            if (cutsceneExecutors[i].flags & CUTSCENE_MAP_ASSET) {
                cutsceneExecutors[i].coordinates.x = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.x;
                cutsceneExecutors[i].coordinates.y = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.y;
                cutsceneExecutors[i].coordinates.z = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.z;
            }

            if (cutsceneExecutors[i].waitFrames != 0) {
                 cutsceneExecutors[i].waitFrames--;
                if (cutsceneExecutors[i].waitFrames != 0) {
                    goto skip_callback;
                }
            }
      
 inner_loop:    
            while (TRUE) {
                
                cutsceneExecutors[i].frameDelta.x = 0;
                cutsceneExecutors[i].frameDelta.y = 0;
                cutsceneExecutors[i].frameDelta.z = 0;

                cutsceneCommandHandlers[*(u16*)(cutsceneExecutors[i].bytecodePtr)](i);
                
skip_callback:
                cutsceneExecutors[i].coordinates.x += cutsceneExecutors[i].frameDelta.x;
                cutsceneExecutors[i].coordinates.y += cutsceneExecutors[i].frameDelta.y;
                cutsceneExecutors[i].coordinates.z += cutsceneExecutors[i].frameDelta.z;

                if (cutsceneExecutors[i].waitFrames == 0)  {
                    goto inner_loop;
                } else {
                    break;
                }
        
            }
            
            if (cutsceneExecutors[i].flags & CUTSCENE_MAP_ASSET) {
                updateCutsceneMapViewPosition(i);
            }
            
            if (cutsceneExecutors[i].flags & CUTSCENE_SPRITE_ASSET) {
                updateCutsceneSpriteAnimation(i);
            }

            if (cutsceneExecutors[i].flags & CUTSCENE_ENTITY_ASSET) {
                updateCutsceneEntityMovement(i);
            }

        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", updateCutsceneSpriteAnimation);

void updateCutsceneSpriteAnimation(u16 index) {

    u16 animation;

    u8 flag;
    u8 flipFlags;

    if (cutsceneExecutors[index].animationFrameCounter == 0)  {
        
        while (cutsceneExecutors[index].animationFrameCounter == 0) {
            
loop:
            resetAnimationState(cutsceneExecutors[index].assetIndex);    
    
            animation = *(u16*)cutsceneExecutors[index].animationDataPtr;
    
            // FIXME: fake, cursed gotos
            if (animation == 0xFFFE) {
                goto label;
            } 
            
            if (animation == 0xFFFF) {
                
                endCutsceneSpriteAnimationSequence(index);
                goto check;
                
label:
                cutsceneHandlerAdjustAnimationDataPtr(index);
                goto check;
                
            } 
    
                cutsceneExecutors[index].animationDataPtr += 2;
    
                flag = *(u8*)cutsceneExecutors[index].animationDataPtr;
    
                cutsceneExecutors[index].animationDataPtr++;
                
                cutsceneExecutors[index].movementVector.x = *(s8*)cutsceneExecutors[index].animationDataPtr;
    
                cutsceneExecutors[index].animationDataPtr++;
    
                cutsceneExecutors[index].movementVector.y = *(s8*)cutsceneExecutors[index].animationDataPtr;
    
                cutsceneExecutors[index].animationDataPtr++;
    
                cutsceneExecutors[index].movementVector.z = *(s8*)cutsceneExecutors[index].animationDataPtr;
    
                cutsceneExecutors[index].animationDataPtr++;
                        
                startSpriteAnimation(cutsceneExecutors[index].assetIndex, animation, flag);
    
                cutsceneExecutors[index].animationFrameCounter = *(u8*)cutsceneExecutors[index].animationDataPtr;
                
                cutsceneExecutors[index].animationDataPtr++;
                
                flipFlags = *(u8*)cutsceneExecutors[index].animationDataPtr;
    
                cutsceneExecutors[index].animationDataPtr++;
                
                setSpriteFlip(cutsceneExecutors[index].assetIndex, flipFlags & 1, flipFlags & 2);
    
                setSpriteViewSpacePosition(cutsceneExecutors[index].assetIndex, 
                    cutsceneExecutors[index].coordinates.x + cutsceneExecutors[index].movementVector.x, 
                    cutsceneExecutors[index].coordinates.y + cutsceneExecutors[index].movementVector.y, 
                    cutsceneExecutors[index].coordinates.z + cutsceneExecutors[index].movementVector.z);
    
                if (flag == 0xFE) {
                    cutsceneExecutors[index].flags |= CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT;
                } else {
                    cutsceneExecutors[index].flags &= ~CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT;
                }
                
            }
    

    } else {
        
       setSpriteViewSpacePosition(cutsceneExecutors[index].assetIndex, 
            cutsceneExecutors[index].coordinates.x + cutsceneExecutors[index].movementVector.x, 
            cutsceneExecutors[index].coordinates.y + cutsceneExecutors[index].movementVector.y, 
            cutsceneExecutors[index].coordinates.z + cutsceneExecutors[index].movementVector.z);
    
        if (cutsceneExecutors[index].animationFrameCounter == 0xFF) {
    
            if (getSpriteAnimationStateChangedFlag(cutsceneExecutors[index].assetIndex) && !(cutsceneExecutors[index].flags & CUTSCENE_ANIMATION_LOOP)) {
                cutsceneExecutors[index].animationFrameCounter = 0;
            } 
            
        } else {
            cutsceneExecutors[index].animationFrameCounter--;
        }

check:
        if (cutsceneExecutors[index].animationFrameCounter == 0) goto loop;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", endCutsceneSpriteAnimationSequence);

void endCutsceneSpriteAnimationSequence(u16 index) {
    cutsceneExecutors[index].animationFrameCounter = 1;
    cutsceneExecutors[index].flags &= ~CUTSCENE_ASSET_BEHAVIOR_WANDER;
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerAdjustAnimationDataPtr); 

void cutsceneHandlerAdjustAnimationDataPtr(u16 index) {

    cutsceneExecutors[index].animationDataPtr += 2;
    cutsceneExecutors[index].animationDataPtr += *(s16*)(cutsceneExecutors[index].animationDataPtr);

}
  
//INCLUDE_ASM("asm/nonmatchings/system/cutscene", updateCutsceneEntityMovement);

void updateCutsceneEntityMovement(u16 index) {

    Vec3f vec;

    u32 temp;

    f32 f1;
    f32 f2;
    f32 f3;
    f32 f4;
    f32 f5;
    f32 f6;
    f32 f7;
    f32 f8;
    
    if ((cutsceneExecutors[index].behaviorFlags & (CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT | CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT))) {

        if (!(entities[cutsceneExecutors[index].assetIndex].flags & ENTITY_PAUSED)) {

            if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_WANDER) {
                
                if (!getRandomNumberInRange(0, 16)) {
    
                    temp = getRandomNumberInRange(0, 16);
    
                    if (temp < 8) {

                        cutsceneExecutors[index].entityDirectionOrMapRotation = ((s32)temp & 0xFFFE);

                        setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].entityDirectionOrMapRotation, gMainMapIndex));

                        if (cutsceneExecutors[index].entityDirectionOrMapRotation) {
                            vec = getMovementVectorFromDirection(cutsceneExecutors[index].movementDistance, convertWorldToSpriteDirection(entities[cutsceneExecutors[index].assetIndex].direction, gMainMapIndex), 0.0f);    
                        } else {
                            vec = getMovementVectorFromDirection(cutsceneExecutors[index].movementDistance, convertWorldToSpriteDirection(entities[cutsceneExecutors[index].assetIndex].direction, gMainMapIndex), 0.0f);
                        }
                        
                        cutsceneExecutors[index].movementVector.x = vec.x;
                        cutsceneExecutors[index].movementVector.y = vec.y;
                        cutsceneExecutors[index].movementVector.z = vec.z;

                        if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT) {

                            if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
                                setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex5);
                            } else {
                                setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex2);
                            }
                            
                        } else if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
                            setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex6);
                        } else {
                            setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex3);
                        }
                        
                    } else {
    
                        cutsceneExecutors[index].movementVector.x = 0.0f;
                        cutsceneExecutors[index].movementVector.y = 0.0f;
                        cutsceneExecutors[index].movementVector.z = 0.0f;
    
                        if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
                            setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex4);
                        } else {
                            setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex1);
                        }
                        
                    }
                    
                }
                
            } else {
                
                if (cutsceneExecutors[index].frameDelta.x == 0.0f && cutsceneExecutors[index].frameDelta.y == 0.0f && cutsceneExecutors[index].frameDelta.z == 0.0f) {
    
                    if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {    
                        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex4);
                    } else {
                        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex1);
                    }

                    cutsceneExecutors[index].movementVector.x = 0.0f;
                    cutsceneExecutors[index].movementVector.y = 0.0f;
                    cutsceneExecutors[index].movementVector.z = 0.0f;
                        
                } else {

                    cutsceneExecutors[index].movementVector.x = cutsceneExecutors[index].frameDelta.x;
                    cutsceneExecutors[index].movementVector.y = cutsceneExecutors[index].frameDelta.y;
                    cutsceneExecutors[index].movementVector.z = cutsceneExecutors[index].frameDelta.z;
                    
                }
            
            }
            
        } else {
            
            if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {    
                setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex4);
            } else {
                setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex1);
            }
    
            cutsceneExecutors[index].movementVector.x = 0.0f;
            cutsceneExecutors[index].movementVector.y = 0.0f;
            cutsceneExecutors[index].movementVector.z = 0.0f;
                
        }
        
    } else {
        cutsceneExecutors[index].movementVector.x = cutsceneExecutors[index].frameDelta.x;
        cutsceneExecutors[index].movementVector.y = cutsceneExecutors[index].frameDelta.y;
        cutsceneExecutors[index].movementVector.z = cutsceneExecutors[index].frameDelta.z;
    }

    if (!(detectEntityOverlap(&entities[cutsceneExecutors[index].assetIndex], 
            cutsceneExecutors[index].entityCollidedWithIndex, 
            cutsceneExecutors[index].movementVector.x, 
            cutsceneExecutors[index].movementVector.z,             
            entityAssetDescriptors[entities[cutsceneExecutors[index].assetIndex].entityAssetIndex].spriteWidth,
            entityAssetDescriptors[entities[cutsceneExecutors[index].assetIndex].entityAssetIndex].spriteHeight)) 
        
        || entities[cutsceneExecutors[index].assetIndex].flags & ENTITY_Y_MOVEMENT) {

            if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_WANDER) {
    
                f1 = cutsceneExecutors[index].targetPosition.x + cutsceneExecutors[index].collisionWidth;
                f2 = cutsceneExecutors[index].coordinates.x + cutsceneExecutors[index].movementVector.x;
    
                if (f2 < f1) {
    
                    f3 = cutsceneExecutors[index].targetPosition.x - cutsceneExecutors[index].collisionWidth;
    
                    if (f3 < f2) {
                        
                        f5 = cutsceneExecutors[index].targetPosition.z + cutsceneExecutors[index].collisionHeight;
                        f6 = cutsceneExecutors[index].coordinates.z + cutsceneExecutors[index].movementVector.z;
    
                        if (f6 < f5) {
    
                            f7 = cutsceneExecutors[index].targetPosition.z - cutsceneExecutors[index].collisionHeight;
                            f8 = cutsceneExecutors[index].coordinates.z + cutsceneExecutors[index].movementVector.z;
                            
                            if (f7 < f8) {
setMovementVector:
                                setEntityMovementVector(cutsceneExecutors[index].assetIndex, 
                                      cutsceneExecutors[index].movementVector.x, 
                                      cutsceneExecutors[index].movementVector.y, 
                                      cutsceneExecutors[index].movementVector.z, 
                                      cutsceneExecutors[index].movementDistance);
                                
                                goto end;
                                
                            }
                                
                        }
                        
                    } 
                      
                } 
                
                setEntityMovementVector(cutsceneExecutors[index].assetIndex, 0.0f, 0.0f, 0.0f, 0.0f);

            } else {
                goto setMovementVector;

            }
        
        }     
end:
    if (cutsceneExecutors[index].behaviorFlags & 1) {
        setCameraTrackingEntity(cutsceneExecutors[index].assetIndex, TRUE);
    } else {
        setCameraTrackingEntity(cutsceneExecutors[index].assetIndex, FALSE);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", updateCutsceneMapViewPosition);

void updateCutsceneMapViewPosition(u16 index) {
    adjustMapControllerViewPosition(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].frameDelta.x, cutsceneExecutors[index].frameDelta.y, cutsceneExecutors[index].frameDelta.z);
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAnimationDataPtrWithFlag);

void cutsceneHandlerSetAnimationDataPtrWithFlag(u16 index) {
     
    s16 offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    offset = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].animationDataPtr = cutsceneExecutors[index].bytecodePtr + offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].animationFrameCounter = 0;
    cutsceneExecutors[index].flags |= CUTSCENE_SPRITE_ASSET;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAnimationDataPtr);

void cutsceneHandlerSetAnimationDataPtr(u16 index) {

    u32 offset;

    CutsceneSetAnimDataPtrCmd *ptr = (CutsceneSetAnimDataPtrCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ptr->offset;
    
    cutsceneExecutors[index].animationDataPtr = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].animationFrameCounter = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;

}

// alternate
/*
void cutsceneHandlerSetAnimationDataPtr(u16 index) {
    
    u32 offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ((UnknownCutsceneMapUpdate*)cutsceneExecutors[index].bytecodePtr)->unk_0;
    
    cutsceneExecutors[index].animationDataPtr = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].animationFrameCounter = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;
}
*/

// alternate
/*
void cutsceneHandlerSetAnimationDataPtr(u16 index) {
    
    u32 offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].animationDataPtr = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].animationFrameCounter = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetCoordinates);

void cutsceneHandlerSetCoordinates(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.x = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.y = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.z = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
        setMapControllerViewPosition(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }

    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
        setEntityCoordinates(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetFrameDelta);

void cutsceneHandlerSetFrameDelta(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].frameDelta.x = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].frameDelta.y = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].frameDelta.z = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].waitFrames = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetWaitFrames);

// idle
void cutsceneHandlerSetWaitFrames(u16 index) {

    CutsceneWaitFramesCmd* ptr = (CutsceneWaitFramesCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 waitFrames;

    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

    waitFrames = ptr->frames;
    
    cutsceneExecutors[index].waitFrames = waitFrames;
    
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

}

// alternate
/*
void cutsceneHandlerSetWaitFrames(u16 index) {
    
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;
    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDeactivateSelf);

void cutsceneHandlerDeactivateSelf(u16 index) {

    cutsceneExecutors[index].waitFrames = 1;
    deactivateCutsceneExecutor(index);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerExecuteSubroutine);

void cutsceneHandlerExecuteSubroutine(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 2;

    cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerReturnFromSubroutine);

void cutsceneHandlerReturnFromSubroutine(u16 index) {
    cutsceneExecutors[index].bytecodePtr = cutsceneExecutors[index].returnPtr;
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnCurrentButton);

void cutsceneHandlerBranchOnCurrentButton(u16 index) {

    CutsceneBranchOnCurrentButtonCmd* ptr = (CutsceneBranchOnCurrentButtonCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].button & ptr->buttonMask) {

        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 0xA;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnButtonPressed);

void cutsceneHandlerBranchOnButtonPressed(u16 index) {

    CutsceneBranchOnButtonPressedCmd* ptr = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonPressed & ptr->buttonMask) {
        
        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 10;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnButtonRepeat);

void cutsceneHandlerBranchOnButtonRepeat(u16 index) {

    CutsceneBranchOnButtonRepeatCmd* ptr = (CutsceneBranchOnButtonRepeatCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonRepeat & ptr->buttonMask) {

        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 10;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSpawnExecutor);

void cutsceneHandlerSpawnExecutor(u16 index) {

    CutsceneSpawnExecutorCmd* ptr = (CutsceneSpawnExecutorCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 executorIndex;
    s32 offset;
    u32 spawnedPtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ptr->offset;
    spawnedPtr = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spawnedPtr += offset;
    
    spawnCutsceneExecutor(executorIndex, spawnedPtr);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetOtherExecutorBytecodePtr);

void cutsceneHandlerSetOtherExecutorBytecodePtr(u16 index) {

    CutsceneSetOtherExecutorPtrCmd* ptr = (CutsceneSetOtherExecutorPtrCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 executorIndex;
    u32 offset;
    u32 spawnedPtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    executorIndex = ptr->executorIndex;

    cutsceneExecutors[index].bytecodePtr += 2; 

    spawnedPtr = cutsceneExecutors[index].bytecodePtr;

    offset = ptr->offset;

    cutsceneExecutors[index].bytecodePtr += 4;

    spawnedPtr += offset;
    
    if (executorIndex < MAX_BYTECODE_EXECUTORS) {
        
        if (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[executorIndex].waitFrames = 0;
            cutsceneExecutors[executorIndex].bytecodePtr = spawnedPtr;
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDeactivateExecutor);

void cutsceneHandlerDeactivateExecutor(u16 index) {

    CutsceneDeactivateExecutorCmd* ptr = (CutsceneDeactivateExecutorCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 executorIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    executorIndex = ptr->executorIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    deactivateCutsceneExecutor(executorIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDMASprite);

void cutsceneHandlerDMASprite(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneDmaSpriteIndexCmd* ptr;
    CutsceneSpriteDMAManagerCmd* ptr2;

    ptr = (CutsceneDmaSpriteIndexCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    ptr2 = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    assetType = ptr2->assetType;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = ptr2->romTextureStart;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = ptr2->romTextureEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexStart = ptr2->romAssetsIndexStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexEnd = ptr2->romAssetsIndexEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexStart = ptr2->romSpritesheetIndexStart;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexEnd = ptr2->romSpritesheetIndexEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture1Vaddr = ptr2->texture1Vaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture2Vaddr = ptr2->texture2Vaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    paletteVaddr = ptr2->paletteVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    animationVaddr = ptr2->animationVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spriteToPaletteVaddr = ptr2->spriteToPaletteVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;
 
    spritesheetIndexVaddr = ptr2->spritesheetIndexVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    dmaSprite(cutsceneExecutors[index].assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   
    setBilinearFiltering(cutsceneExecutors[index].assetIndex, TRUE);
    
}

// alternate
/*
void cutsceneHandlerDMASprite(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneSpriteDMAManagerCmd* ptr = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    assetType = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexStart = *(u32*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexStart = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture1Vaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture2Vaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    paletteVaddr = *(u16**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    animationVaddr = *(u16**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spriteToPaletteVaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spritesheetIndexVaddr = *(u32**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    dmaSprite(cutsceneExecutors[index].assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    setBilinearFiltering(cutsceneExecutors[index].assetIndex, TRUE);
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityAnimations);

void cutsceneHandlerSetEntityAnimations(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].animationIndex1 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].animationIndex2 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].animationIndex3 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].animationIndex4 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].animationIndex5 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].animationIndex6 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex4);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex1);
    }

    setEntityMapSpaceIndependent(cutsceneExecutors[index].assetIndex, FALSE);

    cutsceneExecutors[index].flags |= CUTSCENE_ENTITY_ASSET;
    
    cutsceneExecutors[index].behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY;
    cutsceneExecutors[index].behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_WANDER | CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDoDMA);

void cutsceneHandlerDoDMA(u16 index) {

    CutsceneDMACmd* ptr = (CutsceneDMACmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32 romAddrStart;
    u32 romAddrEnd; 
    void* vaddr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    romAddrStart = ptr->romAddrStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAddrEnd = ptr->romAddrEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    vaddr = ptr->vaddr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    nuPiReadRom(romAddrStart, vaddr, romAddrEnd - romAddrStart);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetU8Value);

// set u8 value
// FIXME: should be reading u32 value for ptr
void cutsceneHandlerSetU8Value(u16 index) {

    u8 value;

    CutsceneSetU8ValueCmd* ptr2;
    
    CutsceneSetU8ValueCmd* ptr = (CutsceneSetU8ValueCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 2;

    // FIXME: something not right
    ptr2 = *(CutsceneSetU8ValueCmd**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    ptr2->variablePtr = value;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetU16Value);

// set u16 value
// sets D_80189824
void cutsceneHandlerSetU16Value(u16 index) {

    CutsceneSetU16ValueCmd* ptr = (CutsceneSetU16ValueCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u16* unk_4;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    *unk_4 = unk_2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetU32Value);

void cutsceneHandlerSetU32Value(u16 index) {

    CutsceneSetU32ValueCmd* ptr = (CutsceneSetU32ValueCmd*)cutsceneExecutors[index].bytecodePtr;

    u32 cutsceneIndex;
    u32* cutsceneIndexPtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    cutsceneIndex = ptr->cutsceneIndex;

    cutsceneExecutors[index].bytecodePtr += 4;

    cutsceneIndexPtr = ptr->cutsceneIndexPtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *cutsceneIndexPtr = cutsceneIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchU8VarInRange);

// branch if u8 variable outside range
// i.e., gHour
void cutsceneHandlerBranchU8VarInRange(u16 index) {

    u8* variablePtr;
    u16 min;
    u16 max;

    cutsceneExecutors[index].bytecodePtr += 2;

    min = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
 
    max = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    variablePtr = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;    

    if (*variablePtr < min) {
        goto func_end;  
    }
    
    if (max >= *variablePtr) {
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
    }
    
func_end:
    cutsceneExecutors[index].bytecodePtr += 4;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchU16VarInRange);

// Branch if u16 variable outside range
void cutsceneHandlerBranchU16VarInRange(u16 index) {
    
    u16* variablePtr;
    u16 min;
    u16 max;

    CutsceneBranchU16VariableWithinRange* ptr = (CutsceneBranchU16VariableWithinRange*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    min = ptr->min;

    cutsceneExecutors[index].bytecodePtr += 2;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;    

    variablePtr = ptr->variablePtr;

    cutsceneExecutors[index].bytecodePtr += 4;    

    if (*variablePtr < min) {
        goto func_end;  
    } 
    
    if (max >= *variablePtr) {
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
    }
    
func_end:
    cutsceneExecutors[index].bytecodePtr += 4;
        
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchU32VarInRange);

// branch if u32 value outside range
void cutsceneHandlerBranchU32VarInRange(u16 index) {

    CutsceneBranchU32VariableWithinRange* ptr = (CutsceneBranchU32VariableWithinRange*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u32 min;
    u32 max;
    u32* variablePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    min = ptr->min;

    cutsceneExecutors[index].bytecodePtr += 4;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    variablePtr = ptr->variablePtr;

    cutsceneExecutors[index].bytecodePtr += 4;
 
    if (*variablePtr >= min && max >= *variablePtr) {

        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSpecialBit);

// sets gCutsceneCompletionFlags
void cutsceneHandlerSetSpecialBit(u16 index) {

    CutsceneSetSpecialBitCmd* ptr = (CutsceneSetSpecialBitCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 bit;
    s32* bitfield;

    cutsceneExecutors[index].bytecodePtr += 2;

    bit = ptr->bit;

    cutsceneExecutors[index].bytecodePtr += 2;

    bitfield = ptr->bitfield;

    cutsceneExecutors[index].bytecodePtr += 4;

    *bitfield |= 1 << bit;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerClearSpecialBit);

void cutsceneHandlerClearSpecialBit(u16 index) {

    CutsceneClearSpecialBitCmd* ptr = (CutsceneClearSpecialBitCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 bit;
    u32* bitfield;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    bit = ptr->bit;

    cutsceneExecutors[index].bytecodePtr += 2;

    bitfield = ptr->bitfield;

    cutsceneExecutors[index].bytecodePtr += 4;

    *bitfield &= ~(1 << bit);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnSpecialBit);

void cutsceneHandlerBranchOnSpecialBit(u16 index) {

    CutsceneBranchOnSpecialBitCmd* ptr = (CutsceneBranchOnSpecialBitCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 bit;
    u32* bitfield;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    bit = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    bitfield= *(u32**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*bitfield & (1 << bit)) {
        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAssetRotation);

void cutsceneHandlerSetAssetRotation(u16 index) {

    CutsceneSetAssetRotationCmd* ptr = (CutsceneSetAssetRotationCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].entityDirectionOrMapRotation = ptr->direction;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
        setInitialMapRotation(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].entityDirectionOrMapRotation);
    }

    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
        setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].entityDirectionOrMapRotation, gMainMapIndex));
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetupMapAsset);

// set up cutscene object as map asset
void cutsceneHandlerSetupMapAsset(u16 index) {

    CutsceneMapControllerIndexCmd *ptr = (CutsceneMapControllerIndexCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->index;

    cutsceneExecutors[index].bytecodePtr += 2;

    setMapViewPositionAndCurrentTile(cutsceneExecutors[index].assetIndex, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.x, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.y, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.z, 8, 8);
    enableMapController(cutsceneExecutors[index].assetIndex);
    setMainMapIndex(cutsceneExecutors[index].assetIndex);

    cutsceneExecutors[index].flags |= CUTSCENE_MAP_ASSET;
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerEntityMoveAndAnimate);

void cutsceneHandlerEntityMoveAndAnimate(u16 index) {

    Vec3f vec;
    
    cutsceneExecutors[index].bytecodePtr += 2;    

    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].entityDirectionOrMapRotation = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].movementDistance = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex5);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex2);
    }

    setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].entityDirectionOrMapRotation, gMainMapIndex));
    
    vec = getMovementVectorFromDirection(cutsceneExecutors[index].movementDistance, convertWorldToSpriteDirection(entities[cutsceneExecutors[index].assetIndex].direction, gMainMapIndex), 0.0f);

    cutsceneExecutors[index].frameDelta.x = vec.x;
    cutsceneExecutors[index].frameDelta.y = vec.y;
    cutsceneExecutors[index].frameDelta.z = vec.z;
    
    cutsceneExecutors[index].behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMapRotation);

void cutsceneHandlerSetMapRotation(u16 index) {

    CutsceneSetMapRotationCmd* ptr = (CutsceneSetMapRotationCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapIndex;
    u8 arg1, rotation;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mapIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg1 = ptr->arg1;

    cutsceneExecutors[index].bytecodePtr++;

    rotation = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    startMapRotation(mapIndex, arg1, rotation);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetBehaviorFlags);

// set behavior flags
void cutsceneHandlerSetBehaviorFlags(u16 index) {

    CutsceneSetBehaviorFlagsCmd* ptr = (CutsceneSetBehaviorFlagsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u8 behaviorFlags;

    cutsceneExecutors[index].bytecodePtr += 2;

    behaviorFlags = ptr->behaviorFlags;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].behaviorFlags &= ~CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY;
    cutsceneExecutors[index].behaviorFlags |= behaviorFlags;  
    cutsceneExecutors[index].behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMovementInformation);

void cutsceneHandlerSetMovementInformation(u16 index) {

    u16 temp1, temp2;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].movementDistance = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].collisionWidth = *(u16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].collisionHeight = *(u16*)(cutsceneExecutors[index].bytecodePtr);

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].targetPosition = cutsceneExecutors[index].coordinates;
    
    cutsceneExecutors[index].behaviorFlags |= (CUTSCENE_ASSET_BEHAVIOR_WANDER | CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerInitializeMessageBoxType1);

void cutsceneHandlerInitializeMessageBoxType1(u16 index) {
    
    CutsceneInitMessageBoxType1Cmd* ptr = (CutsceneInitMessageBoxType1Cmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    textAddressesIndex = ptr->textAddressesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    textIndex = ptr->textIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    initializeMessageBox(messageBoxIndex, textAddressesIndex, textIndex, 0x8000);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitMessageBoxClosed);

void cutsceneHandlerWaitMessageBoxClosed(u16 index) {

    CutsceneWaitMessageBoxClosedCmd* ptr = (CutsceneWaitMessageBoxClosedCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(messageBoxes[messageBoxIndex].flags & MESSAGE_BOX_TEXT_COMPLETE)) {

        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMessageBoxViewSpacePosition);

void cutsceneHandlerSetMessageBoxViewSpacePosition(u16 index) {

    CutsceneSetMessageBoxPositionCmd* ptr = (CutsceneSetMessageBoxPositionCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 messageBoxIndex;
    f32 x, y, z;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    x = ptr->x;

    cutsceneExecutors[index].bytecodePtr += 2;

    y = ptr->y;

    cutsceneExecutors[index].bytecodePtr += 2;

    z = ptr->z;

    cutsceneExecutors[index].bytecodePtr += 4;

    setMessageBoxViewSpacePosition(messageBoxIndex, x, y, z);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerResetMessageBoxAvatar);

// reset message box avatar
void cutsceneHandlerResetMessageBoxAvatar(u16 index) {

    CutsceneResetMessageBoxAvatarCmd* ptr = (CutsceneResetMessageBoxAvatarCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 messageBoxIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxes[messageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;

    resetMessageBoxAnimation(messageBoxIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerEntityMove);

void cutsceneHandlerEntityMove(u16 index) {
        
    Vec3f vec;
    
    cutsceneExecutors[index].bytecodePtr += 2;    

    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].entityDirectionOrMapRotation = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].movementDistance = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex6);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex3);
    }

    setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].entityDirectionOrMapRotation, gMainMapIndex));
    
    vec = getMovementVectorFromDirection(cutsceneExecutors[index].movementDistance, convertWorldToSpriteDirection(entities[cutsceneExecutors[index].assetIndex].direction, gMainMapIndex), 0.0f);

    cutsceneExecutors[index].frameDelta.x = vec.x;
    cutsceneExecutors[index].frameDelta.y = vec.y;
    cutsceneExecutors[index].frameDelta.z = vec.z;
    
    cutsceneExecutors[index].behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityAnimation);

void cutsceneHandlerSetEntityAnimation(u16 index) {

    CutsceneEntitySetAnimationCmd* ptr = (CutsceneEntitySetAnimationCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 animation;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    animation = ptr->animation;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityAnimation(cutsceneExecutors[index].assetIndex, animation);
    
    cutsceneExecutors[index].behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT | CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityAnimationWithDirectionChange);

void cutsceneHandlerSetEntityAnimationWithDirectionChange(u16 index) {

    CutsceneEntitySetAnimationWithDirectionCmd* ptr = (CutsceneEntitySetAnimationWithDirectionCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 animation;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    animation = ptr->animation;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, animation);
    
    cutsceneExecutors[index].behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT | CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetCallbackBytecodePtr);

void cutsceneHandlerSetCallbackBytecodePtr(u16 index) {
        
    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].entityCollidedWithIndex = *(u16*)cutsceneExecutors[index].bytecodePtr; 
        
    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].targetButtonMask = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].callbackBytecodePtr = cutsceneExecutors[index].bytecodePtr + *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerPauseEntity);

void cutsceneHandlerPauseEntity(u16 index) {

    CutscenePauseEntityCmd* ptr = (CutscenePauseEntityCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    pauseEntity(entityIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerTogglePauseEntity);

void cutsceneHandlerTogglePauseEntity(u16 index) {
    
    CutsceneTogglePauseEntityCmd* ptr = (CutsceneTogglePauseEntityCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    unpauseEntity(entityIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerFlipEntityDirection);

void cutsceneHandlerFlipEntityDirection(u16 index) {

    u16 entityIndex;
    
    CutsceneFlipEntityDirectionCmd* ptr = (CutsceneFlipEntityDirectionCmd*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityDirection(cutsceneExecutors[index].assetIndex, (entities[entityIndex].direction + 4) % 8);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerPauseEntities);

// set flags on all active sprites, rendered and global
void cutsceneHandlerPauseEntities(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;
    
    pauseEntities();
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerTogglePauseEntities);

void cutsceneHandlerTogglePauseEntities(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;

    togglePauseEntities(index);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerFlipEntityAnimation);

void cutsceneHandlerFlipEntityAnimation(u16 index) {

    cutsceneExecutors[index].bytecodePtr = cutsceneExecutors[index].bytecodePtr + 4;

    if (cutsceneExecutors[index].behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex4);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].animationIndex1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityNonCollidable);

void cutsceneHandlerSetEntityNonCollidable(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetupEntity);

void cutsceneHandlerSetupEntity(u16 index) {

    CutsceneSetupEntityCmd* ptr = (CutsceneSetupEntityCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;
    u16 entityAssetIndex;
    u16 flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityAssetIndex = ptr->entityAssetIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    flag = ptr->flag;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    loadEntity(entityIndex, entityAssetIndex, flag);
    setEntityTrackingTarget(entityIndex, 0xFFFF, 0xFF);
    setEntityAttachmentOffset(entityIndex, 0, 0, 0);
    setEntityCollidable(entityIndex, TRUE);
    setEntityYMovement(entityIndex, TRUE);
    setEntityTracksCollisions(entityIndex, TRUE);
    enableEntityMovement(entityIndex, TRUE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityMapSpaceIndependentFlag);

// unused handler
void cutsceneHandlerSetEntityMapSpaceIndependentFlag(u16 index) {

    CutsceneSetEntityMapSpaceIndependentFlagCmd* ptr = (CutsceneSetEntityMapSpaceIndependentFlagCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 flag;

    cutsceneExecutors[index].bytecodePtr +=  2;

    flag = ptr->flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    setEntityMapSpaceIndependent(cutsceneExecutors[index].assetIndex, flag);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerLoadMap);

void cutsceneHandlerLoadMap(u16 index) {

    CutsceneLoadMapCmd* ptr = (CutsceneLoadMapCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapDataIndex;
    u16 mapIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapDataIndex = ptr->mapDataIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mainMapIndex;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    loadMap(mapDataIndex, mapIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityMapSpaceIndependent);

void cutsceneHandlerSetEntityMapSpaceIndependent(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;

    setEntityMapSpaceIndependent(cutsceneExecutors[index].assetIndex, TRUE);
    
    cutsceneExecutors[index].flags &= ~CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetRGBA);

void cutsceneHandlerSetRGBA(u16 index) {

    u8 r, g, b, a;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = currentMapLightingRGBA.r;
        g = currentMapLightingRGBA.g;
        b = currentMapLightingRGBA.b;
        a = currentMapLightingRGBA.a;

    }

    if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteColor(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
        setEntityColor(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
        setMapControllerRGBA(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateRGBA);

void cutsceneHandlerUpdateRGBA(u16 index) {
    
    u8 r, g, b, a;
    s16 rate;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    rate = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = targetMapLightingRGBA.r;
        g = targetMapLightingRGBA.g;
        b = targetMapLightingRGBA.b;
        a = targetMapLightingRGBA.a;

        rate = D_8017045A;
        
    }

    if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
        updateSpriteRGBA(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
        updateEntityRGBA(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
        setMapControllerRGBAWithTransition(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateU8Value);

void cutsceneHandlerUpdateU8Value(u16 index) {

    CutsceneU8UpdateCmd* ptr = (CutsceneU8UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u8* valuePtr;
    s8 value;
    u8 max;
    u8 initial;

    cutsceneExecutors[index].bytecodePtr += 2;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 2;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateU16Value);

void cutsceneHandlerUpdateU16Value(u16 index) {

    CutsceneU16UpdateCmd* ptr = (CutsceneU16UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16* valuePtr;
    s16 value;
    u16 max;

    cutsceneExecutors[index].bytecodePtr += 2;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 2;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateU32Value);

void cutsceneHandlerUpdateU32Value(u16 index) {

    CutsceneU32UpdateCmd* ptr = (CutsceneU32UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32* valuePtr;
    s32 value;
    s32 max;

    cutsceneExecutors[index].bytecodePtr += 4;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 4;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDeactivateMapObjects);

// deactivate map auxillary objects (map objects and weather sprites)
void cutsceneHandlerDeactivateMapObjects(u16 index) {

    CutsceneDeactivateMapObjectsCmd* ptr = (CutsceneDeactivateMapObjectsCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 mapIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mapIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    unloadMapAssets(mapIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateGlobalRGBA);

void cutsceneHandlerUpdateGlobalRGBA(u16 index) {

    u8 r, g, b, a;
    s16 rate;

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    rate = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = targetMapLightingRGBA.r;
        g = targetMapLightingRGBA.g;
        b = targetMapLightingRGBA.b;
        a = targetMapLightingRGBA.a;

        rate = D_8017045A;
        
    }

   for (i = 0; i < MAX_ENTITIES; i++) {
       updateEntityRGBA(i, r, g, b, a, rate);
   }

    for (i = 0; i < MAX_MAPS; i++) {
        setMapControllerRGBAWithTransition(i, r, g, b, a, rate);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDeactivateSprites);

void cutsceneHandlerDeactivateSprites(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    deactivateSprites();

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerDeactivateMapControllers);

void cutsceneHandlerDeactivateMapControllers(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    // deactivate all map controllers
    deactivateAllMapControllers();

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitRgbaFinished);

void cutsceneHandlerWaitRgbaFinished(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {

        if (checkSpriteRGBAUpdateFinished(cutsceneExecutors[index].assetIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
        
    }

    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {
        if (checkSpriteRGBAUpdateFinished(entities[cutsceneExecutors[index].assetIndex].globalSpriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }

    if (cutsceneExecutors[index].flags & CUTSCENE_MAP_ASSET) {
        if (checkMapRGBADone(mapControllers[cutsceneExecutors[index].assetIndex].mainMapIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerCheckEntityCollision);

void cutsceneHandlerCheckEntityCollision(u16 index) {

    u16 entityIndex;
    u16 collisionWidth;
    u16 collisionHeight;

    CutsceneCheckEntityCollisionCmd *ptr = (CutsceneCheckEntityCollisionCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;   

    entityIndex = ptr->entityIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    collisionWidth = ptr->collisionWidth;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    collisionHeight = ptr->collisionHeight;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (detectEntityOverlap(&entities[cutsceneExecutors[index].assetIndex], entityIndex, 0.0f, 0.0f, collisionWidth, collisionHeight)) {
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerInitializeDialogueSession);

void cutsceneHandlerInitializeDialogueSession(u16 index) {

    CutsceneInitDialogueSessionCmd* ptr = (CutsceneInitDialogueSessionCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 dialoguesIndex;
    u16 dialogueBytecodeAddressIndex;
    u16 dialogueIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBytecodeAddressIndex = ptr->dialogueBytecodeAddressIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueIndex = ptr->dialogueIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    initializeDialogueSession(dialoguesIndex, dialogueBytecodeAddressIndex, dialogueIndex, 0);
        
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitForDialogueInput);

void cutsceneHandlerWaitForDialogueInput(u16 index) {

    CutsceneWaitForDialogueInputCmd* ptr = (CutsceneWaitForDialogueInputCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(dialogues[dialoguesIndex].sessionManager.flags & DIALOGUE_COMPLETED)) {
        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnDialogue);

// branch on dialogue (unk_17 value)
void cutsceneHandlerBranchOnDialogue(u16 index) {

    CutsceneBranchOnDialogueCmd* ptr = (CutsceneBranchOnDialogueCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialoguesIndex;
    u16 unk;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    unk = ptr->unk;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (dialogues[dialoguesIndex].sessionManager.selectedMenuRow == unk) {

        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitEntityAnimation);

void cutsceneHandlerWaitEntityAnimation(u16 index) {

    u16* base = cutsceneExecutors[index].bytecodePtr; 
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & CUTSCENE_ENTITY_ASSET) {

        if (checkSpriteAnimationStateChanged(entities[cutsceneExecutors[index].assetIndex].globalSpriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2;
        }
                
    } else if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
    
        cutsceneExecutors[index].bytecodePtr = base;
        cutsceneExecutors[index].waitFrames = 1;

    } else {
    
        cutsceneExecutors[index].bytecodePtr += 2;
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMessageBoxAssetIndices);

void cutsceneHandlerSetMessageBoxAssetIndices(u16 index) {

    CutsceneSetMessageBoxAssetsCmd* ptr = (CutsceneSetMessageBoxAssetsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 spriteIndex;
    u8 dialogueWindowIndex;
    u8 overlayIconIndex;
    u8 characterAvatarIndex;

    cutsceneExecutors[index].bytecodePtr += 2;    

    spriteIndex = ptr->spriteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    dialogueWindowIndex = ptr->dialogueWindowIndex;

    cutsceneExecutors[index].bytecodePtr++;

    overlayIconIndex = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    characterAvatarIndex = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;
    
    setMessageBoxSpriteIndices(spriteIndex, dialogueWindowIndex, overlayIconIndex, characterAvatarIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityTrackingTarget);

void cutsceneHandlerSetEntityTrackingTarget(u16 index) {

    CutsceneSetEntityTrackingTargetCmd* ptr = (CutsceneSetEntityTrackingTargetCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 targetEntityIndex;
    s16 x, y, z;
    u16 trackingMode;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    targetEntityIndex = ptr->targetEntityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    x = ptr->arg1;

    cutsceneExecutors[index].bytecodePtr += 2;

    y = ptr->arg2;

    cutsceneExecutors[index].bytecodePtr += 2;

    z = ptr->arg3;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    trackingMode = ptr->trackingMode;
    
    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr++;

    setEntityTrackingTarget(cutsceneExecutors[index].assetIndex, targetEntityIndex, trackingMode);
    setEntityAttachmentOffset(cutsceneExecutors[index].assetIndex, x, y, z);
    setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
    setEntityYMovement(cutsceneExecutors[index].assetIndex, FALSE);
    setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, FALSE);
    enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAltAnimationFlag);

// seems unused
void cutsceneHandlerSetAltAnimationFlag(u16 index) {

    CutsceneSetAltAnimationFlagCmd* ptr = (CutsceneSetAltAnimationFlagCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 useAltAnimations;

    cutsceneExecutors[index].bytecodePtr += 2;

    useAltAnimations = ptr->useAltAnimations;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (useAltAnimations) {
        cutsceneExecutors[index].behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS;
    } else {
        cutsceneExecutors[index].behaviorFlags &= ~CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitMapLoad);

void cutsceneHandlerWaitMapLoad(u16 index) {

    CutsceneMapControllerIndexCmd* ptr = (CutsceneMapControllerIndexCmd*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(mapControllers[ptr->index].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {
        cutsceneExecutors[index].bytecodePtr += 2;
    } else {
        cutsceneExecutors[index].bytecodePtr = ptr;
        cutsceneExecutors[index].waitFrames = 1;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnEntityDirection);

void cutsceneHandlerBranchOnEntityDirection(u16 index) {

    CutsceneBranchOnEntityDirectionCmd* ptr = (CutsceneBranchOnEntityDirectionCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 entityIndex;
    u8 targetDirecton;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    targetDirecton = ptr->targetDirecton;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;
    
    if (convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex) == targetDirecton) {

        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 2;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
        
    }

    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityPhysicsFlags);

void cutsceneHandlerSetEntityPhysicsFlags(u16 index) {

    CutsceneSetEntityPhysicsFlagsCmd* ptr = (CutsceneSetEntityPhysicsFlagsCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 temp;

    cutsceneExecutors[index].bytecodePtr += 2;

    temp = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    switch (temp) {

        case 0:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, FALSE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, FALSE);
            break;
        case 1:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, TRUE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, TRUE);
            break;
        case 2:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, TRUE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, TRUE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, TRUE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, TRUE);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntityPalette);

void cutsceneHandlerSetEntityPalette(u16 index) {

    CutsceneSetEntityPaletteCmd* ptr = (CutsceneSetEntityPaletteCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 paletteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    paletteIndex = ptr->paletteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityPaletteIndex(cutsceneExecutors[index].assetIndex, paletteIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetEntitySpriteDimensions);

void cutsceneHandlerSetEntitySpriteDimensions(u16 index) {

    u8 spriteWidth, spriteHeight;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    spriteWidth = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    spriteHeight = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    setEntitySpriteDimensions(entities[cutsceneExecutors[index].assetIndex].entityAssetIndex, spriteWidth, spriteHeight);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetShadowFlags);

void cutsceneHandlerSetShadowFlags(u16 index) {

    u16 entityAssetIndex;
    u16 flags;
    
    CutsceneSetShadowFlagsCmd* ptr = (CutsceneSetShadowFlagsCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    entityAssetIndex = ptr->entityAssetIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    flags = ptr->flags;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    entityAssetDescriptors[entityAssetIndex].shadowSpriteIndex = flags;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSpriteScale);

void cutsceneHandlerSetSpriteScale(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].scale.x = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].scale.y = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
        
    cutsceneExecutors[index].scale.z = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteScale(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].scale.x, cutsceneExecutors[index].scale.y, cutsceneExecutors[index].scale.z);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSpriteRenderngLayer);

void cutsceneHandlerSetSpriteRenderngLayer(u16 index) {

    CutsceneSetSpriteRenderingLayerCmd* ptr = (CutsceneSetSpriteRenderingLayerCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 renderingLayerFlags;

    cutsceneExecutors[index].bytecodePtr += 2;

    renderingLayerFlags = ptr->renderingLayer;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteBlendMode(cutsceneExecutors[index].assetIndex, renderingLayerFlags);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerInitializeMessageBoxType2);

void cutsceneHandlerInitializeMessageBoxType2(u16 index) {

    CutsceneInitMessageBoxType2Cmd* ptr = (CutsceneInitMessageBoxType2Cmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    textAddressesIndex = ptr->textAddressesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    textIndex = ptr->textIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    initializeMessageBox(messageBoxIndex, textAddressesIndex, textIndex, 0x100000);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerInitMapAddition);

void cutsceneHandlerInitMapAddition(u16 index) {

    u16 mapAdditionIndex;
    u16 flag;
    
    CutsceneInitMapAdditionCmd* ptr = (CutsceneInitMapAdditionCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    mapAdditionIndex = ptr->mapAdditionIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    flag = ptr->flag;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    // initialize mapAdditions struct
    activateMapAddition(MAIN_MAP_INDEX, mapAdditionIndex, flag);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchOnRandom);

// branch on random value
void cutsceneHandlerBranchOnRandom(u16 index) {

    CutsceneBranchOnRandomCmd* ptr= (CutsceneBranchOnRandomCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u16 unk_4;
    u16 temp;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = ptr->unk_4;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    temp = getRandomNumberInRange(0, 100);

    if ((temp < unk_2) ^ 1 && (unk_4 < temp) ^ 1) {

        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 2;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchIfU16PtrInRange);

void cutsceneHandlerBranchIfU16PtrInRange(u16 index) {

    CutsceneBranchIfU16PtrInRangeCmd* ptr = (CutsceneBranchIfU16PtrInRangeCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_8 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_C = ptr->unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*unk_4 >= *unk_8 && unk_C >= *unk_4) {

        cutsceneExecutors[index].returnPtr = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerPauseExecutor);

void cutsceneHandlerPauseExecutor(u16 index) {

    CutscenePauseExecutorCmd* ptr = (CutscenePauseExecutorCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 executorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (executorIndex < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[executorIndex].flags |= CUTSCENE_PAUSE_EXECUTION;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerTogglePauseExecutor);

void cutsceneHandlerTogglePauseExecutor(u16 index) {

    CutsceneTogglePauseExecutorCmd* ptr = (CutsceneTogglePauseExecutorCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 executorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (executorIndex < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[executorIndex].flags &= ~CUTSCENE_PAUSE_EXECUTION;
    }

}
    
//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerPauseAllChildExecutors);

void cutsceneHandlerPauseAllChildExecutors(u16 index) {

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[i].flags |= CUTSCENE_PAUSE_EXECUTION;
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerTogglePauseAllChildExecutors);

void cutsceneHandlerTogglePauseAllChildExecutors(u16 index) {

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[i].flags &= ~CUTSCENE_PAUSE_EXECUTION;
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSpritePalette);

void cutsceneHandlerSetSpritePalette(u16 index) {

    u16 paletteIndex;
    
    CutsceneSetSpritePaletteCmd* ptr = (CutsceneSetSpritePaletteCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    paletteIndex = ptr->paletteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setSpritePaletteIndex(cutsceneExecutors[index].assetIndex, paletteIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerBranchIfU8PtrInRange);

void cutsceneHandlerBranchIfU8PtrInRange(u16 index) {

    CutsceneBranchIfU8PtrInRangeCmd* ptr = (CutsceneBranchIfU8PtrInRangeCmd*)cutsceneExecutors[index].bytecodePtr;

    u8* temp1;
    u8* temp2;
    u32 temp3;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    temp1 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    temp2 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    temp3 = ptr->unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*temp1 >= *temp2) {

        if (temp3 >= *temp1) {
            cutsceneExecutors[index].returnPtr = (u16*)ptr + 10;
            cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
            return;
        }
        
    }

    cutsceneExecutors[index].bytecodePtr += 4;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAudioSequence);

void cutsceneHandlerSetAudioSequence(u16 index) {

    u16 sequenceIndex;
    u8 *sequenceStart;
    u8 *sequenceEnd;

    CutsceneSetAudioSequenceCmd* ptr = (CutsceneSetAudioSequenceCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    sequenceStart = ptr->sequenceStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    sequenceEnd = ptr->sequenceEnd;
    
    cutsceneExecutors[index].bytecodePtr += 4;    

    setAudioSequence(sequenceIndex, sequenceStart, sequenceEnd);
    setAudioSequenceVolumes(sequenceIndex, 0, 0);
    
}

/*
void cutsceneHandlerSetAudioSequence(u16 index) {
 
    u16 index;
    u8 *sequenceStart;
    u8 *sequenceEnd;

    cutsceneExecutors[index].bytecodePtr += 2;

    index = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    sequenceStart = *(u8**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    sequenceEnd = *(u8**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;    

    setAudioSequence(index, sequenceStart, sequenceEnd);
    setAudioSequenceVolumes(index, 0, 0);
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerStopAudioSequenceWithFadeOut);

void cutsceneHandlerStopAudioSequenceWithFadeOut(u16 index) {

    CutsceneStopSequenceWithFadeCmd* ptr = (CutsceneStopSequenceWithFadeCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 sequenceIndex;
    u32 speed;

    cutsceneExecutors[index].bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    speed = ptr->speed;

    cutsceneExecutors[index].bytecodePtr += 4;

    stopAudioSequenceWithFadeOut(sequenceIndex, speed);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetAudioSequenceVolume);

void cutsceneHandlerSetAudioSequenceVolume(u16 index) {

    CutsceneSetAudioSequenceVolumeCmd* ptr = (CutsceneSetAudioSequenceVolumeCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 sequenceIndex;
    s32 targetVolume;
    s16 volume;

    cutsceneExecutors[index].bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    targetVolume = ptr->targetVolume;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    volume = ptr->volume;
    
    cutsceneExecutors[index].bytecodePtr += 2;    

    setAudioSequenceVolumes(sequenceIndex, targetVolume, volume);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSfx);

void cutsceneHandlerSetSfx(u16 index) {

    CutsceneSetSfxCmd* ptr = (CutsceneSetSfxCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32 sfxIndex;
    u16 volume;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    sfxIndex = ptr->sfxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    volume = ptr->volume;

    cutsceneExecutors[index].bytecodePtr += 4;

    sfxIndex++;
    
    setSfx(sfxIndex);
    setSfxVolume(sfxIndex, volume);
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerIdleWhileAudioSequencePlaying);

void cutsceneHandlerIdleWhileAudioSequencePlaying(u16 index) {

    CutsceneIdleWhileSequencePlayingCmd* ptr = (CutsceneIdleWhileSequencePlayingCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 sequenceIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    sequenceIndex = ptr->sequenceIndex;

    if (!gAudioSequences[sequenceIndex].flags) {
        cutsceneExecutors[index].bytecodePtr += 2;
    } else {
        cutsceneExecutors[index].bytecodePtr = ptr;
        cutsceneExecutors[index].waitFrames = 1;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerUpdateMessageBoxRGBA);

void cutsceneHandlerUpdateMessageBoxRGBA(u16 index) {

    u16 messageBoxIndex;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 flags;

    CutsceneUpdateMessageBoxRgbaCmd* ptr = (CutsceneUpdateMessageBoxRgbaCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    flags = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    setMessageBoxRGBAWithTransition(messageBoxIndex, r, g, b, a, flags);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerWaitMessageBoxReady);

void cutsceneHandlerWaitMessageBoxReady(u16 index) {

    u16 messageBoxIndex;
    
    CutsceneWaitMessageBoxReadyCmd* ptr = (CutsceneWaitMessageBoxReadyCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    messageBoxIndex = ptr->messageBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!checkMessageBoxRGBAComplete(messageBoxIndex)) {
        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetSpriteBilinearFiltering);

void cutsceneHandlerSetSpriteBilinearFiltering(u16 index) {

    CutsceneSetSpriteBilinearCmd* ptr = (CutsceneSetSpriteBilinearCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 useBilinearFiltering;

    cutsceneExecutors[index].bytecodePtr += 2;

    useBilinearFiltering = ptr->useBilinearFilterng;

    cutsceneExecutors[index].bytecodePtr += 2;

    setBilinearFiltering(cutsceneExecutors[index].assetIndex, useBilinearFiltering);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMapAddition);

void cutsceneHandlerSetMapAddition(u16 index) {

    CutsceneSetMapAdditionCmd* ptr = (CutsceneSetMapAdditionCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapAdditionIndex;
    u8 x;
    u8 z;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapAdditionIndex = ptr->mapAdditionIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    x = ptr->x;

    cutsceneExecutors[index].bytecodePtr++;

    z = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setMapAdditionIndexFromCoordinates(MAIN_MAP_INDEX, mapAdditionIndex, x, z);
    setGridToTileTextureMappings(MAIN_MAP_INDEX);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMapGroundObject);

void cutsceneHandlerSetMapGroundObject(u16 index) {

    u16 spriteIndex;
    u8 x;
    u8 z;

    CutsceneSetMapGroundObjectCmd* ptr = (CutsceneSetMapGroundObjectCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    spriteIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    x = ptr->x;

    cutsceneExecutors[index].bytecodePtr++;
    
    z = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, spriteIndex, x, z);
    setGroundObjects(MAIN_MAP_INDEX);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", cutsceneHandlerSetMessageInterpolation);

void cutsceneHandlerSetMessageInterpolation(u16 index) {
    
    s16 messageBoxIndex;
    s16 rate;
    u8 flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    rate = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageBoxIndex = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    flag = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setMessageBoxInterpolationWithFlags(messageBoxIndex, rate, flag);
    
}