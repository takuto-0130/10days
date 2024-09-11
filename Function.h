#pragma once
#include <Novice.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <Vector2.h>
#include "Bullet.h"
#include "PositionSwap.h"
#include <assert.h>
#include "BrockEmitter.h"

void FindVertex(Vector2 center, float width, float height, Vector2* lt, Vector2* rt, Vector2* lb, Vector2* rb);

void MoveLimit(Player& a);

Vector2 Normalize(Vector2 v);

void MovePlayer(Player& a);

void winMapRect(WindowMap& a, int x, int y);

void DrawQuadVertex(Vertex a, int texHandle, Vector2 texLeftTop, Vector2 texLength, unsigned int color);

Vector2 Transform(Vector2 vector, Matrix3x3 matrix);

Matrix3x3 MakeAffineMatrix(Vector2 scale, float theta, Vector2 transrate);

Matrix3x3 MakeRotateMatrix(float theta);

Vector2 NormalizeDead(Vector2 v);

void easeOut(ParticleDead& a, EmitterDead b);

void LoadMap(MapChipNum& map, const char* dirName);

void CollisionBlock(MapChipNum& map, Player& player, const float resist, int& stopFrame, int& stopTimer, BrockEmitter* emitter, uint32_t& breakCount);

void PlayerMapCollision(MapChipNum& map, Player& player, const Vector2& shotDir);


#pragma region // Vector2
Vector2 operator+(const Vector2& a, const Vector2& b);

Vector2 operator-(const Vector2& a, const Vector2& b);

Vector2 operator-(const Vector2& a);

Vector2 operator*(const Vector2& v, const float& s);

Vector2 operator*(const float& s, const Vector2& v);

Vector2 operator/(const Vector2& v, const float& s);

Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);
float Length(const Vector2& v);
void TimeDisplay(const uint32_t time, Timedisp& a);
void ScoreDisplay(const uint32_t score, ScoreDisp& a);
#pragma endregion

void ReturnPosition(Player& player, const Vector2& startPos, const MapChipNum map);

void ReturnPosition(Player& player, Vector2& startPos, Vector2& stopPos, const MapChipNum map, float& t);

void ScrollPosition(const Vector2& kResetPos, const Vector2& returnPos, const Vector2& startPos, float* scroll, const float& t, Vector2& topPos);


#pragma region // 発射されてる時の反射

void TestBlockLeftHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance, float k);

void TestBlockRightHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance, float k);

void TestBlocUnderHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance);

void TestBlockLerp(Player& player, Vector2& start, Vector2& stop, float& t);

#pragma endregion


#pragma region // 動かしてる時の反射

void GetSpeed(Player& player, float& speed, Vector2& startPosition, const float frameAngle);

void LeftHitMove(Player& player, float& speed, Vector2& startPosition, Vector2& direction, Vector2& v);

void RightHitMove(Player& player, float& speed, Vector2& startPosition, Vector2& direction, Vector2& v);

void MoveBlockLeftHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance, Vector2& v);

void MoveBlockRightHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance, Vector2& v);

void MoveBlocUnderHit(Player& player, const MapChipNum map, Vector2& start, Vector2& stop, bool& kabe, float distance, Vector2& v);

#pragma endregion