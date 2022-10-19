#include"playerBullet.h"
#include <assert.h>

MatWorld* bulletMatworld = nullptr;

void PlayerBullet::Initialize(Model* model, const Vector3& position )
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("tama.jpg");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	debugText_ = DebugText::GetInstance();

}

//ワールド座標を入れる変数
Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::Update(Vector3& velocity, bool& shootFlag, bool& changeFlag)
{
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	//y座標が21超えたら消える
	if (worldTransform_.translation_.y > 21)
	{
		isDead_ = true;
		shootFlag = 0;
		changeFlag = 0;
	}

	//行列の計算
	worldTransform_.matWorld_ = bulletMatworld->CreateMatWorld(worldTransform_);
	//行列の転送
	worldTransform_.TransferMatrix();

	//デバックテキスト
	debugText_->SetPos(80, 160);
	debugText_->Printf(
		"bullet(Y:%f)", worldTransform_.translation_.y);
}

//衝突判定
void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}