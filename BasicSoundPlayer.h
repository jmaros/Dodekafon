/*
BasicSoundPlayer.h
*/
#pragma once

#ifndef _BASICSOUNDPLAYER_H_
#define _BASICSOUNDPLAYER_H_

#include <windows.h>

class BasicSoundPlayer {
#define _RYTHMSIZE_ 7
	const DWORD basicSounds[DODEKAFON_SIZE]{ 264 , 275 , 297, 316, 330, 352, 367, 396, 422, 440, 475, 496 };
	const DWORD basicRythm[_RYTHMSIZE_]{ 1000 , 500 , 250, 125, 62, 31, 16 };
public:
	BasicSoundPlayer();
	void PlayBasicSounds();
	void PlayBasicSounds(const DodekafonRecord& r, DWORD rythmIndex);
	std::size_t GetRythmSize() const;
};

#endif // _BASICSOUNDPLAYER_H_
