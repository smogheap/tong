/*
 * This file is part of TONG.              http://www.nongnu.org/tong
 * Copyright 2004-2014 Owen Swerkstrom
 *
 * TONG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//TODO:  clean the hell out of this file, do convert surface on all gfx,
//       have pieces share one file/surface, new tetrad waits for break

#define GAME_ID "TONG 1.3 (c) 2004-2014 penduinbits GPLv3"

#ifdef WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_mixer.lib")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#ifdef WII
#include <fat.h>
#endif

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "tetris.h"
#include "pong.h"
#include "text.h"
#include "option.h"
#include "media.h"

#define KEY_DELAY           200
#define KEY_REPEAT          20

#define SCREEN_BLACK        0
#define SCREEN_SPLASH       1
#define SCREEN_TITLE        2
#define SCREEN_GAME         3
#define SCREEN_OPTION       4
#define SCREEN_CONTROL      5
#define SCREEN_HISCORES     6
#define SCREEN_GAMEOVER     7
#define SCREEN_CREDITS      8

#define DEMO_TIME           8000

#define GP2X_BUTTON_UP         0
#define GP2X_BUTTON_UPLEFT     1
#define GP2X_BUTTON_LEFT       2
#define GP2X_BUTTON_DOWNLEFT   3
#define GP2X_BUTTON_DOWN       4
#define GP2X_BUTTON_DOWNRIGHT  5
#define GP2X_BUTTON_RIGHT      6
#define GP2X_BUTTON_UPRIGHT    7
#define GP2X_BUTTON_START      8
#define GP2X_BUTTON_SELECT     9
#define GP2X_BUTTON_L         10
#define GP2X_BUTTON_R         11
#define GP2X_BUTTON_A         12
#define GP2X_BUTTON_B         13
#define GP2X_BUTTON_X         14
#define GP2X_BUTTON_Y         15
#define GP2X_BUTTON_VOLUP     16
#define GP2X_BUTTON_VOLDOWN   17
#define GP2X_BUTTON_CLICK     18

#define WII_BUTTON_A           0
#define WII_BUTTON_B           1
#define WII_BUTTON_1           2
#define WII_BUTTON_2           3
#define WII_BUTTON_MINUS       4
#define WII_BUTTON_PLUS        5
#define WII_BUTTON_HOME        6
#define WII_BUTTON_C           7
#define WII_BUTTON_Z           8
#define WII_BUTTON_a           9
#define WII_BUTTON_b          10
#define WII_BUTTON_x          11
#define WII_BUTTON_y          12
#define WII_BUTTON_l          13
#define WII_BUTTON_r          14
#define WII_BUTTON_zl         15
#define WII_BUTTON_zr         16
#define WII_BUTTON_minus      17
#define WII_BUTTON_plus       18
#define WII_BUTTON_home       19

SDL_Surface *black, *back, *zone, *temp, *title, *scorebg, *timebg, *timeup;
SDL_Surface *nextbg, *rulebg, *ballbounce, *ballignore, *balljoin, *balldie;
SDL_Surface *tetradmove, *tetradignore, *tetradjoin, *tetradbreak, *tetraddie;
SDL_Surface *tetradrotate, *tetraddrop, *stackbreak, *stackignore, *stackjoin;
SDL_Surface *stackdie, *stacklinebreak, *gameover, *youlose;
SDL_Surface *icon, *splash, *splashbehind;
Mix_Music *music;
Mix_Chunk *toing, *newrule, *twobits;
Mix_Chunk *rotate1, *rotate2, *plop, *bust, *tick;
Mix_Chunk *bounce1, *bounce2;

typedef struct input_t {
	int cw;
	int ccw;
	int tetdown;
	int tetleft;
	int tetright;
	int padx;
	int pady;
	int esc;
	int up;
	int down;
	int left;
	int right;
	int ok;
	int volup;
	int voldown;
	SDLKey any;
	Uint16 mod;
	int quit;
	struct state {
		struct repeat {
			Uint32 tetdown;
			Uint32 tetleft;
			Uint32 tetright;
		} repeat;
		int padmouse;
	} state;
} input_t;

bool drop_piece(Tetrad*, Tetrad*, Bucket*, Uint32&, bool&, bool&, Option*,
				int&, input_t *);
void clear_input(input_t *input);
int read_input(input_t *input, Option *options);

#ifdef WII
extern "C" int SDL_main(int argc, char **argv)
#else
int main(int argc, char* argv[])
#endif
{
	SDL_Surface *screen;
	SDL_Joystick *joy=NULL;
	SDL_Event event;
	Uint32 lastupdate=0, lastlastupdate=0, lastpush=0;
	Uint32 lastrulechange=0, lastscoreblurb=0, demostarted=0;
	Uint32 lastvolblurb = 0;
	int i=0, tempint=0;
	long int score=0;
	int display=SCREEN_BLACK;
	int nextdisplay=SCREEN_SPLASH;
	bool gameinprogress=false;
	bool marathon=true;
	bool gothighscore=false;
	int scoredisplay=SCORE_MARATHON;
	Uint32 timeleft=0, endtime=0, timespent=0, starttime=0, musicstarted=0;
	Uint32 timelength=300000; // not60000, 120000, 300000, 600000
	Uint32 creditsstart=0;
	Uint32 fadestart=0;
	bool fading=false;
	bool fadein=true;
	char timetext[10]="";
	int pushes=0;
	int ballstatus;
	int ballslost=0, bucketslost=0;
	bool quitgame=false;
	bool done=false;
	bool wasbreaking=false;
	bool newdown=false;
	bool ruleupdated=false; //temp
	int level_tethover=0;
	int lines=0;
	char scoretext[20] = "";
	char voltext[30] = "";
	char scorename[6]="_";
	Option *options;
	Text *scoreboard, *scoreblurb, *ruledisplay, *timedisplay, *highscores;
	Text *credits, *gameid, *volblurb;
	int creditlines=1;
	Menu *mainmenu;
	Menu *highscoremenu;
	Bucket *bucket;
	Tetrad *tetrad, *nexttetrad;
	Paddle *paddle;
	Ball *ball;
	char datadir[256];
	input_t input;
	int volume = MIX_MAX_VOLUME;

#ifdef GP2X
	volume = 24;
#endif
	memset(&input, 0, sizeof(input_t));
	strcpy(datadir, GAME_DATA_DIR);
	if(argc>1) {
		if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
			printf("%s\n", GAME_ID);
			printf("This is free software; see the source for"
				   " copying conditions.  There is NO\n"
				   "warranty; not even for MERCHANTABILITY or"
				   " FITNESS FOR A PARTICULAR PURPOSE.\n");
			return 0;
		} else if(!strncmp(argv[1], "-p=", 3)) {
			strcpy(datadir, argv[1]+(3*sizeof(char)));
		} else if(!strncmp(argv[1], "--path=", 7)) {
			strcpy(datadir, argv[1]+(7*sizeof(char)));
		} else {
			printf("Usage: %s [OPTION]\n", argv[0]);
			printf("Play TONG, a Free Software game (Tetris meets Pong)\n");
			printf("  -p, --path=DATADIR  override path to TONG data\n");
			printf("  -h, --help          display this help and exit\n");
			printf("  -v, --version       output version information"
				   " and exit\n");
			return 0;
		}
	}

#ifdef WII
	fatInitDefault();
#endif
	if (chdir(datadir)) {
		printf("Could not chdir to %s (attempting to use current dir)\n",
			   datadir);
	}

	if(SDL_Init(SDL_INIT_AUDIO
				|SDL_INIT_VIDEO
				|SDL_INIT_TIMER
				|SDL_INIT_JOYSTICK
				)<0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

#ifdef LOGCRAP
	FILE *foo;
	foo = fopen("out.txt", "w");
	fprintf(foo, "here yo");
	fclose(foo);
#endif

	options = new Option();
	scoreboard = new Text();
	scoreblurb = new Text();
	volblurb = new Text();
	ruledisplay = new Text();
	timedisplay = new Text();
	highscores = new Text();
	credits = new Text();
	gameid = new Text();
	bucket = new Bucket();
	tetrad = new Tetrad();
	nexttetrad = new Tetrad();
	paddle = new Paddle();
	ball = new Ball();

	atexit(SDL_Quit);
	if(SDL_NumJoysticks()) {
		printf("grabbing %s...\n", SDL_JoystickName(0));
		SDL_JoystickEventState(SDL_ENABLE);
		joy=SDL_JoystickOpen(0);
	}

	//let the randomness begin!
	srand((unsigned)time(NULL));

	options->ReadConfig();
	level_tethover=options->GetSpeedTetrad();

	screen=init_video(options->GetVideoBpp(), options->GetVideoFlags());

	volblurb->Init(16 / POSRATIO, 16 / POSRATIO, "Loading...");
	volblurb->Draw(screen);
	SDL_Flip(screen);

	load_graphics(options->GetThemeString());
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "loaded gpx");
fclose(foo);
#endif

//	SDL_EnableKeyRepeat(KEY_DELAY, KEY_REPEAT);
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WM_SetCaption("TONG!", NULL);
	SDL_WM_SetIcon(icon, NULL);

#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "menu?");
fclose(foo);
#endif
	mainmenu=new Menu();
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "mainmenu");
fclose(foo);
#endif
	mainmenu->SetPos(144 / POSRATIO, 176 / POSRATIO);
	mainmenu->AddChoice("Start Game!", NORMAL);
	mainmenu->AddChoice("Rules/Difficulty...", NORMAL);
	mainmenu->AddChoice("Controls...", NORMAL);
	mainmenu->AddChoice("Game Mode  = Marathon", NORMAL);
	mainmenu->AddChoice("Time Limit = 05:00", DISABLED);
	mainmenu->AddChoice("Graphics/Sound...", NORMAL);
	mainmenu->AddChoice("Credits", NORMAL);
	mainmenu->AddChoice("Quit", NORMAL);

	highscoremenu=new Menu();
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "highscoremenu");
fclose(foo);
#endif
	highscoremenu->SetPos(192 / POSRATIO, 192 / POSRATIO);
	highscoremenu->AddChoice("  HIGH SCORE!", NORMAL);
	highscoremenu->AddChoice("Enter your name", NORMAL);
	highscoremenu->AddChoice("     _", HIGHLIGHTED);
	highscoremenu->SetActive(true);
	highscoremenu->SetChoice(3);

	tetrad->Init(rand()%7);
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "tetrad init");
fclose(foo);
#endif
	level_tethover=options->GetSpeedTetrad();
	nexttetrad->Init(rand()%7);
	bucket->Init(options->GetJunk());
	ball->Init(options->GetSpeedBall());
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "ball init");
fclose(foo);
#endif
	paddle->Init(options->GetPadSizeX(), options->GetPadSizeY());
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "paddle init");
fclose(foo);
#endif
	timedisplay->Init(64 / POSRATIO, 80 / POSRATIO, "--:--.--");
#ifdef LOGCRAP
foo = fopen("out.txt", "w");
fprintf(foo, "timedisplay init");
fclose(foo);
#endif
	ruledisplay->Init(16 / POSRATIO, 16 / POSRATIO, "Welcome to TONG!");
	scoreblurb->Init(288 / POSRATIO, 352 / POSRATIO, "GO!!");
	scoreboard->Init(464 / POSRATIO, 80 / POSRATIO, "--------");
	highscores->Init(200 / POSRATIO, 24 / POSRATIO,
					 options->GetHighScoreText(SCORE_MARATHON));
	mainmenu->SetActive(true);
	options->optionmenu->SetActive(true);
	lastupdate=SDL_GetTicks();
	demostarted=SDL_GetTicks();
	theme(options->GetThemeString());
	tetrad->Theme(options->GetThemeString());
	nexttetrad->Theme(options->GetThemeString());
	ball->Theme(options->GetThemeString());
	paddle->Theme(options->GetThemeString());
	bucket->Theme(options->GetThemeString());

	/*
	DIR *dir = opendir(".");
	dirent *ent = NULL;
	while(ent = readdir(dir)) {
foo = fopen("out.txt", "a");
 fprintf(foo, "dirent: %s\n", ent->d_name);
fclose(foo);
	}
	*/

	FILE *creditsfile = fopen("CREDITS", "rb");
	if(!creditsfile) {
		creditsfile = fopen("credits.txt", "rb"); //fat or other goony fs
	}
	int read = 0;
	if(creditsfile) {
		char* creditstring = new char[2048];
		memset(creditstring, 0, 2048);
		read = (int)fread(creditstring, sizeof(char), 2047, creditsfile);
		//printf("%s %d %d\n", creditstring, read, ferror(creditsfile));
		fclose(creditsfile);
		credits->Init(0, 480 / POSRATIO, creditstring);
		creditlines = credits->GetLines();
	} else {
		credits->Init(0, 480 / POSRATIO, "ERROR: 'CREDITS' file not found");
	}
	gameid->Init(8 / POSRATIO, 440 / POSRATIO, GAME_ID);

	init_audio(options, true, musicstarted);
	Mix_VolumeMusic(volume);
	Mix_Volume(-1, volume);

	//get rid of any input that happened during initialization
	while(SDL_PollEvent(&event));
	fadestart=SDL_GetTicks()+500;

	while(!done) {
		done = read_input(&input, options);
		if(done && gothighscore) {
			done = 0;
		}

		if(input.volup) {
			volume += 1;
			if(volume > MIX_MAX_VOLUME) {
				volume = MIX_MAX_VOLUME;
			}
		}
		if(input.voldown) {
			volume -= 1;
			if(volume < 0) {
				volume = 0;
			}
		}
		if(input.volup || input.voldown) {
			Mix_Volume(-1, volume);
			Mix_VolumeMusic(volume);
			//sprintf(voltext, "volume: [-----------] %d", volume);
			sprintf(voltext, "volume: [-----------]");
			tempint = volume * 10 / MIX_MAX_VOLUME;
			voltext[ 9 + tempint ] = '|';
			volblurb->SetText(voltext);
			lastvolblurb = lastupdate;
		}

		if(display!=SCREEN_GAMEOVER) {
			if(options->GetMusic()==MUSIC_SHUFFLE && !Mix_PlayingMusic()) {
				Mix_FreeMusic(music);
				if((music=Mix_LoadMUS(options->GetMusicFile()))) {
					if(music) {
						Mix_PlayMusic(music, 1);
					}
				}
				musicstarted=SDL_GetTicks();
			}
		}
		switch(display) {
		case SCREEN_BLACK:
			draw_fade(255, screen);
			break;

		case SCREEN_SPLASH:
			draw_splash(screen);
			//      gameid->Draw(screen);
			if(input.ok || input.esc) {
				clear_input(&input);
				fadestart = lastlastupdate;
				nextdisplay = SCREEN_TITLE;
				fadein = false;
			}
			break;

		case SCREEN_TITLE:
			draw_bg(false, screen, options);
			draw_title(screen);
			mainmenu->Draw(screen);
			if(SDL_GetTicks()-demostarted>DEMO_TIME) {
				demostarted=SDL_GetTicks();
				display=SCREEN_HISCORES;
			}
			if(input.any) {
				demostarted=SDL_GetTicks();
			}
			if(input.esc) {
				clear_input(&input);
				demostarted = lastupdate;
				if(options->GetSoundFx()) {
					Mix_PlayChannel(0, toing, 0);
				}
				ball->UpdateTime();
				if(!gameinprogress) {
					mainmenu->ModChoice("Resume Game", 1);
					mainmenu->ModChoice("End Current Game", 2);
					mainmenu->SetState(4, DISABLED);
					mainmenu->SetState(5, DISABLED);
					options->RuleFrequencyInit();
					timespent=0;
					fading=false;
					fadestart=0;
				} else {
					options->RuleFrequencyResume();
				}
				starttime=SDL_GetTicks()-timespent;
				endtime=SDL_GetTicks()+timeleft;
				display=SCREEN_GAME;
			}
			if(input.up) {
				clear_input(&input);
				if(mainmenu->MoveCursor(NORTH)) {
					if(options->GetSoundFx()) {
						if(rand()%2) {
							Mix_PlayChannel(0, rotate1, 0);
						} else {
							Mix_PlayChannel(0, rotate2, 0);
						}
					}
				}
			}
			if(input.ok) {
				clear_input(&input);
				if(options->GetSoundFx()) {
					Mix_PlayChannel(0, tick, 0);
				}
				switch(mainmenu->GetChoice()) {
				case 1:
					ball->UpdateTime();
					if(!gameinprogress) {
						mainmenu->ModChoice("Resume Game", 1);
						mainmenu->ModChoice("End Current Game", 2);
						mainmenu->SetState(4, DISABLED);
						mainmenu->SetState(5, DISABLED);
						options->RuleFrequencyInit();
						timespent=0;
						fading=false;
						fadestart=0;
					} else {
						options->RuleFrequencyResume();
					}
					endtime = SDL_GetTicks() + timeleft;
					starttime = SDL_GetTicks() - timespent;
					timespent = 0;
					display = SCREEN_GAME;
					break;

				case 2:
					if(gameinprogress) {
						gameinprogress=false;
						tetrad->Init(rand() % 7);
						nexttetrad->Init(rand() % 7);
						options->RuleFrequencyInit();
						bucket->Init(options->GetJunk());
						ball->Init(options->GetSpeedBall());
						paddle->Init(options->GetPadSizeX(),
									 options->GetPadSizeY());
						ballslost = bucketslost = 0;
						lines = 0;
						lastrulechange=SDL_GetTicks();
						mainmenu->ModChoice("Start Game!", 1);
						mainmenu->ModChoice("Rules/Difficulty...", 2);
						mainmenu->SetState(4, NORMAL);
						if(!marathon) {
							mainmenu->SetState(5, NORMAL);
						}
						ruledisplay->SetText("Welcome to TONG!");
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, plop, 0);
						}
						mainmenu->MoveCursor(NORTH);
					} else {
						options->optionmenu->SetActive(false);
						options->custommenu->SetActive(true);
						display = SCREEN_OPTION;
					}
					break;

				case 3:
					display = SCREEN_CONTROL;
					break;

				case 4:
					if(marathon) {
						mainmenu->ModChoice("Game Mode  = Timed   ", 4);
						mainmenu->SetState(5, NORMAL);
					} else {
						mainmenu->ModChoice("Game Mode  = Marathon", 4);
						mainmenu->SetState(5, DISABLED);
					}
					marathon = !marathon;
					break;

				case 5:
					switch(timelength) {
					case 120000:
						timelength = 300000;
						mainmenu->ModChoice("Time Limit = 05:00", 5);
						break;
					case 300000:
						timelength = 600000;
						mainmenu->ModChoice("Time Limit = 10:00", 5);
						break;
					case 600000:
						timelength = 120000;
						mainmenu->ModChoice("Time Limit = 02:00", 5);
					default:
						break;
					}
					break;

				case 6:
					display = SCREEN_OPTION;
					break;

				case 7:
					display = SCREEN_CREDITS;
					break;

				case 8:
					quitgame = true;
					break;

				default:
					break;
				}
				break;
			}
			if(input.down) {
				clear_input(&input);
				if(mainmenu->MoveCursor(SOUTH)) {
					if(options->GetSoundFx()) {
						if(rand()%2) {
							Mix_PlayChannel(0, rotate1, 0);
						} else {
							Mix_PlayChannel(0, rotate2, 0);
						}
					}
				}
				break;
			}
			break;

		case SCREEN_OPTION:
			draw_bg(false, screen, options);
			if(options->optionmenu->IsActive() ||
			   options->musicmenu->IsActive() ||
			   options->thememenu->IsActive()) {
				options->optionmenu->Draw(screen);
				if(options->musicmenu->IsActive()) {
					options->musicmenu->Draw(screen);
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						options->SetStart();
						options->optionmenu->SetActive(true);
						options->musicmenu->SetActive(false);
					}
					if(input.up) {
						clear_input(&input);
						if(options->musicmenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->musicmenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						if(options->GetMusic()!=options->musicmenu->GetChoice()-1) {
							options->SetMusic(options->musicmenu->GetChoice()-1);
							Mix_FadeOutMusic(100);
							Mix_FreeMusic(music);
							if(options->GetMusicFile()) {
								if((music=Mix_LoadMUS(options->GetMusicFile()))) {
									if(options->GetMusic()==MUSIC_SHUFFLE) {
										if(music) {
											Mix_PlayMusic(music, 1);
										}
									} else {
										if(music) {
											Mix_PlayMusic(music, -1);
										}
									}
									musicstarted=SDL_GetTicks();
								} else {
									printf("Loading music file %s failed.\n",
										   options->GetMusicFile());
								}
							} else {
								music = NULL;
								//no music
							}
						}
						options->optionmenu->SetActive(true);
						options->musicmenu->SetActive(false);
					}
				} else if(options->thememenu->IsActive()) {
					options->thememenu->Draw(screen);
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						options->optionmenu->SetActive(true);
						options->thememenu->SetActive(false);
					}
					if(input.up) {
						clear_input(&input);
						if(options->thememenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->thememenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						if(options->GetTheme()!=options->thememenu->GetChoice()-1) {
							options->SetTheme(options->thememenu->GetChoice()-1);
							theme(options->GetThemeString());
							tetrad->Theme(options->GetThemeString());
							nexttetrad->Theme(options->GetThemeString());
							ball->Theme(options->GetThemeString());
							paddle->Theme(options->GetThemeString());
							bucket->Theme(options->GetThemeString());
						}
						options->optionmenu->SetActive(true);
						options->thememenu->SetActive(false);
					}
				} else {
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, toing, 0);
						}
						demostarted=SDL_GetTicks();
						display=SCREEN_TITLE;
					}
					if(input.up) {
						clear_input(&input);
						if(options->optionmenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->optionmenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						switch(options->optionmenu->GetChoice()) {
						case 1:
							options->SetVideoFlags(options->GetVideoFlags()
												   ^ SDL_FULLSCREEN);
							screen = init_video(options->GetVideoBpp(),
												options->GetVideoFlags());
							SDL_ShowCursor(0);
							break;
						case 2:
							if(options->GetVideoBpp()>24) {
								options->SetVideoBpp(16);
							} else {
								options->SetVideoBpp(options->GetVideoBpp()+8);
							}
							screen=init_video(options->GetVideoBpp(),
											  options->GetVideoFlags());
							break;
						case 3:
							i = options->GetFpsLimit();
							if(i == FPS_24) {
								i = FPS_30;
							} else if(i == FPS_30) {
								i = FPS_60;
							} else if(i == FPS_60) {
								i = FPS_120;
							} else if(i == FPS_120) {
								i = FPS_UNLIMITED;
							} else {
								i = FPS_24;
							}
							options->SetFpsLimit(i);
							break;
						case 4:
							options->SetBgScroll(!options->GetBgScroll());
							break;
						case 5:
							if(options->GetAudioBuffers()>2048) {
								options->SetAudioBuffers(512);
							} else {
								options->SetAudioBuffers(options->GetAudioBuffers()*2);
							}
							init_audio(options, false, musicstarted);
							break;
						case 6:
							if(options->GetAudioFreq()!=22050) {
								options->SetAudioFreq(22050);
							} else {
								options->SetAudioFreq(44100);
							}
							init_audio(options, false, musicstarted);
							break;
						case 7:
							options->SetSoundFx(!options->GetSoundFx());
							break;
						case 8:
							options->optionmenu->SetActive(false);
							options->musicmenu->SetActive(true);
							break;
						case 9:
							options->optionmenu->SetActive(false);
							options->thememenu->SetActive(true);
							break;
						case 10:
							demostarted=SDL_GetTicks();
							display=SCREEN_TITLE;
							break;
						default:
							break;
						}
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
					}
				}
			} else {
				options->custommenu->Draw(screen);
				if(options->ballstackmenu->IsActive()) {
					options->ballstackmenu->Draw(screen);
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						options->SetStart();
						options->ballstackmenu->SetActive(false);
						options->custommenu->SetActive(true);
					}
					if(input.up) {
						clear_input(&input);
						if(options->ballstackmenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->ballstackmenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						switch(options->ballstackmenu->GetChoice()) {
						case 1:
							if(options->IsAllowed(NO_REACTION|RULETYPE_BALLSTACK) ||
							   options->IsAllowed(NO_REACTION|RULETYPE_STACKBALL)) {
								options->DenyRule(NO_REACTION|RULETYPE_BALLSTACK);
								options->DenyRule(NO_REACTION|RULETYPE_STACKBALL);
							} else {
								options->AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
								options->AllowRule(NO_REACTION|RULETYPE_STACKBALL);
							}
							break;
						case 2:
							if(options->IsAllowed(STACK_KILLS_BALL|RULETYPE_STACKBALL)) {
								options->DenyRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
							} else {
								options->AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
							}
							break;
						case 3:
							if(options->IsAllowed(STACK_BOUNCES_BALL
												  |RULETYPE_STACKBALL)) {
								options->DenyRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
							} else {
								options->AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
							}
							break;
						case 4:
							if(options->IsAllowed(BALL_JOINS_STACK|RULETYPE_BALLSTACK)) {
								options->DenyRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
							} else {
								options->AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
							}
							break;
						case 5:
							if(options->IsAllowed(BALL_KILLS_STACK|RULETYPE_BALLSTACK)) {
								options->DenyRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
							} else {
								options->AllowRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
							}
							break;
						case 6:
							if(options->IsAllowed(BALL_BREAKS_STACK
												  |RULETYPE_BALLSTACK)) {
								options->DenyRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
							} else {
								options->AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
							}
							break;
						case 7:
							if(options->IsAllowed(BALL_LINEBREAK_STACK
												  |RULETYPE_BALLSTACK)) {
								options->DenyRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
							} else {
								options->AllowRule(BALL_LINEBREAK_STACK
												   |RULETYPE_BALLSTACK);
							}
							break;
						case 8:
							options->SetStart();
							options->ballstackmenu->SetActive(false);
							options->custommenu->SetActive(true);
							break;
						default:
							break;
						}
					}
				} else if(options->balltetradmenu->IsActive()) {
					options->balltetradmenu->Draw(screen);
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						options->SetStart();
						options->balltetradmenu->SetActive(false);
						options->custommenu->SetActive(true);
					}
					if(input.up) {
						clear_input(&input);
						if(options->balltetradmenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->balltetradmenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						switch(options->balltetradmenu->GetChoice()) {
						case 1:
							if(options->IsAllowed(NO_REACTION|RULETYPE_BALLTETRAD) ||
							   options->IsAllowed(NO_REACTION|RULETYPE_TETRADBALL)) {
								options->DenyRule(NO_REACTION|RULETYPE_BALLTETRAD);
								options->DenyRule(NO_REACTION|RULETYPE_TETRADBALL);
							} else {
								options->AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
								options->AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
							}
							break;
						case 2:
							if(options->IsAllowed(TETRAD_KILLS_BALL
												  |RULETYPE_TETRADBALL)) {
								options->DenyRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
							} else {
								options->AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
							}
							break;
						case 3:
							if(options->IsAllowed(TETRAD_BOUNCES_BALL
												  |RULETYPE_TETRADBALL)) {
								options->DenyRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
							} else {
								options->AllowRule(TETRAD_BOUNCES_BALL
												   |RULETYPE_TETRADBALL);
							}
							break;
						case 4:
							if(options->IsAllowed(BALL_KILLS_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
							}
							break;
						case 5:
							if(options->IsAllowed(BALL_JOINS_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
							}
							break;
						case 6:
							if(options->IsAllowed(BALL_BREAKS_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
							}
							break;
						case 7:
							if(options->IsAllowed(BALL_DROPS_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
							}
							break;
						case 8:
							if(options->IsAllowed(BALL_MOVES_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
							}
							break;
						case 9:
							if(options->IsAllowed(BALL_ROTATES_TETRAD
												  |RULETYPE_BALLTETRAD)) {
								options->DenyRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
							} else {
								options->AllowRule(BALL_ROTATES_TETRAD
												   |RULETYPE_BALLTETRAD);
							}
							break;
						case 10:
							options->SetStart();
							options->balltetradmenu->SetActive(false);
							options->custommenu->SetActive(true);
							break;
						default:
							break;
						}
					}
				} else {
					if(input.esc) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, toing, 0);
						}
						demostarted=SDL_GetTicks();
						display=SCREEN_TITLE;
					}
					if(input.up) {
						clear_input(&input);
						if(options->custommenu->MoveCursor(NORTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.down) {
						clear_input(&input);
						if(options->custommenu->MoveCursor(SOUTH)) {
							if(options->GetSoundFx()) {
								if(rand()%2) {
									Mix_PlayChannel(0, rotate1, 0);
								} else {
									Mix_PlayChannel(0, rotate2, 0);
								}
							}
						}
					}
					if(input.ok) {
						clear_input(&input);
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, tick, 0);
						}
						switch(options->custommenu->GetChoice()) {
						case 1:
									/*
									  options->DefaultRules();
									  options->DefaultSettings();
									  options->SetStart();
									*/
							options->SetLevel();
							level_tethover=options->GetSpeedTetrad();
							ball->Init(options->GetSpeedBall());
							paddle->Init(options->GetPadSizeX(), options->GetPadSizeY());
							bucket->Init(options->GetJunk());
							break;
						case 2:
							switch(options->GetPadPlacement()) {
							case PADS_ALL:
								options->SetPadPlacement(PADS_SIDES);
								break;
							case PADS_SIDES:
								options->SetPadPlacement(PADS_TOPBOT);
								break;
							case PADS_TOPBOT:
							default:
								options->SetPadPlacement(PADS_ALL);
								break;
							}
							paddle->Init(options->GetPadSizeX(),
										 options->GetPadSizeY());
							break;
						case 3:
							switch(options->GetPadSize()) {
							case PADS_SMALL:
								options->SetPadSize(PADS_NORMAL);
								break;
							case PADS_NORMAL:
								options->SetPadSize(PADS_BIG);
								break;
							case PADS_BIG:
								options->SetPadSize(PADS_15);
								break;
							case PADS_15:
								options->SetPadSize(PADS_13);
								break;
							case PADS_13:
								options->SetPadSize(PADS_12);
								break;
							case PADS_12:
							default:
								options->SetPadSize(PADS_SMALL);
								break;
							}
							paddle->Init(options->GetPadSizeX(),
										 options->GetPadSizeY());
							break;
						case 4:
							options->SetTetSpeedUp(!options->GetTetSpeedUp());
							break;
						case 5:
							options->SetBallSpeedsUp(!options->GetBallSpeedsUp());
							break;
						case 6:
							options->SetRandomRule(!options->GetRandomRule());
							break;
						case 7:
							switch(options->GetRuleFrequency()) {
							case EVERY_0_30:
								options->SetRuleFrequency(EVERY_0_45);
								break;
							case EVERY_0_45:
								options->SetRuleFrequency(EVERY_1_00);
								break;
							case EVERY_1_00:
								options->SetRuleFrequency(EVERY_BALL);
								break;
							case EVERY_BALL:
								options->SetRuleFrequency(EVERY_3_BALLS);
								break;
							case EVERY_3_BALLS:
								options->SetRuleFrequency(EVERY_LINE);
								break;
							case EVERY_LINE:
								options->SetRuleFrequency(EVERY_5_LINES);
								break;
							case EVERY_5_LINES:
								options->SetRuleFrequency(EVERY_0_15);
								break;
							case EVERY_0_15:
							default:
								options->SetRuleFrequency(EVERY_0_30);
								break;
							}
							break;
						case 8:
							options->SetSpeed();
							break;
						case 9:
							options->SetJunk();
							bucket->Init(options->GetJunk());
							break;
						case 10:
							options->custommenu->SetActive(false);
							options->balltetradmenu->SetActive(true);
							break;
						case 11:
							options->custommenu->SetActive(false);
							options->ballstackmenu->SetActive(true);
							break;
						case 12:
							options->custommenu->SetActive(false);
							options->optionmenu->SetActive(true);
							mainmenu->SetActive(true);
							demostarted=SDL_GetTicks();
							display=SCREEN_TITLE;
							break;
						default:
							break;
						}
					}
				}
			}
			break;
		case SCREEN_CONTROL:
			draw_bg(false, screen, options);
			options->controlmenu->Draw(screen);
			if(input.esc) {
				if(options->GetSoundFx()) {
					Mix_PlayChannel(0, toing, 0);
				}
				if(options->IsSettingKey()) {
					options->CancelSetKey();
				} else {
					demostarted = SDL_GetTicks();
					display = SCREEN_TITLE;
				}
				clear_input(&input);
			}
			if(input.up) {
				if(options->IsSettingKey()) {
					options->SetKey(input.any);
				} else {
					if(options->controlmenu->MoveCursor(NORTH)) {
						if(options->GetSoundFx()) {
							if(rand()%2) {
								Mix_PlayChannel(0, rotate1, 0);
							} else {
								Mix_PlayChannel(0, rotate2, 0);
							}
						}
					}
				}
				clear_input(&input);
			}
			if(input.down) {
				if(options->IsSettingKey()) {
					options->SetKey(input.any);
				} else {
					if(options->controlmenu->MoveCursor(SOUTH)) {
						if(options->GetSoundFx()) {
							if(rand()%2) {
								Mix_PlayChannel(0, rotate1, 0);
							} else {
								Mix_PlayChannel(0, rotate2, 0);
							}
						}
					}
				}
				clear_input(&input);
			}
			if(input.ok) {
				if(options->IsSettingKey()) {
					options->CancelSetKey();
				} else {
					if(options->GetSoundFx()) {
						Mix_PlayChannel(0, tick, 0);
					}
					switch(options->controlmenu->GetChoice()) {
					case 1:
						options->DefaultControls();
						break;
					case 8:
						demostarted=SDL_GetTicks();
						display = SCREEN_TITLE;
						break;
					default:
						options->SetKey();
						break;
					}
				}
				clear_input(&input);
			}
			if(input.any) {
				if(options->IsSettingKey()) {
					options->SetKey(input.any);
				}
				clear_input(&input);
			}
			break;
		case SCREEN_GAME:
			if(gameinprogress) {
				// pick up where we left off
			} else {
				// or start fresh
				// TODO: is this the place to set up speeds etc?
				lastrulechange = SDL_GetTicks();
				options->SetStart();
				score = 0;
				endtime = SDL_GetTicks() + timelength;
				starttime = SDL_GetTicks();
				timespent = 0;
				ball->Init(options->GetSpeedBall());
				level_tethover=options->GetSpeedTetrad();
				gameinprogress = true;
			}

			if(!marathon && SDL_GetTicks() > endtime) {
				display = SCREEN_GAMEOVER;
				Mix_FadeOutMusic(1000);
				bucket->GreyOut();
				if(options->GetSoundFx()) {
					Mix_PlayChannel(2, twobits, 0);
				}
			}

			if(options->RuleFrequencyNow()) {
				if(options->GetSoundFx()) {
					Mix_PlayChannel(2, newrule, 0);
				}
				options->NewRule();
				ruledisplay->SetText(options->GetRuleText());
				lastrulechange = lastupdate;
				ruleupdated = true;
			}
			ballstatus = ball->Move(paddle, bucket, tetrad, nexttetrad,
									&newdown, options);
			if(ballstatus == OUT_OF_BOUNDS) {
				ball->Init(options->GetSpeedBall());
				options->RuleFrequencyAddBall();
				if(marathon) {
					tetrad->MoveUp(bucket);
					bucket->AddJunk();
					drop_piece(tetrad, nexttetrad, bucket, lastpush, newdown,
							   marathon, options, display, &input);
					//we move the tetrad up and down around the junk adding so
					//it does not get embedded if it's touching during the rise
					score += 1;
				} else {
					tempint = 100 * (++ballslost);
					sprintf(scoretext, "-%i", tempint);
					score -= (tempint - 1);
					scoreblurb->SetText(scoretext);
					lastscoreblurb = lastupdate;
				}
			} else if(ballstatus == BOUNCED) {
				tempint = 100;
				/*
				  sprintf(scoretext, "+%i", tempint);
				  scoreblurb->SetText(scoretext);
				  lastscoreblurb=lastupdate;
				*/
				score += tempint;
				if(options->GetSoundFx()) {
					if(rand() % 2) {
						Mix_PlayChannel(1, bounce1, 0);
					} else {
						Mix_PlayChannel(1, bounce2, 0);
					}
				}
			}
      
			if(wasbreaking) {
				wasbreaking = bucket->IsBreaking();
				if(!wasbreaking && tetrad->HasLanded()) {
					tetrad->Init(nexttetrad->GetType());
					nexttetrad->Init(rand() % 7);
					/* ?
					   FIXME: there's a bug with ball-drops-tetrad,
					   tetrad can  still get rotated/duplicated
fixed i think (in pong.cpp), leaving the comment for now
					*/
					newdown = false;
					input.state.repeat.tetdown = 0;
				}
			} else {
				wasbreaking = bucket->IsBreaking();
				if(wasbreaking && options->GetSoundFx()) {
					Mix_PlayChannel(0, bust, 0);
				}
			}
			//push down every second or so, according to level
			//TODO:  uber-slow machines would not get enough pushes over time
			if(SDL_GetTicks() > lastpush + level_tethover) {
				if(drop_piece(tetrad, nexttetrad, bucket, lastpush, newdown,
							  marathon, options, display, &input)) {
					tempint = 5000 * (++bucketslost);
					sprintf(scoretext, "-%i", tempint);
					scoreblurb->SetText(scoretext);
					lastscoreblurb = lastupdate;
					score -= (tempint - 1);
				}
			}

			/* TODO: take care of this in input
			if(lastbuttondown > lastbuttonup) {
				if(repeatingbutton) {
					if(lastbuttondown + KEY_REPEAT < lastupdate) {
						SDL_PushEvent(&fakeinput);
					}
				} else {
					if(lastbuttondown+KEY_DELAY<lastupdate) {
						SDL_PushEvent(&fakeinput);
						repeatingbutton=true;
					}
				}
			}
			*/

			/* handle input, update gamestate */      
			if(input.esc) {
				if(options->GetSoundFx()) {
					Mix_PlayChannel(0, toing, 0);
				}
				timeleft = endtime - SDL_GetTicks();
				timespent = SDL_GetTicks() - starttime;
				options->RuleFrequencyPause();
				demostarted = SDL_GetTicks();
				display = SCREEN_TITLE;
			}
			if(input.tetright) {
				if(pushes != 0) {
					pushes = (lastupdate - lastlastupdate) / KEY_REPEAT;
				}
				pushes = pushes ? pushes : 1;
				for(i = 0; i < pushes; i++) {
					if(tetrad->MoveRight(bucket) && options->GetSoundFx()) {
						Mix_PlayChannel(0, tick, 0);
					}
				}
			}
			if(input.tetleft) {
				if(pushes != 0) {
					pushes = (lastupdate - lastlastupdate) / KEY_REPEAT;
				}
				pushes = pushes ? pushes : 1;
				for(i = 0; i < pushes; i++) {
					if(tetrad->MoveLeft(bucket) && options->GetSoundFx()) {
						Mix_PlayChannel(0, tick, 0);
					}
				}
			}
			if(input.tetdown) {
				if(!newdown) {
					if(pushes != 0) {
						pushes = (lastupdate - lastlastupdate) / KEY_REPEAT;
					}
					pushes = pushes ? pushes : 1;
					//keeps pushes the same or turns to 1 if it was/got 0
					for(i = 0; i < pushes; i++) {
						if(drop_piece(tetrad, nexttetrad, bucket, lastpush,
									  newdown, marathon, options, display,
									  &input)) {
							tempint = 5000 * (++bucketslost);
							sprintf(scoretext, "-%i", tempint);
							scoreblurb->SetText(scoretext);
							lastscoreblurb = lastupdate;
							score -= tempint - 1;
						} else {
							if(newdown) {
								break;
							}
						}
					}
				} else {
					input.state.repeat.tetdown = 0;
					newdown = false;
				}
			}
			if(input.cw) {
				if(tetrad->Rotate(bucket, true)) {
					if(options->GetSoundFx()) {
						if(rand() % 2) {
							Mix_PlayChannel(0, rotate1, 0);
						} else {
							Mix_PlayChannel(0, rotate2, 0);
						}
					}
				}
			}
			if(input.ccw) {
				if(tetrad->Rotate(bucket, false)) {
					if(rand() % 2) {
						if(options->GetSoundFx()) {
							Mix_PlayChannel(0, rotate1, 0);
						} else {
							Mix_PlayChannel(0, rotate2, 0);
						}
					}
				}
			}
			if(input.any) {
				if(input.any == SDLK_PRINT) {
					SDL_SaveBMP(screen, "screen.bmp");
				}
			}
			paddle->MouseMoved(input.padx, input.pady);
			clear_input(&input);

			/* draw everything */
			draw_bg(true, screen, options);
			tetrad->Draw(bucket, screen);
			nexttetrad->Preview(screen);
			tempint = bucket->Draw(screen);
			paddle->Draw(screen);
			ball->Draw(screen);

			if(marathon) {
				sprintf(timetext, "%02i:%02i.%02i",
						(SDL_GetTicks() - starttime) / 1000 / 60,
						((SDL_GetTicks() - starttime) / 1000) % 60,
						((SDL_GetTicks() - starttime) / 10) % 100);
			} else {
				sprintf(timetext, "%02i:%02i.%02i",
						(endtime - SDL_GetTicks()) / 1000 / 60,
						((endtime - SDL_GetTicks()) / 1000) % 60,
						((endtime - SDL_GetTicks()) / 10) % 100);
			}
			timedisplay->SetText(timetext);
			timedisplay->Draw(screen);
			if(lastscoreblurb && lastupdate-lastscoreblurb < 1500) {
				scoreblurb->Draw(screen);
			}
			if(lastrulechange && lastupdate-lastrulechange < 4000) {
				ruledisplay->Draw(screen);
			}
			//line(s) broken
			if(tempint) {
				if(options->GetTetSpeedUp()) {
					//speed up every 5 lines until very fast
					lines += tempint;
					if(lines > 5) {
						lines = 0;
						if(level_tethover > 100) {
							level_tethover -= 100;
						} else {
							//max speed tetrads
						}
					}
				}
				options->RuleFrequencyAddLines(tempint);
				tempint = 500 * (int)pow(2, tempint);
				sprintf(scoretext, "+%i", tempint);
				scoreblurb->SetText(scoretext);
				lastscoreblurb = lastupdate;
				score += tempint;
			}
			sprintf(scoretext, "%08li", score);
			scoreboard->SetText(scoretext);
			scoreboard->Draw(screen);
			break;
		case SCREEN_GAMEOVER:
			draw_bg(true, screen, options);
			bucket->Draw(screen);
			scoreboard->Draw(screen);
			if(marathon) {
				draw_gameover(screen, SDL_GetTicks() - starttime);
			} else {
				draw_gameover(screen);
				timedisplay->SetText("00:00.00");
			}
			timedisplay->Draw(screen);
			if(input.any || input.ok || input.esc) {
				if(marathon) {
					tempint = SCORE_MARATHON;
				} else {
					switch(timelength) {
					case 120000:
						tempint = SCORE_2_MIN;
						break;
					case 300000:
						tempint = SCORE_5_MIN;
						break;
					case 600000:
						tempint = SCORE_10_MIN;
					default:
						break;
					}
				}
				if(score > options->GetHighScore(tempint, 9)) {
					gothighscore = true;
					display = SCREEN_HISCORES;
				} else {
					demostarted = SDL_GetTicks();
					display = SCREEN_TITLE;
				}
				// TODO: move this stuff to start-over spot?
				gameinprogress = false;
				tetrad->Init(nexttetrad->GetType());
				nexttetrad->Init(rand() % 7);
				bucket->Init(options->GetJunk());
				ball->Init(options->GetSpeedBall());
				paddle->Init(options->GetPadSizeX(),
							 options->GetPadSizeY());
				ballslost = bucketslost = 0;
				lines = 0;
				lastrulechange = SDL_GetTicks();
				mainmenu->ModChoice("Start Game!", 1);
				mainmenu->ModChoice("Rules/Difficulty...", 2);
				mainmenu->SetState(4, NORMAL);
				if(!marathon) {
					mainmenu->SetState(5, NORMAL);
				}
				if(!marathon) {
					mainmenu->SetState(6, NORMAL);
				}
				ruledisplay->SetText("Welcome to TONG!");
				if(options->GetMusic() == MUSIC_SHUFFLE) {
					Mix_FreeMusic(music);
					if((music = Mix_LoadMUS(options->GetMusicFile()))) {
						Mix_PlayMusic(music, 1);
					}
				} else {
					if(music) {
						Mix_PlayMusic(music, -1);
					}
				}
				musicstarted = SDL_GetTicks();
			}
			clear_input(&input);
			break;
		case SCREEN_HISCORES:
			draw_bg(false, screen, options);
			/*
			sprintf(voltext, "u%d d%d l%d r%d a%d ", input.up, input.down, input.left, input.right, (int)input.any);
			volblurb->SetText(voltext);
			volblurb->Draw(screen);
			*/
			if(gothighscore) {
				scoredisplay = tempint; //still set from gameover screen
				highscores->SetText(options->GetHighScoreText(scoredisplay));
			}
			highscores->Draw(screen);
			if(SDL_GetTicks() - demostarted > DEMO_TIME) {
				demostarted = SDL_GetTicks();
				switch(scoredisplay) {
				case SCORE_MARATHON:
				case SCORE_2_MIN:
				case SCORE_5_MIN:
					scoredisplay++;
					break;
				case SCORE_10_MIN:
				default:
					scoredisplay = SCORE_MARATHON;
					display=SCREEN_TITLE;
					break;
				}
				highscores->SetText(options->GetHighScoreText(scoredisplay));
			} else if(gothighscore) {
				//stay in this screen 'til the user's done
				demostarted = SDL_GetTicks();
				highscoremenu->Draw(screen);
			}

			if((input.esc || input.ok || input.any) && !gothighscore) {
				display = SCREEN_TITLE;
				clear_input(&input);
				break;
			}
			if(input.ok && gothighscore) {
				gothighscore = false; //all done
				demostarted = SDL_GetTicks();
				options->InsertScore(score, scorename, tempint);
				if(!options->WriteConfig()) {
					printf("WARNING: Settings were not saved.\n");
				};
				highscores->SetText(options->GetHighScoreText(scoredisplay));
				highscoremenu->ModChoice("     _", 3);
				strcpy(scorename, "_");
				strcpy(scoretext, "--------");
			} else if(input.any ||
			   input.up || input.down || input.left || input.right) {
				if('a' <= input.any && input.any <= 'z') {
					if(input.mod & (KMOD_LSHIFT | KMOD_RSHIFT)) {
						scoretext[0] = ((char)input.any) - 32;
					} else {
						scoretext[0] = (char)input.any;
					}
					scoretext[1] = '\0';
					if(!strcmp(scorename, "_") || strlen(scorename) == 0) {
						strcpy(scorename, scoretext);
					} else {
						if(strlen(scorename) >= 5) {
							scorename[4]='\0';
						}
						strcat(scorename, scoretext);
					}
				} else if((input.any == SDLK_BACKSPACE || input.left) &&
						  strlen(scorename)) {
					scorename[strlen(scorename) - 1] = '\0';
				} else if(input.up) {
					if(strlen(scorename) < 1) {
						strcpy(scorename, "A");
					} else {
						scorename[strlen(scorename) - 1]++;
						if(scorename[strlen(scorename) - 1] > 'Z') {
							scorename[strlen(scorename) - 1] = 'A';
						}
					}
				} else if(input.down) {
					if(strlen(scorename) < 1) {
						strcpy(scorename, "A");
					} else {
						scorename[strlen(scorename) - 1]--;
						if(scorename[strlen(scorename) - 1] < 'A') {
							scorename[strlen(scorename) - 1] = 'Z';
						}
					}
				} else if(input.right && strlen(scorename) < 5) {
					scorename[strlen(scorename)] =
						scorename[strlen(scorename) - 1];
					scorename[strlen(scorename) + 1] = '\0';
				} else if(input.any == SDLK_LSHIFT ||
						  input.any == SDLK_RSHIFT) {
				}
				strcpy(scoretext, "     ");
				strcat(scoretext, scorename);
				if(strlen(scorename) < 5) {
					strcat(scoretext, "_");
				}
				highscoremenu->ModChoice(scoretext, 3);
			}
			clear_input(&input);
			break;
		case SCREEN_CREDITS:
			draw_bg(false, screen, options);
			if(!creditsstart) {
				creditsstart = SDL_GetTicks();
			}
			credits->SetY((480 / POSRATIO) -
						  ((SDL_GetTicks()-creditsstart) / 40));
			credits->Draw(screen);
			if((int)((SDL_GetTicks() - creditsstart ) / 40 /
					 TEXTHEIGHT) - 13 > creditlines) {
				creditsstart = 0;
				demostarted = SDL_GetTicks();
				display = SCREEN_TITLE;
			}
			if(input.ok || input.esc) {
				clear_input(&input);
				creditsstart = 0;
				demostarted = SDL_GetTicks();
				display = SCREEN_TITLE;
			}
			break;
		default:
			break;
		}
		if(lastvolblurb && lastupdate-lastvolblurb < 1500) {
			volblurb->Draw(screen);
		}

		if(quitgame&&!fading) {
			fading=true;
			fadein=false;
			fadestart=SDL_GetTicks();
			if(Mix_PlayingMusic()) {
				Mix_FadeOutMusic(500);
			}
		}
		if(fading) {
			if(fadestart+500<=SDL_GetTicks()) { //don't use lastupdate,flickers!
				fading=false;
				if(quitgame) {
					done=true;
				} else {
					if(fadein) {
						fadein=false;
						switch(display) {
						case SCREEN_SPLASH:
							fadestart=lastupdate+2000;
							nextdisplay=SCREEN_TITLE;
							break;
						case SCREEN_TITLE:
							fadestart=0;
							break;
						default:
							break;
						}
					} else {
						fadein=true;
						if(display==nextdisplay) {
							fading=false;
							fadestart=0;
						} else {
							fading=true;
							fadestart=lastupdate;
							display=nextdisplay;
							demostarted=lastupdate;
							draw_fade(255, screen);
						}
					}
				}
			} else {
				if(fadein) {
					draw_fade(255-(Uint8)(((float)(SDL_GetTicks()-fadestart)
										   /500.0)*256.0), screen);
				} else {
					draw_fade((Uint8)(((float)(SDL_GetTicks()-fadestart)
									   /500.0)*256.0), screen);
				}
			}
		} else {
			if(fadestart && fadestart<lastupdate) {
				if(fadein) {
					display=nextdisplay;
				}
				fading=true;
			}
		}
		if(done) {
			draw_fade(255, screen);
		}
		SDL_Flip(screen);
		lastlastupdate=lastupdate;
		lastupdate=SDL_GetTicks();

		// limit to 1000/MIN_TIME fps
		if(lastupdate-lastlastupdate < options->GetFpsLimit()) {
		    SDL_Delay(options->GetFpsLimit() - (lastupdate - lastlastupdate));
			lastupdate = SDL_GetTicks();
		}
	}  //main game loop

	Mix_FreeMusic(music);
	if(joy) {
		SDL_JoystickClose(joy);
	}
	if(options->WriteConfig()) {
	} else {
		printf("WARNING: Settings could not be saved\n");
	}

	Mix_CloseAudio();
	delete tetrad;
	delete nexttetrad;
	delete bucket;
	delete ball;
	delete paddle;
	delete options;
	delete scoreboard;
	delete scoreblurb;
	delete credits;
	delete gameid;
	delete ruledisplay;
	delete timedisplay;
	delete highscores;
	delete highscoremenu;
	delete mainmenu;
	SDL_Quit();

