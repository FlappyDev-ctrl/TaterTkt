/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_CONTROLS_H
#define GAME_CLIENT_COMPONENTS_CONTROLS_H

#include <base/vmath.h>

#include <engine/client.h>
#include <engine/console.h>

#include <game/client/component.h>
#include <generated/protocol.h>

class CControls : public CComponent
{
public:
	// Distances de souris (utiles pour l'aim-assist, etc.)
	float GetMinMouseDistance() const;
	float GetMaxMouseDistance() const;

	// Données publiques (héritées du client)
	vec2 m_aMousePos[NUM_DUMMIES];
	vec2 m_aMousePosOnAction[NUM_DUMMIES];
	vec2 m_aTargetPos[NUM_DUMMIES];

	int m_aAmmoCount[NUM_WEAPONS];

	int64_t m_LastSendTime;
	CNetObj_PlayerInput m_aInputData[NUM_DUMMIES];
	CNetObj_PlayerInput m_aLastData[NUM_DUMMIES];
	int m_aInputDirectionLeft[NUM_DUMMIES];
	int m_aInputDirectionRight[NUM_DUMMIES];
	int m_aShowHookColl[NUM_DUMMIES];

	CNetObj_PlayerInput m_FastInput;

	CControls();
	int Sizeof() const override { return sizeof(*this); }

	// Hooks de composant
	void OnReset() override;
	void OnRender() override;
	void OnMessage(int MsgType, void *pRawMsg) override;
	bool OnCursorMove(float x, float y, IInput::ECursorType CursorType) override;
	void OnConsoleInit() override;
	virtual void OnPlayerDeath();

	// Snap & input
	int SnapInput(int *pData);
	void ClampMousePos();
	void ResetInput(int Dummy);
	bool CheckNewInput();

	// === API ajoutée (features AvoidFreeze / HookAssist) ===
	void AvoidFreeze();
	void HookAssist();

private:
	// Console helpers
	static void ConKeyInputState(IConsole::IResult *pResult, void *pUserData);
	static void ConKeyInputCounter(IConsole::IResult *pResult, void *pUserData);
	static void ConKeyInputSet(IConsole::IResult *pResult, void *pUserData);
	static void ConKeyInputNextPrevWeapon(IConsole::IResult *pResult, void *pUserData);

	// === États statiques (cooldowns) ===
	static int64_t s_LastAvoidTime;
	static int64_t s_LastActiveCheckTime;
	static const int64_t ACTIVE_COOLDOWN;

	// === Helpers internes ===
	bool IsPlayerInDanger(int LocalPlayerId);
	bool GetFreeze(vec2 Pos, int FreezeTime);
	bool IsAvoidCooldownElapsed(int64_t CurrentTime);
	void UpdateAvoidCooldown(int64_t CurrentTime);
	bool PredictFreeze(const CNetObj_PlayerInput &Input, int Ticks);
	bool TryAvoidFreeze(int LocalPlayerId);
	bool IsPlayerActive(int LocalPlayerId);
};

#endif // GAME_CLIENT_COMPONENTS_CONTROLS_H
