#pragma once
#include <memory>
#include "Object.h"

// �}�`�̕`��
class Shape
{
	// �}�`�f�[�^
	std::shared_ptr<const Object> object;

protected:

	// �}�`�Ɉ������_�̐�
	const GLsizei vertexcount;

public:

	// �R���X�g���N�^
	// size: ���_�̈ʒu�̎���
	// vertexcount: ���_�̐�
	// vertex: ���_�������i�[�����z��
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
		: object(new Object(size, vertexcount, vertex))
		, vertexcount(vertexcount)
	{
		// �{�̂͂Ȃ�
	}

	// �`��
	void draw() const
	{
		// ���_�z��I�u�W�F�N�g������
		object->bind();

		// �`��̎��s
		execute();
	}

	//�@�`��̎��s
	virtual void execute() const
	{
		// �܂���ŕ`�悷��
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}
};