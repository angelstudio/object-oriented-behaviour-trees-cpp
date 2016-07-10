/*
	basictimers_h: Defines portable functions for pausing execution
	and determining if a specified interval has elapsed.
*/

#ifndef __BASICTIMERS_H__
#define __BASICTIMERS_H__

/*	Constant number of milliseconds in a second. */
#define MILLISECONDS 1000

/*	Data structure to keep track of elapsed time. */
typedef struct {
	double reset_time;
	long milliseconds;
} basic_timer_t;

/*	Data type to represent unique timer ID. */
typedef basic_timer_t * basictimers_timer_id;

/*
 *	basictimers_create_timer:
 *
 *	Creates a new timer and sets it up with the required interval.
 *
 *	Input:
 *	-  milliseconds: the length of the desired interval between reset and expiry.
 *
 *	Output:
 *		Returns a non-negative ID number that can be used to check the status 
 *		of the timer if fewer than TIMER_MAX timers have been created. Otherwise, 
 *		returns TIMER_ERROR. 
 */

basictimers_timer_id basictimers_create_timer( long milliseconds );

/*
 *	basictimers_reset_timer:
 *
 *	Reset a timer to start a new interval.
 *
 *	Input:
 *	-	timer: the address of a timer which is to be reset.
 *
 *	Output: void.
 */

void basictimers_reset_timer( basictimers_timer_id id );

/*
 *	basictimers_timer_expired:
 *
 *	Checks a timer to see if the associated interval has passed. If the interval has
 *	elapsed, the timer is reset automatically, ready to start again.
 *
 *	Input:
 *	-	id: The ID of a timer to check and update.
 *
 *	Output:
 *		Returns TRUE if and only if the interval had elapsed.
 */

int basictimers_timer_expired( basictimers_timer_id timer );

/**
	Simply wraps free.
*/
void basictimers_delete_timer(basictimers_timer_id timer);

/*	
 *	basictimers_timer_pause:
 *
 *	Pauses execution in a system-friendly way to allow other processes to work and
 *	conserve clock cycles.
 *
 *	Input:
 *		milliseconds:	The duration of the desired pause.
 *
 *	Output: void.
 */

void basictimers_timer_pause( long milliseconds );

/*
 *	basictimers_get_current_time:
 *
 *	Gets an estimate of the elapsed system time.
 *
 *	Input: no input.
 *
 *	Output: Returns the current system time in measured in whole and fractional seconds.
 */

double basictimers_get_current_time();

#endif
