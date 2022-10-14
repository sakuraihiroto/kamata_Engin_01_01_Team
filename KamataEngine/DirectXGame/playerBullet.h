#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "matWorld.h"
#include "DebugText.h"


class PlayerBullet {
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//衝突判定
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//デバックテキスト
	DebugText* debugText_ = nullptr;

	// 速度
	Vector3 velocity_;

	//デスフラグ
	bool isDead_ = false;
};
