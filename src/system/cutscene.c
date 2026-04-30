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
void cutsceneHandlerEntityWalk(u16);
void cutsceneHandlerSetMapRotation(u16);
void cutsceneHandlerSetBehaviorFlags(u16);
void cutsceneHandlerSetEntityWander(u16);
void cutsceneHandlerInitializeMessageBoxType1(u16);
void cutsceneHandlerWaitMessageBoxClosed(u16);
void cutsceneHandlerSetMessageBoxViewSpacePosition(u16);
void cutsceneHandlerResetMessageBoxAvatar(u16);
void cutsceneHandlerEntityRun(u16);
void cutsceneHandlerSetEntityAnimation(u16);
void cutsceneHandlerSetEntityDirectionalAnimation(u16);
void cutsceneHandlerSetCallbackBytecodePtr(u16);
void cutsceneHandlerPauseEntity(u16);
void cutsceneHandlerUnpauseEntity(u16);
void cutsceneHandlerFlipEntityDirection(u16);
void cutsceneHandlerPauseEntities(u16);
void cutsceneHandlerUnpauseEntities(u16);
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
void cutsceneHandlerSetHoldingAnimationFlag(u16);
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
void cutsceneHandlerUnpauseExecutor(u16);
void cutsceneHandlerPauseAllChildExecutors(u16);
void cutsceneHandlerUnpauseAllChildExecutors(u16);
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
    cutsceneHandlerEntityWalk,
    cutsceneHandlerSetMapRotation,
    cutsceneHandlerSetBehaviorFlags,
    cutsceneHandlerSetEntityWander,
    cutsceneHandlerInitializeMessageBoxType1,
    cutsceneHandlerWaitMessageBoxClosed,
    cutsceneHandlerSetMessageBoxViewSpacePosition,
    cutsceneHandlerResetMessageBoxAvatar,
    cutsceneHandlerEntityRun,
    cutsceneHandlerSetEntityAnimation,
    cutsceneHandlerSetEntityDirectionalAnimation,
    cutsceneHandlerSetCallbackBytecodePtr,
    cutsceneHandlerPauseEntity,
    cutsceneHandlerUnpauseEntity,
    cutsceneHandlerFlipEntityDirection,
    cutsceneHandlerPauseEntities,
    cutsceneHandlerUnpauseEntities,
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
    cutsceneHandlerSetHoldingAnimationFlag,
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
    cutsceneHandlerUnpauseExecutor,
    cutsceneHandlerPauseAllChildExecutors,
    cutsceneHandlerUnpauseAllChildExecutors,
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

        cutsceneExecutors[i].idleAnimation = 0;
        cutsceneExecutors[i].walkingAnimation = 0;
        cutsceneExecutors[i].runningAnimation = 0;
        cutsceneExecutors[i].idleHoldingAnimation = 0;
        cutsceneExecutors[i].walkingHoldingAnimation = 0;
        cutsceneExecutors[i].runningHoldingAnimation = 0;

        cutsceneExecutors[i].entityCollidedWithIndex = 0xFFFF;

        cutsceneExecutors[i].targetPosition.x = 0;
        cutsceneExecutors[i].targetPosition.y = 0;
        cutsceneExecutors[i].targetPosition.z = 0;

        cutsceneExecutors[i].behaviorFlags = 0;
    }

}

bool spawnCutsceneExecutor(u16 index, void *bytecodePtr) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS) {

        if (!(exec->flags & CUTSCENE_ASSET_ACTIVE)) {
            
            exec->bytecodePtr = bytecodePtr;
            
            exec->waitFrames = 0;
            
            exec->frameDelta.x = 0;
            exec->frameDelta.y = 0;
            exec->frameDelta.z = 0;

            exec->movementVector.x = 0;
            exec->movementVector.y = 0;
            exec->movementVector.z = 0;

            exec->flags = CUTSCENE_ASSET_ACTIVE;
            
            result = TRUE;
            
        }
    }

    return result;
    
}

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

bool deactivateCutsceneExecutor(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    bool result = FALSE;

    if (index < MAX_BYTECODE_EXECUTORS && (exec->flags & CUTSCENE_ASSET_ACTIVE)) {

        if (exec->flags & CUTSCENE_SPRITE_ASSET) {
            deactivateSprite(exec->assetIndex);
        }

        if (exec->flags & CUTSCENE_ENTITY_ASSET) {
            deactivateEntity(exec->assetIndex);
        }

        if (exec->flags & CUTSCENE_MAP_ASSET) {
            unloadMapAssets(exec->assetIndex);
        }
        
        result = TRUE;

    }

    exec->entityCollidedWithIndex = 0xFFFF;
    exec->flags = 0;

    return result;

}

void deactivateCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        deactivateCutsceneExecutor(i);
    }

}

bool func_80046BF8(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (exec->flags & CUTSCENE_ASSET_ACTIVE)) {
        exec->flags |= CUTSCENE_PAUSE_EXECUTION;
        result = TRUE;
    }

    return result;
    
}

bool func_80046C48(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (exec->flags & CUTSCENE_ASSET_ACTIVE)) {
        exec->flags &= ~CUTSCENE_PAUSE_EXECUTION;
        result = TRUE;
    }

    return result;
    
}

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

void updateCutsceneExecutors(void) {

    u16 i;
    CutsceneExecutor *exec;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        exec = &cutsceneExecutors[i];

        if (exec->flags & CUTSCENE_ASSET_ACTIVE && !(exec->flags & CUTSCENE_PAUSE_EXECUTION)) {

            if (exec->flags & CUTSCENE_ENTITY_ASSET) {

                Entity *ent = &entities[exec->assetIndex];

                exec->coordinates.x = ent->coordinates.x;
                exec->coordinates.y = ent->coordinates.y;
                exec->coordinates.z = ent->coordinates.z;

                if (exec->entityCollidedWithIndex != 0xFFFF && ent->entityCollidedWithIndex == exec->entityCollidedWithIndex && exec->targetButtonMask == ent->buttonPressed) {
                    exec->entityCollidedWithIndex = 0xFFFF;
                    exec->waitFrames = 0;
                    exec->bytecodePtr = exec->callbackBytecodePtr;
                }

            }

            if (exec->flags & CUTSCENE_MAP_ASSET) {
                MapController *mc = &mapControllers[exec->assetIndex];
                exec->coordinates.x = mc->viewPosition.x;
                exec->coordinates.y = mc->viewPosition.y;
                exec->coordinates.z = mc->viewPosition.z;
            }

            if (exec->waitFrames != 0) {
                 exec->waitFrames--;
                if (exec->waitFrames != 0) {
                    goto skip_callback;
                }
            }

 inner_loop:
            while (TRUE) {

                exec->frameDelta.x = 0;
                exec->frameDelta.y = 0;
                exec->frameDelta.z = 0;

                cutsceneCommandHandlers[*(u16*)(exec->bytecodePtr)](i);

skip_callback:
                exec->coordinates.x += exec->frameDelta.x;
                exec->coordinates.y += exec->frameDelta.y;
                exec->coordinates.z += exec->frameDelta.z;

                if (exec->waitFrames == 0)  {
                    goto inner_loop;
                } else {
                    break;
                }

            }

            if (exec->flags & CUTSCENE_MAP_ASSET) {
                updateCutsceneMapViewPosition(i);
            }

            if (exec->flags & CUTSCENE_SPRITE_ASSET) {
                updateCutsceneSpriteAnimation(i);
            }

            if (exec->flags & CUTSCENE_ENTITY_ASSET) {
                updateCutsceneEntityMovement(i);
            }

        }

    }

}

