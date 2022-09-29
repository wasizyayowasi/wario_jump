#include "DxLib.h"
#include "game.h"
#include "car.h"

namespace{
	//�҂�����
	constexpr int kWaitFrameMin = 60;
	constexpr int kWaitFrameMax = 180;
	//�Ԃ̑��x
	constexpr int kSpeed = -10.0f;

	//�W�����v��
	constexpr float kJumpAcc = -13.0f;
	//�d��
	constexpr float kGravity = 0.8f;


	//�~�܂鎞��
	constexpr int kStopCarMin = 60;
	constexpr int kStopCarMax = 240;
}


Car::Car()
{
	m_handle = -1;
	m_fieldY = 0.0f;
	m_moveType = kMoveTypeNormal;
	m_waitFrame = 0;
	m_stopCar = 0;
	m_isReturn = 0;
}

void Car::setGraphic(int handle)
{
	m_handle = handle;
	GetGraphSizeF(m_handle, &m_size.x, &m_size.y);
}

void Car::setup(float fieldY)
{
	m_fieldY = fieldY;
	m_pos.x = Game::kScreenWidth + 16.0f;
	m_pos.y = m_fieldY - m_size.y;

	m_vec.x = kSpeed;
	m_vec.y = 0.0f;

	//�����̃��@���G�[�V������I��
	int randNum = GetRand(99);
	if (randNum < 38) {
		m_moveType = kMoveTypeNormal;
	}
	else if (randNum < 38+30) {
		m_moveType = kMoveTypeStop;
	}
	else if (randNum < 38 + 30 + 30 ) {
		m_moveType = kMoveTypeJump;
	}
	else {
		m_moveType = kMoveTypeReturn;
	}

	m_moveType = kMoveTypeReturn;

	//�����n�߂�܂ł̎��Ԃ�ݒ�
	m_waitFrame = GetRand(kWaitFrameMax - kWaitFrameMin) + kWaitFrameMin;

	//�Ԃ��~�܂鎞�Ԃ�ݒ�
	m_stopCar = GetRand(kStopCarMax - kStopCarMin) + kStopCarMin;
}

void Car::update()
{
	if (m_waitFrame > 0) {
		m_waitFrame--;
		return;
	}
	switch (m_moveType) {
		case kMoveTypeNormal:
			updateNormal();
			break;
		case kMoveTypeStop:
			updateStop();
			break;
		case kMoveTypeJump:
			updateJump();
			break;
		case kMoveTypeReturn:
			updateReturn();
			break;
		default:
			updateNormal();
			break;
	}
	updateNormal();
}

void Car::draw()
{
	if (m_vec.x <= 0.0f) {
		DrawGraphF(m_pos.x, m_pos.y, m_handle, true);
	}
	else {
		DrawTurnGraph(m_pos.x, m_pos.y, m_handle, true);
	}
}




//�܂������i��
void Car::updateNormal() {
	m_pos += m_vec;
}

//�ꎞ��~�t�F�C���g
void Car::updateStop() {
	
	m_pos += m_vec;
	
	if (m_pos.x < 350) {
		if (m_stopCar > 0) {
			m_stopCar--;
			m_vec.x = 0;
			return;
		}
		else if (m_stopCar == 0) {
			m_vec.x = kSpeed;
		}
	}
}	

//�W�����v����
void Car::updateJump() {
	m_pos += m_vec;

	bool isField = false;

	if (m_pos.y > m_fieldY - m_size.y) {
		m_pos.y = m_fieldY - m_size.y;
		isField = true;
	}

	if (isField) {
		if (m_pos.x < 350) {
			m_vec.y = kJumpAcc; //�W�����v�J�n
		}
	}
	if (m_pos.x < 350) {
		m_vec.y += kGravity;
	}
}	

//�����Ԃ�
void Car::updateReturn() {
	m_pos += m_vec;

	if (m_pos.x < 350) {
		if (m_stopCar > 0) {
			m_stopCar--;
			m_vec.x = 0;
			return;
		}
		else if (m_stopCar == 0) {
			m_isReturn = 1;
			m_vec.x -= kSpeed;
		}
	}
}	
