/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file for digital and     */
/*                analog square wave generation     */
/*                via the LabJack U3-LV USB DAQ     */
/*                                                  */
/****************************************************/

#include "u3.h"

#include <unistd.h>

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include <time.h>

#include <signal.h>


HANDLE global_hDevice;
u3CalibrationInfo * caliInfo;
double max_voltage;
double min_voltage;

int high_dig = 1;
int high_ana = 1;

/* This function should initialize the DAQ and return a device
 * handle. The function takes as a parameter the calibratrion info to
 * be filled up with what obtained from the device. */
HANDLE init_DAQ(u3CalibrationInfo * caliInfo2) {
        HANDLE hDevice;
        int localID = -1; // will attempt to open the first available USB device 

        /* Invoke openUSBConnection function here */
        hDevice = openUSBConnection(localID);

        if (hDevice == NULL) {
                printf("No handle for hDevice %d\n", localID);
                return NULL;
        }

        /* Invoke getCalibrationInfo function here */
        long res = getCalibrationInfo(hDevice, caliInfo2);
        if (res < 0) {
                printf("Error in getting calibration info\n");
        }

        return hDevice;
}

void handler_function_ANA(int signum) {
        if (signum == SIGRTMAX) {
        	//printf("SIGRTMAX received\n");
        	if (high_ana) {
                	eDAC(global_hDevice, caliInfo, 0, 0, max_voltage, 0, 0, 0); // handle, caliInfo, configio, channel, voltage, binary, reserved1, reserved2
                	high_ana = 0;
                } else {
                	eDAC(global_hDevice, caliInfo, 0, 0, min_voltage, 0, 0, 0);
                	high_ana = 1;
                }
        }
}


void handler_function_DIG(int signum) {
        if (signum == SIGRTMIN) {
        	//printf("SIGRTMIN received\n");
                if (high_dig) {
                	eDO(global_hDevice, 0, 2, 1);
                	high_dig = 0;
                } else {
                	eDO(global_hDevice, 0, 2, 0);
                	high_dig = 1;
                }
	
        }
}

