#include "Player.h"

// アフィン行列呼び出し
MatWorld* playerMatworld = nullptr;

//初期化処理
void Player::Initialize(Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//キャラクターの移動ベクトル
	Vector3 move = { 0,-15,0 };//座標{x,y,z}

	//初期座標をセット
	worldTransform_.translation_ = move;
}

//ワールド座標を入れる変数
Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


//衝突判定
void Player::OnCollision()
{

}

//プレイヤーの攻撃処理
void Player::Attack()
{

	//タイマー処理
	if (shootFlag == 1)
	{
		timer += 0.1f;
	}

	//スペースを押したら
	if (input_->TriggerKey(DIK_SPACE) && shootFlag == 0)
	{
		shootFlag = 1;
		if (shootFlag == 1)
		{
			//弾の速度
			const float kBulletSpeed_Y = 0.2f; //Yスピード
			const float kBulletSpeed_X = 0.2f; //Xスピード

			Vector3 velocity(0, kBulletSpeed_Y, 0); //弾の位置

			//弾を生成し、初期化
			std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity); //自キャラの座標

			//弾を登録する
			bullets_.push_back(std::move(newBullet));
		}
	}
	if (timer > 8.5)
	{
		shootFlag = 0;
		timer = 0.0f;
	}

	//デバックテキスト
	debugText_->SetPos(80, 240);
	debugText_->Printf(
		"timer(%f)", timer);
	//デバックテキスト
	debugText_->SetPos(80, 200);
	debugText_->Printf(
		"flag(%d)", shootFlag);
}

//アップデート
void Player::Update()
{
	//更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});



	//行列の計算
	worldTransform_.matWorld_ = playerMatworld->CreateMatWorld(worldTransform_);
	//行列の転送
	worldTransform_.TransferMatrix();

	Attack();

	// 弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

//描画処理
void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}
