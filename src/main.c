#include "sv_utils/system_states.h"
#include "sv_utils/sound_visualizer.h"

int main()
{
	int system_state = INITIALIZE;

	//Main program loop
	unsigned long avgADCValue = 0;
	while(1)
	{
		switch(system_state)
		{
			case IDLE:
				system_state = GetSystemState();
				break;

			case BUFFER_1_COMPLETE:
				avgADCValue = GetBufOneAvg();
				Display(avgADCValue);
				system_state = IDLE;
				break;

			case BUFFER_2_COMPLETE:
				avgADCValue = GetBufTwoAvg();
				Display(avgADCValue);
				system_state = IDLE;
				break;

			case INITIALIZE:
				Initialize();
				system_state = IDLE;
				break;
		}
	}
}