int main(int argc, char ** argv) {
        float wave_freq = -1;
        char display_analog = 0;
        char display_digital = 0;
        caliInfo = malloc(sizeof(u3CalibrationInfo));

        /* Invoke init_DAQ and handle errors if needed */
        global_hDevice = init_DAQ(caliInfo);

        printf("Calibration done\n");

        /* Provide prompt to the user for a voltage range between 0
         * and 5 V. Require a new set of inputs if an invalid range
         * has been entered. */
        printf("Please provide a max voltage range between 0 and 5 V.\n");
        scanf("%lf", &max_voltage);

        printf("Please provide a min voltage range between 0 and 5 V.\n");
        scanf("%lf", &min_voltage);

        printf("min_voltage: %f, max_voltage: %f\n", min_voltage, max_voltage);

        /* Compute the maximum resolutiuon of the CLOCK_REALTIME
         * system clock and output the theoretical maximum frequency
         * for a square wave */
        struct timespec * res = malloc(sizeof(struct timespec));
        if (res == NULL) {
                perror("malloc failed");
                return 1;
        }

        memset(res, 0, sizeof(struct timespec));
        clock_getres(CLOCK_REALTIME, res);
        double hz = 1.0 / (res -> tv_sec + (res -> tv_nsec * 1e-9));
        printf("CLOCK_REALTIME resolution is: %.6f Hz\n", hz);

        /* Provide prompt to the user to input a desired square wave
         * frequency in Hz. */
        printf("Please provide a desired square wave frequency in Hz:\n");
        scanf("%f", &wave_freq);

        printf("Do you want to generate an analog wave? [Y/N]: ");
        scanf(" %c", &display_analog);

        printf("Do you want to generate a digital wave? [Y/N]: ");
        scanf(" %c", &display_digital);
        
        
        // used in both signal handlers
        sigset_t mask, wait_mask;
        struct itimerspec timer2_time;
	double period_sec = 1.0 / wave_freq;
	long long seconds = (long long) period_sec;
	long nanoseconds = (long)((period_sec - seconds) * 1e9);
        
        /* Program a timer to deliver a SIGRTMIN signal, and the
         * corresponding signal handler to output a square wave on
         * BOTH digital output pin FIO2 and analog pin DAC0. */
        if (tolower(display_digital) == 'y') {
        	printf("Setting up for digital signal\n");
		timer_t timer2;
		
		
		if (timer_create(CLOCK_REALTIME, NULL, &timer2) != 0) {
			perror("timer_create");
			exit(1);
		}

		
		// initialize signal handler
		struct sigaction sa;
		struct sigevent timer2_event;
		int res0 = -1;
		
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = handler_function_DIG;

		sigemptyset(&mask);
		sigemptyset(&wait_mask);
		sigemptyset(&sa.sa_mask);
		
		sigaddset(&sa.sa_mask, SIGRTMIN);

		/* Work on mask: singals to be blocked before sigsuspend */
		sigaddset(&mask, SIGRTMIN);	
	 	sigprocmask(SIG_BLOCK, &mask, NULL);
	 	
	 	sigaction(SIGRTMIN, &sa, NULL);

		// Zero out the structure and configure for use of the SIGRTMIN signal.
		memset(&timer2_event, 0, sizeof(timer2_event));

		// send SIGRTMIN to the process
		timer2_event.sigev_notify = SIGEV_SIGNAL;
		timer2_event.sigev_signo = SIGRTMIN;
		/* We are part of the elite, right? */
		timer2_event.sigev_value.sival_int = 1336;
		
		res0 = timer_create(CLOCK_REALTIME, &timer2_event, &timer2);
		
		if (res0 < 0) {
		    perror("Unable to create real-time timer");
		    return EXIT_FAILURE;
		}

		// setting iter timer
		timer2_time.it_interval.tv_sec = seconds;
		timer2_time.it_interval.tv_nsec = nanoseconds;
		timer2_time.it_value.tv_sec = seconds;
		timer2_time.it_value.tv_nsec = nanoseconds;

		// Schedule the timer.
		if (timer_settime(timer2, 0, &timer2_time, NULL) != 0) {
		        // If there is an error, print out a message and exit.
		        perror("timer_settime");
		        exit(1);
		}
		
		printf("Set up for digital signal\n");
	}

        
        if (tolower(display_analog) == 'y') {
        	printf("Setting up for analog\n");
		/* Program a timer to deliver a SIGRTMAX signal, and the
		 * corresponding signal handler to output a square wave on
		 * BOTH digital output pin FIO2 and analog pin DAC0. */
		timer_t timer1;
		if (timer_create(CLOCK_REALTIME, NULL, &timer1) != 0) {
			perror("timer_create");
			exit(1);
		}

		
		// initialize signal handler
		struct sigaction sa1;
		struct sigevent timer1_event;
		int res1 = -1;
		
		sa1.sa_flags = SA_SIGINFO;
		sa1.sa_sigaction = handler_function_ANA;

		sigemptyset(&sa1.sa_mask);
		sigaddset(&sa1.sa_mask, SIGRTMAX);


		/* Work on mask: singals to be blocked before sigsuspend */
		sigaddset(&mask, SIGRTMAX);	
	 	sigprocmask(SIG_BLOCK, &mask, NULL);
	 	
	 	sigaction(SIGRTMAX, &sa1, NULL);

		// Zero out the structure and configure for use of the SIGRTMIN signal.
		memset(&timer1_event, 0, sizeof(timer1_event));

		// send SIGRTMIN to the process
		timer1_event.sigev_notify = SIGEV_SIGNAL;
		timer1_event.sigev_signo = SIGRTMAX;
		/* We are part of the elite, right? */
		timer1_event.sigev_value.sival_int = 1337;
		
		res1 = timer_create(CLOCK_REALTIME, &timer1_event, &timer1);
		
		if (res1 < 0) {
		    perror("Unable to create real-time timer");
		    return EXIT_FAILURE;
		}
		
		// setting iter timer
		struct itimerspec timer1_time;
		timer1_time.it_interval.tv_sec = seconds;
		timer1_time.it_interval.tv_nsec = nanoseconds;
		timer1_time.it_value.tv_sec = seconds;
		timer1_time.it_value.tv_nsec = nanoseconds;

		// Schedule the timer.
		if (timer_settime(timer1, 0, &timer1_time, NULL) != 0) {
		        // If there is an error, print out a message and exit.
		        perror("timer_settime");
		        exit(1);
		}
		
		
		printf("Set up for analog\n");
	}
        
        int printed = 0;
	char str[100];
	while(1) {
		sigsuspend(&wait_mask);
		if (!printed) {
			printf("Enter 'exit' to end:\n");
			printed = 1;
		}
		scanf("%s", str);
		if (strcmp(str, "exit") == 0) {
		        printf("Bye! :)\n");
		        break;
		}
		
	}
	
	//response buffer is too small error

        /* The square wave generated on the DAC0 analog pin should
         * have the voltage range specified by the user in the step
         * above. */

        /* Display a prompt to the user such that if the "exit"
         * command is typed, the USB DAQ is released and the program
         * is terminated. */
        
        
	closeUSBConnection(global_hDevice);
	free(caliInfo);
        return EXIT_SUCCESS;
}
