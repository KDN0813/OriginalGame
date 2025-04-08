#pragma once

#include <windows.h>
#include "System\ClassBase\Singleton.h"

class GameTimer : public Singleton<GameTimer>
{
public:
	GameTimer()
		: Singleton(this)
		, delta_time(-1.0)
		, paused_count(0)
		, stop_count()
		, game_speed(1.0f)
		, stopped(false)
	{
		LONGLONG counts_per_sec;
		// 1�b������̃J�E���g�����擾
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counts_per_sec));
		// 1�J�E���g������̕b�����v�Z
		this->seconds_per_count = 1.0 / static_cast<double>(counts_per_sec);

		// ���݂̃J�E���g�l���擾
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this_count));
		this->base_count = this->this_count;
		this->last_count = this->this_count;
	}

	// �^�C�}�[�����Z�b�g���Ă���̌o�ߎ��Ԃ�b�Ŏ擾
	float TimeStamp() const
	{
		if (this->stopped)
		{
			return static_cast<float>(((this->stop_count - this->paused_count) - this->base_count) * this->seconds_per_count);
		}
		else
		{
			return static_cast<float>(((this->this_count - this->paused_count) - this->base_count) * this->seconds_per_count);
		}
	}

	// �O���Tick����̌o�ߎ��ԁi�t���[���Ԃ̍����j��b�Ŏ擾
	float GetUnscaledDeltaTime() const
	{
		return static_cast<float>(this->delta_time);
	}
	// game_speed���l�������o�ߎ���
	float GetDeltaTime() const
	{
		return static_cast<float>(this->delta_time) * this->game_speed;
	}

	// �^�C�}�[�����Z�b�g�i�������j����
	void TimerReset()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->this_count));
		this->base_count = this->this_count;
		this->last_count = this->this_count;

		this->stop_count = 0;
		this->stopped = false;
	}

	// ��~���Ă����^�C�}�[���ĊJ����
	void TimerStart()
	{
		LONGLONG start_time;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time));

		if (this->stopped)
		{
			// ��~���Ă������Ԃ����Z
			this->paused_count += (start_time - this->stop_count);
			this->last_count = start_time;
			this->stop_count = 0;
			this->stopped = false;
		}
	}

	// �^�C�}�[���~����
	void TimerStop()
	{
		if (!this->stopped)
		{
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->stop_count));
			this->stopped = true;
		}
	}

	// ���݂̎��Ԃ��擾���A�O��Ƃ̌o�ߎ��Ԃ��v�Z�i���t���[���Ăяo���j
	void Tick()
	{
		if (this->stopped)
		{
			this->delta_time = 0.0;
			return;
		}

		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->this_count));
		this->delta_time = (this->this_count - this->last_count) * this->seconds_per_count;

		this->last_count = this->this_count;

		// �o�ߎ��Ԃ����ɂȂ邱�Ƃ�h�~
		if (this->delta_time < 0.0)
		{
			this->delta_time = 0.0;
		}
	}

	void SetGameSpeed(const float speed) { this->game_speed = speed; }
	float GetGameSpeed() const { return this->game_speed; }

	int GetSyncInterval() const { return this->sync_interval; }
private:
	double seconds_per_count;	// 1�J�E���g������̕b��
	double delta_time;			// �O�t���[������̌o�ߎ��ԁi�b�j

	LONGLONG base_count;		// ���Z�b�g���ꂽ���_�̃J�E���g�l
	LONGLONG paused_count;		// ��~���Ă������v���ԁi�J�E���g�P�ʁj
	LONGLONG stop_count;		// �^�C�}�[���~�������̃J�E���g�l
	LONGLONG last_count;		// �O��Tick�̃J�E���g�l
	LONGLONG this_count;		// ���݂̃J�E���g�l

	float game_speed;			// �Q�[�����x

	bool stopped;				// �^�C�}�[����~�����ǂ����̃t���O

	// ���������Ԋu�ݒ�
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
	int sync_interval = 0;
#else
	const int sync_interval = 1;
#endif // _DEBUG || RELEASE_DEBUG
#ifdef _DEBUG
public:
	// �t���[�����[�g�v�Z
	void CalculateFrameStats();

	void DebugDrawGUI();

private:
	// �t���[�����[�g�v�Z�p�ϐ�
	int frames = 0;
	float time_tlapsed = 0.0f;
	float fps = 0.0f;
	float mspf = 0.0f;

#endif // _DEBUG
};
