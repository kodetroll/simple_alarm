/* alarm - A program to countdown a specified time (in seconds) and perform an action after
 * the timeout has expired. The action may be specified or may just be a warning displayed
 * to STDOUT. The actions are selectable via command line argument, as are the timeout value,
 * the 'increment' and various program arguments (verbose, quiet, debug, etc).
 *
 * (C) 2015 KB4OID Labs, A division of Kodetroll Heavy Industries
 * File: alarm.c
 * Version: 0.99
 * Author: Kodetroll (KB4OID - Steve McCarter)
 * Date Created: April 2015
 * Date Previous: None
 * Date Last: April 8 2015
 * Date Last: August 28 2017
 *
 * To Build (Compile): type 'make'
 *
 * Usage: './alarm <OPTIONS>'
 * Where:
 *  --verbose              Turns ON verbose mode!
 *  --quiet                Turns ON quiet mode!
 *  --debug                Turns ON debugging!
 *  --nodebug              Turns OFF debugging!
 *  --actions              Prints a list of possible actions and exits!
 *  --now                  Causes program to jump to actions immediately (now)!
 *  --help, -h             Displays help and exits.
 *  --version, -v          Displays version and exits.
 *  --increment, -i <TIME> Sets time increment (in Secs).
 *  --action, -a <ACTION>  Sets action.
 *  --time, -t <TIME>      Sets timeout time (in Secs).
 *  --wav, -w <WAV>        Sets alarm wavfile name.
 *
 * E.G. './alarm -a PLAY_RING -t 15 -i 5 --verbose --debug'
 *
 * Note:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "alarm.h"


static int verbose;
static int debug;
static int action;
static int actions;
static int now;

char* wavfile;
char* appname;
char* player = DEFAULT_PLAYER;
char tmpbuf[120];	// for getActionFile return value

/* print the copyright info */
void cpright(void)
{
	printf("%s (C) 2015-2017 KB4OID Labs, A division of Kodetroll Heavy Industries\n",appname);
}

/* print the copyright and usage(help) info */
void usage(void)
{
	cpright();
	printf("%s <OPTIONS>\n",appname);
	printf(" Where:\n");
	printf("  --verbose              Turns ON verbose mode! \n");
	printf("  --quiet                Turns ON quiet mode! \n");
	printf("  --debug                Turns ON debugging! \n");
	printf("  --nodebug              Turns OFF debugging! \n");
	printf("  --actions              Prints a list of possible actions and exits! \n");
	printf("  --now                  Causes program to jump to actions immediately (now)! \n");
	printf("  --help, -h             Displays help and exits. \n");
	printf("  --version, -v          Displays version and exits. \n");
	printf("  --increment, -i <TIME> Sets time increment (in Secs). \n");
	printf("  --action, -a <ACTION>  Sets action. \n");
	printf("  --time, -t <TIME>      Sets timeout time (in Secs). \n");
	printf("  --wav, -w <WAV>        Sets alarm wavfile name. \n");
	printf("\n");
	printf("Examples: \n");
	printf("   alarm --now -a PLAY_GLASS - Plays action 'PLAY_GLASS' right now.\n");
	printf("   alarm --actions - Show list of possible actions.\n");
	printf("   alarm --action PLAY_BEEP --time 120 - Play beep in about 120 seconds.\n");
	printf("\n");

}

/* print the copyright and version info */
void version(void)
{
	cpright();
	printf("Version %d.%d\n",VER_MAJOR,VER_MINOR);
}

/* return the ogg file for the specified actions, added to make loading dynamically easier in the future */
/* Eventually, this function will scan through a text file and load the filename associated
 * with the action and return that. For now, we will use the previously hardcoded examples.
 * These filenames should include the full path to the sound samples.
 */
