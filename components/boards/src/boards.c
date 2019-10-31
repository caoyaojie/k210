#include "boards.h"


//cyj add 2019-10-9
#include "fpioa.h"
#include "sysctl.h"
#include "global_config.h"
#include "fpioa.h"
#include "gpiohs.h"
//cyj add end

#if CONFIG_BOARD_M5STICK
#include "m5stick.h"
#endif


int boards_init()
{
    int ret = 0;
#if CONFIG_BOARD_M5STICK
    if( !m5stick_init() )
        ret = -1;
#endif

    //init lcd backlight led 
    //cyj add 2019-10-9 
    fpioa_set_function(23, FUNC_GPIOHS0 + 7);
    gpiohs_set_drive_mode(7, GPIO_DM_OUTPUT);
    gpiohs_set_pin(7, GPIO_PV_HIGH);

    return ret;
}

