
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include <chrono>
#include <ctime>  

#include "./tools/include/threads.h"
#include "./tools/include/pcf8574io.h"
#include "./tools/include/lcd.h"

