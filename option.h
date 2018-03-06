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

#ifndef __OPTION_H__
#define __OPTION_H__

#include <stdlib.h>
#include <stdio.h>
//#include <sys/types.h>
//#include <dirent.h>
class Option;

#include <string.h>

#include "SDL.h"
#include "text.h"

#define NORTH 1
#define SOUTH 2
#define EAST  3
#define WEST  4

#ifdef WIN32
#define CONFIG_FILENAME "config.txt"
#else
#define CONFIG_FILENAME ".tong-config"
#endif
#define MUSIC_PATH "media/music"

#ifdef GP2X
#define POSRATIO 2
#undef  CONFIG_FILENAME
#define CONFIG_FILENAME "config.txt"
#else
#define POSRATIO 1
#endif

#ifdef WII
#undef  CONFIG_FILENAME
#define CONFIG_FILENAME "config.txt"
#endif

#define NO_REACTION          0

#define BALL_BREAKS_STACK    0x00000001
#define BALL_LINEBREAK_STACK 0x00000002
#define BALL_KILLS_STACK     0x00000003
#define BALL_JOINS_STACK     0x00000004
#define BALL_STACK_RULES     0x00000005

#define BALL_MOVES_TETRAD    0x00000001
#define BALL_ROTATES_TETRAD  0x00000002
#define BALL_BREAKS_TETRAD   0x00000003
#define BALL_DROPS_TETRAD    0x00000004
#define BALL_KILLS_TETRAD    0x00000005
#define BALL_JOINS_TETRAD    0x00000006
#define BALL_TETRAD_RULES    0x00000007

#define STACK_BOUNCES_BALL   0x00000001
#define STACK_KILLS_BALL     0x00000002
#define STACK_BALL_RULES     0x00000003

//TODO: add splitting rules
#define TETRAD_BOUNCES_BALL  0x00000001
#define TETRAD_KILLS_BALL    0x00000002
#define TETRAD_BALL_RULES    0x00000003

#define RULETYPE_MASK        0x0000ffff
#define RULETYPE_BALLSTACK   0x00010000
#define RULETYPE_BALLTETRAD  0x00020000
#define RULETYPE_STACKBALL   0x00040000
#define RULETYPE_TETRADBALL  0x00080000

#define LEVEL_EASY     0
#define LEVEL_NORMAL   1
#define LEVEL_HARD     2
#define LEVEL_INSANE   3
#define LEVEL_CUSTOM   4

#define SPEED_SLOW     0
#define SPEED_NORMAL   1
#define SPEED_FAST     2

#define PADS_ALL       0
#define PADS_SIDES     1
#define PADS_TOPBOT    2

#define PADS_SMALL     0
#define PADS_NORMAL    1
#define PADS_BIG       2
#define PADS_15        3
#define PADS_13        4
#define PADS_12        5

#define EVERY_0_15     0
#define EVERY_0_30     1
#define EVERY_0_45     2
#define EVERY_1_00     3
#define EVERY_BALL     4
#define EVERY_3_BALLS  5
#define EVERY_LINE     6
#define EVERY_5_LINES  7

#define JUNK_NONE      0
#define JUNK_2         2
#define JUNK_4         4
#define JUNK_6         6

#define MUSIC_NONE     0
#define MUSIC_SHUFFLE  1
#define MUSIC_TONG     2
#define MUSIC_I12      3
#define MUSIC_STUCK    4
#define MUSIC_DIPLOMAT 5
#define MUSIC_FANFARE  6
#define MUSIC_BEYOND   7
#define MUSIC_DONKEY   8
#define MUSIC_FLOCK    9
#define MUSIC_ONELINER 10
#define MUSIC_REACH    11
#define MUSIC_SQUIB    12
#define MUSIC_LAST     12

#define SCORE_MARATHON 0
#define SCORE_2_MIN    1
#define SCORE_5_MIN    2
#define SCORE_10_MIN   3
#define SCORE_TYPES    4

#define KEY_TETLEFT    0
#define KEY_TETRIGHT   1
#define KEY_TETDOWN    2
#define KEY_TETCW      3
#define KEY_TETCCW     4
#define KEY_TYPES      5

