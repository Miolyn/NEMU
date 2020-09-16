#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cwd-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cwd-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cwd-template.h"
#undef DATA_BYTE

make_helper_v(cwd)
