/*
 * This file is part of TONG.              http://www.nongnu.org/tong
 * Copyright 2004, 2007 Owen Swerkstrom
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

#include "option.h"

Option::Option() {
	char* path=getenv("HOME");
#ifdef GP2X
	path = NULL;
#endif
	//  DIR* musicdir;
	//  dirent* musicfile;
	int i, j;

	for(i=0; i<SCORE_TYPES; i++) {
		for(j=0; j<10; j++) {
			if(i==SCORE_MARATHON) {
				highscores[i][j]=0;
			} else {
				highscores[i][j]=-9999999;
			}
			switch(j) {
			case 0:
				strcpy(highscorenames[i][j], "TONG!");
				break;
			case 1:
				strcpy(highscorenames[i][j], "v1.3");
				break;
			case 2:
				strcpy(highscorenames[i][j], "2014");
				break;
			case 3:
				strcpy(highscorenames[i][j], "Owen");
				break;
			case 4:
				strcpy(highscorenames[i][j], "Swerk");
				break;
			case 5:
				strcpy(highscorenames[i][j], "strom");
				break;
			case 6:
				strcpy(highscorenames[i][j], "pend-");
				break;
			case 7:
				strcpy(highscorenames[i][j], "-uin-");
				break;
			case 8:
				strcpy(highscorenames[i][j], "-bits");
				break;
			case 9:
			default:
				strcpy(highscorenames[i][j], " :^)");
				break;
			}
		}
	}
	strcpy(highscoretext, "  High Scores");
	if(path) {
		configfile=new char[strlen(path)+strlen(CONFIG_FILENAME)+2];
		configfile[0]='\0';
		strcat(configfile, path);
		strcat(configfile, "/");
		strcat(configfile, CONFIG_FILENAME);
	} else {
		configfile=new char[strlen(CONFIG_FILENAME)+1];
		strcpy(configfile, CONFIG_FILENAME);
	}

	optionmenu=new Menu();
	optionmenu->SetPos(112 / POSRATIO, 80 / POSRATIO);
	optionmenu->AddChoice("Full Screen       = ?", NORMAL);
	optionmenu->AddChoice("Color Depth       = ?", NORMAL);
	optionmenu->AddChoice("Frames/Sec Limit  = ?", NORMAL);
	optionmenu->AddChoice("Background Scroll = ?", NORMAL);
	optionmenu->AddChoice("Audio Buffers     = ?", NORMAL);
	optionmenu->AddChoice("Audio Frequency   = ?", NORMAL);
	optionmenu->AddChoice("Sound Effects     = ?", NORMAL);
	optionmenu->AddChoice("Background Music...", NORMAL);
	optionmenu->AddChoice("Graphical Theme...", NORMAL);
	optionmenu->AddChoice("Back to Main Menu", NORMAL);

	custommenu=new Menu();
	custommenu->SetPos(48 / POSRATIO, 48 / POSRATIO);
	custommenu->AddChoice("Difficulty Level        = ??????", NORMAL);
	custommenu->AddChoice("Pong Paddle Placement   = ?", NORMAL);
	custommenu->AddChoice("Pong Paddle Size        = ?", NORMAL);
	custommenu->AddChoice("Tetris Pieces Speed Up  = ?", NORMAL);
	custommenu->AddChoice("Pong Ball Speeds Up     = ?", NORMAL);
	custommenu->AddChoice("Begin with Random Rules = ?", NORMAL);
	custommenu->AddChoice("New Rules Every         = ?", NORMAL);
	custommenu->AddChoice("Speed                   = ?", NORMAL);
	custommenu->AddChoice("Begin with Stack Junk   = ?", NORMAL);
	custommenu->AddChoice("Allowed Ball+Tetrad Reactions...  ", NORMAL);
	custommenu->AddChoice("Allowed Ball+Stack  Reactions...  ", NORMAL);
	custommenu->AddChoice("Back to Main Menu", NORMAL);

	ballstackmenu=new Menu();
	ballstackmenu->SetPos(144 / POSRATIO, 96 / POSRATIO);
	ballstackmenu->AddChoice("No Reaction      = ?", NORMAL);
	ballstackmenu->AddChoice("Kill Ball        = ?", NORMAL);
	ballstackmenu->AddChoice("Bounce Ball      = ?", NORMAL);
	ballstackmenu->AddChoice("Join Together    = ?", NORMAL);
	ballstackmenu->AddChoice("Kill Stack       = ?", NORMAL);
	ballstackmenu->AddChoice("Break Stack      = ?", NORMAL);
	ballstackmenu->AddChoice("Break a Line     = ?", NORMAL);
	ballstackmenu->AddChoice("Back to Rules Menu", NORMAL);

	balltetradmenu=new Menu();
	balltetradmenu->SetPos(144 / POSRATIO, 64 / POSRATIO);
	balltetradmenu->AddChoice("No Reaction      = ?", NORMAL);
	balltetradmenu->AddChoice("Kill Ball        = ?", NORMAL);
	balltetradmenu->AddChoice("Bounce Ball      = ?", NORMAL);
	balltetradmenu->AddChoice("Kill Tetrad      = ?", NORMAL);
	balltetradmenu->AddChoice("Join Together    = ?", NORMAL);
	balltetradmenu->AddChoice("Break Tetrad     = ?", NORMAL);
	balltetradmenu->AddChoice("Drop Tetrad      = ?", NORMAL);
	balltetradmenu->AddChoice("Push Tetrad      = ?", NORMAL);
	balltetradmenu->AddChoice("Rotate Tetrad    = ?", NORMAL);
	balltetradmenu->AddChoice("Back to Rules Menu", NORMAL);

	musicmenu=new Menu();
	musicmenu->SetPos(80 / POSRATIO, 32 / POSRATIO);
	musicmenu->AddChoice("None", NORMAL);
	musicmenu->AddChoice("Shuffle", NORMAL);
	musicmenu->AddChoice("TONG Theme by Marcus Hirt", NORMAL);
	musicmenu->AddChoice("Beta Beat", NORMAL);
	musicmenu->AddChoice("Stuck in a Mailbox", NORMAL);
	musicmenu->AddChoice("The Diplomat", NORMAL);
	musicmenu->AddChoice("Fanfare for the Common Rabbit", NORMAL);
	musicmenu->AddChoice("Beyond 2000", NORMAL);
	musicmenu->AddChoice("Donkey Rhubarb", NORMAL);
	musicmenu->AddChoice("Flock", NORMAL);
	musicmenu->AddChoice("One-liner", NORMAL);
	musicmenu->AddChoice("Reach Reach", NORMAL);
	musicmenu->AddChoice("Squib", NORMAL);

	controlmenu=new Menu();
	controlmenu->SetPos(64 / POSRATIO, 112 / POSRATIO);
	controlmenu->AddChoice("Reset to Defaults              ", NORMAL);
	controlmenu->AddChoice("Tetrad Move Left  = ?", NORMAL);
	controlmenu->AddChoice("Tetrad Move Right = ?", NORMAL);
	controlmenu->AddChoice("Tetrad Move Down  = ?", NORMAL);
	controlmenu->AddChoice("Tetrad Rotate CW  = ?", NORMAL);
	controlmenu->AddChoice("Tetrad Rotate CCW = ?", NORMAL);
	controlmenu->AddChoice("Paddle Movement   = Mouse", DISABLED);
	controlmenu->AddChoice("Back to Main Menu", NORMAL);
	controlmenu->SetActive(true);

	thememenu=new Menu();
	thememenu->SetPos(256 / POSRATIO, 192 / POSRATIO);
	thememenu->AddChoice("Default", NORMAL);
	thememenu->AddChoice("Shiny", NORMAL);
	thememenu->AddChoice("X-Mas", NORMAL);
	thememenu->AddChoice("8-Bit", NORMAL);
	thememenu->AddChoice("Space", NORMAL);
	thememenu->AddChoice("Clean", NORMAL);

	SetMusic(MUSIC_SHUFFLE);
	SetSoundFx(true);
	SetLevel(LEVEL_NORMAL);
	SetTheme(THEME_DEFAULT);
	SetBgScroll(true);

	SetVideoFlags(0
#ifndef GP2X
				  |SDL_DOUBLEBUF
				  |SDL_RLEACCEL
#endif
				  //|SDL_ANYFORMAT //this usually makes win32 fullscreen slow
				  //|SDL_FULLSCREEN //eh, let's not default to it.
				  );
#if defined GP2X || WII
	SetVideoBpp(16); //16, 24, 32
	SetAudioFreq(22050); //22050, 44100
#else
	SetVideoBpp(24); //16, 24, 32
	SetFpsLimit(FPS_60);
	SetAudioFreq(44100); //22050, 44100
#endif
	SetAudioBuffers(512); //512, 1024, 2048, 4096

	DefaultRules();
	DefaultSettings();
	DefaultControls();
	SetStart();
}

Option::~Option() {
	delete optionmenu;
	delete custommenu;
	delete ballstackmenu;
	delete balltetradmenu;
	delete musicmenu;
	delete controlmenu;
	delete thememenu;
}

bool Option::InsertScore(long int score, char* name, int type) {
	int i;
	long int bumpedscore=0, tempscore=0;
	char bumpedname[6]="\0", tempname[6]="\0";
	bool inserted=false;
	for(i=0; i<10; i++) {
		if(inserted) {
			tempscore=highscores[type][i];
			strcpy(tempname, highscorenames[type][i]);
			highscores[type][i]=bumpedscore;
			strcpy(highscorenames[type][i], bumpedname);
			bumpedscore=tempscore;
			strcpy(bumpedname, tempname);
		} else if(score>highscores[type][i]) {
			bumpedscore=highscores[type][i];
			strcpy(bumpedname, highscorenames[type][i]);
			highscores[type][i]=score;
			strcpy(highscorenames[type][i], name);
			inserted=true;
		}
	}
	return inserted;
}

long int Option::GetHighScore(int type, int pos) {
	return highscores[type][pos];
}

const char* Option::GetHighScoreText(int type) {
	int i;
	char tempstring[20];
	strcpy(highscoretext, "  High Scores\n");
	switch(type) {
	case SCORE_MARATHON:
		strcat(highscoretext, " Marathon Mode\n");
		break;
	case SCORE_2_MIN:
		strcat(highscoretext, " 2 Minute Mode\n");
		break;
	case SCORE_5_MIN:
		strcat(highscoretext, " 5 Minute Mode\n");
		break;
	case SCORE_10_MIN:
	default:
		strcat(highscoretext, "10 Minute  Mode\n");
		break;
	}
	strcat(highscoretext, "\n");
	for(i=0; i<10; i++) {
		sprintf(tempstring, "%08li  %s\n",
				highscores[type][i], highscorenames[type][i]);
		strcat(highscoretext, tempstring);
	}
	return highscoretext;
}

void Option::DefaultRules() {
	ruleballstack=BALL_BREAKS_STACK;
	ruleballtetrad=BALL_MOVES_TETRAD;
	rulestackball=STACK_BOUNCES_BALL;
	ruletetradball=TETRAD_BOUNCES_BALL;

	AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
	AllowRule(NO_REACTION|RULETYPE_STACKBALL);
	AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
	AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
	AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
	DenyRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
	AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
	AllowRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);

	AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
	AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
	AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
	AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
	AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
	AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
	AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
	AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
	AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
	AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);

	strcpy(ruletext, "Default rules");
	return;
}

void Option::DefaultSettings() {
	SetMarathon(true);
	SetSpeed(SPEED_NORMAL);
	SetPadPlacement(PADS_ALL);
	SetPadSize(PADS_NORMAL);
	SetJunk(JUNK_NONE);
	SetTetSpeedUp(true);
	SetBallSpeedsUp(true);
	SetRandomRule(false);
	SetRuleFrequency(EVERY_0_30);

	return;
}

void Option::DefaultControls() {
	mapkeys[KEY_TETLEFT]=SDLK_LEFT;
	mapkeys[KEY_TETRIGHT]=SDLK_RIGHT;
	mapkeys[KEY_TETDOWN]=SDLK_DOWN;
	mapkeys[KEY_TETCW]=SDLK_UP;
	mapkeys[KEY_TETCCW]=SDLK_SPACE;
	settingkey=-1;
	UpdateKeys();
	return;
}

void Option::SetStart() {
	int i;

	//set each rule to first available, try to skip 0==no reaction
	for(i=0; i<BALL_STACK_RULES; i++) {
		if(allowballstack[i]) {
			ruleballstack=i;
			if(i>0) {
				break;
			}
		}
	}
	for(i=0; i<BALL_TETRAD_RULES; i++) {
		if(allowballtetrad[i]) {
			ruleballtetrad=i;
			if(i>0) {
				break;
			}
		}
	}
	for(i=0; i<STACK_BALL_RULES; i++) {
		if(allowstackball[i]) {
			rulestackball=i;
			if(i>0) {
				break;
			}
		}
	}
	for(i=0; i<TETRAD_BALL_RULES; i++) {
		if(allowtetradball[i]) {
			ruletetradball=i;
			if(i>0) {
				break;
			}
		}
	}
	for(i=0; i<10; i++) {
		NewRule();
	}

	return;
}

//FIXME: obsolete?
void Option::Reset() {
	printf("option::reset is obsolete...\n");
	ruleballstack=startballstack;
	ruleballtetrad=startballtetrad;
	rulestackball=startstackball;
	ruletetradball=starttetradball;
	return;
}

//allow a rule, which is a specific rule OR'd with a ruletype
void Option::AllowRule(int whichrule) {
	if((whichrule & RULETYPE_BALLSTACK) &&
	   (whichrule & RULETYPE_MASK) < BALL_STACK_RULES) {
		allowballstack[whichrule & RULETYPE_MASK]=true;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			ballstackmenu->ModChoice("No Reaction      = ON", 1);
			break;
		case BALL_JOINS_STACK:
			ballstackmenu->ModChoice("Join Together    = ON", 4);
			break;
		case BALL_KILLS_STACK:
			ballstackmenu->ModChoice("Kill Stack       = ON", 5);
			break;
		case BALL_BREAKS_STACK:
			ballstackmenu->ModChoice("Break Stack      = ON", 6);
			break;
		case BALL_LINEBREAK_STACK:
			ballstackmenu->ModChoice("Break a Line     = ON", 7);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_BALLTETRAD) &&
			  (whichrule & RULETYPE_MASK) < BALL_TETRAD_RULES) {
		allowballtetrad[whichrule & RULETYPE_MASK]=true;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			balltetradmenu->ModChoice("No Reaction      = ON", 1);
			break;
		case BALL_KILLS_TETRAD:
			balltetradmenu->ModChoice("Kill Tetrad      = ON", 4);
			break;
		case BALL_JOINS_TETRAD:
			balltetradmenu->ModChoice("Join Together    = ON", 5);
			break;
		case BALL_BREAKS_TETRAD:
			balltetradmenu->ModChoice("Break Tetrad     = ON", 6);
			break;
		case BALL_DROPS_TETRAD:
			balltetradmenu->ModChoice("Drop Tetrad      = ON", 7);
			break;
		case BALL_MOVES_TETRAD:
			balltetradmenu->ModChoice("Push Tetrad      = ON", 8);
			break;
		case BALL_ROTATES_TETRAD:
			balltetradmenu->ModChoice("Rotate Tetrad    = ON", 9);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_STACKBALL) &&
			  (whichrule & RULETYPE_MASK) < STACK_BALL_RULES) {
		allowstackball[whichrule & RULETYPE_MASK]=true;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			ballstackmenu->ModChoice("No Reaction      = ON", 1);
			break;
		case STACK_KILLS_BALL:
			ballstackmenu->ModChoice("Kill Ball        = ON", 2);
			break;
		case STACK_BOUNCES_BALL:
			ballstackmenu->ModChoice("Bounce Ball      = ON", 3);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_TETRADBALL) &&
			  (whichrule & RULETYPE_MASK) < TETRAD_BALL_RULES) {
		allowtetradball[whichrule & RULETYPE_MASK]=true;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			balltetradmenu->ModChoice("No Reaction      = ON", 1);
			break;
		case TETRAD_KILLS_BALL:
			balltetradmenu->ModChoice("Kill Ball        = ON", 2);
			break;
		case TETRAD_BOUNCES_BALL:
			balltetradmenu->ModChoice("Bounce Ball      = ON", 3);
			break;
		default:
			break;
		}
	}
	SetStart();
	return;
}

void Option::DenyRule(int whichrule) {
	if((whichrule & RULETYPE_BALLSTACK) &&
	   (whichrule & RULETYPE_MASK) < BALL_STACK_RULES) {
		allowballstack[whichrule & RULETYPE_MASK]=false;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			ballstackmenu->ModChoice("No Reaction      = OFF", 1);
			break;
		case BALL_JOINS_STACK:
			ballstackmenu->ModChoice("Join Together    = OFF", 4);
			break;
		case BALL_KILLS_STACK:
			ballstackmenu->ModChoice("Kill Stack       = OFF", 5);
			break;
		case BALL_BREAKS_STACK:
			ballstackmenu->ModChoice("Break Stack      = OFF", 6);
			break;
		case BALL_LINEBREAK_STACK:
			ballstackmenu->ModChoice("Break a Line     = OFF", 7);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_BALLTETRAD) &&
			  (whichrule & RULETYPE_MASK) < BALL_TETRAD_RULES) {
		allowballtetrad[whichrule & RULETYPE_MASK]=false;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			balltetradmenu->ModChoice("No Reaction      = OFF", 1);
			break;
		case BALL_KILLS_TETRAD:
			balltetradmenu->ModChoice("Kill Tetrad      = OFF", 4);
			break;
		case BALL_JOINS_TETRAD:
			balltetradmenu->ModChoice("Join Together    = OFF", 5);
			break;
		case BALL_BREAKS_TETRAD:
			balltetradmenu->ModChoice("Break Tetrad     = OFF", 6);
			break;
		case BALL_DROPS_TETRAD:
			balltetradmenu->ModChoice("Drop Tetrad      = OFF", 7);
			break;
		case BALL_MOVES_TETRAD:
			balltetradmenu->ModChoice("Push Tetrad      = OFF", 8);
			break;
		case BALL_ROTATES_TETRAD:
			balltetradmenu->ModChoice("Rotate Tetrad    = OFF", 9);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_STACKBALL) &&
			  (whichrule & RULETYPE_MASK) < STACK_BALL_RULES) {
		allowstackball[whichrule & RULETYPE_MASK]=false;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			ballstackmenu->ModChoice("No Reaction      = OFF", 1);
			break;
		case STACK_KILLS_BALL:
			ballstackmenu->ModChoice("Kill Ball        = OFF", 2);
			break;
		case STACK_BOUNCES_BALL:
			ballstackmenu->ModChoice("Bounce Ball      = OFF", 3);
			break;
		default:
			break;
		}
	} else if((whichrule & RULETYPE_TETRADBALL) &&
			  (whichrule & RULETYPE_MASK) < TETRAD_BALL_RULES) {
		allowtetradball[whichrule & RULETYPE_MASK]=false;
		switch(whichrule & RULETYPE_MASK) {
		case NO_REACTION:
			balltetradmenu->ModChoice("No Reaction      = OFF", 1);
			break;
		case TETRAD_KILLS_BALL:
			balltetradmenu->ModChoice("Kill Ball        = OFF", 2);
			break;
		case TETRAD_BOUNCES_BALL:
			balltetradmenu->ModChoice("Bounce Ball      = OFF", 3);
			break;
		default:
			break;
		}
	}
	SetStart();
	return;
}

bool Option::IsAllowed(int whichrule) {
	bool isallowed=false;
	if((whichrule & RULETYPE_BALLSTACK) &&
	   (whichrule & RULETYPE_MASK) < BALL_STACK_RULES) {
		isallowed=allowballstack[whichrule & RULETYPE_MASK];
	} else if((whichrule & RULETYPE_BALLTETRAD) &&
			  (whichrule & RULETYPE_MASK) < BALL_TETRAD_RULES) {
		isallowed=allowballtetrad[whichrule & RULETYPE_MASK];
	} else if((whichrule & RULETYPE_STACKBALL) &&
			  (whichrule & RULETYPE_MASK) < STACK_BALL_RULES) {
		isallowed=allowstackball[whichrule & RULETYPE_MASK];
	} else if((whichrule & RULETYPE_TETRADBALL) &&
			  (whichrule & RULETYPE_MASK) < TETRAD_BALL_RULES) {
		isallowed=allowtetradball[whichrule & RULETYPE_MASK];
	}
	//  printf("is %d allowed? %d\n", whichrule, isallowed);
	return isallowed;
}

bool Option::ReadConfig() {
	char key[80];
	int value;
	char val2[80];
	int matches;
	bool success=false;
	FILE *config=fopen(configfile, "r");
	if(config) {
		success=true;
		matches=fscanf(config, "%79s = %d : %5s\n", key, &value, val2);
		while(matches==2 || matches==3) {
			if(!strcmp(key, "fullscreen")) {
				if(value==0) {
					SetVideoFlags(videoflags&(videoflags^SDL_FULLSCREEN));
				} else {
					SetVideoFlags(videoflags|SDL_FULLSCREEN);
				}
			} else if(!strcmp(key, "theme")) {
				if(value>=THEME_DEFAULT && value<=THEME_LAST) {
					SetTheme(value);
				}
			} else if(!strcmp(key, "bgscroll")) {
				if(value==0) {
					SetBgScroll(false);
				} else {
					SetBgScroll(true);
				}
			} else if(!strcmp(key, "music")) {
				if(value>=MUSIC_NONE && value<=MUSIC_LAST) {
					SetMusic(value);
				}
			} else if(!strcmp(key, "soundfx")) {
				if(value==0) {
					SetSoundFx(false);
				} else {
					SetSoundFx(true);
				}
			} else if(!strcmp(key, "marathon")) {
				if(value==0) {
					SetMarathon(false);
				} else {
					SetMarathon(true);
				}
			} else if(!strcmp(key, "level")) {
				if(value>=LEVEL_EASY && value<=LEVEL_CUSTOM) {
					SetLevel(value);
				}
			} else if(!strcmp(key, "speed")) {
				if(value>=SPEED_SLOW && value<=SPEED_FAST) {
					SetSpeed(value);
				}
			} else if(!strcmp(key, "junk")) {
				if(value==JUNK_NONE || value==JUNK_2 ||
				   value==JUNK_4 || value==JUNK_6) {
					SetJunk(value);
				}
			} else if(!strcmp(key, "padplacement")) {
				if(value==PADS_ALL || value==PADS_SIDES || value==PADS_TOPBOT) {
					SetPadPlacement(value);
				}
			} else if(!strcmp(key, "padsize")) {
				if(value>=PADS_SMALL && value<=PADS_12) {
					SetPadSize(value);
				}
			} else if(!strcmp(key, "tetspeedup")) {
				SetTetSpeedUp(value!=0);
			} else if(!strcmp(key, "ballspeedsup")) {
				SetBallSpeedsUp(value!=0);
			} else if(!strcmp(key, "randomrule")) {
				SetRandomRule(value!=0);
			} else if(!strcmp(key, "rulefrequency")) {
				if(value>=EVERY_0_15 && value<=EVERY_5_LINES) {
					SetRuleFrequency(value);
				}
			} else if(!strcmp(key, "videobpp")) {
				if(value==16 || value==24 || value==32) {
					SetVideoBpp(value);
				}
			} else if(!strcmp(key, "fpslimit")) {
				if(value==FPS_UNLIMITED || value==FPS_120 || value==FPS_60 ||
				   value==FPS_30 || value==FPS_24) {
					SetFpsLimit(value);
				}
			} else if(!strcmp(key, "audiobuffers")) {
				if(value==512 || value==1024 || value==2048 || value==4096) {
					SetAudioBuffers(value);
				}
			} else if(!strcmp(key, "audiofreq")) {
				if(value==22050 || value==44100) {
					SetAudioFreq(value);
				}
			} else if(!strcmp(key, "keytetleft")) {
				mapkeys[KEY_TETLEFT]=(SDLKey)value;
			} else if(!strcmp(key, "keytetright")) {
				mapkeys[KEY_TETRIGHT]=(SDLKey)value;
			} else if(!strcmp(key, "keytetdown")) {
				mapkeys[KEY_TETDOWN]=(SDLKey)value;
			} else if(!strcmp(key, "keytetcw")) {
				mapkeys[KEY_TETCW]=(SDLKey)value;
			} else if(!strcmp(key, "keytetccw")) {
				mapkeys[KEY_TETCCW]=(SDLKey)value;
			} else if(!strcmp(key, "ballstacknone")) {
				if(value) {
					AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
					AllowRule(NO_REACTION|RULETYPE_STACKBALL);
				} else {
					DenyRule(NO_REACTION|RULETYPE_BALLSTACK);
					DenyRule(NO_REACTION|RULETYPE_STACKBALL);
				}
			} else if(!strcmp(key, "stackkillsball")) {
				if(value) {
					AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
				} else {
					DenyRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
				}
			} else if(!strcmp(key, "stackbouncesball")) {
				if(value) {
					AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
				} else {
					DenyRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
				}
			} else if(!strcmp(key, "balljoinsstack")) {
				if(value) {
					AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
				} else {
					DenyRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
				}
			} else if(!strcmp(key, "ballkillsstack")) {
				if(value) {
					AllowRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
				} else {
					DenyRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
				}
			} else if(!strcmp(key, "ballbreaksstack")) {
				if(value) {
					AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
				} else {
					DenyRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
				}
			} else if(!strcmp(key, "balllinebreakstack")) {
				if(value) {
					AllowRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
				} else {
					DenyRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
				}
			} else if(!strcmp(key, "balltetradnone")) {
				if(value) {
					AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
					AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
				} else {
					DenyRule(NO_REACTION|RULETYPE_BALLTETRAD);
					DenyRule(NO_REACTION|RULETYPE_TETRADBALL);
				}
			} else if(!strcmp(key, "tetradkillsball")) {
				if(value) {
					AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
				} else {
					DenyRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
				}
			} else if(!strcmp(key, "tetradbouncesball")) {
				if(value) {
					AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
				} else {
					DenyRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
				}
			} else if(!strcmp(key, "balljoinstetrad")) {
				if(value) {
					AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "ballbreakstetrad")) {
				if(value) {
					AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "ballkillstetrad")) {
				if(value) {
					AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "balldropstetrad")) {
				if(value) {
					AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "ballmovestetrad")) {
				if(value) {
					AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "ballrotatestetrad")) {
				if(value) {
					AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
				} else {
					DenyRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
				}
			} else if(!strcmp(key, "scoremarathon")) {
				InsertScore(value, val2, SCORE_MARATHON);
			} else if(!strcmp(key,  "score2min")) {
				InsertScore(value, val2, SCORE_2_MIN);
			} else if(!strcmp(key, "score5min")) {
				InsertScore(value, val2, SCORE_5_MIN);
			} else if(!strcmp(key, "score10min")) {
				InsertScore(value, val2, SCORE_10_MIN);
			}
			matches=fscanf(config, "%79s = %d : %5s\n", key, &value, val2);
		}
		UpdateKeys();
	}
	if(config) {
		fclose(config);
	}
	return success;
}

bool Option::WriteConfig() {
	bool success=false;
	int i, j;
	FILE *config;

	config=fopen(configfile, "w");
	if(config) {
		fprintf(config, "WARNING:_Edit_this_file_at_your_own_risk! = 0\n");
		fprintf(config, "fullscreen = %d\n", videoflags&SDL_FULLSCREEN?1:0);
		fprintf(config, "music = %d\n", music);
		fprintf(config, "theme = %d\n", theme);
		fprintf(config, "bgscroll = %d\n", bgscroll?1:0);
		fprintf(config, "soundfx = %d\n", soundfx);
		fprintf(config, "marathon = %d\n", marathon);
		fprintf(config, "level = %d\n", level);
		fprintf(config, "speed = %d\n", speed);
		fprintf(config, "junk = %d\n", junk);
		fprintf(config, "padplacement = %d\n", padplacement);
		fprintf(config, "padsize = %d\n", padsize);
		fprintf(config, "tetspeedup = %d\n", tetspeedup);
		fprintf(config, "ballspeedsup = %d\n", ballspeedsup);
		fprintf(config, "randomrule = %d\n", randomrule);
		fprintf(config, "rulefrequency = %d\n", rulefrequency);
		fprintf(config, "videobpp = %d\n", videobpp);
		fprintf(config, "fpslimit = %d\n", fpslimit);
		fprintf(config, "audiobuffers = %d\n", audiobuffers);
		fprintf(config, "audiofreq = %d\n", audiofreq);

		fprintf(config, "keytetleft = %d\n", (int)mapkeys[KEY_TETLEFT]);
		fprintf(config, "keytetright = %d\n", (int)mapkeys[KEY_TETRIGHT]);
		fprintf(config, "keytetdown = %d\n", (int)mapkeys[KEY_TETDOWN]);
		fprintf(config, "keytetcw = %d\n", (int)mapkeys[KEY_TETCW]);
		fprintf(config, "keytetccw = %d\n", (int)mapkeys[KEY_TETCCW]);

		fprintf(config, "ballstacknone = %d\n", allowballstack[NO_REACTION]);
		fprintf(config, "stackkillsball = %d\n", allowstackball[STACK_KILLS_BALL]);
		fprintf(config, "stackbouncesball = %d\n",
				allowstackball[STACK_BOUNCES_BALL]);
		fprintf(config, "balljoinsstack = %d\n", allowballstack[BALL_JOINS_STACK]);
		fprintf(config, "ballkillsstack = %d\n", allowballstack[BALL_KILLS_STACK]);
		fprintf(config, "ballbreaksstack = %d\n",
				allowballstack[BALL_BREAKS_STACK]);
		fprintf(config, "balllinebreakstack = %d\n",
				allowballstack[BALL_LINEBREAK_STACK]);

		fprintf(config, "balltetradnone = %d\n", allowballtetrad[NO_REACTION]);
		fprintf(config, "tetradkillsball = %d\n",
				allowtetradball[TETRAD_KILLS_BALL]);
		fprintf(config, "tetradbouncesball = %d\n",
				allowtetradball[TETRAD_BOUNCES_BALL]);
		fprintf(config, "balljoinstetrad = %d\n",
				allowballtetrad[BALL_JOINS_TETRAD]);
		fprintf(config, "ballbreakstetrad = %d\n",
				allowballtetrad[BALL_BREAKS_TETRAD]);
		fprintf(config, "ballkillstetrad = %d\n",
				allowballtetrad[BALL_KILLS_TETRAD]);
		fprintf(config, "balldropstetrad = %d\n",
				allowballtetrad[BALL_DROPS_TETRAD]);
		fprintf(config, "ballmovestetrad = %d\n",
				allowballtetrad[BALL_MOVES_TETRAD]);
		fprintf(config, "ballrotatestetrad = %d\n",
				allowballtetrad[BALL_ROTATES_TETRAD]);
		for(i=0; i<SCORE_TYPES; i++) {
			for(j=0; j<10; j++) {
				switch(i) {
				case SCORE_MARATHON:
					fprintf(config, "scoremarathon = %li : %s\n", highscores[i][j],
							highscorenames[i][j]);
					break;
				case SCORE_2_MIN:
					fprintf(config, "score2min = %li : %s\n", highscores[i][j],
							highscorenames[i][j]);
					break;
				case SCORE_5_MIN:
					fprintf(config, "score5min = %li : %s\n", highscores[i][j],
							highscorenames[i][j]);
					break;
				case SCORE_10_MIN:
				default:
					fprintf(config, "score10min = %li : %s\n", highscores[i][j],
							highscorenames[i][j]);
					break;
				}
			}
		}
		fclose(config);
		success=true;
	}
	return success;
}

void Option::SetKey() {
	settingkey=controlmenu->GetChoice();
	controlmenu->SetState(settingkey, HIGHLIGHTED);
	return;
}

bool Option::SetKey(SDLKey newkey) {
	int i;
	bool mapped=true;
	for(i=0; i<KEY_TYPES; i++) {
		if(i==settingkey-2) { //ok to set a key to the same value
			continue;
		}
		if(mapkeys[i]==newkey) { //swap if they use a duplicate
			mapkeys[i]=mapkeys[settingkey-2];
			//      mapped=false;
		}
	}
	if(mapped) {
		mapkeys[settingkey-2]=newkey;
		controlmenu->SetState(settingkey, NORMAL);
		settingkey=-1;
	}
	UpdateKeys();
	return mapped;
}

SDLKey Option::GetKey(int which) {
	return mapkeys[which];
}

bool Option::IsSettingKey() {
	return (settingkey!=-1);
}

void Option::CancelSetKey() {
	controlmenu->SetState(settingkey, NORMAL);
	UpdateKeys();
	settingkey=-1;
	return;
}

void Option::UpdateKeys() {
	char tempstring[40];
	strcpy(tempstring, "Tetrad Move Left  = ");
	strcat(tempstring, SDL_GetKeyName(mapkeys[KEY_TETLEFT]));
	controlmenu->ModChoice(tempstring, 2);
	strcpy(tempstring, "Tetrad Move Right = ");
	strcat(tempstring, SDL_GetKeyName(mapkeys[KEY_TETRIGHT]));
	controlmenu->ModChoice(tempstring, 3);
	strcpy(tempstring, "Tetrad Move Down  = ");
	strcat(tempstring, SDL_GetKeyName(mapkeys[KEY_TETDOWN]));
	controlmenu->ModChoice(tempstring, 4);
	strcpy(tempstring, "Tetrad Rotate CW  = ");
	strcat(tempstring, SDL_GetKeyName(mapkeys[KEY_TETCW]));
	controlmenu->ModChoice(tempstring, 5);
	strcpy(tempstring, "Tetrad Rotate CCW = ");
	strcat(tempstring, SDL_GetKeyName(mapkeys[KEY_TETCCW]));
	controlmenu->ModChoice(tempstring, 6);
	return;
}

void Option::SetBallSpeedsUp(bool newballspeedsup) {
	ballspeedsup=newballspeedsup;
	if(ballspeedsup) {
		custommenu->ModChoice("Pong Ball Speeds Up     = YES", 5);
	} else {
		custommenu->ModChoice("Pong Ball Speeds Up     = NO", 5);
	}
	return;
}

bool Option::GetBallSpeedsUp() {
	return ballspeedsup;
}

int Option::GetRuleBallStack() {
	return ruleballstack;
}

int Option::GetRuleBallTetrad() {
	return ruleballtetrad;
}

int Option::GetRuleStackBall() {
	return rulestackball;
}

int Option::GetRuleTetradBall() {
	return ruletetradball;
}

const char* Option::GetRuleText() {
	return ruletext;
}

void Option::NewRule() {
	int random=rand()%4;
	int tries=0; //try to change rule but not forever
	if(ruleballstack==BALL_KILLS_STACK) {
		random=0;          //this rule sucks after too long
	}
	switch(random) {     //change one of four rule types
	case 0:                //change ball/stack rule
		random=ruleballstack;
		while((random==ruleballstack || !allowballstack[random]) && tries++<10) {
			random=rand()%BALL_STACK_RULES;
		}
		if(tries<10) {
			ruleballstack=random;
			switch(random) {
			case BALL_BREAKS_STACK:
				strcpy(ruletext, "Ball breaks stack!");
				break;
			case BALL_LINEBREAK_STACK:
				strcpy(ruletext, "Ball clears line!");
				break;
			case BALL_KILLS_STACK:
				strcpy(ruletext, "Ball kills stack!");
				break;
			case BALL_JOINS_STACK: 
				strcpy(ruletext, "Ball joins stack!");
			case NO_REACTION:
			default:
				strcpy(ruletext, "Ball ignores stack!");
				break;
			}
		} else {
			strcpy(ruletext, "Same rules!");
		}
		break;
	case 1:                //change ball/tetrad rule
		random=ruleballtetrad;
		while((random==ruleballtetrad || !allowballtetrad[random]) && tries++<10) {
			random=rand()%BALL_TETRAD_RULES;
		}
		if(tries<10) {
			ruleballtetrad=random;
			switch(random) {
			case BALL_BREAKS_TETRAD:
				strcpy(ruletext, "Ball breaks tetrad!");
				break;
			case BALL_MOVES_TETRAD:
				strcpy(ruletext, "Ball moves tetrad!");
				break;
			case BALL_ROTATES_TETRAD:
				strcpy(ruletext, "Ball rotates tetrad!");
				break;
			case BALL_DROPS_TETRAD:
				strcpy(ruletext, "Ball drops tetrad!");
				break;
			case BALL_KILLS_TETRAD:
				strcpy(ruletext, "Ball kills tetrad!");
				break;
			case BALL_JOINS_TETRAD:
				strcpy(ruletext, "Ball joins tetrad!");
				break;
			case NO_REACTION:
			default:
				strcpy(ruletext, "Ball ignores tetrad!");
				break;
			}
		} else {
			strcpy(ruletext, "Same rules!");
		}
		break;
	case 2:                //change stack/ball rule
		random=rulestackball;
		while((random==rulestackball || !allowstackball[random]) && tries++<10) {
			random=rand()%STACK_BALL_RULES;
		}
		if(tries<10) {
			rulestackball=random;
			if(ruleballstack==BALL_JOINS_STACK) {
				ruleballstack=NO_REACTION;
			}
			switch(random) {
			case STACK_BOUNCES_BALL:
				ruleballstack=NO_REACTION;
				strcpy(ruletext, "Stack bounces ball!");
				break;
			case STACK_KILLS_BALL:
				strcpy(ruletext, "Stack kills ball!");
				break;
			case NO_REACTION:
			default:
				strcpy(ruletext, "Stack Ignores ball!");
				break;
			}
		} else {
			strcpy(ruletext, "Same rules!");
		}
		break;
	case 3:                //change tetrad/ball rule
		random=ruletetradball;
		while((random==ruletetradball || !allowtetradball[random]) && tries++<10) {
			random=rand()%TETRAD_BALL_RULES;
		}
		if(tries<10) {
			ruletetradball=random;
			if(ruleballtetrad==BALL_JOINS_TETRAD) {
				ruleballtetrad=NO_REACTION;
			}
			switch(random) {
			case TETRAD_BOUNCES_BALL:
				ruleballtetrad=NO_REACTION;
				strcpy(ruletext, "Tetrad bounces ball!");
				break;
			case TETRAD_KILLS_BALL:
				strcpy(ruletext, "Tetrad kills ball!");
				break;
			case NO_REACTION:
			default:
				strcpy(ruletext, "Tetrad ignores ball!");
				break;
			}
		} else {
			strcpy(ruletext, "Same rules!");
		}
	default:
		break;
	}
	return;
}

int Option::GetLevel() {
	return level;
}

int Option::GetJunk() {
	return junk;
}

int Option::GetPadPlacement() {
	return padplacement;
}

int Option::GetPadSize() {
	return padsize;
}

int Option::GetPadSizeX() {
	int padsizex;
	if(padplacement==PADS_SIDES) {
		padsizex=10;
	} else {
		switch(padsize) {
		case PADS_BIG:
			padsizex=6;
			break;
		case PADS_SMALL:
			padsizex=2;
			break;
		case PADS_15:
			padsizex=2;
			break;
		case PADS_13:
			padsizex=3;
			break;
		case PADS_NORMAL:
		default:
			padsizex=4;
			break;
		}
	}
	return padsizex;
}

int Option::GetPadSizeY() {
	int padsizey;
	if(padplacement==PADS_TOPBOT) {
		padsizey=20;
	} else {
		switch(padsize) {
		case PADS_BIG:
			padsizey=6;
			break;
		case PADS_SMALL:
			padsizey=2;
			break;
		case PADS_15:
			padsizey=4;
			break;
		case PADS_13:
			padsizey=6;
			break;
		case PADS_NORMAL:
		default:
			padsizey=4;
			break;
		}
	}
	return padsizey;
}

bool Option::GetTetSpeedUp() {
	return tetspeedup;
}

int Option::GetMusic() {
	return music;
}

int Option::GetTheme() {
	return theme;
}

bool Option::GetBgScroll() {
	return bgscroll;
}

const char* Option::GetThemeString() {
	switch(theme) {
	case THEME_SHINY:
		return "-shiny";
		break;
	case THEME_XMAS:
		return "-xmas";
		break;
	case THEME_8BIT:
		return "-8bit";
		break;
	case THEME_SPACE:
		return "-space";
		break;
	case THEME_CLEAN:
		return "-clean";
		break;
	case THEME_DEFAULT:
	default:
		return "";
		break;
	}
}

const char* Option::GetMusicFile() {
	int song=music;
	if(music==MUSIC_SHUFFLE) {
		song=(rand()%11)+2;
		curmusic=song;
	}
	switch(song) {
	case MUSIC_TONG:
		return "media/tong.ogg";
		break;
	case MUSIC_I12:
		return "media/i12bpvd.ogg";
		break;
	case MUSIC_STUCK:
		return "media/stuck-in-a-mailbox.ogg";
		break;
	case MUSIC_DIPLOMAT:
		return "media/the-diplomat.ogg";
		break;
	case MUSIC_FANFARE:
		return "media/fanfare.ogg";
		break;
	case MUSIC_BEYOND:
		return "media/beyond2000.ogg";
		break;
	case MUSIC_DONKEY:
		return "media/donkeyrhubarb.ogg";
		break;
	case MUSIC_FLOCK:
		return "media/flock.ogg";
		break;
	case MUSIC_ONELINER:
		return "media/one-liner.ogg";
		break;
	case MUSIC_REACH:
		return "media/reachreach.ogg";
		break;
	case MUSIC_SQUIB:
		return "media/squib.ogg";
		break;
	case MUSIC_SHUFFLE:
		printf("GetMusicFile: shouldn't ever get here...\n");
	case MUSIC_NONE:
	default:
		break;
	}
	return NULL;
}

const char* Option::GetCurrentMusicFile() {
	if(music==MUSIC_SHUFFLE) {
		music=curmusic;
		strcpy(tempmusic, GetMusicFile());
		music=MUSIC_SHUFFLE;
	} else {
		strcpy(tempmusic, GetMusicFile());
	}
	return (const char*)tempmusic;
}

bool Option::GetRandomRule() {
	return randomrule;
}

int Option::GetRuleFrequency() {
	return rulefrequency;
}

Uint32 Option::GetVideoFlags() {
	return videoflags;
}

int Option::GetVideoBpp() {
	return videobpp;
}

int Option::GetFpsLimit() {
	return fpslimit;
}

int Option::GetAudioBuffers() {
	return audiobuffers;
}

int Option::GetAudioFreq() {
	return audiofreq;
}

bool Option::GetMarathon() {
	return marathon;
}

int Option::GetSpeed() {
	return speed;
}

int Option::GetSpeedTetrad() {
	int tetspeed; //inverted really, number of msecs 'til it drops one space
	switch(speed) {
	case SPEED_SLOW:
		tetspeed=1500;
		break;
	case SPEED_NORMAL:
		tetspeed=1000;
		break;
	case SPEED_FAST:
	default:
		tetspeed=500;
		break;
	}
	return tetspeed;
}

float Option::GetSpeedBall() {
	float ballspeed;
	switch(speed) {
	case SPEED_SLOW:
		ballspeed=48;
		break;
	case SPEED_NORMAL:
		ballspeed=64;
		break;
	case SPEED_FAST:
	default:
		ballspeed=96;
		break;
	}
	return ballspeed;;
}

bool Option::GetSoundFx() {
	return soundfx;
}

void Option::SetLevel() {
	if(level>=LEVEL_CUSTOM) {
		SetLevel(LEVEL_EASY);
	} else {
		SetLevel(level+1);
	}
	return;
}

void Option::SetLevel(int newlevel) {
	level=newlevel;
	switch(level) {
	case LEVEL_EASY:
		SetPadPlacement(PADS_SIDES);
		SetPadSize(PADS_BIG);
		SetTetSpeedUp(false);
		SetBallSpeedsUp(false);
		SetRandomRule(false);
		SetRuleFrequency(EVERY_0_45);
		SetSpeed(SPEED_SLOW);
		SetJunk(JUNK_NONE);
		DenyRule(NO_REACTION|RULETYPE_BALLSTACK);
		DenyRule(NO_REACTION|RULETYPE_STACKBALL);
		AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
		AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
		DenyRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
		DenyRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
		DenyRule(NO_REACTION|RULETYPE_BALLTETRAD);
		DenyRule(NO_REACTION|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
		DenyRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
		DenyRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
		DenyRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
		custommenu->ModChoice("Difficulty Level        = Easy", 1);
		break;
	case LEVEL_NORMAL:
		SetPadPlacement(PADS_ALL);
		SetPadSize(PADS_NORMAL);
		SetTetSpeedUp(true);
		SetBallSpeedsUp(false);
		SetRandomRule(true);
		SetRuleFrequency(EVERY_0_30);
		SetSpeed(SPEED_NORMAL);
		SetJunk(JUNK_2);
		AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_STACKBALL);
		AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
		AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
		AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
		DenyRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
		AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
		AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
		custommenu->ModChoice("Difficulty Level        = Normal", 1);
		break;
	case LEVEL_HARD:
		SetPadPlacement(PADS_ALL);
		SetPadSize(PADS_NORMAL);
		SetTetSpeedUp(true);
		SetBallSpeedsUp(true);
		SetRandomRule(true);
		SetRuleFrequency(EVERY_0_15);
		SetSpeed(SPEED_FAST);
		SetJunk(JUNK_4);
		AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_STACKBALL);
		AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
		AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
		AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
		AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
		AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
		custommenu->ModChoice("Difficulty Level        = Hard", 1);
		break;
	case LEVEL_INSANE:
		SetPadPlacement(PADS_ALL);
		SetPadSize(PADS_SMALL);
		SetTetSpeedUp(true);
		SetBallSpeedsUp(true);
		SetRandomRule(true);
		SetRuleFrequency(EVERY_BALL);
		SetSpeed(SPEED_FAST);
		SetJunk(JUNK_6);
		AllowRule(NO_REACTION|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_STACKBALL);
		AllowRule(STACK_KILLS_BALL|RULETYPE_STACKBALL);
		AllowRule(STACK_BOUNCES_BALL|RULETYPE_STACKBALL);
		AllowRule(BALL_JOINS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_KILLS_STACK|RULETYPE_BALLSTACK);
		AllowRule(BALL_BREAKS_STACK|RULETYPE_BALLSTACK);
		DenyRule(BALL_LINEBREAK_STACK|RULETYPE_BALLSTACK);
		AllowRule(NO_REACTION|RULETYPE_BALLTETRAD);
		AllowRule(NO_REACTION|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_KILLS_BALL|RULETYPE_TETRADBALL);
		AllowRule(TETRAD_BOUNCES_BALL|RULETYPE_TETRADBALL);
		AllowRule(BALL_JOINS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_BREAKS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_KILLS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_DROPS_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_MOVES_TETRAD|RULETYPE_BALLTETRAD);
		AllowRule(BALL_ROTATES_TETRAD|RULETYPE_BALLTETRAD);
		custommenu->ModChoice("Difficulty Level        = Insane", 1);
		break;
	case LEVEL_CUSTOM:
		DefaultSettings();
		DefaultRules();
		custommenu->ModChoice("Difficulty Level        = Custom", 1);
		break;
	}
	if(level==LEVEL_CUSTOM) {
		custommenu->SetState(2, NORMAL);
		custommenu->SetState(3, NORMAL);
		custommenu->SetState(4, NORMAL);
		custommenu->SetState(5, NORMAL);
		custommenu->SetState(6, NORMAL);
		custommenu->SetState(7, NORMAL);
		custommenu->SetState(8, NORMAL);
		custommenu->SetState(9, NORMAL);
		ballstackmenu->SetState(1, NORMAL);
		ballstackmenu->SetState(2, NORMAL);
		ballstackmenu->SetState(3, NORMAL);
		ballstackmenu->SetState(4, NORMAL);
		ballstackmenu->SetState(5, NORMAL);
		ballstackmenu->SetState(6, NORMAL);
		ballstackmenu->SetState(7, NORMAL);
		ballstackmenu->MoveCursor(SOUTH);
		balltetradmenu->SetState(1, NORMAL);
		balltetradmenu->SetState(2, NORMAL);
		balltetradmenu->SetState(3, NORMAL);
		balltetradmenu->SetState(4, NORMAL);
		balltetradmenu->SetState(5, NORMAL);
		balltetradmenu->SetState(6, NORMAL);
		balltetradmenu->SetState(7, NORMAL);
		balltetradmenu->SetState(8, NORMAL);
		balltetradmenu->SetState(9, NORMAL);
		balltetradmenu->MoveCursor(SOUTH);
	} else {
		custommenu->SetState(2, DISABLED);
		custommenu->SetState(3, DISABLED);
		custommenu->SetState(4, DISABLED);
		custommenu->SetState(5, DISABLED);
		custommenu->SetState(6, DISABLED);
		custommenu->SetState(7, DISABLED);
		custommenu->SetState(8, DISABLED);
		custommenu->SetState(9, DISABLED);
		ballstackmenu->SetState(1, DISABLED);
		ballstackmenu->SetState(2, DISABLED);
		ballstackmenu->SetState(3, DISABLED);
		ballstackmenu->SetState(4, DISABLED);
		ballstackmenu->SetState(5, DISABLED);
		ballstackmenu->SetState(6, DISABLED);
		ballstackmenu->SetState(7, DISABLED);
		ballstackmenu->MoveCursor(SOUTH);
		balltetradmenu->SetState(1, DISABLED);
		balltetradmenu->SetState(2, DISABLED);
		balltetradmenu->SetState(3, DISABLED);
		balltetradmenu->SetState(4, DISABLED);
		balltetradmenu->SetState(5, DISABLED);
		balltetradmenu->SetState(6, DISABLED);
		balltetradmenu->SetState(7, DISABLED);
		balltetradmenu->SetState(8, DISABLED);
		balltetradmenu->SetState(9, DISABLED);
		balltetradmenu->MoveCursor(SOUTH);
	}
	return;
}

void Option::SetPadPlacement(int newpadplacement) {
	padplacement=newpadplacement;
	switch(padplacement) {
	case PADS_SIDES:
		custommenu->ModChoice("Pong Paddle Placement   = Sides", 2);
		break;
	case PADS_TOPBOT:
		custommenu->ModChoice("Pong Paddle Placement   = Top&Bot", 2);
		break;
	case PADS_ALL:
		custommenu->ModChoice("Pong Paddle Placement   = All", 2);
		break;
	default:
		break;
	}
	return;
}

void Option::SetPadSize(int newpadsize) {
	padsize=newpadsize;
	switch(padsize) {
	case PADS_NORMAL:
		custommenu->ModChoice("Pong Paddle Size        = Normal", 3);
		break;
	case PADS_BIG:
		custommenu->ModChoice("Pong Paddle Size        = Big", 3);
		break;
	case PADS_15:
		custommenu->ModChoice("Pong Paddle Size        = 1/5", 3);
		break;
	case PADS_13:
		custommenu->ModChoice("Pong Paddle Size        = 1/3", 3);
		break;
	case PADS_12:
		custommenu->ModChoice("Pong Paddle Size        = 1/2", 3);
		break;
	case PADS_SMALL:
		custommenu->ModChoice("Pong Paddle Size        = Small", 3);
		break;
	default:
		break;
	}
	return;
}

void Option::SetTetSpeedUp(bool newtetspeedup) {
	tetspeedup=newtetspeedup;
	if(tetspeedup) {
		custommenu->ModChoice("Tetris Pieces Speed Up  = YES", 4);
	} else {
		custommenu->ModChoice("Tetris Pieces Speed Up  = NO", 4);
	}
	return;
}

void Option::SetMusic(int newmusic) {
	music=newmusic;
	curmusic=music;
	musicmenu->SetChoice(music+1);
	musicmenu->SetState(music+1, HIGHLIGHTED);
	return;
}

void Option::SetTheme(int newtheme) {
	theme=newtheme;
	thememenu->SetChoice(theme+1);
	thememenu->SetState(theme+1, HIGHLIGHTED);
	return;
}

void Option::SetRandomRule(bool newrandomrule) {
	randomrule=newrandomrule;
	if(randomrule) {
		strcpy(ruletext, "Random rules");
		custommenu->ModChoice("Begin with Random Rules = YES", 6);
	} else {
		custommenu->ModChoice("Begin with Random Rules = NO", 6);
	}
	return;
}

void Option::SetRuleFrequency(int newrulefrequency) {
	rulefrequency=newrulefrequency;
	switch(rulefrequency) {
	case EVERY_0_15:
		custommenu->ModChoice("New Rules Every         = 0:15", 7);
		break;
	case EVERY_0_30:
		custommenu->ModChoice("New Rules Every         = 0:30", 7);
		break;
	case EVERY_0_45:
		custommenu->ModChoice("New Rules Every         = 0:45", 7);
		break;
	case EVERY_1_00:
		custommenu->ModChoice("New Rules Every         = 1:00", 7);
		break;
	case EVERY_BALL:
		custommenu->ModChoice("New Rules Every         = Ball", 7);
		break;
	case EVERY_3_BALLS:
		custommenu->ModChoice("New Rules Every         = 3 Balls", 7);
		break;
	case EVERY_LINE:
		custommenu->ModChoice("New Rules Every         = Line", 7);
		break;
	case EVERY_5_LINES:
		custommenu->ModChoice("New Rules Every         = 5 Lines", 7);
		break;
	default:
		break;
	}
	return;
}

void Option::SetVideoFlags(Uint32 newvideoflags) {
	videoflags=newvideoflags;
	if(videoflags & SDL_FULLSCREEN) {
		optionmenu->ModChoice("Full Screen       = ON", 1);
	} else {
		optionmenu->ModChoice("Full Screen       = OFF", 1);
	}
	return;
}

void Option::SetBgScroll(bool newbgscroll) {
	bgscroll=newbgscroll;
	if(bgscroll) {
		optionmenu->ModChoice("Background Scroll = ON", 4);
	} else {
		optionmenu->ModChoice("Background Scroll = OFF", 4);
	}
	return;
}

void Option::SetVideoBpp(int newvideobpp) {
	char tempstring[40];

	videobpp=newvideobpp;
	sprintf(tempstring, "Color Depth       = %d", videobpp);
	optionmenu->ModChoice(tempstring, 2);
	return;
}

void Option::SetFpsLimit(int newfpslimit) {
	char tempstring[40];

	fpslimit=newfpslimit;
	if(fpslimit == FPS_24) {
		sprintf(tempstring, "Frames/Sec Limit  = 24");
	} else if(fpslimit == FPS_30) {
		sprintf(tempstring, "Frames/Sec Limit  = 30");
	} else if(fpslimit == FPS_60) {
		sprintf(tempstring, "Frames/Sec Limit  = 60");
	} else if(fpslimit == FPS_120) {
		sprintf(tempstring, "Frames/Sec Limit  = 120");
	} else {
		sprintf(tempstring, "Frames/Sec Limit  = OFF");
	}
	optionmenu->ModChoice(tempstring, 3);
	return;
}

void Option::SetAudioBuffers(int newaudiobuffers) {
	char tempstring[40];

	audiobuffers=newaudiobuffers;
	sprintf(tempstring, "Audio Buffers     = %d", audiobuffers);
	optionmenu->ModChoice(tempstring, 5);
	return;
}

void Option::SetAudioFreq(int newaudiofreq) {
	char tempstring[40];

	audiofreq=newaudiofreq;
	sprintf(tempstring, "Audio Frequency   = %d", audiofreq);
	optionmenu->ModChoice(tempstring, 6);
	return;
}

void Option::SetMarathon(bool newmarathon) {
	marathon=newmarathon;
	return;
}

void Option::SetSpeed() {
	if(speed>=SPEED_FAST) {
		SetSpeed(SPEED_SLOW);
	} else {
		SetSpeed(speed+1);
	}
	return;
}

void Option::SetSpeed(int newspeed) {
	speed=newspeed;
	switch(speed) {
	case SPEED_SLOW:
		custommenu->ModChoice("Speed                   = Slow", 8);
		break;
	case SPEED_NORMAL:
		custommenu->ModChoice("Speed                   = Normal", 8);
		break;
	case SPEED_FAST:
		custommenu->ModChoice("Speed                   = Fast", 8);
		break;
	default:
		break;
	}
	return;
}

void Option::SetJunk() {
	if(junk>=JUNK_6) {
		SetJunk(JUNK_NONE);
	} else {
		SetJunk(junk+2);
	}
	return;
}

void Option::SetJunk(int newjunk) {
	junk=newjunk;
	switch(junk) {
	case JUNK_NONE:
		custommenu->ModChoice("Begin with Stack Junk   = None", 9);
		break;
	case JUNK_2:
		custommenu->ModChoice("Begin with Stack Junk   = 2 Lines", 9);
		break;
	case JUNK_4:
		custommenu->ModChoice("Begin with Stack Junk   = 4 Lines", 9);
		break;
	case JUNK_6:
	default:
		custommenu->ModChoice("Begin with Stack Junk   = 6 Lines", 9);
		break;
	}
	return;
}

void Option::SetSoundFx(bool newsoundfx) {
	soundfx=newsoundfx;
	if(soundfx) {
		optionmenu->ModChoice("Sound Effects     = ON", 7);
	} else {
		optionmenu->ModChoice("Sound Effects     = OFF", 7);
	}
	return;
}

void Option::RuleFrequencyInit() {
	rulefreqballs=0;
	rulefreqlines=0;
	lastrule=SDL_GetTicks();
	return;
}

void Option::RuleFrequencyPause() {
	lastrule=SDL_GetTicks()-lastrule;
	return;
}

void Option::RuleFrequencyResume() {
	lastrule=SDL_GetTicks()-lastrule;
	return;
}

void Option::RuleFrequencyAddLines(int lines) {
	rulefreqlines+=lines;
	return;
}

void Option::RuleFrequencyAddBall() {
	rulefreqballs++;
	return;
}

bool Option::RuleFrequencyNow() {
	bool changerule=false;
	switch(rulefrequency) {
	case EVERY_0_15:
		if(SDL_GetTicks()-lastrule>15000) {
			changerule=true;
			lastrule+=15000;
		}
		break;
	case EVERY_0_30:
		if(SDL_GetTicks()-lastrule>30000) {
			changerule=true;
			lastrule=lastrule+30000;
		}
		break;
	case EVERY_0_45:
		if(SDL_GetTicks()-lastrule>45000) {
			changerule=true;
			lastrule=lastrule+45000;
		}
		break;
	case EVERY_1_00:
		if(SDL_GetTicks()-lastrule>60000) {
			changerule=true;
			lastrule=lastrule+60000;
		}
		break;
	case EVERY_BALL:
		if(rulefreqballs>0) {
			changerule=true;
			rulefreqballs=0;
		}
		break;
	case EVERY_3_BALLS:
		if(rulefreqballs>2) {
			changerule=true;
			rulefreqballs=0;
		}
		break;
	case EVERY_LINE:
		if(rulefreqlines>0) {
			changerule=true;
			rulefreqlines=0;
		}
		break;
	case EVERY_5_LINES:
		if(rulefreqlines>4) {
			changerule=true;
			rulefreqlines=0;
		}
		break;
	}
	return changerule;
}
