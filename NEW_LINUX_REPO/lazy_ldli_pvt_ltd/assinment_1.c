#include <stdint.h>
#include <stdio.h>

#define NUM_DIGITAL_INPUTS 8

int ISR_DIsampling(void);
volatile uint8_t g_ReadDIpinSts = 0; // real-time input pin status
volatile uint8_t g_AppDIpinSts = 0;  // Stable pin status

static uint8_t olddata = 0; // Previous state of digital input pins
static uint8_t switchcounter[NUM_DIGITAL_INPUTS] = {0}; // Counters for consistent state



// ISR function to sample digital inputs
int ISR_DIsampling() {
	for (int i = 0; i < NUM_DIGITAL_INPUTS; i++) {
	
		uint8_t currentstatus= (g_ReadDIpinSts >> i) & 0x01;
         printf("cur- : 0x%02X  pre- : 0x%02X\n", currentstatus,olddata);
	
		if (currentstatus == ((olddata >> i) & 0x01)) {
		
			switchcounter[i]++;
			printf("i->%d  switch-%d \t",i,switchcounter[i]);
			if (switchcounter[i] >= 10) {
				// check for the counter reaches 10
				g_AppDIpinSts |= (1 << i);
				printf("update-%0.3X\n",g_AppDIpinSts);
			}
		} else {
			// If not reset the counter
			switchcounter[i] = 0;
		}

		// Update the previous state with the current state
		olddata = (olddata & ~(1 << i)) | (currentstatus << i);
	}
 printf("%d-> Stable Digital Input Status: 0x%03X\n",g_ReadDIpinSts, g_AppDIpinSts);
	return 0;
}

int main() {
	// the ISR being called every 100ms
	for (int i = 0; i < 100; i++) {
		g_ReadDIpinSts =i; // assume changing pin status
		ISR_DIsampling();
	}

	// Print stable pin status
	printf("Stable Digital Input Status: 0x%02X\n", g_AppDIpinSts);

	return 0;
}
