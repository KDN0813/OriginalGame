#pragma once
#include <vector>
#include "Misc.h"

// ポインター型を受け取れないように制限をかける
template <typename T>
concept NotPointer = !std::is_pointer_v<T>;

// 順次的に要素を埋めていくstd::vector型のラッパークラス
template<NotPointer T>  // Tがポインター型でないことを要求する
class MySequenceVector
{
public:
    /**
     * コンストラクタ
     * 
     * \param num 最大の要素数
     * \param initial_value 初期化に使う値
     */
    MySequenceVector(ize_t num, T initial_value = T())
        : max_size(num), write_index(0), initial_value(initial_value)
    {
        vec.resize(num, initial_value);
    };

    ~MySequenceVector() {};  // デストラクタ（現在特にクリーンアップ処理なし）

    // データを追加する関数
    void AddData(const T& data)
    {
        _ASSERT_EXPR_W(this->write_index < this->max_size, L"要素数が最大です");    // エラー処理を行う
        if (this->max_size <= this->write_index) return;   // 書き込みインデックスが最大に達していれば何もしない

        this->vec[this->write_index] = data;    // 指定されたインデックスにデータを書き込む
        ++this->write_index;                    // 次の書き込みインデックスに進める
    }

    // データを初期化する関数
    void DataClear()
    {
        // vectorのすべての要素をinitial_valueで初期化
        std::fill(vec.begin(), vec.end(), this->initial_value);  // initial_valueで初期化
    }

    // 現在のvectorを取得する関数
    const std::vector<T>& GetVector() { return this->vec; }

private:
    std::vector<T> vec;         // 要素を格納するstd::vector
    T initial_value;            // 初期化に使う値
    const size_t max_size;      // 最大サイズ
    size_t write_index;         // 次に書き込むインデックス
};