#pragma once
#include <memory>
#include "System/MyHash.h"

class Object;

// ステートマシンで使用するステートの基底クラス
class State
{
public:
	using StateIndex = size_t;
	static const StateIndex INVALID_STATE_INDEX = SIZE_MAX;	// 無効なステートのインデックス(ステートのインデックスの初期値・エラーコードに使用する)

	// ステートマシンでステートを切り替える際に使用する情報を保持する構造体。
	// ステート名のハッシュ値（名前による検索用）と、ステートのインデックス（初回の検索後に使用）を保持する。
	struct ChangeState
	{
		MyHash change_state_name;								// ステート名のハッシュ値。最初のステート検索時に使用される。
		StateIndex change_state_index = INVALID_STATE_INDEX;	// ステートのインデックス。最初の検索後はこのインデックスでステートを参照する。(初期値は最大値)
	};
public:
	// コンストラクタ
	State() = delete;
	State(std::string name) : state_name(name) {}
	virtual ~State() {}
	// ステートに入った時のメソッド
	virtual void Staet() = 0;
	// ステートで実行するメソッド
	virtual void Update(float elapsed_time) = 0;
	// ステートから出ていくときのメソッド
	virtual void End() = 0;

	// ハッシュの取得
	MyHash GetHash() const { return this->state_name; }
	// 自分を所有しているステートの配列の自身のインデックスの取得
	StateIndex GetStateIndex() { return this->state_index; }
	// 自分を所有しているステートの配列の自身のインデックスの設定
	void SetStateIndex(StateIndex index) { this->state_index = index; }

	// 所有者の設定
	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
protected:
	std::weak_ptr<Object> owner;	// 所有者のポインタ
	MyHash state_name;				// ハッシュ()
	StateIndex state_index = INVALID_STATE_INDEX;	// 自分が所属しているステート配列のインデックス
};

class DefaultState : public State
{
public:
	// コンストラクタ
	DefaultState() :State("DefaultState") {};
	~DefaultState() {}
	// ステートに入った時のメソッド
	void Staet() override{};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
};
