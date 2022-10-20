#include "GameScene.h"
#include "TextureManager.h"
#include "matWorld.h"
#include <cassert>

MatWorld* matworld_ = nullptr;

GameScene::GameScene() {
}

GameScene::~GameScene() {
	delete model_;
	delete skydome_;
	delete player_; //自キャラの解放
	//delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();

	//3Dモデルの生成
	model_ = Model::Create();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, Vector3{ -10,-10,0 });

	//天球の生成
	skydome_ = new Skydome();
	//天球モデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);
	
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() {


	//デスグラフが立った敵を削除
	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	//自キャラの更新
	player_->Update();
	time--;
	if (time <= 0)
	{
		time = 60;
		i += 1;
		//敵の生成,初期化
		std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(model_, { i * x,10,0 });
		//敵を登録する
		enemies_.push_back(std::move(newEnemy));
	}
	//敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Update();
	}

	CheakAllCollisions();

}

void GameScene::CheakAllCollisions()
{

	// 判定対象AとBとCの座標
	Vector3 posA, posB, posC;

	// AとBの距離
	float posAB = 0.0f;

	// 弾同士の半径
	float posR = 0.0f;
	float posR1 = 1.5f;
	float posR2 = 1.5f;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>&
		playerBullets = player_->GetBullets();

#pragma region 自弾と敵キャラの当たり判定
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();
		//プレイヤーの座標
		posC = player_->GetWorldPosition();

		// 敵キャラと自弾全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// AB間の距離の計算
			posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);
			posR = (posR1 + posR2) * (posR1 + posR2);

			// 球と球の交差判定
			if (posAB <= posR)
			{
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				player_->ResetFlag();
			}
		}
		//敵とデッドラインの処理
		if (posA.y < posC.y + 7)
		{
			enemy->OnCollision();
			player_->OnCollision();
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//天球の描画
	skydome_->Draw();

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}