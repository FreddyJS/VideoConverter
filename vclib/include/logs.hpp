#include <iostream>

#define VCLOGINFO 0
#define VCLOGERROR 1
#define VCLOGWARNING 2
#define VCLOGLIBAV 3

void vclog(int log_type, const char* fmt, ...);