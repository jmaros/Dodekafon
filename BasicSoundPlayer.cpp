/*
BasicSoundPlayer.cpp
*/

#include <vector>
#include <string>
#include <windows.h>
#pragma comment (lib,"Winmm.lib")


#include "DodekafonRecord.h"
#include "BasicSoundPlayer.h"

static const wchar_t* SoundFile[]{
	L".\\sounds\\w1_c.wav",
	L".\\sounds\\w1_c1.wav",
	L".\\sounds\\w1_d.wav",
	L".\\sounds\\w1_d1.wav",
	L".\\sounds\\w1_e.wav",
	L".\\sounds\\w1_f.wav",
	L".\\sounds\\w1_f1.wav",
	L".\\sounds\\w1_g.wav",
	L".\\sounds\\w1_g1.wav",
	L".\\sounds\\w1_a.wav",
	L".\\sounds\\w1_a1.wav",
	L".\\sounds\\w1_h.wav"
};

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
		if (beep_flag)
			Beep(basicSounds[i], basicRythm[2]);
		else
			sndPlaySound(SoundFile[i], SND_SYNC | SND_NODEFAULT);
}

void BasicSoundPlayer::PlayBasicSounds(const DodekafonRecord& r, DWORD rythmIndex)
{
	const std::array<size_t, DODEKAFON_SIZE>& record = r.GetRecord();
	int ri = 0;
	if (rythmIndex >= 0 && rythmIndex < GetRythmSize())
		ri = rythmIndex;
	for (int i = 0; i < DODEKAFON_SIZE; ++i) {
		if (beep_flag)
			Beep(basicSounds[record[i] - 1], basicRythm[ri]);
		else
			sndPlaySound(SoundFile[record[i] - 1], SND_SYNC | SND_NODEFAULT);
	}
}
