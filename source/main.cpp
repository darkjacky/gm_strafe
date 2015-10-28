#include "mathlib/vector.h"
#define GMMODULE
#include "Interface.h"
using namespace GarrysMod::Lua;

//GLOBAL
int TEAM_SIDEWAYS;
int TEAM_SPECTATOR;
int IN_MOVELEFT;
int IN_MOVERIGHT; 
CFunc CVector;

//Player
CFunc CAlive;
CFunc CEntIndex;
CFunc CIsBot;
CFunc CWaterLevel;
CFunc CTeam;
CFunc COnGround;
CFunc CSetDuckSpeed;
CFunc CSetUnDuckSpeed;
CFunc CSteamID;

//CMOVEDATA
CFunc CSetVelocity;
CFunc CKeyDown;
CFunc CGetForwardSpeed;
CFunc CGetSideSpeed;
CFunc CGetMaxSpeed;
CFunc CGetMoveAngles;
CFunc CGetVelocity;

bool HasCachedCFuncs = false;

void CacheCFuncs(lua_State* state) {
	//GLOBAL
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "Vector");
		CVector = LUA->GetCFunction(-1);
		LUA->Pop(); //pop CFunc Vector of the stack
		LUA->GetField(-1, "IN_MOVELEFT");
		IN_MOVELEFT = LUA->GetNumber(-1);
		LUA->Pop(); //pop number IN_MOVELEFT of the stack
		LUA->GetField(-1, "IN_MOVERIGHT");
		IN_MOVERIGHT = LUA->GetNumber(-1);
		LUA->Pop(); //pop number IN_MOVERIGHT of the stack
		LUA->GetField(-1, "TEAM_SIDEWAYS");
		TEAM_SIDEWAYS = LUA->GetNumber(-1);
		LUA->Pop() ;//pop number TEAM_SIDEWAYS of the stack
		LUA->GetField(-1, "TEAM_SPECTATOR");
		TEAM_SPECTATOR = LUA->GetNumber(-1);
		LUA->Pop(); //pop number TEAM_SPECTATOR of the stack
	LUA->Pop(); //pop global of the stack.

	//Player
	LUA->GetField(1, "Alive");
	CAlive = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc Alive of the stack
	LUA->GetField(1, "IsBot");
	CIsBot = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc IsBot of the stack
	LUA->GetField(1, "WaterLevel");
	CWaterLevel = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc WaterLevel of the stack
	LUA->GetField(1, "Team");
	CTeam = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc Team of the stack
	LUA->GetField(1, "OnGround");
	COnGround = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc OnGround of the stack
	LUA->GetField(1, "SetDuckSpeed");
	CSetDuckSpeed = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc SetDuckSpeed of the stack
	LUA->GetField(1, "SetUnDuckSpeed");
	CSetUnDuckSpeed = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc SetUnDuckSpeed of the stack
	LUA->GetField(1, "SteamID");
	CSteamID = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc SteamID of the stack
	LUA->GetField(1, "EntIndex");
	CEntIndex = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc EntIndex of the stack
		
	//CMoveData
	LUA->GetField(2, "KeyDown");
	CKeyDown = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc KeyDown of the stack
	LUA->GetField(2, "GetForwardSpeed");
	CGetForwardSpeed = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc GetForwardSpeed of the stack
	LUA->GetField(2, "GetSideSpeed");
	CGetSideSpeed = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc GetSideSpeed of the stack
	LUA->GetField(2, "GetMaxSpeed");
	CGetMaxSpeed = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc GetMaxSpeed of the stack
	LUA->GetField(2, "GetMoveAngles");
	CGetMoveAngles = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc GetMoveAngles of the stack
	LUA->GetField(2, "GetVelocity");
	CGetVelocity = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc GetVelocity of the stack
	LUA->GetField(2, "SetVelocity");
	CSetVelocity = LUA->GetCFunction(-1);
	LUA->Pop(); //pop CFunc SetVelocity of the stack

	HasCachedCFuncs = true; //we confirm we have the CFuncs cached.
}