void updateCutsceneSpriteAnimation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 animation;
    u8 animationModeOrFrameIndex;
    u8 flipFlags;

    if (exec->animationFrameCounter != 0) {

        setSpriteViewSpacePosition(exec->assetIndex,
            exec->coordinates.x + exec->movementVector.x,
            exec->coordinates.y + exec->movementVector.y,
            exec->coordinates.z + exec->movementVector.z);

        if (exec->animationFrameCounter == 0xFF) {
            if (getSpriteAnimationStateChangedFlag(exec->assetIndex) && !(exec->flags & CUTSCENE_ANIMATION_LOOP)) {
                exec->animationFrameCounter = 0;
            }
        } else {
            exec->animationFrameCounter--;
        }

    }

    while (exec->animationFrameCounter == 0) {

        resetAnimationState(exec->assetIndex);

        animation = *(u16*)exec->animationDataPtr;

        if (animation == 0xFFFE) {
            cutsceneHandlerAdjustAnimationDataPtr(index);
            continue;
        }

        if (animation == 0xFFFF) {
            endCutsceneSpriteAnimationSequence(index);
            continue;
        }

        exec->animationDataPtr += 2;

        animationModeOrFrameIndex = *(u8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        exec->movementVector.x = *(s8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        exec->movementVector.y = *(s8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        exec->movementVector.z = *(s8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        startSpriteAnimation(exec->assetIndex, animation, animationModeOrFrameIndex);

        exec->animationFrameCounter = *(u8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        flipFlags = *(u8*)exec->animationDataPtr;
        exec->animationDataPtr++;

        setSpriteFlip(exec->assetIndex, flipFlags & 1, flipFlags & 2);
 
        setSpriteViewSpacePosition(exec->assetIndex,
            exec->coordinates.x + exec->movementVector.x,
            exec->coordinates.y + exec->movementVector.y,
            exec->coordinates.z + exec->movementVector.z);

        if (animationModeOrFrameIndex == 0xFE) {
            exec->flags |= CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT;
        } else {
            exec->flags &= ~CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT;
        }

    }

}

void endCutsceneSpriteAnimationSequence(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    exec->animationFrameCounter = 1;
    exec->flags &= ~CUTSCENE_ASSET_BEHAVIOR_WANDER;
}

void cutsceneHandlerAdjustAnimationDataPtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->animationDataPtr += 2;
    exec->animationDataPtr += *(s16*)(exec->animationDataPtr);

}

void updateCutsceneEntityMovement(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


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
    
    if ((exec->behaviorFlags & (CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT | CUTSCENE_ASSET_BEHAVIOR_RUNNING))) {

        if (!(entities[exec->assetIndex].flags & ENTITY_PAUSED)) {

            if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_WANDER) {
                
                if (!getRandomNumberInRange(0, 16)) {
    
                    temp = getRandomNumberInRange(0, 16);
    
                    if (temp < 8) {

                        exec->entityDirectionOrMapRotation = ((s32)temp & 0xFFFE);

                        setEntityDirection(exec->assetIndex, convertWorldDirectionToScreenDirection(exec->entityDirectionOrMapRotation, gMainMapIndex));

                        if (exec->entityDirectionOrMapRotation) {
                            vec = getMovementVectorFromDirection(exec->movementDistance, convertScreenDirectionToWorldDirection(entities[exec->assetIndex].direction, gMainMapIndex), 0.0f);    
                        } else {
                            vec = getMovementVectorFromDirection(exec->movementDistance, convertScreenDirectionToWorldDirection(entities[exec->assetIndex].direction, gMainMapIndex), 0.0f);
                        }
                        
                        exec->movementVector.x = vec.x;
                        exec->movementVector.y = vec.y;
                        exec->movementVector.z = vec.z;

                        if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT) {

                            if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
                                setEntityDirectionalAnimation(exec->assetIndex, exec->walkingHoldingAnimation);
                            } else {
                                setEntityDirectionalAnimation(exec->assetIndex, exec->walkingAnimation);
                            }
                            
                        } else if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
                            setEntityDirectionalAnimation(exec->assetIndex, exec->runningHoldingAnimation);
                        } else {
                            setEntityDirectionalAnimation(exec->assetIndex, exec->runningAnimation);
                        }
                        
                    } else {
    
                        exec->movementVector.x = 0.0f;
                        exec->movementVector.y = 0.0f;
                        exec->movementVector.z = 0.0f;
    
                        if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
                            setEntityDirectionalAnimation(exec->assetIndex, exec->idleHoldingAnimation);
                        } else {
                            setEntityDirectionalAnimation(exec->assetIndex, exec->idleAnimation);
                        }
                        
                    }
                    
                }
                
            } else {
                
                if (exec->frameDelta.x == 0.0f && exec->frameDelta.y == 0.0f && exec->frameDelta.z == 0.0f) {
    
                    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {    
                        setEntityDirectionalAnimation(exec->assetIndex, exec->idleHoldingAnimation);
                    } else {
                        setEntityDirectionalAnimation(exec->assetIndex, exec->idleAnimation);
                    }

                    exec->movementVector.x = 0.0f;
                    exec->movementVector.y = 0.0f;
                    exec->movementVector.z = 0.0f;
                        
                } else {

                    exec->movementVector.x = exec->frameDelta.x;
                    exec->movementVector.y = exec->frameDelta.y;
                    exec->movementVector.z = exec->frameDelta.z;
                    
                }
            
            }
            
        } else {
            
            if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {    
                setEntityDirectionalAnimation(exec->assetIndex, exec->idleHoldingAnimation);
            } else {
                setEntityDirectionalAnimation(exec->assetIndex, exec->idleAnimation);
            }
    
            exec->movementVector.x = 0.0f;
            exec->movementVector.y = 0.0f;
            exec->movementVector.z = 0.0f;
                
        }
        
    } else {
        exec->movementVector.x = exec->frameDelta.x;
        exec->movementVector.y = exec->frameDelta.y;
        exec->movementVector.z = exec->frameDelta.z;
    }

    if (!(detectEntityOverlap(&entities[exec->assetIndex], 
            exec->entityCollidedWithIndex, 
            exec->movementVector.x, 
            exec->movementVector.z,             
            entityAssetDescriptors[entities[exec->assetIndex].entityAssetIndex].spriteWidth,
            entityAssetDescriptors[entities[exec->assetIndex].entityAssetIndex].spriteHeight)) 
        
        || entities[exec->assetIndex].flags & ENTITY_Y_MOVEMENT) {

            if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_WANDER) {
    
                f1 = exec->targetPosition.x + exec->collisionWidth;
                f2 = exec->coordinates.x + exec->movementVector.x;
    
                if (f2 < f1) {
    
                    f3 = exec->targetPosition.x - exec->collisionWidth;
    
                    if (f3 < f2) {
                        
                        f5 = exec->targetPosition.z + exec->collisionHeight;
                        f6 = exec->coordinates.z + exec->movementVector.z;
    
                        if (f6 < f5) {
    
                            f7 = exec->targetPosition.z - exec->collisionHeight;
                            f8 = exec->coordinates.z + exec->movementVector.z;
                            
                            if (f7 < f8) {
setMovementVector:
                                setEntityMovementVector(exec->assetIndex, 
                                      exec->movementVector.x, 
                                      exec->movementVector.y, 
                                      exec->movementVector.z, 
                                      exec->movementDistance);
                                
                                goto end;
                                
                            }
                                
                        }
                        
                    } 
                      
                } 
                
                setEntityMovementVector(exec->assetIndex, 0.0f, 0.0f, 0.0f, 0.0f);

            } else {
                goto setMovementVector;

            }
        
        }     
end:
    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY) {
        setCameraTrackingEntity(exec->assetIndex, TRUE);
    } else {
        setCameraTrackingEntity(exec->assetIndex, FALSE);
    }

}

void updateCutsceneMapViewPosition(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    adjustMapControllerViewPosition(exec->assetIndex, exec->frameDelta.x, exec->frameDelta.y, exec->frameDelta.z);
}

void cutsceneHandlerSetAnimationDataPtrWithFlag(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

     
    s16 offset;
    
    exec->bytecodePtr += 2;

    offset = *(s16*)exec->bytecodePtr;
    
    exec->animationDataPtr = exec->bytecodePtr + offset;
    
    exec->bytecodePtr += 2;
    
    exec->animationFrameCounter = 0;
    exec->flags |= CUTSCENE_SPRITE_ASSET;

}

void cutsceneHandlerSetAnimationDataPtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u32 offset;

    CutsceneSetAnimDataPtrCmd *ptr = (CutsceneSetAnimDataPtrCmd*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;
    
    offset = ptr->offset;
    
    exec->animationDataPtr = exec->bytecodePtr + offset;
    exec->animationFrameCounter = 0;
    
    exec->bytecodePtr += 2;

}

// alternate
/*
void cutsceneHandlerSetAnimationDataPtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    u32 offset;
    
    exec->bytecodePtr += 2;
    
    offset = ((UnknownCutsceneMapUpdate*)exec->bytecodePtr)->unk_0;
    
    exec->animationDataPtr = exec->bytecodePtr + offset;
    exec->animationFrameCounter = 0;
    
    exec->bytecodePtr += 2;
}
*/

// alternate
/*
void cutsceneHandlerSetAnimationDataPtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    u32 offset;
    
    exec->bytecodePtr += 2;
    
    offset = *(s16*)exec->bytecodePtr;
    
    exec->animationDataPtr = exec->bytecodePtr + offset;
    exec->animationFrameCounter = 0;
    
    exec->bytecodePtr += 2;
}
*/

