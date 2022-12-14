#include "Enemy.h"
#include "matWorld.h"

MatWorld* enemyMatworld = nullptr;

void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Enemy.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//キャラクターの移動ベクトル
	Vector3 move = { 0,10,0 }; //座標{x,y,z}

	//初期座標をセット
	worldTransform_.translation_ = position;
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Update()
{
	float speed = 0.05f;

	//行列の計算
	worldTransform_.matWorld_ = enemyMatworld->CreateMatWorld(worldTransform_);

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_.y -= speed;

	//行列の転送
	worldTransform_.TransferMatrix();

}

//衝突判定
void Enemy::OnCollision()
{
	isDead_ = true;
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	//if (flag == 0)
	//{
		//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//}
}