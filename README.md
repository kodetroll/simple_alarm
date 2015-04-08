ALARM
=====

INTRO
-----
alarm - A program to countdown a specified time (in seconds) and perform an 
action after the timeout has expired. The action may be specified or may just 
be a warning displayed to STDOUT. The actions are selectable via command line 
argument, as are the timeout value, the 'increment' and various program 
arguments (verbose, quiet, debug, etc).

LICENSE
-------
See the LICENSE file for more information on the license


Version: 0.99
(C) 2015 KB4OID Labs, A division of Kodetroll Heavy Industries
Author: Kodetroll (KB4OID - Steve McCarter)
Date Created: April 2015
Date Previous: None
Date Last: April 8 2015

INSTALLING
----------
To build (compile): type 'make', see the INSTALL file for more info.

USAGE
-----

Usage: './alarm <OPTIONS>'
Where:
 --verbose              Turns ON verbose mode!
 --quiet                Turns ON quiet mode!
 --debug                Turns ON debugging!
 --nodebug              Turns OFF debugging!
 --actions              Prints a list of possible actions and exits!
 --help, -h             Displays help and exits.
 --version, -v          Displays version and exits.
 --increment, -i <TIME> Sets time increment (in Secs).
 --action, -a <ACTION>  Sets action.
 --time, -t <TIME>      Sets timeout time (in Secs).
 --wav, -w <WAV>        Sets alarm wavfile name.

e.g. type './alarm -a PLAY_RING -t 15 -i 5 --verbose --debug'

This application uses aplay to play wav files and ogg123 to play ogg files.
These applications must be installed in order for the actions to work.
To get aplay, install the alsa package, for ogg123, install the vorbis-tools
package.

ACTIONS
-------
The possible set of actions that may be specified with the --action or -a
command line argument are as follows (Spelling and case are significant):
 NONE - Takes no action
 PLAY_WAV   - Plays the specified (or default) wav file using aplay
 PLAY_BEEP  - Plays phone-outgoing-calling.oga from freedesktop
 PLAY_BEEPS - Plays phone-outgoing-busy.oga from freedekstop
 PLAY_DRIP  - Plays drip.ogg from gnome
 PLAY_SONAR - Plays sonar.ogg from gnome
 PLAY_GLASS - Plays glass.ogg from gnome
 PLAY_BELL  - Plays bell.ogg from ubuntu
 PLAY_POP   - Plays message.ogg from ubuntu
 PLAY_ERRR  - Plays suspend-error.oga from freedesktop
 PLAY_RING  - Plays phone-incoming-call.oga from freedesktop

Note: 
1) ogg and oga files are played using ogg123.
2) wav files using aplay.
3) freedesktop sounds are found in: /usr/share/sounds/freedesktop/stereo/
4) ubuntu sounds are found in: /usr/share/sounds/ubuntu/stereo/
5) gnome sounds are found in: /usr/share/sounds/gnome/default/alerts/

  Copyright (C) 2015 KB4OID Labs, A division of Kodetroll Heavy Industries
