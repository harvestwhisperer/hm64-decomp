#include <nusys.h>

static u8 jis2n64[][2] = {
    {' ',  0x0f},
    {'!',  0x34},
    {'\"', 0x35},
    {'#',  0x36},
    {'\'', 0x37},
    {'*',  0x38},
    {'+',  0x39},
    {',',  0x3a},
    {'-',  0x3b},
    {'.',  0x3c},
    {'/',  0x3d},
    {':',  0x3e},
    {'=',  0x3f},
    {'?',  0x40},
    {'@',  0x41},
    {0xa1, 0x42},	
    {0xde, 0x43},	
    {0xdf, 0x44},	
    {0xa7, 0x45},	
    {0xa8, 0x46},	
    {0xa9, 0x47},	
    {0xaa, 0x48},	
    {0xab, 0x49},	
    {0xaf, 0x4a}, 	    
    {0xac, 0x4b},	
    {0xad, 0x4c},	
    {0xae, 0x4d},	
    {0xa6, 0x4e},	    
    {0xdd, 0x4f},	
    {0x00, 0x00},	
};
    
void nuContPakJisToN64(u8* src, u8* dest, u32 len) {

    u32 	cnt;
    u32		cnt_len;

    for (cnt_len = 0; cnt_len < len; cnt_len++) {
	
		if (*src == '\0') {

			return;

		} else if ((0x41 <= *src) && (*src <= 0x5a)) {
		
			*dest = *src - 0x27;
		
		} else if ((0x61 <= *src) &&  (*src <= 0x7a)) {
		
			*dest = *src - 0x47;
		
		} else if ((0x30 <= *src) && (*src <= 0x39)) {  
		
			*dest = *src -0x20;

		} else if ((0xb1 <= *src) && (*src <= 0xdc)) { 
				
			if (*(src+1) == 0xde) {		

				if ( *src < 0xca) {
					*dest = *src - 0x3a;
				} else {	
					*dest = *src - 0x3f;	
				}
				
				src++;

			} else if (*(src+1) == 0xdf) {	
				
				*dest = *src - 0x3a;
				src++;		

			} else {

				*dest = *src - 0x61;		
			
			}

		} else {	

			cnt = 0;
			
			while (jis2n64[cnt][1]) {
				
				if (jis2n64[cnt][0] == *src) {
					*dest = jis2n64[cnt][1];
					break;
				}

				cnt++;
				
			}
				
			if (jis2n64[cnt][1] == 0x00) {
				*dest = 0x0f;
			}
		}

		dest++;
		src++;
    
	}
	
}

void nuContPakN64ToJis(u8* src, u8* dest, u32 len) {

    u32	cnt_len;
    u32	cnt;
    
    for (cnt_len = 0; cnt_len < len; cnt_len++) {
		
		if (*src == '\0') {
			
			*dest = *src;
		
		} else if ((0x1a <= *src) && (*src <= 0x33)) {
		
			*dest = *src + 0x27;
		
		} else if ((0x10 <= *src) && (*src <= 0x19)) {  
		
			*dest = *src + 0x20;
		
		} else if ((0x50 <= *src) && (*src <= 0x7b)) { 
		
			*dest = *src + 0x61;
		
		} else if ((0x7c <= *src) && (*src <= 0x8a)) { 
		
			*dest = *src + 0x3a;
			*(dest + 1) = 0xde;
			dest++;
		
		} else if ((0x8b <= *src) && (*src <= 0x8f)) { 
		
			*dest = *src + 0x3f;
			*(dest + 1) = 0xde;
		
		} else if ((0x90 <= *src) && (*src <= 0x94)) {
			
			*dest = *src + 0x3a;
			*(dest + 1) = 0xdf;

		} else {		

			cnt = 0;
			
			while (jis2n64[cnt][1]) {
			
				if (jis2n64[cnt][1] == *src) {
					*dest = jis2n64[cnt][0];
					break;
				}

				cnt++;
		
			}
			
			if (jis2n64[cnt][1] == 0x00) {
				*dest = 0x0f;
			}

		}
		
		dest++;
		src++;

    }

}
