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
    // デフォルトコンストラクタ
    // 既定の初期値を持たない、空の状態で作成される
    MySequenceVector()
        : max_size(0), write_index(0), initial_value() {}

    /**
     * @brief コンストラクタ
     *
     * 最大要素数と初期化値を指定してベクターを作成します。
     *
     * @param num 最大の要素数
     * @param initial_value 初期化に使う値（デフォルト値: T()）
     */
    MySequenceVector(size_t num, T initial_value = T())
        : max_size(num), write_index(0), initial_value(initial_value)
    {
        this->vec.resize(num, initial_value); // vecのサイズをnumに設定し、初期値で埋める
    }

    // デストラクタ
    ~MySequenceVector() {}

    /**
     * @brief コンストラクタで値を設定できない時用の初期化関数
     *
     * コンストラクタ以外で初期化を行う場合に呼ばれます。
     * 既に初期化されている場合はエラーを返し、再度初期化できないようにします。
     *
     * @param num 最大の要素数
     * @param initial_value 初期化に使う値（デフォルト値: T()）
     */
    void Initialize(size_t num, T initial_value = T())
    {
        _ASSERT_EXPR_W(this->vec.empty(), L"既に初期設定がされています");   // エラー処理を行う
        if (!this->vec.empty()) return;

        this->max_size = num;                   // 最大要素数の設定
        this->initial_value = initial_value;    // 初期化値の設定
        this->vec.resize(num, initial_value);   // ベクターのサイズをnumに変更し、初期値で埋める
    }

    /**
     * @brief データを追加する関数
     *
     * 次のインデックスに指定されたデータを追加します。
     * ただし、要素数が最大の時は何もしません。
     *
     * @param data 追加するデータ
     */
    void AddData(const T& data)
    {
        //_ASSERT_EXPR_W(this->write_index < this->max_size, L"要素数が最大です");    // エラー処理を行う
        if (this->max_size <= this->write_index) return;   // 書き込みインデックスが最大に達していれば何もしない

        this->vec[this->write_index] = data;    // 指定されたインデックスにデータを書き込む
        ++this->write_index;                    // 次の書き込みインデックスに進める
    }

    /**
     * @brief データを初期化する関数
     *
     * vectorのすべての要素をinitial_valueで初期化します。
     */
    void DataClear()
    {
        // vectorのすべての要素をinitial_valueで初期化
        std::fill(this->vec.begin(), this->vec.end(), this->initial_value);  // initial_valueで初期化
        this->write_index = 0;
    }

    /**
     * @brief 要素数を返す
     *
     * @return 現在のvectorのサイズ
     */
    size_t size() { return this->vec.size(); }
    
    /**
     * @brief 配列のポインタを返す
     *
     * @return vector内部のデータへのポインタ
     */
    const T* data() { return this->vec.data(); }

    /**
     * @brief 現在のvectorを取得する関数
     *
     * @return vectorの参照
     */
    const std::vector<T>& GetVector() { return this->vec; }
private:
    std::vector<T> vec;         // 要素を格納するstd::vector
    T initial_value;            // 初期化に使う値
    size_t max_size;            // 最大サイズ
    size_t write_index;         // 次に書き込むインデックス
};