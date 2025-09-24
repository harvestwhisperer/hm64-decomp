#include "common.h"

#include "system/map.h"

#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/math.h"
#include "system/mapController.h"
#include "system/sceneGraph.h"

#include "mainproc.h" 
  
// forward declarations
Vec3f getTerrainHeight(TerrainQuad *quad, f32 x, f32 z, u8 fallbackHeight);
void updateCameraViewBounds(MapCameraView*);
bool checkTileVisible(MainMap *map, u8 arg1, u8 arg2);       
bool setMapAdditionMetadata(u16, u16, u8, u8, u16);       
Vec3f getMapGroundObjectCoordinates(u16, f32, f32);                  
inline u16 getTileIndexFromGrid(u16 mapIndex, u8 x, u8 z);
Gfx* prepareTileTextures(Gfx* dl, MainMap* map, u8 arg2);
u16 getMapGroundObjectSpriteIndex(u16, f32, f32);  
void initializeMesh(MainMap*); 
void func_80036C08(u16 mapIndex);
void setMapGrid(MapGrid*, u8*);                 
u8* func_800374C0(TileRenderingInfo*, u8*);
u8* setTileVertexData(Tile* vtx, u8* data);    
u16* getTileVtxPtrFromCount(u16 arg0, void *arg1);
u32 setTileVertices(MainMap*, u16, f32, f32, f32);
Gfx* renderTiles(Gfx*, MainMap*, u16, u8);
u16 buildTileRenderingCommands(Gfx*, MainMap*, TileRenderingInfo[]);  
Gfx* appendTileToDL(Gfx* arg0, MainMap* arg1, u16 arg2, f32 arg3, f32 arg4, f32 arg5);
void setCoreMapObjects(MainMap*);  
void setCoreMapObject(CoreMapObject* coreMapObject, u8* data);                        
void setCoreMapObjectsMetadata(CoreMapObjectsMetadata* metadata, u8* data); 
u8* getCoreMapObjectPtrFromCount(u16 arg0, u8* arg1);     
void setTerrainQuad(TerrainQuad *quad, u8* quadData, u16 vtxIndex);
u8* getMapAdditionsMetadataPtrFromIndex(u16 mapAdditionIndex, u8 *ptr);
void setupCoreMapObjectSprites(MainMap*); 
void setupMapObjectSprites(MainMap*); 
void setupWeatherSprites(MainMap*); 
void processMapSceneNode(u16, Gfx*);                 
void renderGroundObjects(MainMap*);         
void func_80039F58(u16);   


// bss
MainMap mainMap[MAX_MAPS];

f32 frustumEdgeCoefficient0;
f32 frustumEdgeCoefficient1;
f32 frustumEdgeCoefficient2;
f32 frustumEdgeCoefficient3;

CoreMapObject coreMapObjects[];
u8 globalBobbingYOffset;
u8 globalBobbingFrameCounter;

u8 gridPositionToX[];
u8 gridPositionToZ[];

Gfx mapDisplayList[2][6912];
Vtx tileVertices[2][2560];

Vtx groundObjectVertices[2][320][4];
Gfx groundObjectBitmapsDisplayList[2][0x1000];

u16 previousGridPositionForTileTexture[MAX_TILE_TEXTURES];

// data 

// unused visibility grid values
u8 D_80112D10[27][27] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00 },
    { 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 },
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 },
    { 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

u8 D_80112FEC[480] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
};

u8 D_801131CC[484] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D,
    0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E,
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
    0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
    0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17,
    0x00, 0x00, 0x00, 0x00,
};

// rodata
static const Gfx D_8011ED68;
static const Gfx D_8011ED70;
static const Gfx D_8011ED78;
static const Gfx D_8011ED88;
static const Gfx D_8011EDC8;
static const Gfx D_8011ED80;
static const Gfx D_8011ED90;
static const Gfx D_8011ED98;
static const Gfx D_8011EDA0;
static const Gfx D_8011EDA8;
static const Gfx D_8011EDD0;
static const Gfx D_8011EDD8;

static const char D_8011EDB0[];
static const char D_8011EDB4[];

static inline u16 swap16TileIndex(Swap16 halfword) {

    Swap16 swap; 
    
    swap.byte[1] = halfword.byte[0];
    swap.byte[0] = halfword.byte[1];

    swap.halfword -= 1;
    
    return swap.halfword; 

}

//INCLUDE_ASM("asm/nonmatchings/system/map", initializeMap);

void initializeMap(void) {
 
    u16 i, j; 
     
    globalBobbingFrameCounter = 0;
    globalBobbingYOffset = 0;
    
    for (i = 0; i < MAX_MAPS; i++) {
        
        mainMap[i].mapState.flags = 0;
        mainMap[i].mapState.renderedVertexCount = 0;
        mainMap[i].mapState.startingVertex = 0;
        mainMap[i].mapState.totalVertexCount = 0;

        mainMap[i].mapGlobals.translation.x = 0;
        mainMap[i].mapGlobals.translation.y = 0;
        mainMap[i].mapGlobals.translation.z = 0;
 
        mainMap[i].mapGlobals.scale.x = 1.0f;
        mainMap[i].mapGlobals.scale.y = 1.0f;
        mainMap[i].mapGlobals.scale.z = 1.0f;
 
        mainMap[i].mapGlobals.rotation.x = 0;
        mainMap[i].mapGlobals.rotation.y = 0;
        mainMap[i].mapGlobals.rotation.z = 0;

        mainMap[i].mapGlobals.currentRGBA.r = 255.0f;
        mainMap[i].mapGlobals.currentRGBA.g = 255.0f;
        mainMap[i].mapGlobals.currentRGBA.b = 255.0f;
        mainMap[i].mapGlobals.currentRGBA.a = 255.0f;

        mainMap[i].mapCameraView.cameraTileX = 0;
        mainMap[i].mapCameraView.cameraTileZ = 0;
        mainMap[i].mapCameraView.viewExtentX = 0;
        mainMap[i].mapCameraView.viewExtentZ = 0;

        mainMap[i].mapCameraView.viewOffset.x = 0;
        mainMap[i].mapCameraView.viewOffset.y = 0;
        mainMap[i].mapCameraView.viewOffset.z = 0;

        mainMap[i].mapCameraView.frustumCorner0.x = 0;
        mainMap[i].mapCameraView.frustumCorner0.y = 0;
        mainMap[i].mapCameraView.frustumCorner0.z = 0;

        mainMap[i].mapCameraView.frustumCorner1.x = 0;
        mainMap[i].mapCameraView.frustumCorner1.y = 0;
        mainMap[i].mapCameraView.frustumCorner1.z = 0;
        
        mainMap[i].mapCameraView.frustumCorner2.x = 0;
        mainMap[i].mapCameraView.frustumCorner2.y = 0;
        mainMap[i].mapCameraView.frustumCorner2.z = 0;

        mainMap[i].mapCameraView.frustumCorner3.x = 0;
        mainMap[i].mapCameraView.frustumCorner3.y = 0;
        mainMap[i].mapCameraView.frustumCorner3.z = 0;

        mainMap[i].mapCameraView.rotation.x = 0;
        mainMap[i].mapCameraView.rotation.y = 0;
        mainMap[i].mapCameraView.rotation.z = 0;
  
        for (j = 0; j < MAX_MAP_TILES; j++) {
            mainMap[i].tiles[j].renderingCommandsOffset = 0;
            mainMap[i].tiles[j].triCount = 0;
            mainMap[i].tiles[j].yOffset = 0;
            mainMap[i].tiles[j].fallbackHeight = 0;
            mainMap[i].tiles[j].textureIndex = 0;
            mainMap[i].tiles[j].verticesPerTile = 0;
        }
    
    }

} 

//INCLUDE_ASM("asm/nonmatchings/system/map", setupMap);

// FIXME: iterators are messed up; loops are probably inline funcs 
bool setupMap(u16 mapIndex, 
    u8* grid, 
    void* mesh, 
    u8* terrainQuads, 
    u8** gridToLevelInteractionIndex, 
    u8* coreMapObjectsData, 
    void* tileTextures, 
    void* tilePalettes, 
    void* coreMapObjectsTextures, 
    void* coreMapObjectsPalettes, 
    u8 *mapAdditionsMetadataPtr) {

    bool result;

    u16 i, k, l, m, n, o, p, q, j, r;

    u8 centerTileX;
    u8 centerTileZ;

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && !(mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapState.flags = 1;
        mainMap[mapIndex].mapState.renderedVertexCount = 0;
        mainMap[mapIndex].mapState.mapObjectCount = 0;
        
        mainMap[mapIndex].mesh = mesh;
        mainMap[mapIndex].terrainQuads = terrainQuads;
        mainMap[mapIndex].gridToLevelInteractionIndex = gridToLevelInteractionIndex;
        mainMap[mapIndex].coreMapObjects = coreMapObjectsData;

        mainMap[mapIndex].tileTextures = tileTextures;
        mainMap[mapIndex].tilePalettes = tilePalettes;
        mainMap[mapIndex].coreMapObjectsTextures = coreMapObjectsTextures;
        mainMap[mapIndex].coreMapObjectsPalettes = coreMapObjectsPalettes;

        mainMap[mapIndex].mapAdditionsMetadataPtr = mapAdditionsMetadataPtr;

        for (j = 0; j < 16; j++) {

            mainMap[mapIndex].coreMapObjectsMetadata[j].spriteIndex = 0; 
            mainMap[mapIndex].coreMapObjectsMetadata[j].totalCoreMapObjects = 0; 
            mainMap[mapIndex].coreMapObjectsMetadata[j].unk_0 = 0; 

        }

        for (l = 0; l < MAX_CORE_MAP_OBJECTS; l++) {
            coreMapObjects[l].flags = 0;
        }

        for (i = 0; i < MAX_MAP_OBJECTS; i++) {
            mainMap[mapIndex].mapObjects[i].flags = 0;
        }

        for (i = 0; i < MAX_GROUND_OBJECTS; i++) {
            mainMap[mapIndex].groundObjects.spriteIndexToGrid[i] = 0xFFFF;
        }

        for (i = 0; i < MAX_WEATHER_SPRITES; i++) {
            mainMap[mapIndex].weatherSprites[i].flags = 0;
        }

        for (p = 0; p < 480; p++) {
            mainMap[mapIndex].groundObjects.gridToSpriteIndex[p] = 0;
            mainMap[mapIndex].groundObjects.previousGridToSpriteIndex[p] = 0;
            mainMap[mapIndex].groundObjects.nextGridToSpriteIndex[p] = 0;
        }

        for (i = 0; i < MAX_MAP_ADDITIONS; i++) {

            for (j = 0; j < 16; j++) {
                mainMap[mapIndex].mapAdditions[i].arr1[j] = 0xFFFF;
                mainMap[mapIndex].mapAdditions[i].arr2[j] = 0;
            }
            
            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[i].flags = 0;
            
        }

        setMapGrid(&mainMap[mapIndex].mapGrid, grid);

        initializeMesh(&mainMap[mapIndex]);
        // tiles
        func_80036C08(mapIndex);
        setCoreMapObjects(&mainMap[mapIndex]);

        // setup camera
        centerTileX = mainMap[mapIndex].mapGrid.mapWidth / 2;
        centerTileZ = mainMap[mapIndex].mapGrid.mapHeight / 2;

        func_800343FC(mapIndex, centerTileX, centerTileZ, centerTileX, centerTileZ, 0.0f, 0.0f, 0.0f, 1);
        updateCameraViewBounds(&mainMap[mapIndex].mapCameraView);

        mainMap[mapIndex].mapState.mapOriginX = ((mainMap[mapIndex].mapGrid.mapWidth * mainMap[mapIndex].mapGrid.tileSizeX) / 2) + (mainMap[mapIndex].mapGrid.tileSizeX / 2);
        mainMap[mapIndex].mapState.mapOriginZ = ((mainMap[mapIndex].mapGrid.mapHeight * mainMap[mapIndex].mapGrid.tileSizeZ) / 2) + (mainMap[mapIndex].mapGrid.tileSizeZ / 2);

        for (r = 0; r < 1596; r++) {
            gridPositionToX[r] = r % mainMap[mapIndex].mapGrid.mapWidth;
            gridPositionToZ[r] = r / mainMap[mapIndex].mapGrid.mapWidth;
        }
        
        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034090);

bool func_80034090(u16 mapIndex) {

    bool result = FALSE;

    u8 i;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        globalBobbingFrameCounter = 0;
        globalBobbingYOffset = 0;

        for (i = 0; i < MAX_MAP_OBJECTS; i++) {
            
            if (mainMap[mapIndex].mapObjects[i].flags & MAP_OBJECT_ACTIVE) {
                deactivateSprite(mainMap[mapIndex].mapObjects[i].spriteIndex);
            }
            
            mainMap[mapIndex].mapObjects[i].flags = 0;

        }
        
        for (i = 0; i < MAX_WEATHER_SPRITES; i++) { 
            
            if (mainMap[mapIndex].weatherSprites[i].flags & MAP_WEATHER_SPRITE_ACTIVE) {
                deactivateSprite(mainMap[mapIndex].weatherSprites[i].spriteIndex);
            }

            mainMap[mapIndex].weatherSprites[i].flags = 0;

        }
        
        mainMap[mapIndex].mapState.flags = 0;

        result = TRUE;
        
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapTranslation);

bool setMapTranslation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapGlobals.translation.x = x;
        mainMap[mapIndex].mapGlobals.translation.y = y;
        mainMap[mapIndex].mapGlobals.translation.z = z;

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034298);

bool setMapScale(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.scale.x = arg1;
        mainMap[mapIndex].mapGlobals.scale.y = arg2;
        mainMap[mapIndex].mapGlobals.scale.z = arg3;
        
        result = TRUE;

    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapRotation);