#define THEME_DEFAULT  0
#define THEME_SHINY    1
#define THEME_XMAS     2
#define THEME_8BIT     3
#define THEME_SPACE    4
#define THEME_CLEAN    5
#define THEME_LAST     5

#define FPS_UNLIMITED  0
#define FPS_120        8
#define FPS_60        16
#define FPS_30        33
#define FPS_24        41

class Option {
 private:
	char* configfile;
	long int highscores[SCORE_TYPES][10];
	char highscorenames[SCORE_TYPES][10][6];
	int ruleballstack, ruleballtetrad, rulestackball, ruletetradball;
	int startballstack, startballtetrad, startstackball, starttetradball;
	int level, padplacement, padsize, music, curmusic, rulefrequency, speed;
	int junk;
	Uint32 lastrule;
	Uint32 videoflags;
	bool bgscroll;
	int theme;
	int videobpp;
	int fpslimit;
	int audiobuffers, audiofreq;
	int rulefreqlines, rulefreqballs;
	bool allowballstack[BALL_STACK_RULES];
	bool allowballtetrad[BALL_TETRAD_RULES];
	bool allowstackball[STACK_BALL_RULES];
	bool allowtetradball[TETRAD_BALL_RULES];
	bool ballspeedsup, tetspeedup, randomrule;
	bool marathon, soundfx;
	char ruletext[100];
	char tempmusic[80];
	char highscoretext[255]; //200 would do, actually...
	SDLKey mapkeys[KEY_TYPES];
	int settingkey;
	void UpdateKeys();
 public:
	Option();
	~Option();
	bool InsertScore(long int, char*, int);
	long int GetHighScore(int, int);
	const char* GetHighScoreText(int);
	Menu *optionmenu, *custommenu, *ballstackmenu, *balltetradmenu, *musicmenu;
	Menu *controlmenu, *thememenu;
	void DefaultRules();
	void DefaultSettings();
	void DefaultControls();
	void SetStart();
	void Reset();
	void AllowRule(int);
	void DenyRule(int);
	bool IsAllowed(int);
	bool ReadConfig();
	bool WriteConfig();
	void SetKey();
	SDLKey GetKey(int);
	bool SetKey(SDLKey);
	bool IsSettingKey();
	void CancelSetKey();
	int GetRuleBallStack();
	int GetRuleBallTetrad();
	int GetRuleStackBall();
	int GetRuleTetradBall();
	const char* GetRuleText();
	void NewRule();
	int GetLevel();
	int GetPadPlacement();
	int GetPadSize();
	int GetPadSizeX();
	int GetPadSizeY();
	bool GetTetSpeedUp();
	int GetMusic();
	const char* GetMusicFile();
	const char* GetCurrentMusicFile();
	int GetRuleFrequency();
	bool GetBallSpeedsUp();
	bool GetRandomRule();
	Uint32 GetVideoFlags();
	int GetVideoBpp();
	int GetFpsLimit();
	int GetAudioBuffers();
	int GetAudioFreq();
	bool GetMarathon();
	int GetJunk();
	int GetSpeed();
	int GetSpeedTetrad();
	float GetSpeedBall();
	bool GetSoundFx();
	void SetTheme(int);
	int GetTheme();
	const char* GetThemeString();
	void SetBgScroll(bool);
	bool GetBgScroll();
	void SetLevel();
	void SetLevel(int);
	void SetPadPlacement(int);
	void SetPadSize(int);
	void SetTetSpeedUp(bool);
	void SetMusic(int);
	void SetBallSpeedsUp(bool);
	void SetRandomRule(bool);
	void SetRuleFrequency(int);
	void SetVideoFlags(Uint32);
	void SetVideoBpp(int);
	void SetFpsLimit(int);
	void SetAudioBuffers(int);
	void SetAudioFreq(int);
	void SetMarathon(bool);
	void SetJunk();
	void SetJunk(int);
	void SetSpeed();
	void SetSpeed(int);
	void SetSoundFx(bool);
	void RuleFrequencyInit();
	void RuleFrequencyPause();
	void RuleFrequencyResume();
	void RuleFrequencyAddLines(int);
	void RuleFrequencyAddBall();
	bool RuleFrequencyNow();
};


#endif
