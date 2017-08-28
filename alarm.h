/* alarm - A program to countdown a specified time (in seconds) and perform an action after
 * the timeout has expired. The action may be specified or may just be a warning displayed
 * to STDOUT. The actions are selectable via command line argument, as are the timeout value,
 * the 'increment' and various program arguments (verbose, quiet, debug, etc).
 *
 *
 * (C) 2015 KB4OID Labs, A division of Kodetroll Heavy Industries
 * File: alarm.h
 * Version: 0.99
 * Author: Kodetroll (KB4OID - Steve McCarter)
 * Date Created: April 2015
 * Date Previous: None
 * Date Last: April 8 2015
 * Date Last: August 28 2017
 */

#ifndef __ALARM_H__
#define __ALARM_H__

#define VER_MAJOR 0
#define VER_MINOR 99

#define LOOP_DELAY 1000000

#define DEFAULT_WAVFILE "./test"

#define DEFAULT_PLAYER "ogg123 -q"

enum Actions {
	ACT_NONE,
	ACT_PLAY_WAV,	// aplay wavfile
	ACT_PLAY_BEEP,	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga
	ACT_PLAY_BEEPS,	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga
	ACT_PLAY_DRIP,	// ogg123 /usr/share/sounds/gnome/default/alerts/drip.ogg
	ACT_PLAY_SONAR,	// ogg123 /usr/share/sounds/gnome/default/alerts/sonar.ogg
	ACT_PLAY_GLASS,	// ogg123 /usr/share/sounds/gnome/default/alerts/glass.ogg
	ACT_PLAY_BELL,	// ogg123 /usr/share/sounds/ubuntu/stereo/bell.ogg
	ACT_PLAY_POP,	// ogg123 /usr/share/sounds/ubuntu/stereo/message.ogg
	ACT_PLAY_ERRR,	// ogg123 /usr/share/sounds/freedesktop/stereo/suspend-error.oga
	ACT_PLAY_RING,	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga
};

void cpright(void);
void usage(void);
void version(void);
char* getActionFile(int action);
void Action(int action);
int getActionTypeFromName(char* atype);
int getActionTypeFromLabel(char* atype);
char* getActionTypeName(int action);

#endif	// __ALARM_H__