char* getActionFile(int action)
{
	if (verbose)
		printf("Action: '%s'\n",getActionTypeName(action));

	switch(action)
	{
		case ACT_PLAY_WAV :	// wavfile
			return(wavfile);
			break;
		case ACT_PLAY_BEEP :	// /usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga
			return("/usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga");
			break;
		case ACT_PLAY_BEEPS :	// /usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga
			return("/usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga");
			break;
		case ACT_PLAY_DRIP :	// /usr/share/sounds/gnome/default/alerts/drip.ogg
			//return("/usr/share/sounds/gnome/default/alerts/drip.ogg");
			return("/usr/share/sounds/misc/drip.ogg");	// location of some sounds changed between versions
			break;
		case ACT_PLAY_SONAR :	// /usr/share/sounds/gnome/default/alerts/sonar.ogg
			//return("/usr/share/sounds/gnome/default/alerts/sonar.ogg");
			return("/usr/share/sounds/misc/sonar.ogg");	// location of some sounds changed between versions
			break;
		case ACT_PLAY_GLASS :	// /usr/share/sounds/gnome/default/alerts/glass.ogg
			//return("/usr/share/sounds/gnome/default/alerts/glass.ogg");
			return("/usr/share/sounds/misc/glass.ogg");	// location of some sounds changed between versions
			break;
		case ACT_PLAY_BELL :	// /usr/share/sounds/ubuntu/stereo/bell.ogg
			return("/usr/share/sounds/ubuntu/stereo/bell.ogg");
			break;
		case ACT_PLAY_POP :	// /usr/share/sounds/ubuntu/stereo/message.ogg
			return("/usr/share/sounds/ubuntu/stereo/message.ogg");
			break;
		case ACT_PLAY_ERRR :	// /usr/share/sounds/freedesktop/stereo/suspend-error.oga
			return("/usr/share/sounds/freedesktop/stereo/suspend-error.oga");
			break;
		case ACT_PLAY_RING :	// /usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga
			return("/usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga");
			break;
		case ACT_NONE :
		default :
			return("");
			break;
	}
}

