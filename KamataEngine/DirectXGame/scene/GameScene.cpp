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
	delete sprite_title;
	delete sprite_gameOver;
	delete sprite_clear;
}

void GameScene::Initialize() {


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();


	//タイトル生成
	textureHandle_title = TextureManager::Load("TD2_GAMETITLE.jpg");
	sprite_title = Sprite::Create(textureHandle_title, { 0,0 });
	//ゲームオーバー生成
	textureHandle_gameOver = TextureManager::Load("gameOver.png");
	sprite_gameOver = Sprite::Create(textureHandle_gameOver, { 0,0 });
	//クリア生成
	textureHandle_clear = TextureManager::Load("TD2_GAMECLEAR.jpg");
	sprite_clear = Sprite::Create(textureHandle_clear, { 0,0 });


	//3Dモデルの生成
	model_ = Model::Create();

	//自キャラの生成
	player_ = new Player();
	//自キャラモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	//自キャラの初期化
	player_->Initialize(modelPlayer_);

	//天球の生成
	skydome_ = new Skydome();
	//天球モデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//敵キャラモデルの生成
	modelEnemy_ = Model::CreateFromOBJ("bat_TD2", true);

	//サウンドデータの読み込み
	soundTitleBGM = audio_->LoadWave("Audio/title.mp3");
	soundPlayBGM = audio_->LoadWave("Audio/play.mp3");
	soundGameOverBGM = audio_->LoadWave("Audio/game_over.mp3");
	soundClearBGM = audio_->LoadWave("Audio/clear.mp3");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() {

	switch (scene)
	{
	case 0:		//タイトル

		

		if (checkSoundFlag == 0)
		{
			//タイトルBGM再生
			voiceTitleBGM = audio_->PlayWave(soundTitleBGM, true);
			checkSoundFlag = 1;
		}
		
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene = 1;
			checkSoundFlag = 0;
			//タイトルBGM停止
			audio_->StopWave(voiceTitleBGM);
		}
		break;
	case 1:		//ゲームシーン

		//ゲームオーバーBGM停止
		audio_->StopWave(voiceGameOverBGM);
		checkSoundFlag3 = 0;
		
		if (checkSoundFlag1 == 0)
		{
			//プレイBGM再生
			voicePlayBGM = audio_->PlayWave(soundPlayBGM, true);

			checkSoundFlag1 = 1;
		}
		if (deadEnemyNum == 18 && hp > 0)
		{
			scene = 2;		//ゲームクリア
			checkSoundFlag1 = 0;
			//プレイBGM停止
			audio_->StopWave(voicePlayBGM);
		}
		else if (hp <= 0)
		{
			scene = 3;		//ゲームオーバー
			checkSoundFlag1 = 0;
			//プレイBGM停止
			audio_->StopWave(voicePlayBGM);
		}
		break;
	case 2:		//ゲームクリア

		if (checkSoundFlag2 == 0)
		{
			//クリアBGM再生
			voiceClearBGM = audio_->PlayWave(soundClearBGM, true);
			
			checkSoundFlag2 = 1;
		}

		if (input_->TriggerKey(DIK_SPACE))
		{
			for (std::unique_ptr<Enemy>& enemy : enemies_)
			{
				enemy->OnCollision(deadEnemyNum);
			}
			//クリアBGM停止
		audio_->StopWave(voiceClearBGM);
		checkSoundFlag2 = 0;

			time = 70;
			time2 = -1;
			time3 = 60;
			time4 = 90;
			enemyNum = 0;
			//死んだ敵の数
			deadEnemyNum = 0;
			left = 0;
			Wave = 1;
			//残機
			hp = 3;
			player_->ResetBullet();
			scene = 0;		//タイトル
		}
		break;
	default://ゲームオーバー

		if (checkSoundFlag3 == 0)
		{
			//クリアBGM再生
			voiceGameOverBGM = audio_->PlayWave(soundGameOverBGM, true);
	
			checkSoundFlag3 = 1;
		}
		if (input_->TriggerKey(DIK_SPACE))
		{
			for (std::unique_ptr<Enemy>& enemy : enemies_)
			{
				enemy->OnCollision(deadEnemyNum);
			}
			time = 70;
			time2 = -1;
			time3 = 60;
			time4 = 90;
			enemyNum = 0;
			//死んだ敵の数
			deadEnemyNum = 0;
			left = 0;
			Wave = 1;
			//残機
			hp = 3;
			scene = 1;		//リトライ
			player_->ResetBullet();
		}
		break;
	}


	if (scene == 1)
	{
		//自キャラの更新 
		player_->Update();
		//デスグラフが立った敵を削除
		enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
			return enemy->IsDead();
			});