void cutsceneHandlerSetCoordinates(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;

    exec->coordinates.x = *(s16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->coordinates.y = *(s16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->coordinates.z = *(s16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (exec->flags & CUTSCENE_MAP_ASSET) {
        setMapControllerViewPosition(exec->assetIndex, exec->coordinates.x, exec->coordinates.y, exec->coordinates.z);
    }

    if (exec->flags & CUTSCENE_ENTITY_ASSET) {
        setEntityCoordinates(exec->assetIndex, exec->coordinates.x, exec->coordinates.y, exec->coordinates.z);
    }
    
}

void cutsceneHandlerSetFrameDelta(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;

    exec->frameDelta.x = (f32)*(s8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->frameDelta.y = (f32)*(s8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->frameDelta.z = (f32)*(s8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->waitFrames = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;
    
    exec->bytecodePtr += 2;
    
}

// idle
void cutsceneHandlerSetWaitFrames(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneWaitFramesCmd* ptr = (CutsceneWaitFramesCmd*)exec->bytecodePtr;
    u16 waitFrames;

    (u16*)exec->bytecodePtr += 1;

    waitFrames = ptr->frames;
    
    exec->waitFrames = waitFrames;
    
    (u16*)exec->bytecodePtr += 1;

}

// alternate
/*
void cutsceneHandlerSetWaitFrames(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    (u16*)exec->bytecodePtr += 1;
    exec->waitFrames = *(u16*)exec->bytecodePtr;
    (u16*)exec->bytecodePtr += 1;

}
*/

void cutsceneHandlerDeactivateSelf(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->waitFrames = 1;
    deactivateCutsceneExecutor(index);

}

void cutsceneHandlerExecuteSubroutine(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;

    exec->returnPtr = exec->bytecodePtr + 2;

    exec->bytecodePtr += *(s16*)exec->bytecodePtr;
    
}

void cutsceneHandlerReturnFromSubroutine(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    exec->bytecodePtr = exec->returnPtr;
}

void cutsceneHandlerBranchOnCurrentButton(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnCurrentButtonCmd* ptr = (CutsceneBranchOnCurrentButtonCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].button & ptr->buttonMask) {

        exec->bytecodePtr += 6;
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr; 
        
    } else {
        exec->bytecodePtr += 0xA;
    }

}

void cutsceneHandlerBranchOnButtonPressed(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnButtonPressedCmd* ptr = exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonPressed & ptr->buttonMask) {
        
        exec->bytecodePtr += 6;
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr; 
        
    } else {
        exec->bytecodePtr += 10;
    }

}

void cutsceneHandlerBranchOnButtonRepeat(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnButtonRepeatCmd* ptr = (CutsceneBranchOnButtonRepeatCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonRepeat & ptr->buttonMask) {

        exec->bytecodePtr += 6;
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr; 
        
    } else {
        exec->bytecodePtr += 10;
    }
    
}

void cutsceneHandlerSpawnExecutor(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSpawnExecutorCmd* ptr = (CutsceneSpawnExecutorCmd*)exec->bytecodePtr;

    u16 executorIndex;
    s32 offset;
    u32 spawnedPtr;
    
    exec->bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    exec->bytecodePtr += 2;
    
    offset = ptr->offset;
    spawnedPtr = exec->bytecodePtr;

    exec->bytecodePtr += 4;

    spawnedPtr += offset;
    
    spawnCutsceneExecutor(executorIndex, spawnedPtr);
    
}

void cutsceneHandlerSetOtherExecutorBytecodePtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetOtherExecutorPtrCmd* ptr = (CutsceneSetOtherExecutorPtrCmd*)exec->bytecodePtr;
    u16 executorIndex;
    u32 offset;
    u32 spawnedPtr;

    exec->bytecodePtr += 2;

    executorIndex = ptr->executorIndex;

    exec->bytecodePtr += 2; 

    spawnedPtr = exec->bytecodePtr;

    offset = ptr->offset;

    exec->bytecodePtr += 4;

    spawnedPtr += offset;
    
    if (executorIndex < MAX_BYTECODE_EXECUTORS) {
        
        if (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[executorIndex].waitFrames = 0;
            cutsceneExecutors[executorIndex].bytecodePtr = spawnedPtr;
        }

    }
    
}

void cutsceneHandlerDeactivateExecutor(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneDeactivateExecutorCmd* ptr = (CutsceneDeactivateExecutorCmd*)exec->bytecodePtr;
    u16 executorIndex;

    exec->bytecodePtr += 2;

    executorIndex = ptr->executorIndex;

    exec->bytecodePtr += 2;
    
    deactivateCutsceneExecutor(executorIndex);
    
}

void cutsceneHandlerDMASprite(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


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

    ptr = (CutsceneDmaSpriteIndexCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->assetIndex = ptr->spriteIndex;
    
    exec->bytecodePtr += 2;

    ptr2 = (CutsceneSpriteDMAManagerCmd*)exec->bytecodePtr;

    assetType = ptr2->assetType;

    exec->bytecodePtr += 4;

    romTextureStart = ptr2->romTextureStart;

    exec->bytecodePtr += 4;

    romTextureEnd = ptr2->romTextureEnd;

    exec->bytecodePtr += 4;

    romAssetsIndexStart = ptr2->romAssetsIndexStart;
    
    exec->bytecodePtr += 4;

    romAssetsIndexEnd = ptr2->romAssetsIndexEnd;

    exec->bytecodePtr += 4;

    romSpritesheetIndexStart = ptr2->romSpritesheetIndexStart;

    exec->bytecodePtr += 4;

    romSpritesheetIndexEnd = ptr2->romSpritesheetIndexEnd;

    exec->bytecodePtr += 4;

    texture1Vaddr = ptr2->texture1Vaddr;

    exec->bytecodePtr += 4;

    texture2Vaddr = ptr2->texture2Vaddr;

    exec->bytecodePtr += 4;

    paletteVaddr = ptr2->paletteVaddr;

    exec->bytecodePtr += 4;

    animationVaddr = ptr2->animationVaddr;

    exec->bytecodePtr += 4;

    spriteToPaletteVaddr = ptr2->spriteToPaletteVaddr;

    exec->bytecodePtr += 4;
 
    spritesheetIndexVaddr = ptr2->spritesheetIndexVaddr;

    exec->bytecodePtr += 4;

    dmaSprite(exec->assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   
    setBilinearFiltering(exec->assetIndex, TRUE);
    
}

// alternate
/*
void cutsceneHandlerDMASprite(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


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

    CutsceneSpriteDMAManagerCmd* ptr = (CutsceneSpriteDMAManagerCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->assetIndex = ptr->spriteIndex;
    
    exec->bytecodePtr += 2;

    assetType = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    romTextureStart = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    romTextureEnd = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    romAssetsIndexStart = *(u32*)exec->bytecodePtr;
    
    exec->bytecodePtr += 4;

    romAssetsIndexEnd = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    romSpritesheetIndexStart = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    romSpritesheetIndexEnd = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    texture1Vaddr = *(u8**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    texture2Vaddr = *(u8**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    paletteVaddr = *(u16**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    animationVaddr = *(u16**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    spriteToPaletteVaddr = *(u8**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    spritesheetIndexVaddr = *(u32**)exec->bytecodePtr;

    exec->bytecodePtr += 4;
    
    dmaSprite(exec->assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);

setBilinearFiltering(exec->assetIndex, TRUE);
    
}
*/

void cutsceneHandlerSetEntityAnimations(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;

    exec->assetIndex = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->idleAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    exec->walkingAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->runningAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->idleHoldingAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->walkingHoldingAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->runningHoldingAnimation = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
        setEntityDirectionalAnimation(exec->assetIndex, exec->idleHoldingAnimation);
    } else {
        setEntityDirectionalAnimation(exec->assetIndex, exec->idleAnimation);
    }

    setEntityMapSpaceIndependent(exec->assetIndex, FALSE);

    exec->flags |= CUTSCENE_ENTITY_ASSET;
    
    exec->behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY;
    exec->behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_WANDER | CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT);
    
}

// unused
void cutsceneHandlerDoDMA(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneDMACmd* ptr = (CutsceneDMACmd*)exec->bytecodePtr;
    
    u32 romAddrStart;
    u32 romAddrEnd; 
    void* vaddr;
    
    exec->bytecodePtr += 4;
    
    romAddrStart = ptr->romAddrStart;
    
    exec->bytecodePtr += 4;

    romAddrEnd = ptr->romAddrEnd;

    exec->bytecodePtr += 4;

    vaddr = ptr->vaddr;
    
    exec->bytecodePtr += 4;
    
    nuPiReadRom(romAddrStart, vaddr, romAddrEnd - romAddrStart);

}

// set u8 value
void cutsceneHandlerSetU8Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetU8ValueCmd* ptr = (CutsceneSetU8ValueCmd*)exec->bytecodePtr;

    u8 value;
    u8* variablePtr;

    exec->bytecodePtr += 2;

    value = ptr->value;

    exec->bytecodePtr += 2;

    variablePtr = ptr->variablePtr;

    exec->bytecodePtr += 4;

    *variablePtr = value;

}

// set u16 value
// sets D_80189824
void cutsceneHandlerSetU16Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetU16ValueCmd* ptr = (CutsceneSetU16ValueCmd*)exec->bytecodePtr;

    u16 unk_2;
    u16* unk_4;
    
    exec->bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    exec->bytecodePtr += 2;

    unk_4 = ptr->unk_4;

    exec->bytecodePtr += 4;
    
    *unk_4 = unk_2;
    
}

void cutsceneHandlerSetU32Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetU32ValueCmd* ptr = (CutsceneSetU32ValueCmd*)exec->bytecodePtr;

    u32 cutsceneIndex;
    u32* cutsceneIndexPtr;

    exec->bytecodePtr += 4;

    cutsceneIndex = ptr->cutsceneIndex;

    exec->bytecodePtr += 4;

    cutsceneIndexPtr = ptr->cutsceneIndexPtr;

    exec->bytecodePtr += 4;

    *cutsceneIndexPtr = cutsceneIndex;
    
}

