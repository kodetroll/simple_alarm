/* alarm - A program to countdown a specified time (in seconds) and perform an action after
 * the timeout has expired. The action may be specified or may just be a warning displayed
 * to STDOUT. The actions are selectable via command line argument, as are the timeout value,
 * the 'increment' and various program arguments (verbose, quiet, debug, etc).
 *
 *
 * Version: 0.99
 * (C) 2015 KB4OID Labs, A division of Kodetroll Heavy Industries
 * Author: Kodetroll (KB4OID - Steve McCarter)
 * Date Created: April 2015
 * Date Previous: None
 * Date Last: April 8 2015
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
//#include "alarm.c"

#define VER_MAJOR 0
#define VER_MINOR 99

#define LOOP_DELAY 1000000

#define DEFAULT_WAVFILE "./test"

static int verbose;
static int debug;
static int action;
static int actions;
char* wavfile;

void usage(char * appname);
void version(void);
void Action(int action);
int getActionTypeFromName(char* atype);
int getActionTypeFromLabel(char* atype);
char* getActionTypeName(int action);

void usage(char * appname)
{
	printf("%s <OPTIONS>\n",appname);
	printf(" Where:\n");
	printf("  --verbose              Turns ON verbose mode! \n");
	printf("  --quiet                Turns ON quiet mode! \n");
	printf("  --debug                Turns ON debugging! \n");
	printf("  --nodebug              Turns OFF debugging! \n");
	printf("  --actions              Prints a list of possible actions and exits! \n");
	printf("  --help, -h             Displays help and exits. \n");
	printf("  --version, -v          Displays version and exits. \n");
	printf("  --increment, -i <TIME> Sets time increment (in Secs). \n");
	printf("  --action, -a <ACTION>  Sets action. \n");
	printf("  --time, -t <TIME>      Sets timeout time (in Secs). \n");
	printf("  --wav, -w <WAV>        Sets alarm wavfile name. \n");
	printf("\n");

}

void version(void)
{
	printf("Version %d.%d\n",VER_MAJOR,VER_MINOR);
}

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

void Action(int action)
{
	char tbuf[120];

	if (verbose)
		printf("Action: '%s'\n",getActionTypeName(action));

	switch(action)
	{
		case ACT_PLAY_WAV :	// aplay wavfile
			sprintf(tbuf,"aplay %s",wavfile);
			system(tbuf);
			break;
		case ACT_PLAY_BEEP :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga
			system("ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga");
			break;
		case ACT_PLAY_BEEPS :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga
			system("ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga");
			break;
		case ACT_PLAY_DRIP :	// ogg123 /usr/share/sounds/gnome/default/alerts/drip.ogg
			system("ogg123 /usr/share/sounds/gnome/default/alerts/drip.ogg");
			break;
		case ACT_PLAY_SONAR :	// ogg123 /usr/share/sounds/gnome/default/alerts/sonar.ogg
			system("ogg123 /usr/share/sounds/gnome/default/alerts/sonar.ogg");
			break;
		case ACT_PLAY_GLASS :	// ogg123 /usr/share/sounds/gnome/default/alerts/glass.ogg
			system("ogg123 /usr/share/sounds/gnome/default/alerts/glass.ogg");
			break;
		case ACT_PLAY_BELL :	// ogg123 /usr/share/sounds/ubuntu/stereo/bell.ogg
			system("ogg123 /usr/share/sounds/ubuntu/stereo/bell.ogg");
			break;
		case ACT_PLAY_POP :	// ogg123 /usr/share/sounds/ubuntu/stereo/message.ogg
			system("ogg123 /usr/share/sounds/ubuntu/stereo/message.ogg");
			break;
		case ACT_PLAY_ERRR :	// ogg123 /usr/share/sounds/freedesktop/stereo/suspend-error.oga
			system("ogg123 /usr/share/sounds/freedesktop/stereo/suspend-error.oga ");
			break;
		case ACT_PLAY_RING :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga
			system("ogg123 /usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga	");
			break;
		case ACT_NONE :
		default :
			printf("No ACTION Selected!\n");
			break;
	}
}

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

char* getActionTypeName(int action)
{
	switch(action)
	{
		case ACT_PLAY_WAV :	// aplay wavfile
			return("ACT_PLAY_WAV");
			break;
		case ACT_PLAY_BEEP :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-calling.oga
			return("ACT_PLAY_BEEP");
			break;
		case ACT_PLAY_BEEPS :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-outgoing-busy.oga
			return("ACT_PLAY_BEEPS");
			break;
		case ACT_PLAY_DRIP :	// ogg123 /usr/share/sounds/gnome/default/alerts/drip.ogg
			return("ACT_PLAY_DRIP");
			break;
		case ACT_PLAY_SONAR :	// ogg123 /usr/share/sounds/gnome/default/alerts/sonar.ogg
			return("ACT_PLAY_SONAR");
			break;
		case ACT_PLAY_GLASS :	// ogg123 /usr/share/sounds/gnome/default/alerts/glass.ogg
			return("ACT_PLAY_GLASS");
			break;
		case ACT_PLAY_BELL :	// ogg123 /usr/share/sounds/ubuntu/stereo/bell.ogg
			return("ACT_PLAY_BELL");
			break;
		case ACT_PLAY_POP :	// ogg123 /usr/share/sounds/ubuntu/stereo/message.ogg
			return("ACT_PLAY_POP");
			break;
		case ACT_PLAY_ERRR :	// ogg123 /usr/share/sounds/freedesktop/stereo/suspend-error.oga
			return("ACT_PLAY_ERRR");
			break;
		case ACT_PLAY_RING :	// ogg123 /usr/share/sounds/freedesktop/stereo/phone-incoming-call.oga
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

int main(int argc, char* argv[])
{
	long timeout = 5;
	long timer = time(NULL);
	int c;
	int i;

	long t_inc = 10;

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
				usage(argv[0]);
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
		printf("t_inc: %ld\n",t_inc);
		printf("timeout: %ld\n",timeout);
		printf("wavfile: '%s'\n",wavfile);
	}

	if (actions)
	{
		printf("Actions are:\n");
		for (i=ACT_NONE;i<=ACT_PLAY_RING;i++)
			printf("%s\n",getActionTypeName(i));
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

	timer += timeout;
	if (debug)
		printf("Timer set for: %ld Secs\n",timer);

	if (verbose)
		printf("Starting with %ld Secs Left!\n",timeout);

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

