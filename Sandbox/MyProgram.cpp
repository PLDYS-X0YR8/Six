#include "MyProgram.h"

bool MyProgram::Init() {
	return true;
}

void MyProgram::Update() {
	auto t = m_owner->GetComponent<Six::STransform>();
	t->Rotate({ 0.f,1.f,0.f }, Six::SGetTimer()->deltatime * 0.1);
	std::cout << Six::SGetTimer()->deltatime << "\n";
}

void MyProgram::End() {

}