// branch if u8 variable outside range
// i.e., gHour
void cutsceneHandlerBranchU8VarInRange(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u8* variablePtr;
    u16 min;
    u16 max;

    exec->bytecodePtr += 2;

    min = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;
 
    max = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;
    
    variablePtr = *(u32*)exec->bytecodePtr;

    exec->bytecodePtr += 4;    

    if (*variablePtr < min) {
        goto func_end;  
    }
    
    if (max >= *variablePtr) {
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        return;
    }
    
func_end:
    exec->bytecodePtr += 4;
    
}

// Branch if u16 variable outside range
void cutsceneHandlerBranchU16VarInRange(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    u16* variablePtr;
    u16 min;
    u16 max;

    CutsceneBranchU16VariableWithinRange* ptr = (CutsceneBranchU16VariableWithinRange*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    min = ptr->min;

    exec->bytecodePtr += 2;

    max = ptr->max;

    exec->bytecodePtr += 4;    

    variablePtr = ptr->variablePtr;

    exec->bytecodePtr += 4;    

    if (*variablePtr < min) {
        goto func_end;  
    } 
    
    if (max >= *variablePtr) {
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        return;
    }
    
func_end:
    exec->bytecodePtr += 4;
        
}

// branch if u32 value outside range
void cutsceneHandlerBranchU32VarInRange(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchU32VariableWithinRange* ptr = (CutsceneBranchU32VariableWithinRange*)exec->bytecodePtr;

    u16 unk_2;
    u32 min;
    u32 max;
    u32* variablePtr;

    exec->bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    exec->bytecodePtr += 2;

    min = ptr->min;

    exec->bytecodePtr += 4;

    max = ptr->max;

    exec->bytecodePtr += 4;

    variablePtr = ptr->variablePtr;

    exec->bytecodePtr += 4;
 
    if (*variablePtr >= min && max >= *variablePtr) {

        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        
    } else {
        exec->bytecodePtr += 4;
    }

}

// sets gCutsceneCompletionFlags
void cutsceneHandlerSetSpecialBit(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetSpecialBitCmd* ptr = (CutsceneSetSpecialBitCmd*)exec->bytecodePtr;
    
    u16 bit;
    s32* bitfield;

    exec->bytecodePtr += 2;

    bit = ptr->bit;

    exec->bytecodePtr += 2;

    bitfield = ptr->bitfield;

    exec->bytecodePtr += 4;

    *bitfield |= 1 << bit;

}

void cutsceneHandlerClearSpecialBit(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneClearSpecialBitCmd* ptr = (CutsceneClearSpecialBitCmd*)exec->bytecodePtr;

    u16 bit;
    u32* bitfield;
    
    exec->bytecodePtr += 2;

    bit = ptr->bit;

    exec->bytecodePtr += 2;

    bitfield = ptr->bitfield;

    exec->bytecodePtr += 4;

    *bitfield &= ~(1 << bit);
    
}

void cutsceneHandlerBranchOnSpecialBit(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnSpecialBitCmd* ptr = (CutsceneBranchOnSpecialBitCmd*)exec->bytecodePtr;

    u16 bit;
    u32* bitfield;

    exec->bytecodePtr += 2;
    
    bit = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    bitfield= *(u32**)exec->bytecodePtr;

    exec->bytecodePtr += 4;

    if (*bitfield & (1 << bit)) {
        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
    } else {
        exec->bytecodePtr += 4;
    }
    
}

void cutsceneHandlerSetAssetRotation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetAssetRotationCmd* ptr = (CutsceneSetAssetRotationCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    exec->entityDirectionOrMapRotation = ptr->direction;

    exec->bytecodePtr += 2;

    if (exec->flags & CUTSCENE_MAP_ASSET) {
        setInitialMapRotation(exec->assetIndex, exec->entityDirectionOrMapRotation);
    }

    if (exec->flags & CUTSCENE_ENTITY_ASSET) {
        setEntityDirection(exec->assetIndex, convertWorldDirectionToScreenDirection(exec->entityDirectionOrMapRotation, gMainMapIndex));
    } 
    
}

// set up cutscene object as map asset
void cutsceneHandlerSetupMapAsset(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneMapControllerIndexCmd *ptr = (CutsceneMapControllerIndexCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->assetIndex = ptr->index;

    exec->bytecodePtr += 2;

    setMapViewPositionAndCurrentTile(exec->assetIndex, mapControllers[exec->assetIndex].viewPosition.x, mapControllers[exec->assetIndex].viewPosition.y, mapControllers[exec->assetIndex].viewPosition.z, 8, 8);
    enableMapController(exec->assetIndex);
    setMainMapIndex(exec->assetIndex);

    exec->flags |= CUTSCENE_MAP_ASSET;
    
} 

void cutsceneHandlerEntityWalk(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    Vec3f vec;
    
    exec->bytecodePtr += 2;    

    exec->waitFrames = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->entityDirectionOrMapRotation = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->movementDistance = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;
    exec->bytecodePtr += 2;
    
    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
        setEntityDirectionalAnimation(exec->assetIndex, exec->walkingHoldingAnimation);
    } else {
        setEntityDirectionalAnimation(exec->assetIndex, exec->walkingAnimation);
    }

    setEntityDirection(exec->assetIndex, convertWorldDirectionToScreenDirection(exec->entityDirectionOrMapRotation, gMainMapIndex));
    
    vec = getMovementVectorFromDirection(exec->movementDistance, convertScreenDirectionToWorldDirection(entities[exec->assetIndex].direction, gMainMapIndex), 0.0f);

    exec->frameDelta.x = vec.x;
    exec->frameDelta.y = vec.y;
    exec->frameDelta.z = vec.z;
    
    exec->behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT;
    
}

void cutsceneHandlerSetMapRotation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetMapRotationCmd* ptr = (CutsceneSetMapRotationCmd*)exec->bytecodePtr;
    
    u16 mapIndex;
    u8 arg1, rotation;

    exec->bytecodePtr += 2;

    mapIndex = ptr->mapIndex;

    exec->bytecodePtr += 2;

    arg1 = ptr->arg1;

    exec->bytecodePtr++;

    rotation = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;
    
    exec->bytecodePtr += 2;

    startMapRotation(mapIndex, arg1, rotation);
    
}

// set behavior flags
void cutsceneHandlerSetBehaviorFlags(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetBehaviorFlagsCmd* ptr = (CutsceneSetBehaviorFlagsCmd*)exec->bytecodePtr;
    
    u8 behaviorFlags;

    exec->bytecodePtr += 2;

    behaviorFlags = ptr->behaviorFlags;

    exec->bytecodePtr += 2;
    
    exec->behaviorFlags &= ~CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY;
    exec->behaviorFlags |= behaviorFlags;  
    exec->behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT;
    
}