static const double fixnum = 57.295780532961; // number required to turn the angle into a "normalized" Vector.
Vector GetForward(QAngle ang) {
	double Pitch = ang.x / fixnum;
	double Yaw = ang.y / fixnum;
	//double z = -sin(Pitch); // no need for this we can just return 0
	return Vector(cos(Pitch)*cos(Yaw), cos(Pitch)*sin(Yaw), 0);
}

Vector GetRight(QAngle ang) {
	double Yaw = ang.y / fixnum;
	double Roll = ang.z / fixnum;
	//double z = sin(Roll); // no need for this we can just return 0
	return Vector(cos(Roll)*sin(Yaw), -(cos(Roll)*cos(Yaw)), 0);
}

int LuaVector(lua_State* state, Vector Velocity) {
	LUA->PushCFunction(CVector);
	LUA->PushNumber(Velocity.x);
	LUA->PushNumber(Velocity.y);
	LUA->PushNumber(Velocity.z);
	LUA->Call(3, 1);
	int vel = LUA->ReferenceCreate();
	return vel;
}

bool Alive(lua_State* state) {
	LUA->PushCFunction(CAlive);
	LUA->Push(1);
	LUA->Call(1, 1);
	bool Alive = LUA->GetBool(-1);
	LUA->Pop();
	return Alive;
}

bool IsBot(lua_State* state) {
	LUA->PushCFunction(CIsBot);
	LUA->Push(1);
	LUA->Call(1, 1);
	bool IsBot = LUA->GetBool(-1);
	LUA->Pop();
	return IsBot;
}

bool OnGround(lua_State* state) {
	LUA->PushCFunction(COnGround);
	LUA->Push(1);
	LUA->Call(1, 1);
	bool OG = LUA->GetBool(-1);
	LUA->Pop();
	return OG;
}

const char *SteamID(lua_State* state) {
	LUA->PushCFunction(CSteamID);
	LUA->Push(1);
	LUA->Call(1, 1);
	const char *steamid = LUA->GetString(-1);
	LUA->Pop(); 
	return steamid;
}

int Team(lua_State* state) {
	LUA->PushCFunction(CTeam);
	LUA->Push(1);
	LUA->Call(1, 1);
	int Team = LUA->GetNumber(-1);
	LUA->Pop();
	return Team;
}

int WaterLevel(lua_State* state) {
	LUA->PushCFunction(CWaterLevel);
	LUA->Push(1);
	LUA->Call(1, 1);
	int WL = LUA->GetNumber(-1);
	LUA->Pop();
	return WL;
}

int GetForwardSpeed(lua_State* state) {
	LUA->PushCFunction(CGetForwardSpeed);
	LUA->Push(2);
	LUA->Call(1, 1);
	int fmove = LUA->GetNumber(-1);
	LUA->Pop();
	return fmove;
}

int GetSideSpeed(lua_State* state) {
	LUA->PushCFunction(CGetSideSpeed);
	LUA->Push(2);
	LUA->Call(1, 1);
	int smove = LUA->GetNumber(-1);
	LUA->Pop();
	return smove;
}

bool InRight(lua_State* state) {
	LUA->PushCFunction(CKeyDown);
	LUA->Push(2);
	LUA->PushNumber(IN_MOVERIGHT);
	LUA->Call(2, 1);
	bool IR = LUA->GetBool(-1);
	LUA->Pop();
	return IR;
}

bool InLeft(lua_State* state) {
	LUA->PushCFunction(CKeyDown);
	LUA->Push(2);
	LUA->PushNumber(IN_MOVELEFT);
	LUA->Call(2, 1);
	bool IL = LUA->GetBool(-1);
	LUA->Pop();
	return IL;
}

int GetMaxSpeed(lua_State* state) {
	LUA->PushCFunction(CGetMaxSpeed);
	LUA->Push(2);
	LUA->Call(1, 1);
	int MaxSp = LUA->GetNumber(-1);
	LUA->Pop();
	return MaxSp;
}

QAngle GetMoveAngles(lua_State* state) {
	LUA->PushCFunction(CGetMoveAngles);
	LUA->Push(2);
	LUA->Call(1, 1);
	UserData * udata = (UserData *)LUA->GetUserdata(-1);
	vec_t* ang = (vec_t*)udata->data;
	LUA->Pop();
	return QAngle(ang[0], ang[1], ang[2]);
}

