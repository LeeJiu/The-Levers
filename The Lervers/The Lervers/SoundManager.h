#ifndef INCLUDED_SOUND_MANAGER_H
#define INCLUDED_SOUND_MANAGER_H

#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"


class SoundManager{
public:
	static SoundManager* instance();
	static void create();
	static void destroy();
	bool hasLoaded();

	enum Bgm{
		BGM_TITLE, //타이틀 브금
		BGM_GAME, //게임브금

		BGM_MAX,
	};
	enum Se{
		SE_DEATH, //사망시 소리
		SE_CURSOR_MOVE, //커서 소리
		SE_SELECTION, //선택소리
		SE_SET_LEVER, //레버소리

		SE_MAX,
	};
	void playBgm( Bgm );
	void stopBgm();
	void playSe( Se );
private:
	SoundManager();
	~SoundManager();

	GameLib::Sound::Wave mBgmWaves[ BGM_MAX ];
	GameLib::Sound::Wave mSeWaves[ SE_MAX ];

	GameLib::Sound::Player mBgmPlayer;
	static const int SE_PLAYER_MAX = 4;
	GameLib::Sound::Player mSePlayers[ SE_PLAYER_MAX ];
	int mSePlayerPos;

	static SoundManager* mInstance;
};

#endif
