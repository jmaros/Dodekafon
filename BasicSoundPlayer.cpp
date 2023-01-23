/*
BasicSoundPlayer.cpp
*/

#include <vector>
#include <string>
#include <windows.h>

#include "DodekafonRecord.h"
#include "BasicSoundPlayer.h"

BasicSoundPlayer::BasicSoundPlayer()
{

}

std::size_t BasicSoundPlayer::GetRythmSize() const
{
	return _RYTHMSIZE_;
}

void BasicSoundPlayer::PlayBasicSounds()
{
	for (int i = 0; i < DODEKAFON_SIZE; ++i)
		Beep(basicSounds[i], basicRythm[2]);
}

void BasicSoundPlayer::PlayBasicSounds(const DodekafonRecord& r, DWORD rythmIndex)
{
	const std::array<int, DODEKAFON_SIZE>& record = r.GetRecord();
	int ri = 0;
	if (rythmIndex >= 0 && rythmIndex < GetRythmSize())
		ri = rythmIndex;
	for (int i = 0; i < DODEKAFON_SIZE; ++i) {
		Beep(basicSounds[record[i] - 1], basicRythm[ri]);
	}
}
