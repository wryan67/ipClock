/*
 * Author: wryan
 * Date:2018/12/27
 * Compiling :gcc -Wall name.c -lwiringPi -lwiringPiDev
 */
 // Global Variables
#include "main.h"

static unsigned int lcdAddress;
static int          tries = 5;
static char         dateFormat = '1';
static volatile int lcdHandle;

int marquee = 0;

#define lcdWidth 16
#define maxMessageSize 128
char msg[1024];

bool usage() {
	fprintf(stderr, "usage: lcdClock [-d] [-a 00-FF] [-p 0-40] [-f n] [-s speed] [-m motd]\n");
	fprintf(stderr, "a = hexadecimal i2c address ($gpio i2cd)\n");
	fprintf(stderr, "f = clock format\n");
	fprintf(stderr, "    1 - Weekday Month Date HH24:SS\n");
	fprintf(stderr, "    2 - Weekday Date  HH:SS AM/PM\n");

	return false;
}




bool setup() {

	if (int ret = wiringPiSetup()) {
		fprintf(stderr, "Wiring Pi setup failed, ret=%d\n", ret);
		return false;
	}

	int seed;
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, fp);
	fclose(fp);
	srand(seed);

	//  the following statements setup the proper input or output for their respective 
	//  inputs or outputs

	lcdHandle=lcdSetup(lcdAddress);
	if (lcdHandle < 0) {
		fprintf(stderr, "lcdInit failed\n");
		return false;
	}
	return true;
}


bool commandLineOptions(int argc, char **argv) {
	int c, index;

	if (argc < 2) {
		return usage();
	}

	while ((c = getopt(argc, argv, "a:f:")) != -1)
		switch (c) {
		case 'a':
            if (strcmp(optarg, "0x") == 0) {
                sscanf(&optarg[2], "%x", &lcdAddress);
            } else {
                sscanf(optarg, "%x", &lcdAddress);
            }
			break;

        case 'f':
			dateFormat = optarg[0];
			break;
		
        case '?':
			if (optopt == 'a' || optopt == 'p' || optopt == 'f' || optopt == 'm' || optopt == 's')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character \\x%x.\n", optopt);

			return usage();

		default:
			abort();
		}


	//	for (index = optind; index < argc; index++)
	//		printf("Non-option argument %s\n", argv[index]);
	return true;
}


static uint8_t sizecvt(const int read)
{
	/* digitalRead() and friends from wiringpi are defined as returning a value
	< 256. However, they are returned as int() types. This is a safety function */

	if (read > 255 || read < 0)
	{
		printf("Invalid data from wiringPi library\n");
		exit(EXIT_FAILURE);
	}
	return (uint8_t)read;
}




void updateClock() {
	auto now = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(now);
	char vtime[64];

	switch (dateFormat) {
	case '2':	std::strftime(vtime, 64, "%a %e  %I:%M %p", std::localtime(&end_time));
		break;
	default:
		if (std::localtime(&end_time)->tm_mday < 10) {
			char xtime[64];
			std::strftime(xtime, 64, "%a %b %%d  %H:%M", std::localtime(&end_time));
			sprintf(vtime, xtime, std::localtime(&end_time)->tm_mday);
		}
		else {
			std::strftime(vtime, 64, "%a %b %e %H:%M", std::localtime(&end_time));
		}
	}



	lcdPosition(lcdHandle, 0, 0);           //Position cursor on the first line in the first column
	if (strlen(vtime) > 16) {
		vtime[16] = 0;
	}
	lcdPuts(lcdHandle, vtime);  //Print the text on the LCD at the current cursor postion

    FILE *input;
    char  wlan[256]="";
    char  eth0[256]="";

	lcdPosition(lcdHandle, 0, 1);
    input = popen("/sbin/ifconfig eth0 | awk '{if ($1==\"inet\") printf(\"%s\", $2)}'", "r");
    fgets(eth0, 255, input);
    fclose(input);
    
    lcdPosition(lcdHandle, 0, 1);
    input = popen("/sbin/ifconfig wlan0 | awk '{if ($1==\"inet\") printf(\"%s\", $2)}'", "r");
    fgets(wlan, 255, input);
    fclose(input);

    if (strlen(wlan) > 0) {
        lcdPrintf(lcdHandle, "w=%s", wlan);
    } else {
        lcdPrintf(lcdHandle, "e=%s", eth0);
    }
    

}


int main(int argc, char **argv)
{
	int i;

	if (!commandLineOptions(argc, argv)) {
		return 1;
	}

	if (!setup()) {
		printf("setup failed\n");
		return 1;
	}


	printf("Press ^c to exit IP Clock\n");

	while (1) {
		updateClock();
        delay(1000);
	}


	return 0;
}