void cutsceneHandlerSetEntityWander(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 temp1, temp2;

    exec->bytecodePtr += 2;

    exec->movementDistance = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;
    exec->bytecodePtr++;

    exec->collisionWidth = *(u16*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;

    exec->collisionHeight = *(u16*)(exec->bytecodePtr);

    exec->bytecodePtr += 2;
    
    exec->targetPosition = exec->coordinates;
    
    exec->behaviorFlags |= (CUTSCENE_ASSET_BEHAVIOR_WANDER | CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT);
    
}

void cutsceneHandlerInitializeMessageBoxType1(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    CutsceneInitMessageBoxType1Cmd* ptr = (CutsceneInitMessageBoxType1Cmd*)exec->bytecodePtr;
    
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;

    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    textAddressesIndex = ptr->textAddressesIndex;

    exec->bytecodePtr += 2;

    textIndex = ptr->textIndex;

    exec->bytecodePtr += 2;

    initializeMessageBox(messageBoxIndex, textAddressesIndex, textIndex, 0x8000);
    
}

void cutsceneHandlerWaitMessageBoxClosed(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneWaitMessageBoxClosedCmd* ptr = (CutsceneWaitMessageBoxClosedCmd*)exec->bytecodePtr;

    u16 messageBoxIndex;

    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    if (!(messageBoxes[messageBoxIndex].flags & MESSAGE_BOX_TEXT_COMPLETE)) {

        exec->waitFrames = 1;
        exec->bytecodePtr -= 4;
        
    }
    
}

void cutsceneHandlerSetMessageBoxViewSpacePosition(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetMessageBoxPositionCmd* ptr = (CutsceneSetMessageBoxPositionCmd*)exec->bytecodePtr;

    u16 messageBoxIndex;
    f32 x, y, z;

    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    x = ptr->x;

    exec->bytecodePtr += 2;

    y = ptr->y;

    exec->bytecodePtr += 2;

    z = ptr->z;

    exec->bytecodePtr += 4;

    setMessageBoxViewSpacePosition(messageBoxIndex, x, y, z);
    
}

// reset message box avatar
void cutsceneHandlerResetMessageBoxAvatar(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneResetMessageBoxAvatarCmd* ptr = (CutsceneResetMessageBoxAvatarCmd*)exec->bytecodePtr;

    u16 messageBoxIndex;
    
    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    messageBoxes[messageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;

    resetMessageBoxAnimation(messageBoxIndex);
    
}

void cutsceneHandlerEntityRun(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

        
    Vec3f vec;
    
    exec->bytecodePtr += 2;    

    exec->waitFrames = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->entityDirectionOrMapRotation = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->movementDistance = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;
    exec->bytecodePtr += 2;
    
    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
        setEntityDirectionalAnimation(exec->assetIndex, exec->runningHoldingAnimation);
    } else {
        setEntityDirectionalAnimation(exec->assetIndex, exec->runningAnimation);
    }

    setEntityDirection(exec->assetIndex, convertWorldDirectionToScreenDirection(exec->entityDirectionOrMapRotation, gMainMapIndex));
    
    vec = getMovementVectorFromDirection(exec->movementDistance, convertScreenDirectionToWorldDirection(entities[exec->assetIndex].direction, gMainMapIndex), 0.0f);

    exec->frameDelta.x = vec.x;
    exec->frameDelta.y = vec.y;
    exec->frameDelta.z = vec.z;
    
    exec->behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_RUNNING;
    
}

void cutsceneHandlerSetEntityAnimation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneEntitySetAnimationCmd* ptr = (CutsceneEntitySetAnimationCmd*)exec->bytecodePtr;

    u16 animation;
    
    exec->bytecodePtr += 2;
    
    animation = ptr->animation;

    exec->bytecodePtr += 2;
    
    setEntityAnimation(exec->assetIndex, animation);
    
    exec->behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT | CUTSCENE_ASSET_BEHAVIOR_RUNNING);
    
}

void cutsceneHandlerSetEntityDirectionalAnimation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneEntitySetAnimationWithDirectionCmd* ptr = (CutsceneEntitySetAnimationWithDirectionCmd*)exec->bytecodePtr;

    u16 animation;
    
    exec->bytecodePtr += 2;
    
    animation = ptr->animation;

    exec->bytecodePtr += 2;
    
    setEntityDirectionalAnimation(exec->assetIndex, animation);
    
    exec->behaviorFlags &= ~(CUTSCENE_ASSET_BEHAVIOR_MOVING_DEFAULT | CUTSCENE_ASSET_BEHAVIOR_RUNNING);
    
}

void cutsceneHandlerSetCallbackBytecodePtr(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

        
    exec->bytecodePtr += 2;

    exec->entityCollidedWithIndex = *(u16*)exec->bytecodePtr; 
        
    exec->bytecodePtr += 2;

    exec->targetButtonMask = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->callbackBytecodePtr = exec->bytecodePtr + *(s16*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;
    
}

void cutsceneHandlerPauseEntity(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutscenePauseEntityCmd* ptr = (CutscenePauseEntityCmd*)exec->bytecodePtr;
    
    u16 entityIndex;

    exec->bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    exec->bytecodePtr += 2;

    pauseEntity(entityIndex);
    
}

void cutsceneHandlerUnpauseEntity(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    CutsceneUnpauseEntityCmd* ptr = (CutsceneUnpauseEntityCmd*)exec->bytecodePtr;
    
    u16 entityIndex;

    exec->bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    exec->bytecodePtr += 2;
    
    unpauseEntity(entityIndex);
    
}

void cutsceneHandlerFlipEntityDirection(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 entityIndex;
    
    CutsceneFlipEntityDirectionCmd* ptr = (CutsceneFlipEntityDirectionCmd*)exec->bytecodePtr; 

    exec->bytecodePtr += 2;

    entityIndex = ptr->entityIndex;
    
    exec->bytecodePtr += 2;
    
    setEntityDirection(exec->assetIndex, (entities[entityIndex].direction + 4) % 8);

}

// set flags on all active sprites, rendered and global
void cutsceneHandlerPauseEntities(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 4;
    
    pauseEntities();
    
}

void cutsceneHandlerUnpauseEntities(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 4;

    unpauseEntities(index);

}

void cutsceneHandlerFlipEntityAnimation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr = exec->bytecodePtr + 4;

    if (exec->behaviorFlags & CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS) {
        setEntityDirectionalAnimation(exec->assetIndex, exec->idleHoldingAnimation);
    } else {
        setEntityDirectionalAnimation(exec->assetIndex, exec->idleAnimation);
    }
    
}

void cutsceneHandlerSetEntityNonCollidable(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    exec->bytecodePtr += 4;
    
    setEntityCollidable(exec->assetIndex, FALSE);
    
}

void cutsceneHandlerSetupEntity(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetupEntityCmd* ptr = (CutsceneSetupEntityCmd*)exec->bytecodePtr;
    
    u16 entityIndex;
    u16 entityAssetIndex;
    u16 flag;

    exec->bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    exec->bytecodePtr += 2;

    entityAssetIndex = ptr->entityAssetIndex;
    
    exec->bytecodePtr += 2;

    flag = ptr->flag;
    
    exec->bytecodePtr += 2;

    loadEntity(entityIndex, entityAssetIndex, flag);
    setEntityTrackingTarget(entityIndex, 0xFFFF, 0xFF);
    setEntityAttachmentOffset(entityIndex, 0, 0, 0);
    setEntityCollidable(entityIndex, TRUE);
    setEntityYMovement(entityIndex, TRUE);
    setEntityTracksCollisions(entityIndex, TRUE);
    enableEntityMovement(entityIndex, TRUE);
    
}

// unused handler
void cutsceneHandlerSetEntityMapSpaceIndependentFlag(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetEntityMapSpaceIndependentFlagCmd* ptr = (CutsceneSetEntityMapSpaceIndependentFlagCmd*)exec->bytecodePtr;

    u16 flag;

    exec->bytecodePtr +=  2;

    flag = ptr->flag;

    exec->bytecodePtr += 2;

    setEntityMapSpaceIndependent(exec->assetIndex, flag);

}

void cutsceneHandlerLoadMap(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneLoadMapCmd* ptr = (CutsceneLoadMapCmd*)exec->bytecodePtr;
    
    u16 mapDataIndex;
    u16 mapIndex;

    exec->bytecodePtr += 2;

    mapDataIndex = ptr->mapDataIndex;

    exec->bytecodePtr += 2;

    mapIndex = ptr->mainMapIndex;

    exec->bytecodePtr += 4;
    
    loadMap(mapDataIndex, mapIndex);

}

void cutsceneHandlerSetEntityMapSpaceIndependent(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 4;

    setEntityMapSpaceIndependent(exec->assetIndex, TRUE);
    
    exec->flags &= ~CUTSCENE_ASSET_BEHAVIOR_RUNNING;
    
}

void cutsceneHandlerSetRGBA(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u8 r, g, b, a;

    exec->bytecodePtr += 2;

    r = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    g = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    b = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    a = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = currentMapLightingRGBA.r;
        g = currentMapLightingRGBA.g;
        b = currentMapLightingRGBA.b;
        a = currentMapLightingRGBA.a;

    }

    if (exec->flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteColor(exec->assetIndex, r, g, b, a);
    }
    if (exec->flags & CUTSCENE_ENTITY_ASSET) {
        setEntityColor(exec->assetIndex, r, g, b, a);
    }
    if (exec->flags & CUTSCENE_MAP_ASSET) {
        setMapControllerRGBA(exec->assetIndex, r, g, b, a);
    }
    
}