bool setMapRotation(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.rotation.x = arg1;
        mainMap[mapIndex].mapGlobals.rotation.y = arg2;
        mainMap[mapIndex].mapGlobals.rotation.z = arg3;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapRGBA);

bool setMapRGBA(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.currentRGBA.r = arg1;
        mainMap[mapIndex].mapGlobals.currentRGBA.g = arg2;
        mainMap[mapIndex].mapGlobals.currentRGBA.b = arg3;
        mainMap[mapIndex].mapGlobals.currentRGBA.a = arg4;
        
        mainMap[mapIndex].mapGlobals.targetRGBA.r = arg1;
        mainMap[mapIndex].mapGlobals.targetRGBA.g = arg2;
        mainMap[mapIndex].mapGlobals.targetRGBA.b = arg3;
        mainMap[mapIndex].mapGlobals.targetRGBA.a = arg4;
    
        result = TRUE; 

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800343FC);

bool func_800343FC(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4, f32 x, f32 y, f32 z, u8 arg8) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapCameraView.cameraTileX = arg1;
        mainMap[mapIndex].mapCameraView.cameraTileZ = arg2;
        mainMap[mapIndex].mapCameraView.viewExtentX = arg3;
        mainMap[mapIndex].mapCameraView.viewExtentZ = arg4;

        mainMap[mapIndex].mapCameraView.viewOffset.x = -x;
        mainMap[mapIndex].mapCameraView.viewOffset.y = -y;
        mainMap[mapIndex].mapCameraView.viewOffset.z = -z;
        
        if (arg8 == 1) {
            mainMap[mapIndex].mapState.flags |= 4;
        } else {
            mainMap[mapIndex].mapState.flags &= ~4;
        }

        updateCameraViewBounds(&mainMap[mapIndex].mapCameraView);
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapTranslation);

bool adjustMapTranslation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.translation.x += x;
        mainMap[mapIndex].mapGlobals.translation.y += y;
        mainMap[mapIndex].mapGlobals.translation.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapScale);

// unused
bool adjustMapScale(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.scale.x += x;
        mainMap[mapIndex].mapGlobals.scale.y += y;
        mainMap[mapIndex].mapGlobals.scale.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapRotation);

bool adjustMapRotation(u16 mapIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.rotation.x += x;
        mainMap[mapIndex].mapGlobals.rotation.y += y;
        mainMap[mapIndex].mapGlobals.rotation.z += z;
        
        result = TRUE;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", adjustMapRGBA);

bool adjustMapRGBA(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].mapGlobals.currentRGBA.r += arg1;
        mainMap[mapIndex].mapGlobals.currentRGBA.g += arg2;
        mainMap[mapIndex].mapGlobals.currentRGBA.b += arg3;
        mainMap[mapIndex].mapGlobals.currentRGBA.a += arg4;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034738);

bool func_80034738(u16 mapIndex, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    bool result;

    f32 temp;

    s16 absRate = getAbsoluteValue(rate);

    result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].mapGlobals.targetRGBA.r = r;
        mainMap[mapIndex].mapGlobals.targetRGBA.g = g;
        mainMap[mapIndex].mapGlobals.targetRGBA.b = b;
        mainMap[mapIndex].mapGlobals.targetRGBA.a = a;

        mainMap[mapIndex].mapState.RGBARate = absRate;

        mainMap[mapIndex].mapState.flags &= ~8;

        if (mainMap[mapIndex].mapGlobals.targetRGBA.r < mainMap[mapIndex].mapGlobals.currentRGBA.r) {
            temp = mainMap[mapIndex].mapGlobals.currentRGBA.r - mainMap[mapIndex].mapGlobals.targetRGBA.r;
        } else {
            temp = mainMap[mapIndex].mapGlobals.targetRGBA.r - mainMap[mapIndex].mapGlobals.currentRGBA.r;
        }

        mainMap[mapIndex].mapGlobals.deltaRGBA.r = (temp * absRate) / 255.0f;
        
        if (mainMap[mapIndex].mapGlobals.targetRGBA.g < mainMap[mapIndex].mapGlobals.currentRGBA.g) {
            temp = mainMap[mapIndex].mapGlobals.currentRGBA.g - mainMap[mapIndex].mapGlobals.targetRGBA.g;
        } else {
            temp = mainMap[mapIndex].mapGlobals.targetRGBA.g - mainMap[mapIndex].mapGlobals.currentRGBA.g;
        }

        mainMap[mapIndex].mapGlobals.deltaRGBA.g = (temp * absRate) / 255.0f;

        if (mainMap[mapIndex].mapGlobals.targetRGBA.b < mainMap[mapIndex].mapGlobals.currentRGBA.b) {
            temp = mainMap[mapIndex].mapGlobals.currentRGBA.b - mainMap[mapIndex].mapGlobals.targetRGBA.b;
        } else {
            temp = mainMap[mapIndex].mapGlobals.targetRGBA.b - mainMap[mapIndex].mapGlobals.currentRGBA.b;
        }

        mainMap[mapIndex].mapGlobals.deltaRGBA.b = (temp * absRate) / 255.0f;
        
        if (mainMap[mapIndex].mapGlobals.targetRGBA.a < mainMap[mapIndex].mapGlobals.currentRGBA.a) {
            temp = mainMap[mapIndex].mapGlobals.currentRGBA.a - mainMap[mapIndex].mapGlobals.targetRGBA.a;
        } else {
            temp = mainMap[mapIndex].mapGlobals.targetRGBA.a - mainMap[mapIndex].mapGlobals.currentRGBA.a;
        }

        mainMap[mapIndex].mapGlobals.deltaRGBA.a = (temp * absRate) / 255.0f;

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034A6C);

// unused
bool func_80034A6C(u16 mapIndex, s8 arg1, s8 arg2, s8 arg3, u8 arg4, f32 arg5, f32 arg6, f32 arg7) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 

        mainMap[mapIndex].mapCameraView.cameraTileX += arg1;

        mainMap[mapIndex].mapCameraView.frustumCorner0.y = 0;
        mainMap[mapIndex].mapCameraView.frustumCorner1.y = 0;
        mainMap[mapIndex].mapCameraView.frustumCorner2.y = 0;
        mainMap[mapIndex].mapCameraView.frustumCorner3.y = 0;
        
        mainMap[mapIndex].mapCameraView.cameraTileZ += arg2;
        mainMap[mapIndex].mapCameraView.viewExtentX += arg3;
        mainMap[mapIndex].mapCameraView.viewExtentZ += arg4;

        mainMap[mapIndex].mapCameraView.viewOffset.x += arg5;
        mainMap[mapIndex].mapCameraView.viewOffset.y += arg6;
        mainMap[mapIndex].mapCameraView.viewOffset.z += arg7;

        mainMap[mapIndex].mapCameraView.frustumCorner0.x += arg1;
        mainMap[mapIndex].mapCameraView.frustumCorner0.z += arg2;

        mainMap[mapIndex].mapCameraView.frustumCorner1.x += arg1;
        mainMap[mapIndex].mapCameraView.frustumCorner1.z += arg2;

        mainMap[mapIndex].mapCameraView.frustumCorner2.x += arg1;
        mainMap[mapIndex].mapCameraView.frustumCorner2.z += arg2;

        mainMap[mapIndex].mapCameraView.frustumCorner3.x += arg1;
        mainMap[mapIndex].mapCameraView.frustumCorner3.z += arg2;
        
        result = TRUE;
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapObject);

bool setMapObject(u16 mapIndex, u8 index, u16 spriteIndex, u16 animationIndex, f32 x, f32 y, f32 z, u8 arg7, u8 animationMode, bool arg9, bool argA) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
         
        mainMap[mapIndex].mapObjects[index].spriteIndex = spriteIndex; 
        mainMap[mapIndex].mapObjects[index].animationIndex = animationIndex;

        mainMap[mapIndex].mapObjects[index].coordinates.x = x;
        mainMap[mapIndex].mapObjects[index].coordinates.y = y;
        mainMap[mapIndex].mapObjects[index].coordinates.z = z;

        mainMap[mapIndex].mapObjects[index].unk_10 = arg7;
        mainMap[mapIndex].mapObjects[index].animationMode = animationMode;

        mainMap[mapIndex].mapObjects[index].flags |= 1;
        
        if (arg9) {
            mainMap[mapIndex].mapObjects[index].flags |= 4;
        }

        if (argA) {
            mainMap[mapIndex].mapObjects[index].flags |= 8;
        }

        result = TRUE;
        
        mainMap[mapIndex].mapState.mapObjectCount++;

    }
    
    return result;

}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", setMapWeatherSprite);

bool setMapWeatherSprite(u16 mapIndex, u8 index, u16 spriteIndex, u16 animationIndex) {

    bool result = FALSE;
 
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        mainMap[mapIndex].weatherSprites[index].spriteIndex = spriteIndex;
        mainMap[mapIndex].weatherSprites[index].animationIndex = animationIndex;
        mainMap[mapIndex].weatherSprites[index].flags = 1;

        result = TRUE;

    }

    return result; 
        
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034DC8);