#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif

	return 0;
}


bool drop_piece(Tetrad* tetrad, Tetrad* nexttetrad, Bucket* bucket,
				Uint32& lastpush, bool& newdown, bool& marathon, Option* ops,
				int& display, input_t *input) {
	bool lostbucket=false;
	if(!tetrad->MoveDown(bucket, true)) {
		if(!tetrad->GetMoves()) {
			if(marathon) {
				display=SCREEN_GAMEOVER;
				input->any = SDLK_UNKNOWN;
				input->ok = 0;
				bucket->GreyOut();
				Mix_FadeOutMusic(1000);
				if(ops->GetSoundFx()) {
					Mix_PlayChannel(2, twobits, 0);
				}
			} else {
				//Mix_PlayChannel(0, bust, 0);  //TODO:  bucket reset sound
				//bucket->Init(rand()%5);
				bucket->StartOver();
				lostbucket=true;
			}
		}
		if(!bucket->IsBreaking()) {
			tetrad->Init(nexttetrad->GetType());
			nexttetrad->Init(rand()%7);
			if(ops->GetSoundFx()) {
				Mix_PlayChannel(0, plop, 0);
			}
		}
		newdown=true;
	}
	lastpush=SDL_GetTicks();
	return lostbucket;
}


void clear_input(input_t *input) {
	input->cw = 0;
	input->ccw = 0;
	input->tetdown = 0;
	input->tetleft = 0;
	input->tetright = 0;
	input->esc = 0;
	input->up = 0;
	input->down = 0;
	input->left = 0;
	input->right = 0;
	input->ok = 0;
	input->any = SDLK_UNKNOWN;
	input->mod = 0;
	input->quit = 0;
	//input->volup = 0;
	//input->voldown = 0;

	if(input->state.padmouse) {
		input->padx = 0;
		input->pady = 0;
	}

	return;
}

