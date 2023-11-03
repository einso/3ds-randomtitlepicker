#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <time.h>

int main()
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	amInit();
	amAppInit();
	time_t t;

	u32 readTitlesAmount;
	u64 readTitlesID[900] = {};
	Result WhyBroke;
	WhyBroke = AM_GetTitleList(&readTitlesAmount, MEDIATYPE_SD, 900, readTitlesID);
	for (int i = 0; i < readTitlesAmount; i++) {
  			printf("%016llx\n", readTitlesID[i]);
	}

	if (WhyBroke != 0) {
		printf("%ld\n", WhyBroke);
	}

	printf("\nRead title amount: %lu\n", readTitlesAmount);
	printf("\nMy awesome random app loader\n(NOT the Apt Chain Loader Sample)\n\n");
	printf("Press START to exit\n");
	

	srand((unsigned) time(&t));
	
randomPicker:
	u64 randomTitle = 0;
	while(randomTitle == 0){
		u32 randomTitlePicked = rand() % readTitlesAmount;

		unsigned char contentCategory = ((unsigned char*)(&readTitlesID[randomTitlePicked]))[4];

		switch (contentCategory)
		{
		case 0x00:
		case 0x02:
			randomTitle = readTitlesID[randomTitlePicked];
			break;
		default:
			printf("Discarded TitleID %016llx, rerolling...\n\n", readTitlesID[randomTitlePicked]);
		}
	}

	printf("Press A to chainload this random titleID:\n%016llx\n\n", randomTitle);
	printf("Press Y to reroll.\n\n");

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START)
			break;

		if (kDown & KEY_A) // If the A button got pressed, start the app launch
		{
			aptSetChainloader(randomTitle, MEDIATYPE_SD); // NOT the *EUR* camera app title ID anymore
			break;
		}

		if (kDown & KEY_Y)
		{
			printf("Rerolling!\n");
			goto randomPicker;
		}

	}
	
	amExit();
	gfxExit();
	return 0;
}
