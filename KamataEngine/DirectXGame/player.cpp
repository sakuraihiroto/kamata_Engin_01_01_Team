#include "Player.h"

// �A�t�B���s��Ăяo��
MatWorld* playerMatworld = nullptr;

//����������
void Player::Initialize(Model* model, const Vector3& position)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,-15,0 };//���W{x,y,z}

	//�������W���Z�b�g
	worldTransform_.translation_ = move;
}

//���[���h���W������ϐ�
Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


//�Փ˔���
void Player::OnCollision()
{

}

//�v���C���[�̍U������
void Player::Attack()
{

	//�^�C�}�[����
	if (shootFlag == 1)
	{
		timer += 0.1f;
	}

	//�X�y�[�X����������
	if (input_->TriggerKey(DIK_SPACE) && shootFlag == 0)
	{
		shootFlag = 1;
		if (shootFlag == 1)
		{
			//�e�̑��x
			const float kBulletSpeed_Y = 0.2f; //Y�X�s�[�h
			const float kBulletSpeed_X = 0.2f; //X�X�s�[�h

			Vector3 velocity(0, kBulletSpeed_Y, 0); //�e�̈ʒu

			//�e�𐶐����A������
			std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity); //���L�����̍��W

			//�e��o�^����
			bullets_.push_back(std::move(newBullet));
		}
	}
	if (timer > 8.5)
	{
		shootFlag = 0;
		timer = 0.0f;
	}

	//�f�o�b�N�e�L�X�g
	debugText_->SetPos(80, 240);
	debugText_->Printf(
		"timer(%f)", timer);
	//�f�o�b�N�e�L�X�g
	debugText_->SetPos(80, 200);
	debugText_->Printf(
		"flag(%d)", shootFlag);
}

//�A�b�v�f�[�g
void Player::Update()
{
	//�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});



	//�s��̌v�Z
	worldTransform_.matWorld_ = playerMatworld->CreateMatWorld(worldTransform_);
	//�s��̓]��
	worldTransform_.TransferMatrix();

	Attack();

	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

//�`�揈��
void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// �e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}