void cutsceneHandlerUpdateRGBA(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    u8 r, g, b, a;
    s16 rate;

    exec->bytecodePtr += 2;

    r = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    g = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    b = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    a = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    rate = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = targetMapLightingRGBA.r;
        g = targetMapLightingRGBA.g;
        b = targetMapLightingRGBA.b;
        a = targetMapLightingRGBA.a;

        rate = D_8017045A;
        
    }

    if (exec->flags & CUTSCENE_SPRITE_ASSET) {
        updateSpriteRGBA(exec->assetIndex, r, g, b, a, rate);
    }
    if (exec->flags & CUTSCENE_ENTITY_ASSET) {
        updateEntityRGBA(exec->assetIndex, r, g, b, a, rate);
    }
    if (exec->flags & CUTSCENE_MAP_ASSET) {
        setMapControllerRGBAWithTransition(exec->assetIndex, r, g, b, a, rate);
    }
    
}

void cutsceneHandlerUpdateU8Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneU8UpdateCmd* ptr = (CutsceneU8UpdateCmd*)exec->bytecodePtr;
    
    u8* valuePtr;
    s8 value;
    u8 max;
    u8 initial;

    exec->bytecodePtr += 2;

    value = ptr->value;

    exec->bytecodePtr += 2;

    max = ptr->max;

    exec->bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    exec->bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

void cutsceneHandlerUpdateU16Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneU16UpdateCmd* ptr = (CutsceneU16UpdateCmd*)exec->bytecodePtr;
    
    u16* valuePtr;
    s16 value;
    u16 max;

    exec->bytecodePtr += 2;

    value = ptr->value;

    exec->bytecodePtr += 2;

    max = ptr->max;

    exec->bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    exec->bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

void cutsceneHandlerUpdateU32Value(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneU32UpdateCmd* ptr = (CutsceneU32UpdateCmd*)exec->bytecodePtr;
    
    u32* valuePtr;
    s32 value;
    s32 max;

    exec->bytecodePtr += 4;

    value = ptr->value;

    exec->bytecodePtr += 4;

    max = ptr->max;

    exec->bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    exec->bytecodePtr += 4;

    *valuePtr += cutsceneUpdateValue(*valuePtr, value, max);
    
}

// deactivate map auxillary objects (map objects and weather sprites)
void cutsceneHandlerDeactivateMapObjects(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneDeactivateMapObjectsCmd* ptr = (CutsceneDeactivateMapObjectsCmd*)exec->bytecodePtr;

    u16 mapIndex;
    
    exec->bytecodePtr += 2;

    mapIndex = ptr->mapIndex;
    
    exec->bytecodePtr += 2;

    unloadMapAssets(mapIndex);

}

