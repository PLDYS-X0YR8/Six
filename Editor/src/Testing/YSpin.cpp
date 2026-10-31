#include "YSpin.h"

bool YSpin::Init() {
	return true;
}

void YSpin::Update() {
	auto t = m_owner->GetComponent<Six::STransform>();
	t->Rotate({ 0.f,1.f,0.f }, Six::SGetTimer()->deltatime * speed);
}

void YSpin::End() {

}