#pragma once
#include <SixEngine.h>

class YSpin : public Six::SProgram {
	SIX_PROGRAM(YSpin)
public:
	virtual bool Init() override;
	virtual void Update() override;
	virtual void End() override;
	std::string toStr() override { return "Spinner"; }
public:
	float speed = 0.2f;
};