bool func_80034DC8(u16 mapIndex, u8 index, u16 arg2) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 
        
        resetAnimationState(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
        mainMap[mapIndex].mapObjects[index].animationIndex = arg2;
        mainMap[mapIndex].mapObjects[index].flags &= ~2;

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", deactivateMapObject);

bool deactivateMapObject(u16 mapIndex, u8 index) {

    bool result = FALSE; 

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) { 

        if (mainMap[mapIndex].mapObjects[index].flags & 2) {
        
            deactivateSprite(mainMap[mapIndex].mapObjects[index].spriteIndex);
        
            mainMap[mapIndex].mapObjects[index].flags = 0;
        
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80034EF0);

// load and set texture for map spawnable sprite
// called by level.c
bool func_80034EF0(u16 mapIndex, u8 arg1, u8 arg2, u32* textureIndex, u32* paletteIndex, u8* spriteToPaletteIndex, u32 romTextureStart, u32 arg7, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u8 argA) {

    bool result = FALSE;
    
    u32 assetIndex[8];

    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        mainMap[mapIndex].groundObjects.textureIndex = textureIndex;
        mainMap[mapIndex].groundObjects.paletteIndex = paletteIndex;
        mainMap[mapIndex].groundObjects.spriteToPaletteIndex = spriteToPaletteIndex;

        // grid positions
        // D_801FD624 
        mainMap[mapIndex].groundObjects.x = arg1;
        // D_801C3F35
        mainMap[mapIndex].groundObjects.z = arg2;

        mainMap[mapIndex].groundObjects.unk_12 = argA;
        
        nuPiReadRom(romAssetsIndexStart, assetIndex, romAssetsIndexEnd - romAssetsIndexStart);
        
        offset1 = assetIndex[0];
        offset2 = assetIndex[1];
        offset3 = assetIndex[2];
        offset4 = assetIndex[3];
        offset5 = assetIndex[4];
        
        nuPiReadRom(romTextureStart + offset1, mainMap[mapIndex].groundObjects.textureIndex, offset2 - offset1);
        nuPiReadRom(romTextureStart + offset2, mainMap[mapIndex].groundObjects.paletteIndex, offset3 - offset2);
        nuPiReadRom(romTextureStart + offset4, mainMap[mapIndex].groundObjects.spriteToPaletteIndex, offset5 - offset4);
        
        result = TRUE;

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapGroundObjectSpriteIndex);

bool setMapGroundObjectSpriteIndex(u16 mapIndex, u16 spriteIndex, u8 x, u8 z) {
    
    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        // FIXME: likely a multidimensional array
        mainMap[mapIndex].groundObjects.gridToSpriteIndex[z * 0x14 + x] = spriteIndex;
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035054);

// used for maps with foragable items
bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
    
        if (bitmapIndex) {
            
            setBitmapFormat(&mainMap[mapIndex].groundObjectBitmaps[bitmapIndex], getTexturePtr(spriteIndex, mainMap[mapIndex].groundObjects.textureIndex), getPalettePtrType2(spriteIndex, mainMap[mapIndex].groundObjects.paletteIndex, mainMap[mapIndex].groundObjects.spriteToPaletteIndex));
              
            mainMap[mapIndex].groundObjectBitmaps[bitmapIndex].coordinates.x = x; 
            mainMap[mapIndex].groundObjectBitmaps[bitmapIndex].coordinates.y = y;
            mainMap[mapIndex].groundObjectBitmaps[bitmapIndex].coordinates.z = z;
            
            result = TRUE;

        }    
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTerrainHeightAtPosition);

// FIXME: inline function used before it's defined means it doesn't properly inline; also can't use `swap16TileIndex` directly
static inline u16 getTileIndexFromGrid_static_inline(u16 mapIndex, u8 x, u8 z) {
    return swap16TileIndex((&mainMap[mapIndex].mapGrid.gridToTileIndex[mainMap[mapIndex].mapGrid.mapWidth * z])[x]);
}

f32 getTerrainHeightAtPosition(u16 mapIndex, f32 x, f32 z) {
    
    Vec3f vec;
    TerrainQuad quad;

    f32 result;

    f32 worldX, worldZ, tileX, tileZ;
    f32 tileOriginZ;
    f32 xRemainder, zRemainder;
    
    u8 tileIndexX, tileIndexZ;
    u16 tileIndex;

    result = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        worldX = x + mainMap[mapIndex].mapState.mapOriginX;
        tileX = worldX / mainMap[mapIndex].mapGrid.tileSizeX;
        
        tileIndexX = (u8)tileX;
        
        worldZ = z + mainMap[mapIndex].mapState.mapOriginZ;
        tileZ = worldZ / mainMap[mapIndex].mapGrid.tileSizeZ;
        
        xRemainder = worldX - tileIndexX * mainMap[mapIndex].mapGrid.tileSizeX;
        
        tileIndexZ = (u8)tileZ; 
        
        tileOriginZ = tileIndexZ * mainMap[mapIndex].mapGrid.tileSizeZ;        

        tileIndex = getTileIndexFromGrid_static_inline(mapIndex, tileIndexX, tileIndexZ);
        
        zRemainder = worldZ - tileOriginZ;
        
        setTerrainQuad(&quad, mainMap[mapIndex].terrainQuads, tileIndex);
        
        vec = getTerrainHeight(&quad, xRemainder, zRemainder, mainMap[mapIndex].tiles[tileIndex].fallbackHeight);
        
        result = mainMap[mapIndex].tiles[tileIndex].yOffset + vec.y;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTerrainHeight);

Vec3f getTerrainHeight(TerrainQuad *quad, f32 x, f32 z, u8 fallbackHeight) {
  
    Plane planeEquation;
    
    u32 padding[6];
    u8 padding2[16];

    Vec3f heightResult;

    f32 height;

    heightResult.x = 0.0f;
    heightResult.y = 0.0f;
    heightResult.z = 0.0f;

    planeEquation = calculatePlaneEquation( 
        quad->triangle1[0][0], 
        quad->triangle1[0][1], 
        quad->triangle1[0][2], 
        quad->triangle1[1][0], 
        quad->triangle1[1][1], 
        quad->triangle1[1][2], 
        quad->triangle1[2][0], 
        quad->triangle1[2][1], 
        quad->triangle1[2][2]);

    height = getHeightFromPlane(x, z, planeEquation);

    if (isPointInTriangle(x, 
            height, 
            z, 
            quad->triangle1[0][0], 
            quad->triangle1[0][1], 
            quad->triangle1[0][2], 
            quad->triangle1[1][0], 
            quad->triangle1[1][1], 
            quad->triangle1[1][2], 
            quad->triangle1[2][0], 
            quad->triangle1[2][1], 
            quad->triangle1[2][2])) {

        if (height != 0.0f) {
            heightResult.y = height;
        } else {
            heightResult.y = fallbackHeight;            
        }

    } else {

        planeEquation = calculatePlaneEquation( 
            quad->triangle2[0][0], 
            quad->triangle2[0][1], 
            quad->triangle2[0][2], 
            quad->triangle2[1][0], 
            quad->triangle2[1][1], 
            quad->triangle2[1][2], 
            quad->triangle2[2][0], 
            quad->triangle2[2][1], 
            quad->triangle2[2][2]);

        height = getHeightFromPlane(x, z, planeEquation);
        
        if (isPointInTriangle(x, 
                height, 
                z, 
                quad->triangle2[0][0], 
                quad->triangle2[0][1], 
                quad->triangle2[0][2], 
                quad->triangle2[1][0], 
                quad->triangle2[1][1], 
                quad->triangle2[1][2], 
                quad->triangle2[2][0], 
                quad->triangle2[2][1], 
                quad->triangle2[2][2])) {
            
            if (height != 0.0f) {
                heightResult.y = height;
            } else {
                heightResult.y = fallbackHeight;
            }

        } else if (evaluatePlaneEquation(x, height, z, planeEquation) < 0.0f) {
            heightResult.y = quad->baseHeight;
        } else {
            heightResult.y = fallbackHeight;
        }

    }

    return heightResult;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", checkTileHasGroundObject);

bool checkTileHasGroundObject(u16 mapIndex, f32 x, f32 z) {

    Vec3f vec;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {

        vec = getMapGroundObjectCoordinates(0, x, z);

        if (vec.y != 65535.0f) {
            result = getMapGroundObjectSpriteIndex(MAIN_MAP_INDEX, vec.x, vec.z) != 0;
        }

    }
    
    return result;

}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", getTileCoordinates);

// alternate
/*
Vec3f* getTileCoordinates(Vec3f* arg0, MainMap* map, f32 x, f32 z) {
    
    Vec3f vec;

    vec.x = (x + map->mapState.mapOriginX) / map->mapGrid.tileSizeX;
    vec.y = 0;
    vec.z = (z + map->mapState.mapOriginZ) / map->mapGrid.tileSizeZ;

    *arg0 = vec;
    
    return arg0;
    
}
*/

Vec3f getTileCoordinates(MainMap* map, f32 x, f32 z) {
    
    Vec3f tileCoordinates;

    tileCoordinates.x = (x + map->mapState.mapOriginX) / map->mapGrid.tileSizeX;
    tileCoordinates.y = 0;
    tileCoordinates.z = (z + map->mapState.mapOriginZ) / map->mapGrid.tileSizeZ;

    return tileCoordinates;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", updateCameraViewBounds);

void updateCameraViewBounds(MapCameraView* mapCameraView) {

    Vec3f padding[3];
    
    Vec3f cornerOffset;
    Vec3f rotatedCenterOffset;

    cornerOffset.x = -mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = -mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->frustumCorner0, mapCameraView->rotation);

    cornerOffset.x = -mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->frustumCorner1, mapCameraView->rotation);

    cornerOffset.x = mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->frustumCorner2, mapCameraView->rotation);

    cornerOffset.x = mapCameraView->viewExtentX;
    cornerOffset.y = 0;
    cornerOffset.z = -mapCameraView->viewExtentZ;

    rotateVector3D(cornerOffset, &mapCameraView->frustumCorner3, mapCameraView->rotation);

    rotatedCenterOffset.x = 0.0f;
    rotatedCenterOffset.y = 0;
    rotatedCenterOffset.z = 0.0f;
    
    rotateVector3D(rotatedCenterOffset, &rotatedCenterOffset, mapCameraView->rotation);
    
    mapCameraView->frustumCorner0.x += mapCameraView->cameraTileX + rotatedCenterOffset.x;
    mapCameraView->frustumCorner0.y = 0;
    mapCameraView->frustumCorner0.z += mapCameraView->cameraTileZ + rotatedCenterOffset.z;

    mapCameraView->frustumCorner1.x += mapCameraView->cameraTileX + rotatedCenterOffset.x;
    mapCameraView->frustumCorner1.y = 0;
    mapCameraView->frustumCorner1.z += mapCameraView->cameraTileZ + rotatedCenterOffset.z;

    mapCameraView->frustumCorner2.x += mapCameraView->cameraTileX + rotatedCenterOffset.x;
    mapCameraView->frustumCorner2.y = 0;
    mapCameraView->frustumCorner2.z += mapCameraView->cameraTileZ + rotatedCenterOffset.z;
    
    mapCameraView->frustumCorner3.x += mapCameraView->cameraTileX + rotatedCenterOffset.x;
    mapCameraView->frustumCorner3.y = 0;
    mapCameraView->frustumCorner3.z += mapCameraView->cameraTileZ + rotatedCenterOffset.z;
    
    frustumEdgeCoefficient0 = mapCameraView->frustumCorner0.x - mapCameraView->frustumCorner1.x;
    frustumEdgeCoefficient1 = mapCameraView->frustumCorner1.x - mapCameraView->frustumCorner2.x;
    frustumEdgeCoefficient2 = mapCameraView->frustumCorner2.x - mapCameraView->frustumCorner3.x;
    frustumEdgeCoefficient3 = mapCameraView->frustumCorner3.x - mapCameraView->frustumCorner0.x;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", checkTileVisible);

bool checkTileVisible(MainMap *map, u8 x, u8 z) {
    
  bool result = FALSE;

  // FIXME: likely macros
  if (-1.0f <= (((map->mapCameraView.frustumCorner0.z * (map->mapCameraView.frustumCorner1.x - x)) + (map->mapCameraView.frustumCorner1.z * (x - map->mapCameraView.frustumCorner0.x))) + (z * frustumEdgeCoefficient0))) {
    if (-1.0f <= (((map->mapCameraView.frustumCorner1.z * (map->mapCameraView.frustumCorner2.x - x)) + (map->mapCameraView.frustumCorner2.z * (x - map->mapCameraView.frustumCorner1.x))) + (z * frustumEdgeCoefficient1))) {
      if (-1.0f <= (((map->mapCameraView.frustumCorner2.z * (map->mapCameraView.frustumCorner3.x - x)) + (map->mapCameraView.frustumCorner3.z * (x - map->mapCameraView.frustumCorner2.x))) + (z * frustumEdgeCoefficient2))) {
        if (-1.0f <= (((map->mapCameraView.frustumCorner3.z * (map->mapCameraView.frustumCorner0.x - x)) + (map->mapCameraView.frustumCorner0.z * (x - map->mapCameraView.frustumCorner3.x))) + (z * frustumEdgeCoefficient3))) {
          result = TRUE;
        }
      }
    }
  }
    
  return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80035EE0);

// unused
void func_80035EE0(MainMap* mainMap) {

    u8 i, j;
    u8 temp1, temp2, temp3, temp4;
    
    mainMap->mapCameraView.viewExtentX = 13;
    mainMap->mapCameraView.viewExtentZ = 13;

    for (i = 0; i < 38; i++) {

        for (j = 0; j < 42; j++) {
            mainMap->visibilityGrid[i][j] = 0;
        }
        
    }

    if (mainMap->mapCameraView.cameraTileX < mainMap->mapCameraView.viewExtentX) {
        temp1 = mainMap->mapCameraView.viewExtentX - mainMap->mapCameraView.cameraTileX;    
    } else {
        temp1 = 0;
    }

    if (mainMap->mapCameraView.cameraTileZ < mainMap->mapCameraView.viewExtentZ) {
        temp2 = mainMap->mapCameraView.viewExtentZ - mainMap->mapCameraView.cameraTileZ;
    } else {    
        temp2 = 0;
    }

    if ((mainMap->mapCameraView.cameraTileX + mainMap->mapCameraView.viewExtentX) >= mainMap->mapGrid.mapWidth) {
        temp3 = mainMap->mapCameraView.viewExtentX + (mainMap->mapGrid.mapWidth - mainMap->mapCameraView.cameraTileX);
    } else {
        temp3 = (mainMap->mapCameraView.viewExtentX * 2) | 1;
    } 

    if ((mainMap->mapCameraView.cameraTileZ + mainMap->mapCameraView.viewExtentZ) >= mainMap->mapGrid.mapHeight) {
        temp4 = mainMap->mapCameraView.viewExtentZ + (mainMap->mapGrid.mapHeight - mainMap->mapCameraView.cameraTileZ);
    } else {
        temp4 = (mainMap->mapCameraView.viewExtentZ * 2) | 1;
    } 

    for (i = temp2; i < temp4; i++) {
        
        for (j = temp1; j < temp3; j++) {
            mainMap->visibilityGrid[(mainMap->mapCameraView.cameraTileZ - mainMap->mapCameraView.viewExtentZ) + i][(mainMap->mapCameraView.cameraTileX - mainMap->mapCameraView.viewExtentX) + j] = D_80112D10[i][j];
        } 
    }
        
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapAdditionMetadata);

u8 setMapAdditionMetadata(u16 mapIndex, u16 mapAdditionIndex, u8 xCoord, u8 zCoord, u16 flag) {

    Swap16 swap;
    
    bool result = FALSE;
    
    u8 i, j;
    u8 height, width;
    u8 x, z;
    
    u16 tempIndex;
    u16 temp2;
    
    u8 *ptr;
    u8 *ptr2;

    if (mapIndex  == MAIN_MAP_INDEX) {

        if (mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

            ptr = getMapAdditionsMetadataPtrFromIndex(mapAdditionIndex, mainMap[mapIndex].mapAdditionsMetadataPtr);

            x = *ptr++;
            z = *ptr++;
            
            height = *ptr++;
            width = *ptr++;

            if (flag) {
                x = xCoord;
                z = zCoord;
            }

            // append new tiles and/or update tile indices on grid 
            for (i = 0; i < width; i++) {

                for (j = 0; j < height; j++) {

                    swap.byte[0] = *ptr++;
                    swap.byte[1] = *ptr++;

                    tempIndex = (mainMap[mapIndex].mapGrid.mapWidth * (z + i)) + x + j;
                    
                    mainMap[mapIndex].mapGrid.gridToTileIndex[tempIndex] = swap.halfword;

                }
                
            }

            for (i = 0; i < width; i++) {

                for (j = 0; j < height; j++) {

                    swap.byte[0] = *ptr++;
                    
                    ptr2 = mainMap[mapIndex].gridToLevelInteractionIndex + 1;

                    tempIndex = (mainMap[mapIndex].mapGrid.mapWidth * (z + i)) + x + j;

                    ptr2[tempIndex] = swap.byte[0];

                }
                
            }
            
            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getLevelInteractionIndexFromPosition);

// takes in entity coordinates and returns index for interactable object/exit from rodata array per level
u8 getLevelInteractionIndexFromPosition(u16 mapIndex, f32 x, f32 z) {

    Vec3f vec1;
    Vec3f vec2;
    u8 levelInteractionIndex = 0;
    u8* ptr;
    
    if (mapIndex == MAIN_MAP_INDEX) {
        
        if (mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {
        
            vec2.x = (x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX; 
            vec2.y = 0;
            vec2.z = (z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;

            vec1 = vec2;

            ptr = mainMap[mapIndex].gridToLevelInteractionIndex + 1; 

            levelInteractionIndex = (ptr + (mainMap[mapIndex].mapGrid.mapWidth * (u8)vec1.z))[(u8)vec1.x];
            
        }
        
    }

    return levelInteractionIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036490);

// unused
// manually set level interaction index on grid
bool func_80036490(u16 mapIndex, u8 levelInteractionIndex, f32 x, f32 z) {

    Vec3f vec1;
    Vec3f vec2;

    u8 *ptr;
    
    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        vec2.x = ((x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX);
        vec2.y = 0;
        vec2.z = ((z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ);

        vec1 = vec2;

        ptr = mainMap[mapIndex].gridToLevelInteractionIndex + 1;
        
        (ptr + (mainMap[mapIndex].mapGrid.mapWidth * (u8)vec1.z))[(u8)vec1.x] = levelInteractionIndex;

        result = TRUE;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", convertWorldToTileCoordinates);

Vec3f convertWorldToTileCoordinates(u16 mapIndex, f32 x, f32 z) {

    Vec3f tileCoordinates;
    Vec3f cameraTiles;

    tileCoordinates.x = 0.0f;
    tileCoordinates.z = 0.0f;
    tileCoordinates.y = 65535.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        cameraTiles.x = (x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX;
        cameraTiles.y = 0;
        cameraTiles.z = (z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;

        tileCoordinates = cameraTiles; 

    }
 
    return tileCoordinates;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", getMapGroundObjectCoordinates);

Vec3f getMapGroundObjectCoordinates(u16 mapIndex, f32 x, f32 z) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        vec2.x = (x + mainMap[mapIndex].mapState.mapOriginX) / mainMap[mapIndex].mapGrid.tileSizeX;
        vec2.y = 0;
        vec2.z = (z + mainMap[mapIndex].mapState.mapOriginZ) / mainMap[mapIndex].mapGrid.tileSizeZ;
        
        vec = vec2;
        
        if (mainMap[mapIndex].groundObjects.x < vec.x && mainMap[mapIndex].groundObjects.z < vec.z && vec.x < (mainMap[mapIndex].groundObjects.x + 0x14) && vec.z < (mainMap[mapIndex].groundObjects.z + 0x18)) {
            vec.x -= mainMap[mapIndex].groundObjects.x;
            vec.z -= mainMap[mapIndex].groundObjects.z;
        } else {
            vec.y = 65535.0f;
        }
    }

    return vec;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getMapGroundObjectSpriteIndex);

u16 getMapGroundObjectSpriteIndex(u16 mapIndex, f32 x, f32 z) {

    u32 padding[4];
    
    u16 result = 0;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        result = mainMap[mapIndex].groundObjects.gridToSpriteIndex[((u8)z * 20) + (u8)x];        
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapGroundObjectSpriteIndexFromFloat);

bool setMapGroundObjectSpriteIndexFromFloat(u16 mapIndex, u16 spriteIndex, f32 x, f32 z) {
    
    u32 padding[4];

    bool result = FALSE;
    
    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        // FIXME: array indexing
        mainMap[mapIndex].groundObjects.gridToSpriteIndex[(u8)z * 0x14 + (u8)x] = spriteIndex;    
        result = TRUE;
        
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", checkMapRGBADone);

bool checkMapRGBADone(u16 mapIndex) {

    bool result = FALSE;
     
    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        result = (mainMap[mapIndex].mapState.flags >> 3) & 1;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", initializeMesh);

void initializeMesh(MainMap* mainMap) {

    u16 i, j;
    
    u16 currentVtx;
    u16 triCount;

    u8* ptr;

    TileRenderingInfo tileRenderingInfo[32]; 

    currentVtx = 0;
    i = 0;
  
    if (mainMap->mapGrid.meshVtxCount) {
        
        do {

            ptr = setTileVertexData(&mainMap->tiles[i], getTileVtxPtrFromCount(i, mainMap->mesh));

            j = 0;
            
            while (TRUE) {

                ptr = func_800374C0(&tileRenderingInfo[j], ptr);
    
                if (tileRenderingInfo[j].flags & 0x80) {
                    mainMap->tiles[i].textureIndex |= 0x80;
                }

                if (tileRenderingInfo[j++].flags & 0x10) {
                    break;
                } 

            } 

            mainMap->tiles[i].renderingCommandsOffset = currentVtx;

            // set coloring and triangles
            triCount = buildTileRenderingCommands(&mainMap->tileRenderingCommands[currentVtx], mainMap, tileRenderingInfo);

            currentVtx += triCount;
            mainMap->tiles[i].triCount = triCount;
            
            i++;
            
        } while (i < mainMap->mapGrid.meshVtxCount);

    }  

} 

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80036C08);

// implements a linked list for tile textures --> grid positions to allow for batch renderings
// tiles are rendered by texture order, not by grid position or tile number
void func_80036C08(u16 mapIndex) {

    u16 padding[0x140];
    
    u8 i, j, k;
    u16 gridPosition;
    u16 tileIndex;
    u32 index;

    u32 temp;

    for (i = 0; i < MAX_TILE_TEXTURES + 1; i++) {

        previousGridPositionForTileTexture[i] = 0xFFFF;
        mainMap[mapIndex].textureToFirstGrid[i] = 0xFFFF;
        
    }

    j = 0;
    k = 0;
    gridPosition = 0;
    
    for (j = 0; j < mainMap[mapIndex].mapGrid.mapHeight; j++) {

        for (k = 0; k < mainMap[mapIndex].mapGrid.mapWidth; k++) {

            tileIndex = getTileIndexFromGrid(mapIndex, k, j);
            
            if (tileIndex != 0xFFFF) {

                if (!(mainMap[mapIndex].tiles[tileIndex].textureIndex & 0x80)) {
                    index = MAX_TILE_TEXTURES;
                } else {
                    index = mainMap[mapIndex].tiles[tileIndex].textureIndex & 0x7F;
                }

                // FIXME: dead code
                if (gridPosition) {
                    temp = index & 0xFF;
                } else {
                    temp = index & 0xFF;
                }

                // FIXME: dead code
                if (previousGridPositionForTileTexture[temp] == 0xFFFF) {
                    __asm__ __volatile__("" : : : "memory");
                    mainMap[mapIndex].textureToFirstGrid[temp] = gridPosition;
                } else {
                    __asm__ __volatile__("" : : : "memory");
                    mainMap[mapIndex].nextGridPositionWithSharedTile[previousGridPositionForTileTexture[temp]] = gridPosition;
                }
                
                // FIXME: dead code
                if (gridPosition) {
                    temp = index & 0xFF;
                } else {
                    temp = index & 0xFF;
                }

                previousGridPositionForTileTexture[temp] = gridPosition;
                
            }
            
            gridPosition++;
            
        }
        
    }

    for (i = 0; i < MAX_TILE_TEXTURES + 1; i++) {
        
        if (previousGridPositionForTileTexture[i] != 0xFFFF) {
            
            // FIXME: dead code
            __asm__ __volatile__("" : : : "memory");
            mainMap[mapIndex].nextGridPositionWithSharedTile[previousGridPositionForTileTexture[i]] = 0xFFFF;

        } 
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setGroundObjects);

void setGroundObjects(u16 mapIndex) {

    u16 tempArr[0x40];
    
    u16 i, j;
    u8 k, m;
    u16 l;
    
    u16 temp;

    for (i = 0; i < MAX_GROUND_OBJECTS; i++) {
        mainMap[mapIndex].groundObjects.spriteIndexToGrid[i] = 0xFFFF;
    }

    for (j = 0; j < 0x1E0; j++) {
        mainMap[mapIndex].groundObjects.previousGridToSpriteIndex[j] = 0;
        mainMap[mapIndex].groundObjects.nextGridToSpriteIndex[j] = 0;
    }

    for (k = 0; k < MAX_GROUND_OBJECTS; k++) {
        mainMap[mapIndex].groundObjects.spriteIndexToGrid[k] = 0xFFFF;
        tempArr[k] = 0xFFFF;
    }
    
    for (l = 0; l < 0x1E0; l++) {

        if (mainMap[mapIndex].groundObjects.gridToSpriteIndex[l] && mainMap[mapIndex].groundObjects.gridToSpriteIndex[l] != 0xFFFF) {

            if (mainMap[mapIndex].groundObjects.spriteIndexToGrid[mainMap[mapIndex].groundObjects.gridToSpriteIndex[l]] == 0xFFFF) {

                mainMap[mapIndex].groundObjects.spriteIndexToGrid[mainMap[mapIndex].groundObjects.gridToSpriteIndex[l]] = l;
                tempArr[mainMap[mapIndex].groundObjects.gridToSpriteIndex[l]] = l;
                mainMap[mapIndex].groundObjects.previousGridToSpriteIndex[l] = 0xFFFF;
                
            } else {

                temp = tempArr[mainMap[mapIndex].groundObjects.gridToSpriteIndex[l]];
                
                tempArr[mainMap[mapIndex].groundObjects.gridToSpriteIndex[l]] = l;
                
                mainMap[mapIndex].groundObjects.nextGridToSpriteIndex[temp] = l;
                mainMap[mapIndex].groundObjects.previousGridToSpriteIndex[l] = temp;

            }
            
        }
        
    }

    for (m = 0; m < MAX_GROUND_OBJECTS; m++) {

        if (tempArr[m] != 0xFFFF) {
            mainMap[mapIndex].groundObjects.nextGridToSpriteIndex[tempArr[m]] = 0xFFFF;
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/map", getTileIndexFromGrid);

inline u16 getTileIndexFromGrid(u16 mapIndex, u8 x, u8 z) {
    return swap16TileIndex((&mainMap[mapIndex].mapGrid.gridToTileIndex[mainMap[mapIndex].mapGrid.mapWidth * z])[x]);
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapGrid);

void setMapGrid(MapGrid* mapGrid, u8* data) {

    Swap16 swap;
    
    mapGrid->tileSizeX = data[4];
    mapGrid->tileSizeZ = data[5];
    mapGrid->mapWidth = data[6];
    mapGrid->mapHeight = data[7];

    swap.byte[1] = data[8];
    swap.byte[0] = data[9];
    
    mapGrid->unused = swap.halfword;

    swap.byte[1] = data[10];
    swap.byte[0] = data[11];
        
    mapGrid->meshVtxCount = swap.halfword;

    // set ptr
    mapGrid->gridToTileIndex = data + 12;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037350);

// unused
u8* func_80037350(TileRenderingInfo* tileRenderingInfo, u8* data) {

    // arg1 = &*(arg1+4);
    // skip header
    data = &data[4];
    
    tileRenderingInfo->colors[0][0] = *data++;
    tileRenderingInfo->colors[0][1] = *data++;
    tileRenderingInfo->colors[0][2] = *data++;
    tileRenderingInfo->colors[0][3] = *data++;

    return data;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037388);

// unused or inline
u8* func_80037388(TileRenderingInfo* tileRenderingInfo, u8* data, u8 arg2) {
    
    Swap16 swap;

    tileRenderingInfo->vtx[0] = *data++;
    tileRenderingInfo->colors[0][0] = *data++;
    tileRenderingInfo->colors[1][0] = *data++;

    swap.byte[1] = *data++;
    swap.byte[0] = *data++;
    
    tileRenderingInfo->triangle1Bitfield = swap.halfword;

    if (arg2) {

        swap.byte[1] = *data++;
        swap.byte[0] = *data++;
        
        tileRenderingInfo->triangle2Bitfield = swap.halfword;
        
    }
    
    return data;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80037400);

inline u8* func_80037400(TileRenderingInfo* tileRenderingInfo, u8* data, bool flag) {
    
    Swap16 swap;
    u8 i;
        
    for (i = 0; i < 3; i++) {
        tileRenderingInfo->vtx[i] = *data++;
        tileRenderingInfo->colors[0][i] = *data++;
        tileRenderingInfo->colors[1][i] = *data++;
    }

    if (flag) {
        tileRenderingInfo->vtx[3] = *data++;
        tileRenderingInfo->colors[0][3] = *data++;
        tileRenderingInfo->colors[1][3] = *data++;
    }
    
    swap.byte[1] = *data++;
    swap.byte[0] = *data++;

    tileRenderingInfo->triangle1Bitfield = swap.halfword;

    if (flag) {

        swap.byte[1] = *data++;
        swap.byte[0] = *data++;

        tileRenderingInfo->triangle2Bitfield = swap.halfword;
        
    }
    
    return data;
    
}

static inline u8* getTriangle2Bitfield(TileRenderingInfo* tileRenderingInfo, u8* data, u8 flag) {

    u16 temp;
    u8 temp1, temp2;
    
    if (flag) {

        temp1 = data[1];
        temp2 = data[0];

        temp = temp1 << 8 | temp2;

        tileRenderingInfo->triangle2Bitfield = temp;
        
        // skip end padding
        data += 2;
        
    }
    
    return data;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800374C0);

u8* func_800374C0(TileRenderingInfo* tileRenderingInfo, u8* data) {

    Swap16 swap;
    u32 padding[2];
    
    u8 i;
    u8 flags;
    
    tileRenderingInfo->flags = *data++;
    
    if (tileRenderingInfo->flags & 0x20) {
        tileRenderingInfo->unused1 = *data++;
        tileRenderingInfo->unused2 = *data++;
    }

    flags = tileRenderingInfo->flags;

    if (tileRenderingInfo->flags & 0x80) {

        data = func_80037400(tileRenderingInfo, data, flags & 0x40);
        
    } else {

        tileRenderingInfo->vtx[0] = data[0];
        tileRenderingInfo->colors[0][0] = data[1];
        tileRenderingInfo->colors[1][0] = data[2];

        swap.byte[1] = data[3];
        swap.byte[0] = data[4];

        tileRenderingInfo->triangle1Bitfield = swap.halfword;
        
        data = getTriangle2Bitfield(tileRenderingInfo, data + 5, flags & 0x40);
                
    }

    return data;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/map", setTileVertexData);

u8* setTileVertexData(Tile* tile, u8* data) {

    u8 *ptr;
    
    tile->yOffset = data[4];
    tile->fallbackHeight = data[5];
    tile->textureIndex = data[6];
    tile->verticesPerTile = data[7];

    data += 8;
    ptr = data;
    
    tile->coordinates = ptr;
    
    return ptr + tile->verticesPerTile * 3; 
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/map", getTileVtxPtrFromCount);

u16* getTileVtxPtrFromCount(u16 count, void* vtxDataPtr) {

    u32* ptr = (u32*)vtxDataPtr;
    u32 offset = ptr[count];
    
    return (u16*)(vtxDataPtr + offset);

}

// alternate for reference
/*
s32 getTileVtxPtrFromCount(u16 arg0, s32 arg1) {
    return arg1 + *(s32*)(arg0*4 + arg1);
}
*/

// alternate
/*
u16* getTileVtxPtrFromCount(u16 arg0, void *arg1) {
    return (u16*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/map", setTileVertices);

u32 setTileVertices(MainMap* mainMap, u16 tileIndex, f32 x, f32 y, f32 z) {

    u8 count = 0;
    u16 vtxNumber;
    s8* vtx;

    vtx = mainMap->tiles[tileIndex].coordinates;

    vtxNumber = mainMap->mapState.startingVertex + mainMap->mapState.renderedVertexCount; 
    
    if (mainMap->tiles[tileIndex].verticesPerTile) {
        
        do {
            
            tileVertices[gGraphicsBufferIndex][vtxNumber].v.ob[0] = *vtx + x;
            vtx++;
 
            tileVertices[gGraphicsBufferIndex][vtxNumber].v.ob[1] = *(u8*)vtx + y;
            vtx++;

            tileVertices[gGraphicsBufferIndex][vtxNumber].v.ob[2] = *vtx + z;
            vtx++;

            tileVertices[gGraphicsBufferIndex][vtxNumber].v.cn[0] = mainMap->mapGlobals.currentRGBA.r;
            tileVertices[gGraphicsBufferIndex][vtxNumber].v.cn[1] = mainMap->mapGlobals.currentRGBA.g;
            tileVertices[gGraphicsBufferIndex][vtxNumber].v.cn[2] = mainMap->mapGlobals.currentRGBA.b;
            tileVertices[gGraphicsBufferIndex][vtxNumber].v.cn[3] = mainMap->mapGlobals.currentRGBA.a;
            
            vtxNumber++;
            count++;
            
        } while (count < mainMap->tiles[tileIndex].verticesPerTile);

    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", prepareTileTextures);

Gfx* prepareTileTextures(Gfx* dl, MainMap* mainMap, u8 textureIndex) {

    TileBitmap bitmap;

    setBitmapFormat((BitmapObject*)&bitmap, getTexturePtr(textureIndex, mainMap->tileTextures), getPalettePtrType1(textureIndex, mainMap->tilePalettes));

    gDPLoadTextureTile_4b(dl++, bitmap.timg, bitmap.fmt, bitmap.width, bitmap.height, 0, 0, bitmap.width - 1, bitmap.height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(dl++, 0, bitmap.pal);

    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", renderTiles);

Gfx* renderTiles(Gfx* dl, MainMap* mainMap, u16 gridIndex, u8 textureIndex) {

    Swap16 swap;
    
    u16 temp;
    u16 tileIndex;
    
    do {

        if (checkTileVisible(mainMap, gridPositionToX[gridIndex], gridPositionToZ[gridIndex])) {

            mainMap->visibilityGrid[gridPositionToZ[gridIndex]][gridPositionToX[gridIndex]] = TRUE;
            
            // only load texture once for every tile that shares the texture
            if (textureIndex != MAX_TILE_TEXTURES) {
                dl = prepareTileTextures(dl, mainMap, textureIndex);
                textureIndex = MAX_TILE_TEXTURES;
            }

            // FIXME: should be inline swap16TileIndex?
            swap.byte[1] = mainMap->mapGrid.gridToTileIndex[gridIndex] >> 8;
            swap.byte[0] = mainMap->mapGrid.gridToTileIndex[gridIndex];
        
            temp = swap.halfword;

            tileIndex = temp - 1;
            
            dl = appendTileToDL(dl, 
                    mainMap,
                    tileIndex, 
                    (gridPositionToX[gridIndex] - mainMap->mapCameraView.cameraTileX) * mainMap->mapGrid.tileSizeX,
                    mainMap->tiles[tileIndex].yOffset,
                    (gridPositionToZ[gridIndex] - mainMap->mapCameraView.cameraTileZ) * mainMap->mapGrid.tileSizeZ
                );
        
        } else {
            mainMap->visibilityGrid[gridPositionToZ[gridIndex]][gridPositionToX[gridIndex]] = FALSE;
        }

        // get next grid position that shares the tile
        gridIndex = mainMap->nextGridPositionWithSharedTile[gridIndex];
        
    } while (gridIndex != 0xFFFF);

    return dl;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/map", buildMapDisplayList);

inline Gfx* buildMapDisplayList(Gfx* dl, MainMap* mainMap, u16 startingVertex) {

    u8 i;
    u16 gridIndex;
    
    mainMap->mapState.renderedVertexCount = 0;
    mainMap->mapState.startingVertex = startingVertex;

    // gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)
    *dl++ = D_8011ED68; 
    // gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2)
    *dl++ = D_8011ED70;
    // gsDPSetTextureFilter(G_TF_BILERP)
    *dl++ = D_8011ED78; 

    // loop through all possible tile texture slots and for each iteration, process every tile that shares the texture of the given tile 
    for (i = 0; i < MAX_TILE_TEXTURES + 1; i++) {
  
        gridIndex = mainMap->textureToFirstGrid[i];
        
        // if this texture is used
        if (gridIndex != 0xFFFF) {
            dl = renderTiles(dl, mainMap, gridIndex, i);
        }

    } 

    // gsSPEndDisplayList()
    *dl++ = D_8011ED80;

    mainMap->mapState.totalVertexCount = mainMap->mapState.renderedVertexCount;
    
    return dl;
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/map", buildTileRenderingCommands);

// setup modify vertex and triangle rendering commands to append to main display list
u16 buildTileRenderingCommands(Gfx* dl, MainMap* mainMap, TileRenderingInfo tileRenderingInfo[]) {
    
    u16 i = 0;
    u16 count = 0;
    
    Gfx dl2[2];
    Gfx *tempDl;
    
    u8 flags;
    
    do {

        if (tileRenderingInfo[i].flags & 0x80) {

            count++;

            // gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON)
            *dl++ = D_8011ED88;
            // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)
            *dl++ = D_8011ED90;
            
            gSPModifyVertex(&dl2[1], tileRenderingInfo[i].vtx[0], G_MWO_POINT_ST, (tileRenderingInfo[i].colors[0][0] << 0x16) | (tileRenderingInfo[i].colors[1][0] << 6));

            dl2[0] = dl2[1];
            *dl++ = dl2[0];
            
            gSPModifyVertex(&dl2[1], tileRenderingInfo[i].vtx[1], G_MWO_POINT_ST, (tileRenderingInfo[i].colors[0][1] << 0x16) | (tileRenderingInfo[i].colors[1][1] << 6));

            dl2[0] = dl2[1];
            *dl++ = dl2[0];
            
            gSPModifyVertex(&dl2[1], tileRenderingInfo[i].vtx[2], G_MWO_POINT_ST, (tileRenderingInfo[i].colors[0][2] << 0x16) | (tileRenderingInfo[i].colors[1][2] << 6));

            dl2[0] = dl2[1];
            *dl++ = dl2[0];
            
            count += 4;
            
            if (tileRenderingInfo[i].flags & 0x40) {
                
                count++;

                gSPModifyVertex(&dl2[1], tileRenderingInfo[i].vtx[3], G_MWO_POINT_ST, (tileRenderingInfo[i].colors[0][3] << 0x16) | (tileRenderingInfo[i].colors[1][3] << 6));

                dl2[0] = dl2[1];
                *dl++ = dl2[0];
                
            } 
        
        } else {
            
            count += 3;
            
            // gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF)
            *dl++ = D_8011ED98;
            // gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0)
            *dl++ = D_8011EDA0;
    
            // FIXME
            asm("");
    
            // tileRenderingInfo[i].vtx[0] being used as r color value?
            gDPSetPrimColor(&dl2[1], 0, 0, tileRenderingInfo[i].vtx[0], tileRenderingInfo[i].colors[0][0], tileRenderingInfo[i].colors[1][0], 255);
            
            dl2[0] = dl2[1];
            *dl++ = dl2[0];
            
        } 
        
        if (tileRenderingInfo[i].flags & 0x40) {
            
            // FIXME: no idea what's going on here
            // have to manually use the stack DL with the triangle macros
            __asm__ __volatile__("" : : : "memory");
            
            {
                
                tempDl = dl++;
                dl2[1].words.w0 = (((unsigned int) (((unsigned int)(0x06) & ((0x01 << (8)) - 1)) << (24)))| ((((tileRenderingInfo[i].flags & 0xC) >> 2) == 0) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (0)))): (((tileRenderingInfo[i].flags & 0xC) >> 2) == 1) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (0)))): (((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (0))))));
                dl2[1].words.w1 = (((tileRenderingInfo[i].flags & 3) == 0) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle2Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (0)))): ((tileRenderingInfo[i].flags & 3) == 1) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle2Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (0)))): (((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle2Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle2Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (0))))); 
            
            };
            
            // should be:
            /*
            gSP2Triangles(dl++, 
                (tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10, 
                (tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5, 
                tileRenderingInfo[i].triangle1Bitfield & 0x1F, 
                (tileRenderingInfo[i].flags & 0xC) >> 2,  
                (tileRenderingInfo[i].triangle2Bitfield & 0x7C00) >> 10,
                (tileRenderingInfo[i].triangle2Bitfield & 0x3E0) >> 5,
                tileRenderingInfo[i].triangle2Bitfield & 0x1F,
                tileRenderingInfo[i].flags & 3);
            */

        } else {
            
             {  
                 
                tempDl = dl++;
                dl2[1].words.w0 = ((unsigned int) (((unsigned int)(0x05) & ((0x01 << (8)) - 1)) << (24)))| ((((tileRenderingInfo[i].flags & 0xC) >> 2) == 0) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (0)))): (((tileRenderingInfo[i].flags & 0xC) >> 2) == 1) ? (((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (0)))): (((unsigned int) (((unsigned int)((tileRenderingInfo[i].triangle1Bitfield & 0x1F)*2) & ((0x01 << (8)) - 1)) << (16)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10)*2) & ((0x01 << (8)) - 1)) << (8)))|((unsigned int) (((unsigned int)(((tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5)*2) & ((0x01 << (8)) - 1)) << (0)))));
                dl2[1].words.w1 = 0; 
                
             };

             // should be:
             /*
            gSP1Triangle(dl++, 
                (tileRenderingInfo[i].triangle1Bitfield & 0x7C00) >> 10, 
                (tileRenderingInfo[i].triangle1Bitfield & 0x3E0) >> 5, 
                tileRenderingInfo[i].triangle1Bitfield & 0x1F, 
                (tileRenderingInfo[i].flags & 0xC) >> 2);
                
             */
            
        }

        dl2[0] = dl2[1];
        *tempDl = dl2[0];

        // FIXME: dead code, I guess
        if (count) {
            count++;
        } else {
            count++;
        }
        
        *dl++ = D_8011EDA8;

        count++;

    } while (!(tileRenderingInfo[i++].flags & 0x10));

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", appendTileToDL);

Gfx* appendTileToDL(Gfx* dl, MainMap* mainMap, u16 tileIndex, f32 x, f32 y, f32 z) {

    u16 i;
    
    u32 count;
    
    Gfx* tempDl;
    Gfx tempDl2;

    // FIXME: shouldn't be necessary
    f32 temp;
    u32 padding[2];

    temp = y;

    // FIXME:
    // ??
    count = setTileVertices(mainMap, tileIndex, x, *(f32*)&y, z);

    // FIXME: might be a wrapper around gSPVertex
    gSPVertex(&tempDl2 + 1, &tileVertices[gGraphicsBufferIndex][mainMap->mapState.startingVertex + mainMap->mapState.renderedVertexCount], mainMap->tiles[tileIndex].verticesPerTile, 0);

    tempDl2 = *(&tempDl2 + 1);
    *dl++ = tempDl2;
    
    mainMap->mapState.renderedVertexCount += count;

    // get and appends modify vertex and triangle commands
    tempDl = &mainMap->tileRenderingCommands[mainMap->tiles[tileIndex].renderingCommandsOffset];
    
    for (i = 0; i < mainMap->tiles[tileIndex].triCount; i++) {
        *dl++ = *tempDl++;
    }

    return dl++;
    
}

void func_80038514(void) {}

//INCLUDE_ASM("asm/nonmatchings/system/map", setCoreMapObjects);

void setCoreMapObjects(MainMap* mainMap) {

    u16 i, j;
    u8 temp;
    u16 count = 0;
    u8 *coreMapObjectDataPtr;
    
    count = 0;
    i = 0;
    
    // skip header
    mainMap->mapState.coreMapObjectsCount = *(mainMap->coreMapObjects + 4);
    
    while (i < mainMap->mapState.coreMapObjectsCount) {

        // skip header
        coreMapObjectDataPtr = mainMap->coreMapObjects + 4;
        coreMapObjectDataPtr = getCoreMapObjectPtrFromCount(i, coreMapObjectDataPtr);

        setCoreMapObjectsMetadata(&mainMap->coreMapObjectsMetadata[i], coreMapObjectDataPtr);

        mainMap->coreMapObjectsMetadata[i].unk_0 = count;
        j = 0;
        
        coreMapObjectDataPtr += 2;
        
        while (j < mainMap->coreMapObjectsMetadata[i].totalCoreMapObjects) {

            setCoreMapObject(&coreMapObjects[count], coreMapObjectDataPtr);
            
            coreMapObjectDataPtr += 7;
            count++;
            j++;

        }
        
        i++;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setCoreMapObject);

void setCoreMapObject(CoreMapObject* coreMapObject, u8* data) {

    // FIXME: shouldn't be necessary/likely a union for byte-swapping
    u32 padding[8];
    
    coreMapObject->flags = data[0];
    
    coreMapObject->coordinates.x = (s16)(data[1] | (data[2] << 8));
    coreMapObject->coordinates.y = (s16)(data[3] | (data[4] << 8));
    coreMapObject->coordinates.z = (s16)(data[5] | (data[6] << 8));

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setCoreMapObjectsMetadata);

void setCoreMapObjectsMetadata(CoreMapObjectsMetadata* metadata, u8* data) {
    metadata->spriteIndex = data[0];
    metadata->totalCoreMapObjects = data[1];
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getCoreMapObjectPtrFromCount);

u8* getCoreMapObjectPtrFromCount(u16 objectCount, u8* ptr) {

    u16 i;
    u8 arr[4];

    u8 temp;

    ptr++;

    for (i = 0; i < objectCount; i++) {
        
        arr[2] = ptr[0];
        temp = ptr[1];
        
        ptr += 2;
        ptr += (temp *  7);
        
        arr[3] = temp;
   
    }

    return ptr;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setTerrainQuad);
 
void setTerrainQuad(TerrainQuad* quad, u8* quadPtr, u16 tileIndex) {

    u8* arr;

    // skip header
    arr = quadPtr + 4;

    arr += tileIndex * sizeof(TerrainQuad);

    quad->baseHeight = arr[0];

    quad->triangle1[0][0] = arr[1];
    quad->triangle1[0][1] = arr[2];
    quad->triangle1[0][2] = arr[3];
    
    quad->triangle1[1][0] = arr[4];
    quad->triangle1[1][1] = arr[5];
    quad->triangle1[1][2] = arr[6];
    
    quad->triangle1[2][0] = arr[7];
    quad->triangle1[2][1] = arr[8];
    quad->triangle1[2][2] = arr[9];

    quad->triangle2[0][0] = arr[10];
    quad->triangle2[0][1] = arr[11];
    quad->triangle2[0][2] = arr[12];
    
    quad->triangle2[1][0] = arr[13];
    quad->triangle2[1][1] = arr[14];
    quad->triangle2[1][2] = arr[15];
    
    quad->triangle2[2][0] = arr[16];
    quad->triangle2[2][1] = arr[17];
    quad->triangle2[2][2] = arr[18];
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800387E0);

u32* func_800387E0(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_800387F8);

u32* func_800387F8(u16 arg0, u32* arg1) {
    return (u8*)arg1 + arg1[arg0];
}

//INCLUDE_ASM("asm/nonmatchings/system/map", updateGroundObjects);

bool updateGroundObjects(u16 mapIndex) {

    bool result;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        
        if (!(mainMap[mapIndex].mapState.flags & 2)) {
            mainMap[mapIndex].mapState.flags |= 2;
            setGroundObjects(MAIN_MAP_INDEX);
        }

    }

    // never initialized; whoops
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003886C);

bool func_8003886C(u16 mapIndex) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        mainMap[mapIndex].mapState.flags &= ~2;
        result = TRUE;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", getMapAdditionsMetadataPtrFromIndex);

u8* getMapAdditionsMetadataPtrFromIndex(u16 mapAdditionIndex, u8 *ptr) {

    u16 i;
    u32 size;
    u32 temp;
    
    // skip header
    ptr += 6;

    i = 0;

    while (i < mapAdditionIndex) {
            
        ptr += 2;
    
        size = *ptr++;

        temp = *ptr++;
        temp *= size;

        ptr += (temp * 2);
        
        i++;

        ptr += temp;
            
    } 

    return ptr;
    
}

// alternate 
/*
u8* getMapAdditionsMetadataPtrFromIndex(u16 arg0, u8 *arg1) {
    
    u16 i;

    u32 temp1;
    u32 temp2;
    
    arg1 += 6;
    
    i = 0;
    
    if (arg0) {

        do {
            
            arg1 += 2;
            
            temp1 = *arg1;
            
            arg1++;
            
            temp2 = *arg1;
            temp2 = temp2 * temp1;

            arg1++;
            
            arg1 += temp2 * 2;
            arg1 += temp2;

            i++;
            
        } while (i < arg0);
    }
    
    return arg1;

}
*/

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038900);

// called by func_800735FC
bool func_80038900(u16 mapIndex, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {
        if (arg1 < 0x20 && !(mainMap[mapIndex].mapAdditions[arg1].flags & 1)) {
            if (arg2 < 0x10) {
                mainMap[mapIndex].mapAdditions[arg1].arr1[arg2] = arg3;
                mainMap[mapIndex].mapAdditions[arg1].arr2[arg2] = arg4;
                result = TRUE;
            }
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038990);

bool func_80038990(u16 mapIndex, u16 mapAdditionIndex, u8 arg2) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE && mapAdditionIndex < 0x20) {

        if (!(mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags & 1)) {

            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 1;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_42 = 0;

            result = TRUE;

            if (arg2) {
                mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 1 | 4;
            }
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80038A2C);

bool func_80038A2C(u16 mapIndex, u16 mapAdditionIndex, u8 x, u8 z) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE && mapAdditionIndex < 0x20) {

        if (!(mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags & 1)) {
            
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_40 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].unk_42 = 0;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].x = x;
            mainMap[mapIndex].mapAdditions[mapAdditionIndex].z = z;

            mainMap[mapIndex].mapAdditions[mapAdditionIndex].flags |= 3;
            
            result = TRUE;

        }
        
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapAddition);

bool setMapAddition(u16 mapIndex, u16 mapAdditionIndex) {

    bool result = FALSE;

    if (mapIndex == MAIN_MAP_INDEX && mainMap[mapIndex].mapState.flags & MAP_ACTIVE) {

        setMapAdditionMetadata(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], mainMap[mapIndex].mapAdditions[mapAdditionIndex].x, mainMap[mapIndex].mapAdditions[mapAdditionIndex].z, 0);
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setMapAdditionIndexFromCoordinates);

bool setMapAdditionIndexFromCoordinates(u16 mapIndex, u16 mapAdditionIndex, u8 x, u8 z) {
    
    bool result  = 0;

    if (mapIndex == MAIN_MAP_INDEX && (mainMap[mapIndex].mapState.flags & MAP_ACTIVE)) {
        setMapAdditionMetadata(MAIN_MAP_INDEX, mainMap[mapIndex].mapAdditions[mapAdditionIndex].arr1[0], x , z, 2);
        result = TRUE;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setupCoreMapObjectSprites);

static inline u8* getTexturePtrInline(u8 spriteIndex, u32* textureIndex) {
    return (u8*)textureIndex + textureIndex[spriteIndex];
}

static inline u8 *getPalettePtrTypeInline(u8 index, u32 *paletteIndex) {
  return (u8*)paletteIndex + paletteIndex[index];
}

void setupCoreMapObjectSprites(MainMap* mainMap) {
    
    Vec3f vec;
    Vec3f vec2;

    MainMap* mapPtr;
    
    f32 coordinateX, coordinateY, coordinateZ;
    f32 rotationX, rotationY, rotationZ;
    f32 scaleX, scaleY;
    f32 xPosition, yPosition, zPosition;
    f32 tileOffsetX, tileOffsetZ;
    f32 cameraWorldX, cameraWorldZ;
    f32 mapWorldCenterX, mapWorldCenterZ;

    u16 bitmapIndex;
    u16 spriteIndex;    
    u32* textureIndexPtr;
    u32* paletteIndexPtr;
    u8* texturePtr;
    u16* palettePtr;
    u8 total;

    u16 i, j, k;

    i = 0;

    while (i < mainMap->mapState.coreMapObjectsCount) {

        j = 0;
        k = mainMap->coreMapObjectsMetadata[i].unk_0;

        total = mainMap->coreMapObjectsMetadata[i].totalCoreMapObjects;  
        
        while (j < total) {

            coordinateX = coreMapObjects[k].coordinates.x;
            
            coordinateY = coreMapObjects[k].coordinates.y;
            yPosition = coordinateY;
        
            mapWorldCenterX = (mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2;
            mapWorldCenterZ = (mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2;

            coordinateZ = coreMapObjects[k].coordinates.z;
            
            tileOffsetX = mainMap->mapGrid.tileSizeX / 2;
            tileOffsetZ = mainMap->mapGrid.tileSizeZ / 2;

            cameraWorldX = mainMap->mapCameraView.cameraTileX * mainMap->mapGrid.tileSizeX;
            cameraWorldZ = mainMap->mapCameraView.cameraTileZ * mainMap->mapGrid.tileSizeZ;

            xPosition = (coordinateX + mapWorldCenterX) - tileOffsetX;
            zPosition = (coordinateZ + mapWorldCenterZ) - tileOffsetZ;
            
            xPosition -= cameraWorldX;
            zPosition -= cameraWorldZ;

            xPosition += mainMap->mapCameraView.viewOffset.x;
            yPosition += mainMap->mapCameraView.viewOffset.y;
            zPosition += mainMap->mapCameraView.viewOffset.z;

            vec2.x = ((coordinateX - tileOffsetX) + mainMap->mapState.mapOriginX) / mainMap->mapGrid.tileSizeX;
            vec2.y = 0;
            vec2.z = ((coordinateZ - tileOffsetZ) + mainMap->mapState.mapOriginZ) / mainMap->mapGrid.tileSizeZ;
            
            vec = vec2;

            if (mainMap->visibilityGrid[(u8)vec.z][(u8)vec.x]) {

                texturePtr = getTexturePtrInline(mainMap->coreMapObjectsMetadata[i].spriteIndex, mainMap->coreMapObjectsTextures);
                palettePtr = getPalettePtrTypeInline(mainMap->coreMapObjectsMetadata[i].spriteIndex, mainMap->coreMapObjectsPalettes);

                switch (coreMapObjects[k].flags & 0xC) {

                    case 0:
                        scaleY = 1.0f;
                        scaleX = scaleY;
                        break;
                    
                    case 4:
                        scaleY = 2.0f;
                        scaleX = scaleY;
                        break;

                    case 8:
                        scaleY = 4.0f;
                        scaleX = scaleY;
                        break;

                    case 0xC:
                        scaleY = 8.0f;
                        scaleX = scaleY;
                        break;
                    
                }

                if (coreMapObjects[k].flags & 0x80) {

                    rotationX = mainMap->mapGlobals.rotation.x;
                    rotationY = mainMap->mapGlobals.rotation.y;
                    rotationZ = mainMap->mapGlobals.rotation.z;

                    switch (coreMapObjects[k].flags & 0x70) {

                        case 0x70:
                            rotationY = (s32)(rotationY + 45.0f) % 360;
                            break;
                        case 0x10:
                            rotationY = (s32)(rotationY + 315.0f) % 360;
                            break;
                        case 0x20:
                            rotationY = (s32)(rotationY + 270.0f) % 360;
                            break;
                        case 0x30:
                            rotationY = (s32)(rotationY + 225.0f) % 360;
                            break;
                        case 0x40:
                            rotationY = (s32)(rotationY + 180.0f) % 360;
                            break;
                        case 0x50:
                            rotationY = (s32)(rotationY + 135.0f) % 360;
                            break;
                        case 0x60:
                            rotationY = (s32)(rotationY + 90.0f) % 360;
                            break;
                        case 0:
                            break;
                    
                    }
                
                } else {
                    rotationX = 0.0f;
                    rotationY = 0.0f;
                    rotationZ = 0.0f;
                }

                bitmapIndex = setBitmap(texturePtr, palettePtr, (0x8 | 0x10 | 0x20));

                setBitmapViewSpacePosition(bitmapIndex, xPosition, yPosition, zPosition);
                setBitmapRotation(bitmapIndex, rotationX, rotationY, rotationZ);
                setBitmapScale(bitmapIndex, scaleX, scaleY, 1.0f);

                func_80029F14(bitmapIndex, 0);
                func_80029E2C(bitmapIndex, 2, 3);
                func_80029EA4(bitmapIndex, 2);
                                        
                if (coreMapObjects[k].flags & 2) {
                    func_8002A02C(bitmapIndex, 4);
                } else {
                    func_8002A02C(bitmapIndex, 3);
                }

                setBitmapRGBA(bitmapIndex, mainMap->mapGlobals.currentRGBA.r, mainMap->mapGlobals.currentRGBA.g, mainMap->mapGlobals.currentRGBA.b, mainMap->mapGlobals.currentRGBA.a);
                setBitmapAnchor(bitmapIndex, 0, 0);
                
                bitmaps[bitmapIndex].flags |= 4;
                    
            }
                
            asm("");
            
            j++;
            total = mainMap->coreMapObjectsMetadata[i].totalCoreMapObjects;  
            k++;
                
        } 

        i++;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setupMapObjectSprites);

void setupMapObjectSprites(MainMap *mainMap) {
    
    u16 i;

    u8 flags;
    
    f32 xPosition, yPosition, zPosition;
    f32 tileOffsetX, tileOffsetZ;
    f32 cameraWorldX, cameraWorldZ;
    f32 mapWorldCenterX, mapWorldCenterZ;
    
    s32 screenWrapOffsetX, screenWrapOffsetZ;
    f32 tempF;
    
    for (i = 0; i < MAX_MAP_OBJECTS; i++) {

        flags = mainMap->mapObjects[i].flags; 
        
        if (flags & MAP_OBJECT_ACTIVE) {
            
            if (!(flags & MAP_OBJECT_STATIC)) {
                startSpriteAnimation(mainMap->mapObjects[i].spriteIndex, mainMap->mapObjects[i].animationIndex, mainMap->mapObjects[i].animationMode);
                mainMap->mapObjects[i].flags |= 2;
            }
             
            xPosition = mainMap->mapObjects[i].coordinates.x;
            zPosition = mainMap->mapObjects[i].coordinates.z;
             
            mapWorldCenterX = (mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2;
            mapWorldCenterZ = (mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2;
            
            tileOffsetX = mainMap->mapGrid.tileSizeX / 2;
            tileOffsetZ = mainMap->mapGrid.tileSizeZ / 2;
            
            xPosition = (mainMap->mapObjects[i].coordinates.x + mapWorldCenterX) - tileOffsetX;
            yPosition = mainMap->mapObjects[i].coordinates.y;
            zPosition = (mainMap->mapObjects[i].coordinates.z + mapWorldCenterZ) - tileOffsetZ;
            
            cameraWorldX = mainMap->mapCameraView.cameraTileX * mainMap->mapGrid.tileSizeX;
            cameraWorldZ = mainMap->mapCameraView.cameraTileZ * mainMap->mapGrid.tileSizeZ;
            
            xPosition -= cameraWorldX;
            zPosition -= cameraWorldZ;
            
            // FIXME: fake
            tempF = mainMap->mapCameraView.viewOffset.z;
            
            xPosition += mainMap->mapCameraView.viewOffset.x;
            zPosition += mainMap->mapCameraView.viewOffset.z;
            
            if (mainMap->mapObjects[i].flags & 4) {
    
                screenWrapOffsetX = (mainMap->mapCameraView.cameraTileX - (mainMap->mapGrid.mapWidth) / 2) * mainMap->mapGrid.tileSizeX;
                screenWrapOffsetZ = (mainMap->mapCameraView.cameraTileZ - (mainMap->mapGrid.mapHeight) / 2) * mainMap->mapGrid.tileSizeZ;
    
                if (screenWrapOffsetX >= 0) {
                    screenWrapOffsetX += ((mainMap->mapCameraView.viewExtentX * mainMap->mapGrid.tileSizeX) / 2); 
                } else {
                    screenWrapOffsetX -= ((mainMap->mapCameraView.viewExtentX * mainMap->mapGrid.tileSizeX) / 2); 
                }
    
                if (screenWrapOffsetZ >= 0) {
                    screenWrapOffsetZ += ((mainMap->mapCameraView.viewExtentZ * mainMap->mapGrid.tileSizeZ) / 2); 
                } else {
                    screenWrapOffsetZ -= ((mainMap->mapCameraView.viewExtentZ * mainMap->mapGrid.tileSizeZ) / 2); 
                }

                screenWrapOffsetX = (s32)((screenWrapOffsetX - xPosition) / 320) * 320;
                screenWrapOffsetZ = (s32)((screenWrapOffsetZ - zPosition) / 240) * 240;

                xPosition = (s32)xPosition + screenWrapOffsetX;
                yPosition = (s32)yPosition;
                zPosition = (s32)zPosition + screenWrapOffsetZ;

            }
        
            tempF = mainMap->mapCameraView.viewOffset.y;
            flags = mainMap->mapObjects[i].flags & 8;

            // FIXME: likely fake
            do {
                
                yPosition += tempF;
                
                if (flags) {
                    
                    if (globalBobbingFrameCounter < 24) {
                        globalBobbingYOffset++;
                    } else {
                        globalBobbingYOffset--;
                    }
                    
                    globalBobbingFrameCounter++;
                    
                    if (globalBobbingFrameCounter == 48) {
                        globalBobbingFrameCounter = 0;
                    }
                    
                    yPosition += globalBobbingYOffset / 2;
                
                }
                
            } while (0);
        
            setSpriteViewSpacePosition(mainMap->mapObjects[i].spriteIndex, xPosition, yPosition, zPosition);
            
            if (mainMap->mapObjects[i].unk_10 != 0xFF) {
                setSpriteRotation(mainMap->mapObjects[i].spriteIndex, mainMap->mapGlobals.rotation.x, mainMap->mapGlobals.rotation.y + mainMap->mapObjects[i].unk_10, mainMap->mapGlobals.rotation.z);
            }
            
            setSpriteColor(mainMap->mapObjects[i].spriteIndex, mainMap->mapGlobals.currentRGBA.r, mainMap->mapGlobals.currentRGBA.g, mainMap->mapGlobals.currentRGBA.b, mainMap->mapGlobals.currentRGBA.a);
            setBilinearFiltering(mainMap->mapObjects[i].spriteIndex, 1);
        
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", setupWeatherSprites);

void setupWeatherSprites(MainMap* mainMap) {

    u16 i;

    s16 temp1;
    s32 temp2;
    s16 temp3;
    s32 temp4;
    
    f32 xPosition, yPosition, zPosition;
    f32 cameraWorldX, cameraWorldZ;
    f32 tileCenterOffsetX, tileCenterOffsetZ;
    f32 spriteWorldX, spriteWorldZ;
    f32 mapWorldCenterX, mapWorldCenterZ;
    f32 tileOffsetX, tileOffsetZ; 
    
    for (i = 0; i < MAX_WEATHER_SPRITES; i++) {

        if (mainMap->weatherSprites[i].flags & MAP_WEATHER_SPRITE_ACTIVE) {

            if (func_8002BCC8(mainMap->weatherSprites[i].spriteIndex) || !(mainMap->weatherSprites[i].flags & 2)) {
                
                resetAnimationState(mainMap->weatherSprites[i].spriteIndex);

                if (!(getRandomNumberInRange(0, 4))) {
                
                    globalSprites[mainMap->weatherSprites[i].spriteIndex].stateFlags &= ~0x40;
                    startSpriteAnimation(mainMap->weatherSprites[i].spriteIndex, mainMap->weatherSprites[i].animationIndex, 0xFE);
                    
                    mainMap->weatherSprites[i].flags |= 2;
                    
                    temp1 = getRandomNumberInRange(0, 320);
                    temp2 = (((mainMap->mapCameraView.cameraTileX - (mainMap->mapGrid.mapWidth / 2)) * mainMap->mapGrid.tileSizeX)) - 160;

                    mainMap->weatherSprites[i].coordinates.x = temp1 + temp2;
                        
                    mainMap->weatherSprites[i].coordinates.y = 0;
                    
                    temp3 = getRandomNumberInRange(0, 240);
                    temp4 = (((mainMap->mapCameraView.cameraTileZ - (mainMap->mapGrid.mapHeight / 2)) * mainMap->mapGrid.tileSizeZ)) - 120;
                    
                    mainMap->weatherSprites[i].coordinates.z = temp3 + temp4;
                    
                }
            
            }

            mapWorldCenterX = (mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2;
            mapWorldCenterZ = (mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2;

            tileOffsetX = mainMap->mapGrid.tileSizeX / 2;
            tileOffsetZ = mainMap->mapGrid.tileSizeZ / 2;

            cameraWorldX = mainMap->mapCameraView.cameraTileX * mainMap->mapGrid.tileSizeX;
            cameraWorldZ = mainMap->mapCameraView.cameraTileZ * mainMap->mapGrid.tileSizeZ;

            spriteWorldX = mainMap->weatherSprites[i].coordinates.x;
            spriteWorldZ = mainMap->weatherSprites[i].coordinates.z;

            xPosition = spriteWorldX + mapWorldCenterX - tileOffsetX;
            zPosition = spriteWorldZ + mapWorldCenterZ - tileOffsetZ;
            
            xPosition -= cameraWorldX;
            zPosition -= cameraWorldZ;
            
            xPosition += mainMap->mapCameraView.viewOffset.x;
            zPosition += mainMap->mapCameraView.viewOffset.z;

            yPosition = mainMap->weatherSprites[i].coordinates.y;
            
            if (globalSprites[mainMap->weatherSprites[i].spriteIndex].renderingFlags & 0x200) {
                yPosition -= mainMap->mapCameraView.viewOffset.y;
            } else {
                yPosition += mainMap->mapCameraView.viewOffset.y;
            }

            setSpriteViewSpacePosition(mainMap->weatherSprites[i].spriteIndex,
                xPosition, yPosition, zPosition);

            setSpriteColor(mainMap->weatherSprites[i].spriteIndex, 
                   mainMap->mapGlobals.currentRGBA.r, 
                   mainMap->mapGlobals.currentRGBA.g, 
                   mainMap->mapGlobals.currentRGBA.b, 
                   mainMap->mapGlobals.currentRGBA.a);
            
            setBilinearFiltering(mainMap->weatherSprites[i].spriteIndex, TRUE);
                
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", processMapSceneNode);

void processMapSceneNode(u16 mapIndex, Gfx* dl) {

    u16 temp = addSceneNode(dl, (8 | 0x20));
    
    addSceneNodePosition(temp, mainMap[mapIndex].mapGlobals.translation.x + mainMap[mapIndex].mapCameraView.viewOffset.x, 
        mainMap[mapIndex].mapGlobals.translation.y + mainMap[mapIndex].mapCameraView.viewOffset.y, 
        mainMap[mapIndex].mapGlobals.translation.z + mainMap[mapIndex].mapCameraView.viewOffset.z);

    addSceneNodeScaling(temp, mainMap[mapIndex].mapGlobals.scale.x, mainMap[mapIndex].mapGlobals.scale.y, mainMap[mapIndex].mapGlobals.scale.z);
    
    addSceneNodeRotation(temp, mainMap[mapIndex].mapGlobals.rotation.x, mainMap[mapIndex].mapGlobals.rotation.y, mainMap[mapIndex].mapGlobals.rotation.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_80039F58);

void func_80039F58(u16 mapIndex) {

    bool found = FALSE;
    bool set = FALSE;
    u16 i = 0;

    do {

        if ((mainMap[mapIndex].mapAdditions[i].flags & MAP_ADDITION_ACTIVE) && !set) {
        
            do {

                if (mainMap[mapIndex].mapAdditions[i].unk_40 == 0) { 

                    if (mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42] != 0xFFFF) {
 
                        setMapAdditionMetadata(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].x, mainMap[mapIndex].mapAdditions[i].z, mainMap[mapIndex].mapAdditions[i].flags & 2);
                        found = TRUE;
                        
                    } else {
                        
                        if (mainMap[mapIndex].mapAdditions[i].flags & 4) {
                            
                            mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
                            mainMap[mapIndex].mapAdditions[i].unk_42 = 0;
                            
                            setMapAdditionMetadata(mapIndex, mainMap[mapIndex].mapAdditions[i].arr1[mainMap[mapIndex].mapAdditions[i].unk_42], mainMap[mapIndex].mapAdditions[i].x, mainMap[mapIndex].mapAdditions[i].z, mainMap[mapIndex].mapAdditions[i].flags & 2);
                            found = TRUE;
                            
                        } else {
                            mainMap[mapIndex].mapAdditions[i].flags &= ~MAP_ADDITION_ACTIVE;
                        }

                    }
                }
                
                if (mainMap[mapIndex].mapAdditions[i].unk_40 >= mainMap[mapIndex].mapAdditions[i].arr2[mainMap[mapIndex].mapAdditions[i].unk_42]) {

                    mainMap[mapIndex].mapAdditions[i].unk_42++;
                    
                    if (mainMap[mapIndex].mapAdditions[i].unk_42 == 16) {
                        mainMap[mapIndex].mapAdditions[i].flags &= ~MAP_ADDITION_ACTIVE;
                        set = TRUE;
                    } 

                    mainMap[mapIndex].mapAdditions[i].unk_40 = 0;
                     
                } else {
                    mainMap[mapIndex].mapAdditions[i].unk_40++;
                    set = TRUE;
                    
                }
                
            } while (!set);
            
        }

        i++;
            
    } while (i < MAX_MAP_ADDITIONS);

    if (found) {
        func_80036C08(MAIN_MAP_INDEX);
    }
    
}

static inline u8 updateMapRGBA(u16 i) {

    u8 count = 0;
    
    if (mainMap[i].mapGlobals.currentRGBA.r < mainMap[i].mapGlobals.targetRGBA.r) {
                
        mainMap[i].mapGlobals.currentRGBA.r += mainMap[i].mapGlobals.deltaRGBA.r; 
        
        if (mainMap[i].mapGlobals.targetRGBA.r <= mainMap[i].mapGlobals.currentRGBA.r) {
            mainMap[i].mapGlobals.currentRGBA.r = mainMap[i].mapGlobals.targetRGBA.r;
        } else {
            count = 1;
        } 
    }

    if (mainMap[i].mapGlobals.currentRGBA.r > mainMap[i].mapGlobals.targetRGBA.r) {
        
        mainMap[i].mapGlobals.currentRGBA.r -= mainMap[i].mapGlobals.deltaRGBA.r;

        if (mainMap[i].mapGlobals.currentRGBA.r <= mainMap[i].mapGlobals.targetRGBA.r) {
            mainMap[i].mapGlobals.currentRGBA.r = mainMap[i].mapGlobals.targetRGBA.r;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapGlobals.currentRGBA.g < mainMap[i].mapGlobals.targetRGBA.g) {
        
        mainMap[i].mapGlobals.currentRGBA.g += mainMap[i].mapGlobals.deltaRGBA.g; 
        
        if (mainMap[i].mapGlobals.targetRGBA.g <= mainMap[i].mapGlobals.currentRGBA.g) {
            mainMap[i].mapGlobals.currentRGBA.g = mainMap[i].mapGlobals.targetRGBA.g;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapGlobals.currentRGBA.g > mainMap[i].mapGlobals.targetRGBA.g) {
        
        mainMap[i].mapGlobals.currentRGBA.g -= mainMap[i].mapGlobals.deltaRGBA.g;

        if (mainMap[i].mapGlobals.currentRGBA.g <= mainMap[i].mapGlobals.targetRGBA.g) {
            mainMap[i].mapGlobals.currentRGBA.g = mainMap[i].mapGlobals.targetRGBA.g;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapGlobals.currentRGBA.b < mainMap[i].mapGlobals.targetRGBA.b) {
        
        mainMap[i].mapGlobals.currentRGBA.b += mainMap[i].mapGlobals.deltaRGBA.b; 
        
        if (mainMap[i].mapGlobals.targetRGBA.b <= mainMap[i].mapGlobals.currentRGBA.b) {
            mainMap[i].mapGlobals.currentRGBA.b = mainMap[i].mapGlobals.targetRGBA.b;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapGlobals.currentRGBA.b > mainMap[i].mapGlobals.targetRGBA.b) {
        
        mainMap[i].mapGlobals.currentRGBA.b -= mainMap[i].mapGlobals.deltaRGBA.b;

        if (mainMap[i].mapGlobals.currentRGBA.b <= mainMap[i].mapGlobals.targetRGBA.b) {
            mainMap[i].mapGlobals.currentRGBA.b = mainMap[i].mapGlobals.targetRGBA.b;
        } else {
            count += 1;
        } 
        
    }

    if (mainMap[i].mapGlobals.currentRGBA.a < mainMap[i].mapGlobals.targetRGBA.a) {
        
        mainMap[i].mapGlobals.currentRGBA.a += mainMap[i].mapGlobals.deltaRGBA.a; 
        
        if (mainMap[i].mapGlobals.targetRGBA.a <= mainMap[i].mapGlobals.currentRGBA.a) {
            mainMap[i].mapGlobals.currentRGBA.a = mainMap[i].mapGlobals.targetRGBA.a;
        } else {
            count += 1;
        } 
    }

    if (mainMap[i].mapGlobals.currentRGBA.a > mainMap[i].mapGlobals.targetRGBA.a) {
        
        mainMap[i].mapGlobals.currentRGBA.a -= mainMap[i].mapGlobals.deltaRGBA.a;

        if (mainMap[i].mapGlobals.currentRGBA.a <= mainMap[i].mapGlobals.targetRGBA.a) {
            mainMap[i].mapGlobals.currentRGBA.a = mainMap[i].mapGlobals.targetRGBA.a;
        } else {
            count += 1;
        } 
        
    }

    return count;
    
}

static inline void handleRotation(u16 i) {
    
    mainMap[i].mapCameraView.rotation.x = 0.0f;
    mainMap[i].mapCameraView.rotation.y = 360.0f - currentWorldRotationAngles.y;
    mainMap[i].mapCameraView.rotation.z = 0.0f;

    if (mainMap[i].mapCameraView.rotation.y < 0.0f) {
        mainMap[i].mapCameraView.rotation.y += 360.0f;
    }

    if (mainMap[i].mapCameraView.rotation.y >= 360.0f) {
        mainMap[i].mapCameraView.rotation.y -= 360.0f;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", updateMapGraphics);

void updateMapGraphics(void) {

    u16 height = 0;
    u16 i;
    
    Gfx *dl = mapDisplayList[gGraphicsBufferIndex];
    Gfx *dlStartingPosition;
    
    // FIXME: shoudn't be necessary
    u32 padding[4];
    
    for (i = 0; i < MAX_MAPS; i++) {

        if ((mainMap[i].mapState.flags & MAP_ACTIVE) && (mainMap[i].mapState.flags & 2)) {
            
            // map additions
            func_80039F58(i);

            if (updateMapRGBA(i) == 0) {
                mainMap[i].mapState.flags |= MAP_RGBA_FINISHED;
            } else {
                mainMap[i].mapState.flags &= ~MAP_RGBA_FINISHED;
            }

            if (mainMap[i].mapGlobals.rotation.x < 0) {
                mainMap[i].mapGlobals.rotation.x += 360.0f;
            }

            if (mainMap[i].mapGlobals.rotation.x >= 360.0f) {
                mainMap[i].mapGlobals.rotation.x -= 360.0f;
            }
            
            if (mainMap[i].mapGlobals.rotation.y < 0) {
                mainMap[i].mapGlobals.rotation.y += 360.0f;
            }

            if (mainMap[i].mapGlobals.rotation.y >= 360.0f) {
                mainMap[i].mapGlobals.rotation.y -= 360.0f;
            }

            if (mainMap[i].mapGlobals.rotation.z < 0) {
                mainMap[i].mapGlobals.rotation.z += 360.0f;
            }

            if (mainMap[i].mapGlobals.rotation.z >= 360.0f) {
                mainMap[i].mapGlobals.rotation.z -= 360.0f;
            }

            handleRotation(i);
            
            dlStartingPosition = dl;
            
            // tiles
            dl = buildMapDisplayList(dlStartingPosition, &mainMap[i], height);
            
            // these sprites are rendered via display lists in sprite.c
            setupCoreMapObjectSprites(&mainMap[i]);
            setupMapObjectSprites(&mainMap[i]);
            setupWeatherSprites(&mainMap[i]);
            
            processMapSceneNode(i, dlStartingPosition);

            renderGroundObjects(&mainMap[i]);

            height += mainMap[i].mapState.totalVertexCount;

        }
        
    }

    if (dl - mapDisplayList[gGraphicsBufferIndex] >= 0x1B00) {
        // TODO: replace with string literals
        __assert(D_8011EDB0, D_8011EDB4, 3912);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/map", prepareGroundObjectBitmap);

inline Gfx* prepareGroundObjectBitmap(Gfx* dl, GroundObjectBitmap* sprite) {
    
    // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)
    *dl++ = D_8011ED90;
    // gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)
    *dl++ = D_8011EDC8;
    // gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON)
    *dl++ = D_8011EDD0;
    // gsDPSetTextureFilter(G_TF_BILERP)
    *dl++ = D_8011ED78;

    return loadBitmapTexture(dl++, (BitmapObject*)sprite, 0, sprite->height);
    
}
 
//INCLUDE_ASM("asm/nonmatchings/system/map", renderGroundObject);

Gfx* renderGroundObject(Gfx* dl, MainMap* map, GroundObjectBitmap* bitmap, u16 vtxIndex) {

    Gfx tempDl[2];
      
    setupBitmapVertices(&groundObjectVertices[gGraphicsBufferIndex][vtxIndex], 
        bitmap->width, 
        bitmap->height,
        bitmap->height, 
        0, 
        0,
        0, 
        0, 
        0, 
        (0x10 | 0x40 | 0x100), 
        map->mapGlobals.currentRGBA.r, 
        map->mapGlobals.currentRGBA.g, 
        map->mapGlobals.currentRGBA.b,
        map->mapGlobals.currentRGBA.a);
 
    // FIXME: might be a wrapper around gSPVertex
    gSPVertex(&tempDl[1], &groundObjectVertices[gGraphicsBufferIndex][vtxIndex][0], 4, 0);

    *tempDl = *(tempDl+1);
    *dl++ = *tempDl;
    
    // gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)
    *dl++ = D_8011EDD8;
    // gsDPPipeSync()
    *dl++ = D_8011EDA8;
    // gsSPEndDisplayList()
    *dl++ = D_8011ED80;
    
    return dl++;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", func_8003AF58);

Vec3f func_8003AF58(u16 mapIndex, u8 x, u8 z) {

    Vec3f vec;

    f32 temp1;
    f32 temp2;

    // these two statements have to be on the same line to match lmao (otherwise compiler inserts a nop)
    temp1 = mainMap[mapIndex].mapGrid.tileSizeX; vec.x = ((x *  temp1) - mainMap[mapIndex].mapState.mapOriginX) + (mainMap[mapIndex].mapGrid.tileSizeX / 2);

    temp2 = mainMap[mapIndex].mapGrid.tileSizeZ; vec.z = ((z * temp2) - mainMap[mapIndex].mapState.mapOriginZ) + (mainMap[mapIndex].mapGrid.tileSizeZ / 2);
    
    vec.y = getTerrainHeightAtPosition(MAIN_MAP_INDEX, vec.x, vec.z);
    
    if (x < mainMap[mapIndex].groundObjects.x || z < mainMap[mapIndex].groundObjects.z || x >= mainMap[mapIndex].groundObjects.x + 0x14 || z >= mainMap[mapIndex].groundObjects.z + 0x18) {
        vec.y = 65535.0f;
    }
    
    return vec;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/map", addGroundObjectToSceneGraph);

inline void addGroundObjectToSceneGraph(MainMap* map, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, Gfx* dl) {

    Vec3f vec;
    
    f32 x = (arg1 + map->mapState.mapOriginX + map->mapCameraView.viewOffset.x) - arg4;
    f32 y = arg2 + map->mapCameraView.viewOffset.y;
    f32 z = (arg3 + map->mapState.mapOriginZ + map->mapCameraView.viewOffset.z) - arg5;

    addSceneNodePosition(addSceneNode(dl, 8), x, y, z);

}

//INCLUDE_ASM("asm/nonmatchings/system/map", renderGroundObjects);

void renderGroundObjects(MainMap* mainMap) {

    u32 padding1[4];
    
    Gfx* dl;
    Gfx* startingPositionDl;

    u16 i;
    u16 count;
    u16 gridIndex;
    u32 index;

    s16 temp1;
    s16 temp2;
    
    f32 arr[8];
    
    arr[6] = mainMap->mapCameraView.cameraTileX * mainMap->mapGrid.tileSizeX;

    index = gGraphicsBufferIndex;
    
    if (index) {
        arr[7] = mainMap->mapCameraView.cameraTileZ * mainMap->mapGrid.tileSizeZ;
    } else {
        arr[7] = mainMap->mapCameraView.cameraTileZ * mainMap->mapGrid.tileSizeZ;
    }

    temp1 = mainMap->mapGrid.tileSizeX;
    temp1 = (-(mainMap->mapGrid.mapWidth * mainMap->mapGrid.tileSizeX) / 2) 
         + (temp1 * mainMap->groundObjects.x) - (mainMap->mapGrid.tileSizeX / 2);

    temp2 = mainMap->mapGrid.tileSizeZ;
    temp2 = ((-(mainMap->mapGrid.mapHeight * mainMap->mapGrid.tileSizeZ) / 2) 
         + (temp2 * mainMap->groundObjects.z)) - (mainMap->mapGrid.tileSizeZ / 2);

    count = 0;
    
    dl = groundObjectBitmapsDisplayList[index];

     if (mainMap->groundObjects.unk_12) {

        for (i = 0; i < MAX_GROUND_OBJECTS; i++) {

             if (mainMap->groundObjects.spriteIndexToGrid[i] != 0xFFFF) {

                 startingPositionDl = dl;
                 
                 dl = prepareGroundObjectBitmap(dl, &mainMap->groundObjectBitmaps[i]);

                 gridIndex = mainMap->groundObjects.spriteIndexToGrid[i];

                 do {

                     if (mainMap->visibilityGrid[D_801131CC[gridIndex] + mainMap->groundObjects.z][D_80112FEC[gridIndex] + mainMap->groundObjects.x]) {

                         dl = renderGroundObject(dl, mainMap, &mainMap->groundObjectBitmaps[i], count);

                         addGroundObjectToSceneGraph(mainMap, 
                            temp1 + (D_80112FEC[gridIndex] * 32) + mainMap->groundObjectBitmaps[i].coordinates.x, 
                            mainMap->groundObjects.unk_12 + mainMap->groundObjectBitmaps[i].coordinates.y, 
                            temp2 + (D_801131CC[gridIndex] * 32) + mainMap->groundObjectBitmaps[i].coordinates.z, 
                            arr[6], arr[7], startingPositionDl);
                         
                         count++;

                         startingPositionDl = dl;
                         
                     }
                     
                     gridIndex = mainMap->groundObjects.nextGridToSpriteIndex[gridIndex];
                     
                 } while (gridIndex != 0xFFFF);
 
             }
            
        }
            
    } else {

        for (i = 0; i < MAX_GROUND_OBJECTS; i++) {

             if (mainMap->groundObjects.spriteIndexToGrid[i] != 0xFFFF) {

                 startingPositionDl = dl;
                 
                 dl = prepareGroundObjectBitmap(dl, &mainMap->groundObjectBitmaps[i]);
                
                gridIndex = mainMap->groundObjects.spriteIndexToGrid[i];

                do {

                    if (mainMap->visibilityGrid[D_801131CC[gridIndex] + mainMap->groundObjects.z][D_80112FEC[gridIndex] + mainMap->groundObjects.x]) {
                    
                        dl = renderGroundObject(dl, mainMap, &mainMap->groundObjectBitmaps[i], count);

                        arr[2] = temp1 + (D_80112FEC[gridIndex] << 5) + mainMap->groundObjectBitmaps[i].coordinates.x - 0.0f;
                        arr[4] = temp2 + (D_801131CC[gridIndex] << 5) + mainMap->groundObjectBitmaps[i].coordinates.z - 4.0f;
                         
                        arr[3] = getTerrainHeightAtPosition(0, 
                            arr[2],
                            arr[4]); 

                        addGroundObjectToSceneGraph(mainMap, 
                            arr[2], 
                            arr[3], 
                            arr[4], 
                            arr[6], arr[7], startingPositionDl);
                        
                         count++;

                         startingPositionDl = dl;
                        
                    }
                    
                    gridIndex = mainMap->groundObjects.nextGridToSpriteIndex[gridIndex];
                    
                } while (gridIndex != 0xFFFF);

             }

        }
        
    }
    
    if (dl - groundObjectBitmapsDisplayList[gGraphicsBufferIndex] >= 0x1000) {
        __assert(D_8011EDB0, D_8011EDB4, 4221);
    }
    
}

static const Gfx D_8011ED68 = gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED68);

static const Gfx D_8011ED70 = gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED70);

static const Gfx D_8011ED78 = gsDPSetTextureFilter(G_TF_BILERP);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED78);

static const Gfx D_8011ED80 = gsSPEndDisplayList();

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED80);

static const Gfx D_8011ED88 = gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED88);

static const Gfx D_8011ED90 = gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED90);

static const Gfx D_8011ED98 = gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011ED98);

static const Gfx D_8011EDA0 = gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDA0);

static const Gfx D_8011EDA8 = gsDPPipeSync();

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDB0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDB4);

static const char D_8011EDB0[] = "EX";
static const char D_8011EDB4[] = "s:/system/map.c";

static const Gfx D_8011EDC8 = gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDC8);

static const Gfx D_8011EDD0 = gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDD0);

static const Gfx D_8011EDD8 = gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemmap", D_8011EDD8);