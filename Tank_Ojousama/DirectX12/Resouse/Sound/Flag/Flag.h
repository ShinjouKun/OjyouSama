#pragma once

class Flag {
public:
	Flag();
	~Flag();
	//�t���O�𗧂Ă�
	void set(unsigned value);
	//�t���O��܂�
	void reset(unsigned value);
	//�t���O�������Ă��邩
	bool check(unsigned value) const;
	//�S�t���O��Ԃ�
	unsigned get() const;
	//�S�t���O��܂�
	void clear();

private:
	unsigned mFlags;
};