/* take the specified action */
void Action(int action)
{
	char tbuf[120];

	if (verbose)
		printf("Action: '%s'\n",getActionTypeName(action));

	switch(action)
	{
		case ACT_PLAY_WAV :	// play wavfile
			sprintf(tbuf,"aplay %s",getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_BEEP :	// Play BEEP
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_BEEPS :	// Play BEEPs
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_DRIP :	// Play Drip
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_SONAR :	// Play Sonar
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_GLASS :	// Play Glass
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_BELL :	// Play Bell
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_POP :	// Play Pop
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_ERRR :	// Play Errr
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_PLAY_RING :	// Play Ring
			sprintf(tbuf,"%s %s",player,getActionFile(action));
			system(tbuf);
			break;
		case ACT_NONE :	// Do nothing
		default :
			printf("No ACTION Selected!\n");
			break;
	}
}

/* get action type from name, returns the enumerated int of the action specified by the char string name */
int getActionTypeFromName(char* atype)
{
	if (strcmp(atype,"NONE") == 0)
		return(ACT_NONE);
	else if (strcmp(atype,"PLAY_WAV") == 0)
		return(ACT_PLAY_WAV);
	else if (strcmp(atype,"PLAY_BEEP") == 0)
		return(ACT_PLAY_BEEP);
	else if (strcmp(atype,"PLAY_BEEPS") == 0)
		return(ACT_PLAY_BEEPS);
	else if (strcmp(atype,"PLAY_DRIP") == 0)
		return(ACT_PLAY_DRIP);
	else if (strcmp(atype,"PLAY_SONAR") == 0)
		return(ACT_PLAY_SONAR);
	else if (strcmp(atype,"PLAY_GLASS") == 0)
		return(ACT_PLAY_GLASS);
	else if (strcmp(atype,"PLAY_BELL") == 0)
		return(ACT_PLAY_BELL);
	else if (strcmp(atype,"PLAY_POP") == 0)
		return(ACT_PLAY_POP);
	else if (strcmp(atype,"PLAY_ERRR") == 0)
		return(ACT_PLAY_ERRR);
	else if (strcmp(atype,"PLAY_RING") == 0)
		return(ACT_PLAY_RING);
	else
		return(ACT_NONE);
}

/* get action type from label, returns the enumerated int of the action specified by the char string label */
int getActionTypeFromLabel(char* atype)
{
	if (strcmp(atype,"ACT_NONE") == 0)
		return(ACT_NONE);
	else if (strcmp(atype,"ACT_PLAY_WAV") == 0)
		return(ACT_PLAY_WAV);
	else if (strcmp(atype,"ACT_PLAY_BEEP") == 0)
		return(ACT_PLAY_BEEP);
	else if (strcmp(atype,"ACT_PLAY_BEEPS") == 0)
		return(ACT_PLAY_BEEPS);
	else if (strcmp(atype,"ACT_PLAY_DRIP") == 0)
		return(ACT_PLAY_DRIP);
	else if (strcmp(atype,"ACT_PLAY_SONAR") == 0)
		return(ACT_PLAY_SONAR);
	else if (strcmp(atype,"ACT_PLAY_GLASS") == 0)
		return(ACT_PLAY_GLASS);
	else if (strcmp(atype,"ACT_PLAY_BELL") == 0)
		return(ACT_PLAY_BELL);
	else if (strcmp(atype,"ACT_PLAY_POP") == 0)
		return(ACT_PLAY_POP);
	else if (strcmp(atype,"ACT_PLAY_ERRR") == 0)
		return(ACT_PLAY_ERRR);
	else if (strcmp(atype,"ACT_PLAY_RING") == 0)
		return(ACT_PLAY_RING);
	else
		return(ACT_NONE);
}

/* get action type name, returns the char string describing the action specified by the provided int */
char* getActionTypeName(int action)
{
	switch(action)
	{
		case ACT_PLAY_WAV :
			return("ACT_PLAY_WAV");
			break;
		case ACT_PLAY_BEEP :
			return("ACT_PLAY_BEEP");
			break;
		case ACT_PLAY_BEEPS :
			return("ACT_PLAY_BEEPS");
			break;
		case ACT_PLAY_DRIP :
			return("ACT_PLAY_DRIP");
			break;
		case ACT_PLAY_SONAR :
			return("ACT_PLAY_SONAR");
			break;
		case ACT_PLAY_GLASS :
			return("ACT_PLAY_GLASS");
			break;
		case ACT_PLAY_BELL :
			return("ACT_PLAY_BELL");
			break;
		case ACT_PLAY_POP :
			return("ACT_PLAY_POP");
			break;
		case ACT_PLAY_ERRR :
			return("ACT_PLAY_ERRR");
			break;
		case ACT_PLAY_RING :
			return("ACT_PLAY_RING");
			break;
		case ACT_NONE :
			return("ACT_NONE");
			break;
		default :
			return("ACT_UNK");
			break;
	}

}

/* get action name, returns the char string name of the action specified by the provided int */
char* getActionName(int action)
{
	switch(action)
	{
		case ACT_PLAY_WAV :
			return("PLAY_WAV");
			break;
		case ACT_PLAY_BEEP :
			return("PLAY_BEEP");
			break;
		case ACT_PLAY_BEEPS :
			return("PLAY_BEEPS");
			break;
		case ACT_PLAY_DRIP :
			return("PLAY_DRIP");
			break;
		case ACT_PLAY_SONAR :
			return("PLAY_SONAR");
			break;
		case ACT_PLAY_GLASS :
			return("PLAY_GLASS");
			break;
		case ACT_PLAY_BELL :
			return("PLAY_BELL");
			break;
		case ACT_PLAY_POP :
			return("PLAY_POP");
			break;
		case ACT_PLAY_ERRR :
			return("PLAY_ERRR");
			break;
		case ACT_PLAY_RING :
			return("PLAY_RING");
			break;
		case ACT_NONE :
			return("NONE");
			break;
		default :
			return("UNKNOWN");
			break;
	}

}

int main(int argc, char* argv[])
{
	long timeout = 5;
	long timer = time(NULL);	// sets timer to current time
	int c;
	int i;

	long t_inc = 10;

	appname = strdup(argv[0]);

	verbose = 0;
	debug = 0;
	action = ACT_NONE;
	wavfile = strdup(DEFAULT_WAVFILE);

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,     &verbose, 1},
			{"quiet",   no_argument,     &verbose, 0},
			{"debug",   no_argument,       &debug, 1},
			{"nodebug", no_argument,       &debug, 0},
			{"actions", no_argument,      &actions, 1},
			{"now",     no_argument,          &now, 1},
			/* These options don’t set a flag.
			   We distinguish them by their indices. */
			{"help",       no_argument,       0, 'h'},
			{"version",    no_argument,       0, 'v'},
			{"increment",  required_argument, 0, 'i'},
			{"action",     required_argument, 0, 'a'},
			{"time",       required_argument, 0, 't'},
			{"wav",        required_argument, 0, 'w'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "hvi:a:t:w:",
			long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 'h':	// show help and exit
				if (debug)
					puts ("option -h\n");
				usage();
				exit(0);
				break;

			case 'v':	// show version and exit
				if (debug)
					puts ("option -v\n");
				version();
				exit(0);
				break;

			case 'i':	// time increment
				if (debug)
					printf ("option -i with value `%s'\n", optarg);
				t_inc = atol(optarg);
				if (verbose)
					printf("Setting increment to %ld Seconds\n",t_inc);
				break;

			case 'a':	// action on timeout
				if (debug)
					printf ("option -a with value `%s'\n", optarg);
				action = getActionTypeFromName(optarg);
				if (verbose)
					printf("Setting action to '%s'\n",getActionTypeName(action));
				break;

			case 't':	// Timeout time
				if (debug)
					printf ("option -t with value `%s'\n", optarg);
				timeout = atol(optarg);
				if (verbose)
					printf("Setting timeout to %ld Seconds\n",timeout);
				break;

			case 'w':	// WAV file name
				if (debug)
					printf ("option -w with value `%s'\n", optarg);
				wavfile = strdup(optarg);
				if (verbose)
					printf("Setting wavfile name to '%s'\n",wavfile);
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	}

	if (verbose)
		cpright();

	/* Instead of reporting ‘--verbose’
	   and ‘--brief’ as they are encountered,
	   we report the final status resulting from them. */
	if (debug)
		if (verbose)
			puts ("verbose flag is set");

	if (debug)
		puts ("debug flag is set");

	if (debug)
	{
		printf("action: '%s'\n",getActionTypeName(action));
		printf("actions: %d\n",actions);
		printf("now: %d\n",now);
		printf("t_inc: %ld\n",t_inc);
		printf("timeout: %ld\n",timeout);
		printf("wavfile: '%s'\n",wavfile);
	}

	if (actions)
	{
		printf("Possible actions are: ");
		for (i=ACT_NONE;i<=ACT_PLAY_RING;i++)
			printf("%s, ",getActionName(i));
		printf("\n");
		exit(0);
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}

	// add timeout value to current time
	timer += timeout;
	if (debug)
		printf("Timer set for: %ld Secs\n",timer);

	if (verbose)
		printf("Starting with %ld Secs Left!\n",timeout);

	if (now)
		timer = time(NULL);

	do
	{
		timeout = timer-time(NULL);
		if (timeout > t_inc)
		{
			if ((timeout % t_inc) == 0)
				if (verbose)
					printf("%ld Secs Left!\n",timeout);
		}
		else
			if (verbose)
				printf("%ld Secs Left!\n",timeout);
		usleep(LOOP_DELAY);
	} while(time(NULL) < timer);

	if (verbose)
		printf("Done!\n");

	if (action != ACT_NONE)
		Action(action);

	exit(0);
}

