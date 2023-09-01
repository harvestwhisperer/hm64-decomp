#ifndef _GAME_STATUS_H_
#define _GAME_STATUS_H_

extern u32 checkDailyEventBit(u16 bitIndex);
extern void setDailyEventBit(u16 bitIndex);
extern void toggleDailyEventBit(u16 bitIndex);   

extern void setLifeEventBit(u16);                    
extern void toggleLifeEventBit(u16);                            
extern u32 checkLifeEventBit(u16 bitIndex);   

extern u32 checkSpecialDialogueBit(u16 bitIndex);
extern void setSpecialDialogueBit(u16 bitIndex);
extern void toggleSpecialDialogueBit(u16 bitIndex);    

extern u32 checkMailRead(u16);                            
extern void setMail(u16);      
extern void toggleReadLetterBit(u16);      

void setAlbumPicture(u8 pictureBitIndex);
extern void setRecipes(void);  

extern void func_80063AF0(void); 
extern void func_80064114(void);
extern void func_800644B0(void);     
extern void func_80064814(void);     
extern u8 func_8006536C();

extern u32 dailyEventBits[];
extern u32 lifeEventBits[];
extern u32 specialDialogueBits[];
extern u32 recipesBitField[];
extern u32 D_801C3F38[];
// mailbox
extern u32 D_8016FFEC[];

#endif