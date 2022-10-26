#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "WinApp.h"
#include "Audio.h"
#include "Hp.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と対応
	/// </summary>
	void CheakAllCollisions();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//テクスチャハンドル
	uint32_t textureHandle_title = 0; //タイトル
	uint32_t textureHandle_gameOver = 0; //ゲームオーバー
	uint32_t textureHandle_clear = 0; //クリア

	//スプライト
	Sprite* sprite_title = nullptr; //タイトル
	Sprite* sprite_gameOver = nullptr; //ゲームオーバー
	Sprite* sprite_clear = nullptr; //クリア

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr; //ゲーム背景モデル
	Model* modelPlayer_ = nullptr; //プレイヤーモデル
	Model* modelEnemy_ = nullptr; //敵モデル
	Model* modelHp_ = nullptr;	//残機モデル

	//サウンド
	uint32_t soundTitleBGM = 0; //タイトルBGM
	uint32_t soundPlayBGM = 0; //プレイBGM
	uint32_t soundGameOverBGM = 0; //ゲームオーバーBGM
	uint32_t soundClearBGM = 0; //クリアBGM

	uint32_t voiceTitleBGM = 0; //音声再生ハンドル
	uint32_t voicePlayBGM = 0; 
	uint32_t voiceGameOverBGM = 0; 
	uint32_t voiceClearBGM = 0; 

	int checkSoundFlag = 0;    //サウンドフラグ 
	int checkSoundFlag1 = 0;
	int checkSoundFlag2 = 0;
	int checkSoundFlag3 = 0;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;
	//自キャラの弾
	PlayerBullet* playerBullet_ = nullptr;
	//敵
	std::list<std::unique_ptr<Enemy>>enemies_;
	//天球
	Skydome* skydome_ = nullptr;
	//画面
	WinApp* winApp_ = nullptr;

	//残機描画
	Hp* hp_ = nullptr;
	Hp* hp_2 = nullptr;
	Hp* hp_3 = nullptr;

	int scene = 0;

	int time = 70;
	int time2 = -1;
	int time3 = 60;
	int time4 = 60;

	int enemyNum = 0;
	//死んだ敵の数
	int deadEnemyNum = 0;
	int left = 0;
	int Wave = 1;

	//残機
	int hp = 3;

	//音量
	float volume = 0.05f;
};