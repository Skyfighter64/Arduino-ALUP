#pragma once
/*
 This header file imports the correct configuration definitions
 depending on the used platform.

 The config file is specified in platformio.ini using:
 
 build_flags = -D CONFIG_FILE=\"configs/d1mini_tcp.h\"

 or by using 

 #define CONFIG_FILE "configs/d1mini_tcp.h"
*/
#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)



#ifdef CONFIG_FILE
#include CONFIG_FILE
#pragma message("Using Custom Config File:")
#pragma message("Using Config File: " CONFIG_FILE)
#else
// load a default config file
#include "configs/default.h"
#pragma message("Using Default Config File.")
#endif 