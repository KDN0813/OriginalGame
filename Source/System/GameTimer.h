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
		// 1秒あたりのカウント数を取得
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counts_per_sec));
		// 1カウントあたりの秒数を計算
		this->seconds_per_count = 1.0 / static_cast<double>(counts_per_sec);

		// 現在のカウント値を取得
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this_count));
		this->base_count = this->this_count;
		this->last_count = this->this_count;
	}

	// タイマーをリセットしてからの経過時間を秒で取得
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

	// 前回のTickからの経過時間（フレーム間の差分）を秒で取得
	float GetUnscaledDeltaTime() const
	{
		return static_cast<float>(this->delta_time);
	}
	// game_speedを考慮した経過時間
	float GetDeltaTime() const
	{
		return static_cast<float>(this->delta_time) * this->game_speed;
	}

	// タイマーをリセット（初期化）する
	void TimerReset()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->this_count));
		this->base_count = this->this_count;
		this->last_count = this->this_count;

		this->stop_count = 0;
		this->stopped = false;
	}

	// 停止していたタイマーを再開する
	void TimerStart()
	{
		LONGLONG start_time;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time));

		if (this->stopped)
		{
			// 停止していた時間を加算
			this->paused_count += (start_time - this->stop_count);
			this->last_count = start_time;
			this->stop_count = 0;
			this->stopped = false;
		}
	}

	// タイマーを停止する
	void TimerStop()
	{
		if (!this->stopped)
		{
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->stop_count));
			this->stopped = true;
		}
	}

	// 現在の時間を取得し、前回との経過時間を計算（毎フレーム呼び出す）
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

		// 経過時間が負になることを防止
		if (this->delta_time < 0.0)
		{
			this->delta_time = 0.0;
		}
	}

	void SetGameSpeed(const float speed) { this->game_speed = speed; }
	float GetGameSpeed() const { return this->game_speed; }

	int GetSyncInterval() const { return this->sync_interval; }
private:
	double seconds_per_count;	// 1カウントあたりの秒数
	double delta_time;			// 前フレームからの経過時間（秒）

	LONGLONG base_count;		// リセットされた時点のカウント値
	LONGLONG paused_count;		// 停止していた合計時間（カウント単位）
	LONGLONG stop_count;		// タイマーを停止した時のカウント値
	LONGLONG last_count;		// 前回Tickのカウント値
	LONGLONG this_count;		// 現在のカウント値

	float game_speed;			// ゲーム速度

	bool stopped;				// タイマーが停止中かどうかのフラグ

	// 垂直同期間隔設定
#if defined(_DEBUG) || defined(RELEASE_DEBUG)
	int sync_interval = 0;
#else
	const int sync_interval = 1;
#endif // _DEBUG || RELEASE_DEBUG
#ifdef _DEBUG
public:
	// フレームレート計算
	void CalculateFrameStats();

	void DebugDrawGUI();

private:
	// フレームレート計算用変数
	int frames = 0;
	float time_tlapsed = 0.0f;
	float fps = 0.0f;
	float mspf = 0.0f;

#endif // _DEBUG
};
