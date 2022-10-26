#pragma once

#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Model.h"
#include"matWorld.h"
#include"player.h"

class Enemy
{
public:

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		MoveLeft,	//左移動
		MoveRight,	//右移動
		MoveLeft2,	//左移動
		MoveRight2,	//右移動
		Leave,		//離脱する
	};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateW1();
	void UpdateW2(int& left);
	void UpdateW3();

	//接近
	void Approach(float& appSpeed, float goal);

	//左移動
	void MoveLeft(float& appSpeed);

	//右移動
	void MoveRight(float& appSpeed);

	//左移動
	void MoveLeft2(float& appSpeed);

	//右移動
	void MoveRight2(float& appSpeed);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//衝突判定
	void OnCollision(int& deadEnemyNum);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//敵の番号を取得
	int GetEnemyNum();

	bool IsDead() const { return isDead_; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//速度
	Vector3 velocity_;
	float speed = 0;
	//道のり
	float m = 0;
	float n = 0;
	float m2 = 0;
	float n2 = 0;
	int count = 5;
	int count2 = 5;
	float time = 0.5f;
	float time2 = 0.5f;
	int timer = 120;

	//速度を初期化
	float speedY = -0.05f;
	float speedX = 0.05f;
	float speedX2 = 0.03f;

	bool changeFlagL = 0;
	bool changeFlagR = 0;
	//デスフラグ
	bool isDead_ = false;
	//目的地
	float goal = 0;

};