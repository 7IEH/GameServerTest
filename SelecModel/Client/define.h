#pragma once
#define Single(classtype) private:\
							classtype();\
							~classtype();\
							friend class Singleton<classtype>;

#define DT TimeMgr::GetInst()->GetDeltaTime();

#define KEY_PRESSED(key) InputMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESSED
#define KEY_TAP(key) InputMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define KEY_RELEASED(key) InputMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASED
#define KEY_NOTPRESSED(key) InputMgr::GetInst()->GetKeyState(key) == KEY_STATE::NONE