int read_input(input_t *input, Option *options) {
	SDL_Event event;
	int done = 0;
	int not_any = 0;

	int tetleftsym = options->GetKey(KEY_TETLEFT);
	int tetrightsym = options->GetKey(KEY_TETRIGHT);
	int tetdownsym = options->GetKey(KEY_TETDOWN);
	int tetcwsym = options->GetKey(KEY_TETCW);
	int tetccwsym = options->GetKey(KEY_TETCCW);
	Uint32 now = SDL_GetTicks();

	while(SDL_PollEvent(&event)) {
		not_any = 0;

		switch(event.type) {
		case SDL_QUIT:
			done = true;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE) {
				input->esc = 1;
			} else if(event.key.keysym.sym == SDLK_UP ||
					  event.key.keysym.sym == tetcwsym) {
				input->cw = 1;
				input->up = 1;
			} else if(event.key.keysym.sym == tetccwsym) {
				input->ccw = 1;
			} else if(event.key.keysym.sym == SDLK_LEFT ||
					  event.key.keysym.sym == tetleftsym) {
				input->left = 1;
				input->tetleft = 1;
				input->state.repeat.tetleft = now + KEY_DELAY;
			} else if(event.key.keysym.sym == SDLK_RIGHT ||
					  event.key.keysym.sym == tetrightsym) {
				input->right = 1;
				input->tetright = 1;
				input->state.repeat.tetright = now + KEY_DELAY;
			} else if(event.key.keysym.sym == SDLK_DOWN ||
					  event.key.keysym.sym == tetdownsym) {
				input->down = 1;
				input->tetdown = 1;
				input->state.repeat.tetdown = now + KEY_DELAY;
			} else if(event.key.keysym.sym == SDLK_RETURN) {
				input->ok = 1;
			} else if(event.key.keysym.sym == 'q') {
				done = 1;
			}
			if(!not_any) {
				input->mod = event.key.keysym.mod;
				input->any = event.key.keysym.sym;
			}
			break;
		case SDL_KEYUP:
			if(event.key.keysym.sym == SDLK_ESCAPE) {
				input->esc = 0;
			} else if(event.key.keysym.sym == SDLK_UP ||
					  event.key.keysym.sym == tetcwsym) {
				input->cw = 0;
				input->up = 0;
			} else if(event.key.keysym.sym == tetccwsym) {
				input->ccw = 0;
			} else if(event.key.keysym.sym == SDLK_LEFT ||
					  event.key.keysym.sym == tetleftsym) {
				input->left = 0;
				input->tetleft = 0;
				input->state.repeat.tetleft = 0;
			} else if(event.key.keysym.sym == SDLK_RIGHT ||
					  event.key.keysym.sym == tetrightsym) {
				input->right = 0;
				input->tetright = 0;
				input->state.repeat.tetright = 0;
			} else if(event.key.keysym.sym == SDLK_DOWN ||
					  event.key.keysym.sym == tetdownsym) {
				input->down = 0;
				input->tetdown = 0;
				input->state.repeat.tetdown = 0;
			} else if(event.key.keysym.sym == SDLK_RETURN) {
				input->ok = 0;
			}
			input->mod = 0;
			input->any = SDLK_UNKNOWN;
			break;
		case SDL_MOUSEMOTION:
#ifdef WII
			break;
#endif
			input->state.padmouse = 1;
			input->padx = event.motion.xrel;
			input->pady = event.motion.yrel;
			/*
					menumouse+=event.motion.yrel;
					fakeinput.type=SDL_KEYDOWN;
					fakeinput.key.type=SDL_KEYDOWN;
					fakeinput.key.state=SDL_PRESSED;
					if(menumouse>MENUMOUSE_THRESHOLD) {
						fakeinput.key.keysym.sym=SDLK_DOWN;
						SDL_PushEvent(&fakeinput);
						menumouse=0;
					} else if(menumouse<0-MENUMOUSE_THRESHOLD) {
						fakeinput.key.keysym.sym=SDLK_UP;
						SDL_PushEvent(&fakeinput);
						menumouse=0;
					}
			*/
			if(!input->any) {
				input->any = SDLK_RETURN;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
#ifdef WII
			break;
#endif
			//TODO: cursor, click
			if(!input->any) {
				input->any = SDLK_RETURN;
			}
			break;
		case SDL_JOYHATMOTION:
			input->left = 0;
			input->right = 0;
			input->down = 0;
			input->up = 0;
			input->tetleft = 0;
			input->tetright = 0;
			input->tetdown = 0;
			switch(event.jhat.value) {
			case SDL_HAT_CENTERED:
				input->state.repeat.tetleft = 0;
				input->state.repeat.tetright = 0;
				input->state.repeat.tetdown = 0;
				break;
			case SDL_HAT_LEFTUP:
				input->up = 1; //NOTE: fallthrough
			case SDL_HAT_LEFT:
				input->left = 1;
				input->tetleft = 1;
				input->state.repeat.tetleft = now + KEY_DELAY;
				break;
			case SDL_HAT_UP:
				input->up = 1;
				break;
			case SDL_HAT_RIGHTUP:
				input->up = 1; //NOTE: fallthrough
			case SDL_HAT_RIGHT:
				input->right = 1;
				input->tetright = 1;
				input->state.repeat.tetright = now + KEY_DELAY;
				break;
			case SDL_HAT_LEFTDOWN:
				input->left = 1;
				input->down = 1;
				input->tetleft = 1;
				input->tetdown = 1;
				input->state.repeat.tetleft = now + KEY_DELAY;
				input->state.repeat.tetdown = now + KEY_DELAY;
				break;
			case SDL_HAT_DOWN:
				input->down = 1;
				input->tetdown = 1;
				input->state.repeat.tetdown = now + KEY_DELAY;
				break;
			case SDL_HAT_RIGHTDOWN:
				input->left = 1;
				input->down = 1;
				input->tetleft = 1;
				input->tetdown = 1;
				input->state.repeat.tetright = now + KEY_DELAY;
				input->state.repeat.tetdown = now + KEY_DELAY;
				break;
			default:
				break;
			}
			break;
		case SDL_JOYAXISMOTION:
			switch(event.jaxis.axis) {
			case 0: //left/right
			case 2:
			case 6: //wiimote tilt left/right
				input->padx = event.jaxis.value / 3200;
#ifdef WII
				input->padx = event.jaxis.value * 3 / 3200;
#endif
				/*
				if(event.jaxis.value < -3200) {
					input.left = 1;
				} else if(event.jaxis.value > 3200) {
					input.right = 1;
				}
				*/
				break;
			case 1: //up/down
			case 3:
			case 7: //wiimote rock up/down
				input->pady = event.jaxis.value / 3200;
#ifdef WII
				input->pady = event.jaxis.value * -3 / 3200;
#endif
				/*
				if(event.jaxis.value < -3200) {
				} else if(event.jaxis.value > 3200) {
					input->down = 1;
				}
				*/
				break;
			}
			/* makes game over/high scores screens go away.
			   //whatever this was for, do it elsewhere :^)
			if(!input->any) {
				input->any = SDLK_RETURN;
			}
			*/
			break;
		case SDL_JOYBUTTONDOWN:
			switch(event.jbutton.button) {
#ifdef GP2X
			case GP2X_BUTTON_UP:
				input->up = 1;
				input->pady = -4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_DOWN:
				input->down = 1;
				input->pady = 4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_LEFT:
				input->left = 1;
				input->padx = -4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_RIGHT:
				input->right = 1;
				input->padx = 4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_A:
				input->ok = 1;
				input->tetleft = 1;
				input->state.repeat.tetleft = now + KEY_DELAY;
				break;
			case GP2X_BUTTON_B:
				input->ok = 1;
				input->tetright = 1;
				input->state.repeat.tetright = now + KEY_DELAY;
				break;
			case GP2X_BUTTON_X:
				input->ok = 1;
				input->tetdown = 1;
				input->state.repeat.tetdown = now + KEY_DELAY;
				break;
			case GP2X_BUTTON_Y:
				input->ok = 1;
				input->cw = 1;
				break;
			case GP2X_BUTTON_START:
			case GP2X_BUTTON_SELECT:
				input->esc = 1;
				break;
			case GP2X_BUTTON_L:
				input->ccw = 1;
				break;
			case GP2X_BUTTON_R:
				input->cw = 1;
				break;
			case GP2X_BUTTON_VOLUP:
				input->volup = 1;
				not_any = 1;
				break;
			case GP2X_BUTTON_VOLDOWN:
				input->voldown = 1;
				not_any = 1;
				break;
			case GP2X_BUTTON_UPLEFT:
				input->up = 1;
				input->left = 1;
				input->pady = -4;
				input->padx = -4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_UPRIGHT:
				input->up = 1;
				input->right = 1;
				input->pady = -4;
				input->padx = 4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_DOWNLEFT:
				input->down = 1;
				input->left = 1;
				input->pady = 4;
				input->padx = -4;
				input->state.padmouse = 0;
				break;
			case GP2X_BUTTON_DOWNRIGHT:
				input->down = 1;
				input->right = 1;
				input->pady = 4;
				input->padx = 4;
				input->state.padmouse = 0;
				break;
#endif
#ifdef WII
			case WII_BUTTON_A:
			case WII_BUTTON_Z:
			case WII_BUTTON_a:
			case WII_BUTTON_y:
			case WII_BUTTON_2:
				input->ok = 1;
				input->cw = 1;
				break;
			case WII_BUTTON_B:
			case WII_BUTTON_C:
			case WII_BUTTON_b:
			case WII_BUTTON_x:
			case WII_BUTTON_1:
				input->ok = 1;
				input->ccw = 1;
				break;
			case WII_BUTTON_MINUS:
			case WII_BUTTON_PLUS:
			case WII_BUTTON_HOME:
			case WII_BUTTON_minus:
			case WII_BUTTON_plus:
			case WII_BUTTON_home:
				input->esc = 1;
				break;
#endif
			default:
				input->ok = 1;
				input->cw = 1;
				break;
			}
			if(!input->any) {
				input->any = SDLK_RETURN;
			}
			break;
		case SDL_JOYBUTTONUP:
			switch(event.jbutton.button) {
#ifdef GP2X
			case GP2X_BUTTON_UP:
				input->up = 0;
				input->pady = 0;
				break;
			case GP2X_BUTTON_DOWN:
				input->down = 0;
				input->pady = 0;
				break;
			case GP2X_BUTTON_LEFT:
				input->left = 0;
				input->padx = 0;
				break;
			case GP2X_BUTTON_RIGHT:
				input->right = 0;
				input->padx = 0;
				break;
			case GP2X_BUTTON_A:
				input->ok = 0;
				input->tetleft = 0;
				input->state.repeat.tetleft = 0;
				break;
			case GP2X_BUTTON_B:
				input->ok = 0;
				input->tetright = 0;
				input->state.repeat.tetright = 0;
				break;
			case GP2X_BUTTON_X:
				input->ok = 0;
				input->tetdown = 0;
				input->state.repeat.tetdown = 0;
				break;
			case GP2X_BUTTON_Y:
				input->ok = 0;
				input->cw = 0;
				break;
			case GP2X_BUTTON_START:
			case GP2X_BUTTON_SELECT:
				input->esc = 0;
				break;
			case GP2X_BUTTON_L:
				input->ccw = 0;
				break;
			case GP2X_BUTTON_R:
				input->cw = 0;
				break;
			case GP2X_BUTTON_VOLUP:
				input->volup = 0;
				break;
			case GP2X_BUTTON_VOLDOWN:
				input->voldown = 0;
				break;
			case GP2X_BUTTON_UPLEFT:
				input->up = 0;
				input->left = 0;
				input->pady = 0;
				input->padx = 0;
				break;
			case GP2X_BUTTON_UPRIGHT:
				input->up = 0;
				input->right = 0;
				input->pady = 0;
				input->padx = 0;
				break;
			case GP2X_BUTTON_DOWNLEFT:
				input->down = 0;
				input->left = 0;
				input->pady = 0;
				input->padx = 0;
				break;
			case GP2X_BUTTON_DOWNRIGHT:
				input->down = 0;
				input->right = 0;
				input->pady = 0;
				input->padx = 0;
				break;
#endif
			default:
				input->ok = 0;
				input->cw = 0;
				break;
			}
		}
	}
	if(input->state.repeat.tetleft && input->state.repeat.tetleft < now) {
		input->tetleft = 1;
		input->state.repeat.tetleft += KEY_REPEAT;
	}
	if(input->state.repeat.tetright && input->state.repeat.tetright < now) {
		input->tetright = 1;
		input->state.repeat.tetright += KEY_REPEAT;
	}
	if(input->state.repeat.tetdown && input->state.repeat.tetdown < now) {
		input->tetdown = 1;
		input->state.repeat.tetdown += KEY_REPEAT;
	}
	return done;
}