Vector GetVelocity(lua_State* state) {
	LUA->PushCFunction(CGetVelocity);
	LUA->Push(2);
	LUA->Call(1, 1);
	UserData *GetVelocity = (UserData *)LUA->GetUserdata(-1);
	vec_t *v = (vec_t *)GetVelocity->data;
	LUA->Pop();
	return Vector(v[0], v[1], v[2]);
}

void SetVelocity(lua_State* state, Vector Velocity) {
	int vel = LuaVector(state, Velocity);
	LUA->PushCFunction(CSetVelocity);
	LUA->Push(2);
	LUA->ReferencePush(vel);
	LUA->Call(2, 0);
	LUA->ReferenceFree(vel);
}

int GF[129]; // Who has a server with 128 slots anyway? (129 because lua starts at 1 So I need one spare.)

void GroundFix(lua_State* state, bool OG) {
	LUA->PushCFunction(CEntIndex);
	LUA->Push(1);
	LUA->Call(1, 1); // Call function
	int index = LUA->GetNumber(-1); // This will be our array pointer.
	LUA->Pop();
	if (OG == true) {
		if (GF[index] < 5) { // stop counting after 5
			GF[index] = GF[index] + 1;
			if (GF[index] > 4) {
				LUA->PushCFunction(CSetDuckSpeed);
				LUA->Push(1);
				LUA->PushNumber(0.4);
				LUA->Call(2, 0);
				LUA->PushCFunction(CSetUnDuckSpeed);
				LUA->Push(1);
				LUA->PushNumber(0.2);
				LUA->Call(2, 0);
			}
		}
		return;
	}

	if (GF[index] != 0) { // only change if it needs to be changed.
		GF[index] = 0; // Player is not on ground so reset value.
		LUA->PushCFunction(CSetDuckSpeed);
		LUA->Push(1);
		LUA->PushNumber(0);
		LUA->Call(2, 0);
		LUA->PushCFunction(CSetUnDuckSpeed);
		LUA->Push(1);
		LUA->PushNumber(0);
		LUA->Call(2, 0);
	}
	return;
}

int Move(lua_State* state) {

	LUA->CheckType(1, Type::ENTITY);
	LUA->CheckType(2, Type::MOVEDATA);

	if (!HasCachedCFuncs) {
		CacheCFuncs(state);
	}
	
	if (IsBot(state) || !Alive(state)) return 0;
	if (WaterLevel(state) > 0) return 0;
	int PlayerTeam = Team(state);
	if (PlayerTeam == TEAM_SIDEWAYS || PlayerTeam == TEAM_SPECTATOR) return 0;

	bool OG = OnGround(state);
	GroundFix(state, OG);
	if (OG) return 0;
	
	int fmove = GetForwardSpeed(state);
	int smove = GetSideSpeed(state);

	if (InRight(state)) {
		smove = (smove * 10) + 500;
	} else if (InLeft(state)) {
		smove = (smove * 10) - 500;
	}
	if (fmove == 0 && smove == 0) return 0;
	
	QAngle ang = GetMoveAngles(state);

	Vector wishvel = (GetForward(ang) * fmove) + (GetRight(ang) * smove);
	double wishspeed = wishvel.Length2D();

	if (wishspeed > 0) {
		wishvel = wishvel * (GetMaxSpeed(state) / wishspeed);
	}
	wishvel.NormalizeInPlace();

	Vector Velocity = GetVelocity(state);
	double addspeed = 0;
	if (wishspeed != 0) {
		addspeed = 35 - Velocity.Dot(wishvel);
	}
	if (addspeed <= 0) return 0;

	SetVelocity(state, Velocity + (wishvel * addspeed));

	LUA->PushBool(false); //Push false to stop it from calling the GM:Move function.
	return 1;
}

GMOD_MODULE_OPEN() {
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			LUA->GetField(-1, "Add");
			LUA->PushString("Move");
			LUA->PushString("StrafeModule");
			LUA->PushCFunction(Move);
			LUA->Call(3, 0);
		LUA->Pop();
	LUA->Pop();
	// If you want to manually call this you can do the following:
	// local Move = hook.GetTable().Move.StrafeModule
	// The C++ function will then be available to call.

	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}