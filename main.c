/*****************************************************************************/
/*  Author        : Дракончик												 */
/*  Machine       : Sony PlayStation 2										 */
/*  OS		  	  : OSDSYS													 */
/*  Language      : GNU C                                                    */
/*                                                                           */
/*  File Contents : MAIN BODY                                                */
/*  File Attribute: SOURCE                                                   */
/*  File Name     : main.c                                                   */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <malloc.h>

#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>
#include <iopcontrol.h> 
#include <sbv_patches.h>
#include "Data/include/menuState.h"
#include "Data/include/gameState.h"
#include "Data/include/overState.h"

#include "Data/include/pad.h"

#define IMPORT_BIN2C(_n) \
    extern unsigned char _n[];   \
    extern int size_##_n

IMPORT_BIN2C(padman_irx);
IMPORT_BIN2C(libsd_irx);
IMPORT_BIN2C(audsrv_irx);

StateMachine GameMachineState;

void reset_iop()
{
    SifInitRpc(0);
    // Reset IOP
    while (!SifIopReset("", 0x0))
        ;
    while (!SifIopSync())
        ;
    SifInitRpc(0);

    sbv_patch_enable_lmb();
}

int main()
{

	reset_iop();
	
	SifExecModuleBuffer(padman_irx, size_padman_irx, 0, NULL, NULL);
	SifExecModuleBuffer(libsd_irx, size_libsd_irx, 0, NULL, NULL);
	SifExecModuleBuffer(audsrv_irx, size_audsrv_irx, 0, NULL, NULL);
	
	// Initialize GamePad
	PadInitialize();
	
	u64 Black = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00);
	
	u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
	
	GSGLOBAL *gsGlobal = gsKit_init_global();
	
	// GS_MODE_PAL_I
	// GS_MODE_VGA_640_60
	
	// Check PAL or NTSC (Code by Daniel Santos)
	gsGlobal->PSM = GS_PSM_CT24;
	gsGlobal->PSMZ = GS_PSMZ_16S;
	
	gsGlobal->Mode = gsKit_check_rom();
	
	if(gsGlobal->Mode == GS_MODE_PAL)
	{
		gsGlobal->Height = 512;
	}
	else
	{ 
		gsGlobal->Height = 448;
	}
	
	dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		    D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	// Initialize the DMAC
	dmaKit_chan_init(DMA_CHANNEL_GIF);

	gsKit_init_screen(gsGlobal);
	
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

	// Sets up the mode for drawing.
	// GS_ONESHOT-> every draw clears the draw queue
	// GS_PERSISTENT-> gskit stores a draw queue and whenever you draw again, it just adds onto that queue
	gsKit_mode_switch(gsGlobal, GS_ONESHOT);

	gsKit_clear(gsGlobal, Black);

	StateMachineStart(&GameMachineState, &GameState, gsGlobal);
	
	while(1)
	{		
		// Update GamePads
		UpdatePad();
		
		StateMachineUpdate(&GameMachineState, gsGlobal);
		
		StateMachineDraw(&GameMachineState, gsGlobal, TexCol);
		
		gsKit_queue_exec(gsGlobal);
		
		gsKit_sync_flip(gsGlobal);
		
	}
	
	sleep(60); // sleep before boing bai bai
	
	return 0;
}

