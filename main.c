EE_BIN = hola.elf
EE_OBJS = main.o

GSKIT ?= $(PS2DEV)/gsKit

EE_INCS += -I$(GSKIT)/include
EE_LDFLAGS += -L$(GSKIT)/lib
EE_LIBS = -lgskit -ldmakit -lc -lkernel

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

// Prueba minima para PS2: pinta la pantalla y un cuadrado que cambia de color.
// Si ves esto en PCSX2, tu cadena de herramientas funciona.

#include <kernel.h>
#include <gsKit.h>
#include <dmaKit.h>

int main(void)
{
    GSGLOBAL *gs = gsKit_init_global();

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_init_screen(gs);
    gsKit_mode_switch(gs, GS_ONESHOT);

    int frame = 0;

    while (1) {
        // Fondo azul oscuro
        u64 fondo = GS_SETREG_RGBAQ(0x10, 0x10, 0x40, 0x00, 0x00);
        gsKit_clear(gs, fondo);

        // Cuadrado que cambia de color con el tiempo
        u8 r = (frame * 2) & 0xFF;
        u8 g = (frame * 3) & 0xFF;
        u8 b = (frame * 5) & 0xFF;
        u64 color = GS_SETREG_RGBAQ(r, g, b, 0x80, 0x00);
        gsKit_prim_sprite(gs, 200.0f, 150.0f, 440.0f, 330.0f, 1, color);

        gsKit_queue_exec(gs);
        gsKit_sync_flip(gs);
        frame++;
    }

    return 0;
}