#pragma region Wave管理
		if (deadEnemyNum == 3)
		{
			Wave = 2;
		}
		if (deadEnemyNum == 13)
		{
			Wave = 3;
		}
#pragma endregion

#pragma region 敵の生成
		if (time > 10) {
			time--;
		}
		if (time2 >= 0) {
			time2--;
		}
		if (Wave == 2)
		{
			time3--;
		}
		if (Wave == 3)
		{
			time4--;
		}
		/// <summary>
		/// Wave1
		/// </summary>
		if (time % 60 == 0 && Wave == 1)
		{
			time2 = 120;
			time = 10;
			//敵の生成,初期化
			std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(modelEnemy_, { 0,21,0 });
			enemyNum = 1;
			//敵を登録する
			enemies_.push_back(std::move(newEnemy));
		}

		if (time2 == 0 && Wave == 1 && enemyNum < 3)
		{
			//敵の生成,初期化
			std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
			if (left % 2 == 0)
			{
				newEnemy->Initialize(modelEnemy_, { -10,21,0 });
				left = 1;
				time2 = 1;
				enemyNum += 1;
			}
			else if (left % 2 == 1)
			{
				newEnemy->Initialize(modelEnemy_, { 10,21,0 });
				left = 0;
				time2 = 60;
				enemyNum += 1;
			}
			//敵を登録する
			enemies_.push_back(std::move(newEnemy));
		}
		/// <summary>
		/// Wave2
		/// </summary>
		if (time3 <= 0 && Wave == 2 && enemyNum != 13)
		{
			//敵の生成,初期化
			std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
			if (left % 2 == 0)
			{
				newEnemy->Initialize(modelEnemy_, { -10,21,0 });
				left = 1;
				time3 = 1;
				enemyNum += 1;
			}
			else if (left % 2 == 1)
			{
				newEnemy->Initialize(modelEnemy_, { 10,21,0 });
				left = 0;
				time3 = 120;
				enemyNum += 1;
			}

			//敵を登録する
			enemies_.push_back(std::move(newEnemy));
		}

		if (time4 <= 0 && Wave == 3 && enemyNum != 18)
		{
			//敵の生成,初期化
			std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(modelEnemy_, { 0,21,0 });
			time4 = 90;
			enemyNum += 1;

			//敵を登録する
			enemies_.push_back(std::move(newEnemy));
		}
#pragma endregion


		//敵の更新
		for (std::unique_ptr<Enemy>& enemy : enemies_)
		{
			switch (Wave) {
			case 1:
				enemy->UpdateW1();
				break;
			case 2:
				enemy->UpdateW2(left);
				break;
			case 3:
				enemy->UpdateW3();
				break;
			}
		}
		CheakAllCollisions();
	}
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
				enemy->OnCollision(deadEnemyNum);
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				player_->ResetFlag();
			}
		}
		//敵とデッドラインの処理
		if (posA.y < posC.y + 7)
		{
			enemy->OnCollision(deadEnemyNum);
			player_->OnCollision(hp);
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	if (scene == 0)
	{
		//タイトル描画
		sprite_title->Draw();
	}
	if (scene == 2)
	{
		//クリア
		sprite_clear->Draw();
	}
	if (scene == 3)
	{
		//ゲームオーバー
		sprite_gameOver->Draw();
	}
	
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
	if (scene == 1)
	{
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
	}
	
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
	//デバックテキスト
	debugText_->SetPos(80, 240);
	debugText_->Printf(
		"timer(%d)", time);
	//デバックテキスト
	debugText_->SetPos(80, 280);
	debugText_->Printf(
		"HP(%d)", hp);
	// デバックテキスト
	debugText_->SetPos(80, 300);
	debugText_->Printf(
		"Wave(%d)", Wave);
	// デバックテキスト
	debugText_->SetPos(80, 320);
	debugText_->Printf(
		"scene(%d)", scene);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}