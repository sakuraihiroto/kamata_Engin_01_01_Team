#include "Enemy.h"
#include "matWorld.h"

MatWorld* enemyMatworld = nullptr;

//接近フェーズ
void Enemy::Approach(float& appSpeed, float goal)
{
	//道のり
	m += -appSpeed;
	//移動(ベクトルを加算)
	worldTransform_.translation_.y += appSpeed;
	//指定の座標に到達したら次フェーズへ
	if (m >= 5
		&& worldTransform_.translation_.x <= 0) {
		phase_ = Enemy::Phase::MoveLeft;
		m = 0;
		if (changeFlagL == 1)//前のフェーズで右移動をしていない
		{
			phase_ = Enemy::Phase::MoveRight;
			changeFlagL = 0;
			m = 0;
		}
	}
	if (m >= 5
		&& worldTransform_.translation_.x >= 0)
	{
		phase_ = Enemy::Phase::MoveRight;
		m = 0;
		if (changeFlagR == 1)//前のフェーズで左移動をしていない
		{
			phase_ = Enemy::Phase::MoveLeft;
			changeFlagR = 0;
			m = 0;
		}
	}
}

void Enemy::MoveLeft(float& appSpeed)
{
	//道のり
	n += appSpeed;
	//移動(ベクトルを加算)
	worldTransform_.translation_.x -= appSpeed;
	if (worldTransform_.translation_.x <= 0)
	{
		changeFlagL = 1;
	}
	if (n > 5)
	{
		phase_ = Enemy::Phase::Approach;
		n = 0;
	}
}

void Enemy::MoveRight(float& appSpeed)
{
	//道のり
	n += appSpeed;
	//移動(ベクトルを加算)
	worldTransform_.translation_.x += appSpeed;
	if (worldTransform_.translation_.x >= 0)
	{
		changeFlagR = 1;
	}
	if (n > 5)
	{
		phase_ = Enemy::Phase::Approach;
		n = 0;
	}
}

void Enemy::MoveLeft2(float& appSpeed)
{

	//移動(ベクトルを加算)
	worldTransform_.translation_.x += appSpeed;
	if (count <= 0)
	{
		appSpeed -= speedX2;
		count = 5;
	}
	if (worldTransform_.translation_.x <= -2)
	{
		count = 5;
		speedX2 += 0.01f;
		phase_ = Enemy::Phase::MoveRight2;
	}
}
void Enemy::MoveRight2(float& appSpeed)
{
	worldTransform_.translation_.x += appSpeed;
	if (count <= 0)
	{
		appSpeed += speedX2;
		count = 5;
	}
	//移動(ベクトルを加算)
	if (worldTransform_.translation_.x >= 2)
	{
		count = 5;
		speedX2 += 0.01f;
		phase_ = Enemy::Phase::MoveLeft2;
	}
}


void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//初期座標をセット
	worldTransform_.translation_ = position;

	float goal = 0;

	//速度を初期化
	float speed = 0.05f;
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

void Enemy::UpdateW1()
{
	float speed = -0.05f;

	//行列の計算
	worldTransform_.matWorld_ = enemyMatworld->CreateMatWorld(worldTransform_);

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_.y += speed;

	//行列の転送
	worldTransform_.TransferMatrix();

}

void Enemy::UpdateW2(int& left)
{
	//速度を初期化
	float speedY = -0.05f;
	float speedX = 0.05f;
	//移動パターン
	switch (phase_) {
	case Phase::Approach://接近
		Approach(speedY, 15);//画面端
		break;
	case Phase::MoveLeft:
		MoveLeft(speedX);
		break;
	case Phase::MoveRight://右移動
		MoveRight(speedX);
		break;
	default:
		break;
	}
	//行列の計算
	worldTransform_.matWorld_ = enemyMatworld->CreateMatWorld(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}
void Enemy::UpdateW3()
{
	count--;
	//移動パターン

	switch (phase_) {
	case Phase::Approach:
		phase_ = Phase::MoveLeft2;
		break;
	case Phase::MoveLeft://
		phase_ = Phase::MoveRight2;
		break;
	case Phase::MoveRight://右移動
		phase_ = Phase::MoveLeft2;
		break;
	case Phase::MoveLeft2:
		MoveLeft2(speedX);
		break;
	default:
		MoveRight2(speedX);
		break;
	}
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_.y += speedY;

	//worldTransform_.translation_.x += speedX;

	//行列の計算
	worldTransform_.matWorld_ = enemyMatworld->CreateMatWorld(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}
//衝突判定
void Enemy::OnCollision(int& deadEnemyNum)
{
	isDead_ = true;
	deadEnemyNum += 1;
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}