//
// Created by itsmeayoub on 20/08/24.
//
#include "Appinfo.h"


ApplicationInfo const AppHeader __attribute__((section(".AppInfoBlock"))) = {
        .Signature      = SIG_BTM,
        .type           = TYPE_BTM,
        .version        = 1,
        .name           = "Boot Manager",
        .hwModel        = 1,
        .StartAdd       = 0x8000000,  // To be filled after
        .Size           = 128*1024,   // To be filled after
};
