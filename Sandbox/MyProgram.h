#pragma once
#include <SixEngine.h>

class MyProgram : public Six::SProgram {
	SIX_PROGRAM(MyProgram)
public:
	virtual bool Init() override;
	virtual void Update() override;
	virtual void End() override;
    std::string toStr() override { return "MyProgram"; }
};