void cutsceneHandlerUpdateGlobalRGBA(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u8 r, g, b, a;
    s16 rate;

    u16 i;

    exec->bytecodePtr += 2;

    r = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    g = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    b = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    a = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    rate = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

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

void cutsceneHandlerDeactivateSprites(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    exec->bytecodePtr += 4;
    deactivateSprites();

}

void cutsceneHandlerDeactivateMapControllers(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    exec->bytecodePtr += 4;
    // deactivate all map controllers
    deactivateAllMapControllers();

}

void cutsceneHandlerWaitRgbaFinished(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;
    
    if (exec->flags & CUTSCENE_SPRITE_ASSET) {

        if (checkSpriteRGBAUpdateFinished(exec->assetIndex)) {
            exec->bytecodePtr += 2;
        } else {
            exec->waitFrames = 1;
            exec->bytecodePtr -= 2; 
        }
        
    }

    if (exec->flags & CUTSCENE_ENTITY_ASSET) {
        if (checkSpriteRGBAUpdateFinished(entities[exec->assetIndex].globalSpriteIndex)) {
            exec->bytecodePtr += 2;
        } else {
            exec->waitFrames = 1;
            exec->bytecodePtr -= 2; 
        }
    }

    if (exec->flags & CUTSCENE_MAP_ASSET) {
        if (checkMapRGBADone(mapControllers[exec->assetIndex].mainMapIndex)) {
            exec->bytecodePtr += 2;
        } else {
            exec->waitFrames = 1;
            exec->bytecodePtr -= 2; 
        }
    }
    
}

void cutsceneHandlerCheckEntityCollision(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 entityIndex;
    u16 collisionWidth;
    u16 collisionHeight;

    CutsceneCheckEntityCollisionCmd *ptr = (CutsceneCheckEntityCollisionCmd*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;   

    entityIndex = ptr->entityIndex;
    
    exec->bytecodePtr += 2;
    
    collisionWidth = ptr->collisionWidth;
    
    exec->bytecodePtr += 2;
    
    collisionHeight = ptr->collisionHeight;
    
    exec->bytecodePtr += 2;

    if (detectEntityOverlap(&entities[exec->assetIndex], entityIndex, 0.0f, 0.0f, collisionWidth, collisionHeight)) {
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
    } else {
        exec->bytecodePtr += 4;
    }
    
}

void cutsceneHandlerInitializeDialogueSession(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneInitDialogueSessionCmd* ptr = (CutsceneInitDialogueSessionCmd*)exec->bytecodePtr;
    
    u16 dialoguesIndex;
    u16 dialogueBytecodeAddressIndex;
    u16 dialogueIndex;

    exec->bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    exec->bytecodePtr += 2;

    dialogueBytecodeAddressIndex = ptr->dialogueBytecodeAddressIndex;

    exec->bytecodePtr += 2;

    dialogueIndex = ptr->dialogueIndex;

    exec->bytecodePtr += 2;

    initializeDialogueSession(dialoguesIndex, dialogueBytecodeAddressIndex, dialogueIndex, 0);
        
}

void cutsceneHandlerWaitForDialogueInput(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneWaitForDialogueInputCmd* ptr = (CutsceneWaitForDialogueInputCmd*)exec->bytecodePtr;

    u16 dialoguesIndex;

    exec->bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    exec->bytecodePtr += 2;

    if (!(dialogues[dialoguesIndex].sessionManager.flags & DIALOGUE_COMPLETED)) {
        exec->waitFrames = 1;
        exec->bytecodePtr -= 4;
    }
    
}

// branch on dialogue (unk_17 value)
void cutsceneHandlerBranchOnDialogue(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnDialogueCmd* ptr = (CutsceneBranchOnDialogueCmd*)exec->bytecodePtr;

    u16 dialoguesIndex;
    u16 unk;

    exec->bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    exec->bytecodePtr += 2;
    
    unk = ptr->unk;

    exec->bytecodePtr += 4;

    if (dialogues[dialoguesIndex].sessionManager.selectedMenuRow == unk) {

        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        
    } else {
        exec->bytecodePtr += 4;    
    }
    
}

void cutsceneHandlerWaitEntityAnimation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16* base = exec->bytecodePtr; 
    
    exec->bytecodePtr += 2;

    if (exec->flags & CUTSCENE_ENTITY_ASSET) {

        if (checkSpriteAnimationCycleEnded(entities[exec->assetIndex].globalSpriteIndex)) {
            exec->bytecodePtr += 2;
        } else {
            exec->waitFrames = 1;
            exec->bytecodePtr -= 2;
        }
                
    } else if (exec->flags & CUTSCENE_SPRITE_ASSET) {
    
        exec->bytecodePtr = base;
        exec->waitFrames = 1;

    } else {
    
        exec->bytecodePtr += 2;
    
    }

}

void cutsceneHandlerSetMessageBoxAssetIndices(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetMessageBoxAssetsCmd* ptr = (CutsceneSetMessageBoxAssetsCmd*)exec->bytecodePtr;
    
    u16 spriteIndex;
    u8 dialogueWindowIndex;
    u8 overlayIconIndex;
    u8 characterAvatarIndex;

    exec->bytecodePtr += 2;    

    spriteIndex = ptr->spriteIndex;

    exec->bytecodePtr += 2;
    
    dialogueWindowIndex = ptr->dialogueWindowIndex;

    exec->bytecodePtr++;

    overlayIconIndex = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    characterAvatarIndex = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;
    exec->bytecodePtr++;
    
    setMessageBoxSpriteIndices(spriteIndex, dialogueWindowIndex, overlayIconIndex, characterAvatarIndex);

}

void cutsceneHandlerSetEntityTrackingTarget(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetEntityTrackingTargetCmd* ptr = (CutsceneSetEntityTrackingTargetCmd*)exec->bytecodePtr;
    
    u16 targetEntityIndex;
    s16 x, y, z;
    u16 trackingMode;

    exec->bytecodePtr += 2;
    
    targetEntityIndex = ptr->targetEntityIndex;

    exec->bytecodePtr += 2;

    x = ptr->arg1;

    exec->bytecodePtr += 2;

    y = ptr->arg2;

    exec->bytecodePtr += 2;

    z = ptr->arg3;

    exec->bytecodePtr += 2;
    
    trackingMode = ptr->trackingMode;
    
    exec->bytecodePtr++;

    exec->bytecodePtr++;

    setEntityTrackingTarget(exec->assetIndex, targetEntityIndex, trackingMode);
    setEntityAttachmentOffset(exec->assetIndex, x, y, z);
    setEntityCollidable(exec->assetIndex, FALSE);
    setEntityYMovement(exec->assetIndex, FALSE);
    setEntityTracksCollisions(exec->assetIndex, FALSE);
    enableEntityMovement(exec->assetIndex, FALSE);
    
}

void cutsceneHandlerSetHoldingAnimationFlag(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetHoldingAnimationFlagCmd* ptr = (CutsceneSetHoldingAnimationFlagCmd*)exec->bytecodePtr;

    u16 useCarryingAnimation;

    exec->bytecodePtr += 2;

    useCarryingAnimation = ptr->useCarryingAnimations;

    exec->bytecodePtr += 2;

    if (useCarryingAnimation) {
        exec->behaviorFlags |= CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS;
    } else {
        exec->behaviorFlags &= ~CUTSCENE_ASSET_BEHAVIOR_HOLDING_ANIMATIONS;
    }
    
}

void cutsceneHandlerWaitMapLoad(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneMapControllerIndexCmd* ptr = (CutsceneMapControllerIndexCmd*)exec->bytecodePtr; 

    exec->bytecodePtr += 2;

    if (!(mapControllers[ptr->index].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {
        exec->bytecodePtr += 2;
    } else {
        exec->bytecodePtr = ptr;
        exec->waitFrames = 1;
    }

}

void cutsceneHandlerBranchOnEntityDirection(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnEntityDirectionCmd* ptr = (CutsceneBranchOnEntityDirectionCmd*)exec->bytecodePtr;

    u16 entityIndex;
    u8 targetDirecton;

    exec->bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    exec->bytecodePtr += 2;
    
    targetDirecton = ptr->targetDirecton;
    
    exec->bytecodePtr++;
    exec->bytecodePtr++;
    
    if (convertScreenDirectionToWorldDirection(entities[entityIndex].direction, gMainMapIndex) == targetDirecton) {

        exec->returnPtr = exec->bytecodePtr + 2;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        return;
        
    }

    exec->bytecodePtr += 2;
    
}

void cutsceneHandlerSetEntityPhysicsFlags(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetEntityPhysicsFlagsCmd* ptr = (CutsceneSetEntityPhysicsFlagsCmd*)exec->bytecodePtr;

    u16 temp;

    exec->bytecodePtr += 2;

    temp = ptr->unk_2;

    exec->bytecodePtr += 2;

    switch (temp) {

        case 0:
            setEntityCollidable(exec->assetIndex, FALSE);
            setEntityTracksCollisions(exec->assetIndex, FALSE);
            enableEntityMovement(exec->assetIndex, FALSE);
            setEntityYMovement(exec->assetIndex, FALSE);
            break;
        case 1:
            setEntityCollidable(exec->assetIndex, FALSE);
            setEntityTracksCollisions(exec->assetIndex, TRUE);
            enableEntityMovement(exec->assetIndex, FALSE);
            setEntityYMovement(exec->assetIndex, TRUE);
            break;
        case 2:
            setEntityCollidable(exec->assetIndex, TRUE);
            setEntityTracksCollisions(exec->assetIndex, TRUE);
            enableEntityMovement(exec->assetIndex, TRUE);
            setEntityYMovement(exec->assetIndex, TRUE);
            break;
        
    }
    
}

void cutsceneHandlerSetEntityPalette(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetEntityPaletteCmd* ptr = (CutsceneSetEntityPaletteCmd*)exec->bytecodePtr;
    
    u16 paletteIndex;

    exec->bytecodePtr += 2;

    paletteIndex = ptr->paletteIndex;

    exec->bytecodePtr += 2;
    
    setEntityPaletteIndex(exec->assetIndex, paletteIndex);

}

void cutsceneHandlerSetEntitySpriteDimensions(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u8 spriteWidth, spriteHeight;

    exec->bytecodePtr += 2;
    
    spriteWidth = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    spriteHeight = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    setEntitySpriteDimensions(entities[exec->assetIndex].entityAssetIndex, spriteWidth, spriteHeight);
    
}

void cutsceneHandlerSetShadowFlags(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 entityAssetIndex;
    u16 flags;
    
    CutsceneSetShadowFlagsCmd* ptr = (CutsceneSetShadowFlagsCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    entityAssetIndex = ptr->entityAssetIndex;
    
    exec->bytecodePtr += 2;
    
    flags = ptr->flags;
    
    exec->bytecodePtr += 4;

    entityAssetDescriptors[entityAssetIndex].shadowSpriteIndex = flags;

}

void cutsceneHandlerSetSpriteScale(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    exec->bytecodePtr += 2;

    exec->scale.x = *(s16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    exec->scale.y = *(s16*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;
        
    exec->scale.z = *(s16*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;

    if (exec->flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteScale(exec->assetIndex, exec->scale.x, exec->scale.y, exec->scale.z);
    }
    
}

void cutsceneHandlerSetSpriteRenderngLayer(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetSpriteRenderingLayerCmd* ptr = (CutsceneSetSpriteRenderingLayerCmd*)exec->bytecodePtr;

    u16 renderingLayerFlags;

    exec->bytecodePtr += 2;

    renderingLayerFlags = ptr->renderingLayer;

    exec->bytecodePtr += 2;

    if (exec->flags & CUTSCENE_SPRITE_ASSET) {
        setSpriteBlendMode(exec->assetIndex, renderingLayerFlags);
    }
    
}

void cutsceneHandlerInitializeMessageBoxType2(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneInitMessageBoxType2Cmd* ptr = (CutsceneInitMessageBoxType2Cmd*)exec->bytecodePtr;
    
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;

    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    textAddressesIndex = ptr->textAddressesIndex;

    exec->bytecodePtr += 2;

    textIndex = ptr->textIndex;

    exec->bytecodePtr += 2;

    initializeMessageBox(messageBoxIndex, textAddressesIndex, textIndex, 0x100000);
    
}

void cutsceneHandlerInitMapAddition(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 mapAdditionIndex;
    u16 flag;
    
    CutsceneInitMapAdditionCmd* ptr = (CutsceneInitMapAdditionCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    mapAdditionIndex = ptr->mapAdditionIndex;
    
    exec->bytecodePtr += 2;
    
    flag = ptr->flag;
    
    exec->bytecodePtr += 4;

    // initialize mapAdditions struct
    activateMapAddition(MAIN_MAP_INDEX, mapAdditionIndex, flag);

}

// branch on random value
void cutsceneHandlerBranchOnRandom(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchOnRandomCmd* ptr= (CutsceneBranchOnRandomCmd*)exec->bytecodePtr;

    u16 unk_2;
    u16 unk_4;
    u16 temp;

    exec->bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    exec->bytecodePtr += 2;

    unk_4 = ptr->unk_4;
    
    exec->bytecodePtr += 2;

    temp = getRandomNumberInRange(0, 100);

    if ((temp < unk_2) ^ 1 && (unk_4 < temp) ^ 1) {

        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        
    } else {
        exec->bytecodePtr += 2;
    }

}

void cutsceneHandlerBranchIfU16PtrInRange(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchIfU16PtrInRangeCmd* ptr = (CutsceneBranchIfU16PtrInRangeCmd*)exec->bytecodePtr;
    
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;

    exec->bytecodePtr += 4;

    unk_4 = ptr->unk_4;

    exec->bytecodePtr += 4;

    unk_8 = ptr->unk_8;

    exec->bytecodePtr += 4;

    unk_C = ptr->unk_C;

    exec->bytecodePtr += 4;

    if (*unk_4 >= *unk_8 && unk_C >= *unk_4) {

        exec->returnPtr = exec->bytecodePtr + 4;
        exec->bytecodePtr += *(s16*)exec->bytecodePtr;
        
    } else {
        exec->bytecodePtr += 4;
    }

}

void cutsceneHandlerPauseExecutor(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutscenePauseExecutorCmd* ptr = (CutscenePauseExecutorCmd*)exec->bytecodePtr;
    u16 executorIndex;
    
    exec->bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    exec->bytecodePtr += 2;

    if (executorIndex < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[executorIndex].flags |= CUTSCENE_PAUSE_EXECUTION;
    }
}

void cutsceneHandlerUnpauseExecutor(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneUnpauseExecutorCmd* ptr = (CutsceneUnpauseExecutorCmd*)exec->bytecodePtr;
    u16 executorIndex;
    
    exec->bytecodePtr += 2;

    executorIndex = ptr->executorIndex;
    
    exec->bytecodePtr += 2;

    if (executorIndex < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[executorIndex].flags & CUTSCENE_ASSET_ACTIVE)) {
        cutsceneExecutors[executorIndex].flags &= ~CUTSCENE_PAUSE_EXECUTION;
    }

}

void cutsceneHandlerPauseAllChildExecutors(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 i;

    exec->bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[i].flags |= CUTSCENE_PAUSE_EXECUTION;
        }

    }

}

void cutsceneHandlerUnpauseAllChildExecutors(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 i;

    exec->bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & CUTSCENE_ASSET_ACTIVE) {
            cutsceneExecutors[i].flags &= ~CUTSCENE_PAUSE_EXECUTION;
        }

    }
    
}

void cutsceneHandlerSetSpritePalette(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 paletteIndex;
    
    CutsceneSetSpritePaletteCmd* ptr = (CutsceneSetSpritePaletteCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    paletteIndex = ptr->paletteIndex;
    
    exec->bytecodePtr += 2;
    
    setSpritePaletteIndex(exec->assetIndex, paletteIndex);
    
}

void cutsceneHandlerBranchIfU8PtrInRange(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneBranchIfU8PtrInRangeCmd* ptr = (CutsceneBranchIfU8PtrInRangeCmd*)exec->bytecodePtr;

    u8* temp1;
    u8* temp2;
    u32 temp3;
    
    exec->bytecodePtr += 4;

    temp1 = ptr->unk_4;

    exec->bytecodePtr += 4;

    temp2 = ptr->unk_8;

    exec->bytecodePtr += 4;

    temp3 = ptr->unk_C;

    exec->bytecodePtr += 4;

    if (*temp1 >= *temp2) {

        if (temp3 >= *temp1) {
            exec->returnPtr = (u16*)ptr + 10;
            exec->bytecodePtr += *(s16*)exec->bytecodePtr;
            return;
        }
        
    }

    exec->bytecodePtr += 4;
    
}

void cutsceneHandlerSetAudioSequence(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 sequenceIndex;
    u8 *sequenceStart;
    u8 *sequenceEnd;

    CutsceneSetAudioSequenceCmd* ptr = (CutsceneSetAudioSequenceCmd*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    exec->bytecodePtr += 2;
    
    sequenceStart = ptr->sequenceStart;
    
    exec->bytecodePtr += 4;
    
    sequenceEnd = ptr->sequenceEnd;
    
    exec->bytecodePtr += 4;    

    setAudioSequence(sequenceIndex, sequenceStart, sequenceEnd);
    setAudioSequenceVolumes(sequenceIndex, 0, 0);
    
}

/*
void cutsceneHandlerSetAudioSequence(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

 
    u16 index;
    u8 *sequenceStart;
    u8 *sequenceEnd;

    exec->bytecodePtr += 2;

    index = *(s16*)exec->bytecodePtr;
    
    exec->bytecodePtr += 2;
    
    sequenceStart = *(u8**)exec->bytecodePtr;
    
    exec->bytecodePtr += 4;
    
    sequenceEnd = *(u8**)exec->bytecodePtr;
    
    exec->bytecodePtr += 4;    

    setAudioSequence(index, sequenceStart, sequenceEnd);
    setAudioSequenceVolumes(index, 0, 0);
    
}
*/

void cutsceneHandlerStopAudioSequenceWithFadeOut(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneStopSequenceWithFadeCmd* ptr = (CutsceneStopSequenceWithFadeCmd*)exec->bytecodePtr;
    
    u16 sequenceIndex;
    u32 speed;

    exec->bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    exec->bytecodePtr += 2;

    speed = ptr->speed;

    exec->bytecodePtr += 4;

    stopAudioSequenceWithFadeOut(sequenceIndex, speed);
    
}

void cutsceneHandlerSetAudioSequenceVolume(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetAudioSequenceVolumeCmd* ptr = (CutsceneSetAudioSequenceVolumeCmd*)exec->bytecodePtr;
    
    u16 sequenceIndex;
    s32 targetVolume;
    s16 volume;

    exec->bytecodePtr += 2;

    sequenceIndex = ptr->sequenceIndex;
    
    exec->bytecodePtr += 2;
    
    targetVolume = ptr->targetVolume;
    
    exec->bytecodePtr += 2;
    
    volume = ptr->volume;
    
    exec->bytecodePtr += 2;    

    setAudioSequenceVolumes(sequenceIndex, targetVolume, volume);
    
}

void cutsceneHandlerSetSfx(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetSfxCmd* ptr = (CutsceneSetSfxCmd*)exec->bytecodePtr;
    
    u32 sfxIndex;
    u16 volume;

    exec->bytecodePtr += 2;
    
    sfxIndex = ptr->sfxIndex;

    exec->bytecodePtr += 2;

    volume = ptr->volume;

    exec->bytecodePtr += 4;

    sfxIndex++;
    
    setSfx(sfxIndex);
    setSfxVolume(sfxIndex, volume);
    
} 

void cutsceneHandlerIdleWhileAudioSequencePlaying(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneIdleWhileSequencePlayingCmd* ptr = (CutsceneIdleWhileSequencePlayingCmd*)exec->bytecodePtr;

    u16 sequenceIndex;

    exec->bytecodePtr += 2;
    
    sequenceIndex = ptr->sequenceIndex;

    if (!gAudioSequences[sequenceIndex].flags) {
        exec->bytecodePtr += 2;
    } else {
        exec->bytecodePtr = ptr;
        exec->waitFrames = 1;
    }

}

void cutsceneHandlerUpdateMessageBoxRGBA(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 messageBoxIndex;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 flags;

    CutsceneUpdateMessageBoxRgbaCmd* ptr = (CutsceneUpdateMessageBoxRgbaCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    r = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;

    g = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;

    b = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    a = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    flags = *(u16*)exec->bytecodePtr;

    exec->bytecodePtr += 4;
    
    setMessageBoxRGBAWithTransition(messageBoxIndex, r, g, b, a, flags);
    
}

void cutsceneHandlerWaitMessageBoxReady(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 messageBoxIndex;
    
    CutsceneWaitMessageBoxReadyCmd* ptr = (CutsceneWaitMessageBoxReadyCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;
    
    messageBoxIndex = ptr->messageBoxIndex;

    exec->bytecodePtr += 2;

    if (!checkMessageBoxRGBAComplete(messageBoxIndex)) {
        exec->waitFrames = 1;
        exec->bytecodePtr -= 4;
    }
    
}

void cutsceneHandlerSetSpriteBilinearFiltering(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetSpriteBilinearCmd* ptr = (CutsceneSetSpriteBilinearCmd*)exec->bytecodePtr;
    u16 useBilinearFiltering;

    exec->bytecodePtr += 2;

    useBilinearFiltering = ptr->useBilinearFilterng;

    exec->bytecodePtr += 2;

    setBilinearFiltering(exec->assetIndex, useBilinearFiltering);
    
}

void cutsceneHandlerSetMapAddition(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    CutsceneSetMapAdditionCmd* ptr = (CutsceneSetMapAdditionCmd*)exec->bytecodePtr;
    
    u16 mapAdditionIndex;
    u8 x;
    u8 z;

    exec->bytecodePtr += 2;

    mapAdditionIndex = ptr->mapAdditionIndex;

    exec->bytecodePtr += 2;

    x = ptr->x;

    exec->bytecodePtr++;

    z = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->bytecodePtr += 2;
    
    setMapAdditionIndexFromCoordinates(MAIN_MAP_INDEX, mapAdditionIndex, x, z);
    setGridToTileTextureMappings(MAIN_MAP_INDEX);
    
}

void cutsceneHandlerSetMapGroundObject(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];


    u16 spriteIndex;
    u8 x;
    u8 z;

    CutsceneSetMapGroundObjectCmd* ptr = (CutsceneSetMapGroundObjectCmd*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    spriteIndex = ptr->spriteIndex;
    
    exec->bytecodePtr += 2;
    
    x = ptr->x;

    exec->bytecodePtr++;
    
    z = *(u8*)exec->bytecodePtr;
    
    exec->bytecodePtr++;
    
    exec->bytecodePtr += 2;
    
    setMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, spriteIndex, x, z);
    setGroundObjects(MAIN_MAP_INDEX);
    
}

void cutsceneHandlerSetMessageInterpolation(u16 index) {
    CutsceneExecutor *exec = &cutsceneExecutors[index];

    
    s16 messageBoxIndex;
    s16 rate;
    u8 flag;

    exec->bytecodePtr += 2;

    rate = *(s16*)exec->bytecodePtr;

    exec->bytecodePtr += 2;

    messageBoxIndex = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    flag = *(u8*)exec->bytecodePtr;

    exec->bytecodePtr++;

    exec->bytecodePtr += 2;
    
    setMessageBoxInterpolationWithFlags(messageBoxIndex, rate, flag);
    
}