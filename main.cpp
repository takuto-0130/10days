#include <Novice.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Vector2.h>
#include "Bullet.h"
#include "PositionSwap.h"
#include <assert.h>

const char kWindowTitle[] = "LC1A_26_ヤマグチ_SwapPosition";

// キー入力結果を受け取る箱
char keys[256] = { 0 };
char preKeys[256] = { 0 };

struct Player
{
	Vector2 worldPos;
	Vector2 center;
	Vector2 len;
	Vector2 sizeChange;
	Vector2 direction;
	Vector2 moveSpeed;
	Vector2 normalize;
	Vector2 velocity;
	Vector2 tempVelo;
	float jump;
	float gravity;
	float fallSpeedMax;
	bool isAir;
	Vector2 lt;
	Vector2 rt;
	Vector2 lb;
	Vector2 rb;
	bool isAlive;
	int deadTimer;
	int clearTimer;
};

struct Enemy
{
	Vector2 worldPos;
	Vector2 center;
	Vector2 len;
	Vector2 lt;
	Vector2 rt;
	Vector2 lb;
	Vector2 rb;
	bool isAlive;
	float e;
	int deadTimer;
};

void FindVertex(Vector2 center, float width, float height, Vector2* lt, Vector2* rt, Vector2* lb, Vector2* rb)
{
	*lt = { center.x - width / 2, center.y - height / 2 };
	*rt = { (center.x + width / 2 - 1), center.y - height / 2 };
	*lb = { center.x - width / 2, (center.y + height / 2 - 1) };
	*rb = { (center.x + width / 2 - 1), (center.y + height / 2 - 1) };
}

void MoveLimit(Player& a) {
	if (a.worldPos.x < 0 + a.len.x / 2) {
		a.worldPos.x = 0 + a.len.x / 2;
	}
	if (a.worldPos.x > kWorldEnd) {
		a.worldPos.x = float(kWorldEnd);
	}
	if (a.worldPos.y < 0) {
		a.worldPos.y = 0;
	}
	if (a.worldPos.y > 800) {
		a.worldPos.y = 800;
	}
}

Vector2 Normalize(Vector2 v) {
	float denominator = sqrtf(v.x * v.x + v.y * v.y);
	Vector2 result = { 0,0 };
	if (denominator != 0) {
		result = { sqrtf(v.x * v.x) / denominator, sqrtf(v.y * v.y) / denominator };
	}
	return result;
}


void MovePlayer(Player& a) {
	a.velocity = {
		(a.moveSpeed.x * a.direction.x * a.normalize.x),
		a.velocity.y
	};
	a.worldPos = {
		a.worldPos.x + a.velocity.x,
		a.worldPos.y + a.velocity.y
	};
	MoveLimit(a);
}

void winMapRect(WindowMap& a, int x, int y) {
	a.localRect[y][x].leftTop = { float(-a.tileSize / 2.0f), float(-a.tileSize / 2.0f) };
	a.localRect[y][x].rightTop = { float(a.tileSize / 2.0f), float(-a.tileSize / 2.0f) };
	a.localRect[y][x].leftBottom = { float(-a.tileSize / 2.0f), float(a.tileSize / 2.0f) };
	a.localRect[y][x].rightBottom = { float(a.tileSize / 2.0f), float(a.tileSize / 2.0f) };
	a.pos[y][x] = { float(a.tileSize * x) + float(a.tileSize / 2.0f), float(a.tileSize * y) + float(a.tileSize / 2.0f) };
};

void DrawQuadVertex(Vertex a, int texHandle, Vector2 texLeftTop, Vector2 texLength, unsigned int color) {
	Novice::DrawQuad(int(a.leftTop.x), int(a.leftTop.y), int(a.rightTop.x), int(a.rightTop.y), int(a.leftBottom.x), int(a.leftBottom.y), int(a.rightBottom.x), int(a.rightBottom.y),
		int(texLeftTop.x), int(texLeftTop.y), int(texLength.x), int(texLength.y), texHandle, color);
}

Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Matrix3x3 MakeAffineMatrix(Vector2 scale, float theta, Vector2 transrate) {
	Matrix3x3 m1;
	m1.m[0][0] = scale.x * cosf(theta);  m1.m[0][1] = scale.x * sinf(theta); m1.m[0][2] = 0;
	m1.m[1][0] = scale.y * -sinf(theta); m1.m[1][1] = scale.y * cosf(theta); m1.m[1][2] = 0;
	m1.m[2][0] = transrate.x;			 m1.m[2][1] = transrate.y;			 m1.m[2][2] = 1;
	return m1;
}

const int kParticleNumber = 300;

struct Particle {
	Vector2 position[kParticleNumber];
	Vector2 velocity[kParticleNumber];
	float life[kParticleNumber];
	float radius[kParticleNumber];
	unsigned int color[kParticleNumber];
};

struct Emitter {
	Vector2 position;
	Vector2 radius;
	float life;
};

Vector2 NormalizeDead(Vector2 v) {
	float denominator = sqrtf(v.x * v.x + v.y * v.y);
	Vector2 result = { 0,0 };
	if (denominator != 0) {
		result = { v.x / denominator, v.y / denominator };
	}
	return result;
}

const int kDeadParticleNumber = 100;

struct ParticleDead {
	Vector2 position[kDeadParticleNumber];
	Vector2 startPos[kDeadParticleNumber];
	Vector2 moveAngle[kDeadParticleNumber];
	Vector2 velocity[kDeadParticleNumber];
	float life[kDeadParticleNumber];
	float radius[kDeadParticleNumber];
	float theta[kDeadParticleNumber];
	unsigned int color[kDeadParticleNumber];
};

struct EmitterDead {
	Vector2 position;
	float radius;
	Vector2 moveSpeed;
	float life;
	float startLen;
};

void easeOut(ParticleDead& a, EmitterDead b) {
	float ease = 0;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		ease = powf(a.life[i] / b.life, 4);
		a.position[i].x = a.startPos[i].x + ((b.position.x - a.startPos[i].x) * ease);
		a.position[i].y = a.startPos[i].y + ((b.position.y - a.startPos[i].y) * ease);
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, 720);

	ParticleDead particleDead;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		particleDead.position[i] = { 0,0 };
		particleDead.startPos[i] = { 0,0 };
		particleDead.moveAngle[i] = { 0,0 };
		particleDead.velocity[i] = { 0,0 };
		particleDead.life[i] = 0;
		particleDead.radius[i] = 15;
		particleDead.theta[i] = 0;
		particleDead.color[i] = 0xFFFFFFFF;
	}

	ParticleDead particle1Dead;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		particle1Dead.position[i] = { 0,0 };
		particle1Dead.startPos[i] = { 0,0 };
		particle1Dead.moveAngle[i] = { 0,0 };
		particle1Dead.velocity[i] = { 0,0 };
		particle1Dead.life[i] = 0;
		particle1Dead.radius[i] = 15;
		particle1Dead.theta[i] = 0;
		particleDead.color[i] = 0xFFFFFFFF;
	}

	ParticleDead particleDeade;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		particleDeade.position[i] = { 0,0 };
		particleDeade.startPos[i] = { 0,0 };
		particleDeade.moveAngle[i] = { 0,0 };
		particleDeade.velocity[i] = { 0,0 };
		particleDeade.life[i] = 0;
		particleDeade.radius[i] = 15;
		particleDeade.theta[i] = 0;
		particleDeade.color[i] = 0xFFFFFFFF;
	}

	ParticleDead particle1Deade;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		particle1Deade.position[i] = { 0,0 };
		particle1Deade.startPos[i] = { 0,0 };
		particle1Deade.moveAngle[i] = { 0,0 };
		particle1Deade.velocity[i] = { 0,0 };
		particle1Deade.life[i] = 0;
		particle1Deade.radius[i] = 15;
		particle1Deade.theta[i] = 0;
		particle1Deade.color[i] = 0xFFFFFFFF;
	}

	ParticleDead clearparticleDeade;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		clearparticleDeade.position[i] = { 0,0 };
		clearparticleDeade.startPos[i] = { 0,0 };
		clearparticleDeade.moveAngle[i] = { 0,0 };
		clearparticleDeade.velocity[i] = { 0,0 };
		clearparticleDeade.life[i] = 0;
		clearparticleDeade.radius[i] = 15;
		clearparticleDeade.theta[i] = 0;
		clearparticleDeade.color[i] = 0xFFFFFFFF;
	}

	ParticleDead clearparticle1Deade;
	for (int i = 0; i < kDeadParticleNumber; i++)
	{
		clearparticle1Deade.position[i] = { 0,0 };
		clearparticle1Deade.startPos[i] = { 0,0 };
		clearparticle1Deade.moveAngle[i] = { 0,0 };
		clearparticle1Deade.velocity[i] = { 0,0 };
		clearparticle1Deade.life[i] = 0;
		clearparticle1Deade.radius[i] = 15;
		clearparticle1Deade.theta[i] = 0;
		clearparticle1Deade.color[i] = 0xFFFFFFFF;
	}
	srand(1);

	EmitterDead emitterDead = {
		300,
		200,
		5,
		1,
		1,
		kDeadParticleNumber,
		130
	};

	EmitterDead emitterDeade = {
		300,
		200,
		5,
		1,
		1,
		kDeadParticleNumber,
		130
	};

	Particle particle;
	for (int i = 0; i < kParticleNumber; i++)
	{
		particle.position[i] = { 0,0 };
		particle.velocity[i] = { 0,0 };
		particle.life[i] = 0;
		particle.radius[i] = 5;
		particle.color[i] = 0xFFFFFFFF;
	}

	srand(1);

	Emitter emitter = {
		320,
		180,
		640,
		30,
		kParticleNumber
	};


	emitter.position = { float(640),float(-50) };

	float gravity = 0.02f;

	bool isParticle = 0;

	int mapTex = Novice::LoadTexture("./Resources/mapBlock.png");
	int playerTex = Novice::LoadTexture("./Resources/playerRect.png");
	//int enemyTex = Novice::LoadTexture("./Resources/enemy.png");
	int bgTex = Novice::LoadTexture("./Resources/bg.png");
	int block = Novice::LoadTexture("white1x1.png");
	int titleTex = Novice::LoadTexture("./Resources/title.png");
	//int setumeiTex = Novice::LoadTexture("./Resources/setumei.png");
	int clearTex = Novice::LoadTexture("./Resources/clear.png");

	int bg2Tex = Novice::LoadTexture("./Resources/load.png");

	MapChipNum map;

	int c;//文字を格納する変数
	int retu = 0;
	int gyou = 0;
	char buf[10];//文字列を格納するバッファ
	memset(buf, 0, sizeof(buf));//←↑2桁以上のマップチップ番号を読み込むときに必要
	bool eofFlag = 0;
	FILE* fp;//ファイルポインタを宣言
	errno_t error;//ファイルが読み取れているかどうかa
	error = fopen_s(&fp, "./Resources/mapSample.csv", "r");//ファイルを開いてポインタに結び付ける
	if (error != 0) {//ファイルが読み取れていないときに動かさないようにする
		return 1;
	}
	else {
		while (1) {
			while (1) {
				c = fgetc(fp);//文字を読み込みcに格納
				if (c == EOF) {
					int num = atoi(buf);//文字列をint型に変換
					map.mapData[gyou][retu] = num;//マップ番号の割り振り
					memset(buf, 0, sizeof(buf));//バッファの値をリセット
					eofFlag = 1;//EndOfFileの時にループを抜ける
					break;
				}
				if (c != ',' && c != '\n') {
					strcat_s(buf, (const char*)&c);//cが区切りや改行でなければバッファの数字に連結(２桁以上で必要)
				}
				else {
					int num = atoi(buf);//文字列をint型に変換
					map.mapData[gyou][retu] = num;//マップ番号の割り振り
					memset(buf, 0, sizeof(buf));//バッファの値をリセット
					break;//区切りか改行ならループを抜ける
				}
			}
			if (eofFlag != 0) {
				break;
			}
			if (c == ',') {
				retu++;
			}
			if (c == '\n') {
				gyou++;
				retu = 0;
			}
		}
	}
	fclose(fp);//ファイルを閉じる

	Player player;
	player.worldPos = { 1 * blockSize + blockSize / 2, 1 * blockSize + blockSize / 2 };
	player.center = player.worldPos;
	player.len = { blockSize,blockSize };
	player.sizeChange = { 0,0 };
	player.direction = { 0,0 };
	player.moveSpeed = { 4,4 };
	player.normalize = { 0,0 };
	player.isAir = 0;
	player.velocity = { 0,0 };
	player.tempVelo = { 0,0 };
	player.gravity = 0.2f;
	player.fallSpeedMax = 12;
	player.jump = -8;
	player.isAlive = 1;
	FindVertex(player.center, player.len.x, player.len.y, &player.lt, &player.rt, &player.lb, &player.rb);

	Player oldPlayer1 = player;

	Enemy enemy;
	enemy.len = { blockSize,blockSize };
	enemy.worldPos = { 30 * blockSize - enemy.len.x / 2,6 * blockSize - enemy.len.y / 2 };
	enemy.center = { enemy.worldPos.x,  enemy.worldPos.y };
	enemy.isAlive = 1;
	enemy.e = -0.5f;
	FindVertex(enemy.center, enemy.len.x, enemy.len.y, &enemy.lt, &enemy.rt, &enemy.lb, &enemy.rb);

	Enemy oldEnemy = enemy;

	Bullet bullet;

	PositionSwap positionSwap;

	enum Scene {
		TITLE,
		GAME
	};

	int scene = GAME;


	WindowMap winMap;
	winMap.scaleChange = 0.04f;
	winMap.timer = 0;
	winMap.i = 0;
	winMap.isScaleChanging = 0;
	winMap.isChangeStart = 0;
	for (int y = 0; y < kWindowMapHeight; y++) {
		for (int x = 0; x < kWindowMapWidth; x++) {
			winMap.scale[y][x] = { 0,0 };
			winMap.isScaleChange[y][x] = 0;
			winMapRect(winMap, x, y);
			winMap.affineMatrix[y][x] = MakeAffineMatrix(winMap.scale[y][x], 0.0f, winMap.pos[y][x]);
			winMap.rect[y][x].leftTop = Transform(winMap.localRect[y][x].leftTop, winMap.affineMatrix[y][x]);
			winMap.rect[y][x].rightTop = Transform(winMap.localRect[y][x].rightTop, winMap.affineMatrix[y][x]);
			winMap.rect[y][x].leftBottom = Transform(winMap.localRect[y][x].leftBottom, winMap.affineMatrix[y][x]);
			winMap.rect[y][x].rightBottom = Transform(winMap.localRect[y][x].rightBottom, winMap.affineMatrix[y][x]);
		}
	}
	bool isScaleDown = 0;

	bool isClear = 0;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		



		if (winMap.timer > 0 && winMap.timer < winMap.kMaxTimer) {
			winMap.timer++;
		}
		if (winMap.timer == winMap.kMaxTimer) {
			winMap.timer = 0;
		}

		if (winMap.isScaleChanging == 1) {
			if (isScaleDown == 1) {
				for (int y = 0; y < kWindowMapHeight; y++) {
					for (int x = 0; x < kWindowMapWidth; x++) {
						if (winMap.isScaleChange[y][x] == 1) {
							winMap.scale[y][x].x -= winMap.scaleChange;
							winMap.scale[y][x].y = winMap.scale[y][x].x;
						}
						if (winMap.scale[y][x].x <= 0) {
							winMap.scale[y][x].x = 0.0f;
							winMap.scale[y][x].y = winMap.scale[y][x].x;
							winMap.isScaleChange[y][x] = 0;
							if (y + x == kWindowMapHeight + kWindowMapWidth - 2) {
								winMap.isChangeStart = 0;
								winMap.isScaleChanging = 0;
							}
						}
						winMap.affineMatrix[y][x] = MakeAffineMatrix(winMap.scale[y][x], 0.0f, winMap.pos[y][x]);
						winMap.rect[y][x].leftTop = Transform(winMap.localRect[y][x].leftTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightTop = Transform(winMap.localRect[y][x].rightTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].leftBottom = Transform(winMap.localRect[y][x].leftBottom, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightBottom = Transform(winMap.localRect[y][x].rightBottom, winMap.affineMatrix[y][x]);
					}
				}
			}
			else {
				for (int y = 0; y < kWindowMapHeight; y++) {
					for (int x = 0; x < kWindowMapWidth; x++) {
						if (winMap.isScaleChange[y][x] == 1) {
							winMap.scale[y][x].x += winMap.scaleChange;
							winMap.scale[y][x].y = winMap.scale[y][x].x;
						}
						if (winMap.scale[y][x].x >= 1.0f) {
							winMap.scale[y][x].x = 1.0f;
							winMap.scale[y][x].y = winMap.scale[y][x].x;
							winMap.isScaleChange[y][x] = 0;
							if (y + x == kWindowMapHeight + kWindowMapWidth - 2) {
								winMap.isChangeStart = 0;
								winMap.isScaleChanging = 0;
							}
						}
						winMap.affineMatrix[y][x] = MakeAffineMatrix(winMap.scale[y][x], 0.0f, winMap.pos[y][x]);
						winMap.rect[y][x].leftTop = Transform(winMap.localRect[y][x].leftTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightTop = Transform(winMap.localRect[y][x].rightTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].leftBottom = Transform(winMap.localRect[y][x].leftBottom, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightBottom = Transform(winMap.localRect[y][x].rightBottom, winMap.affineMatrix[y][x]);
					}
				}
			}

			if (winMap.timer == 0) {
				for (int y = 0; y < kWindowMapHeight; y++) {
					for (int x = 0; x < kWindowMapWidth; x++) {
						if (y + x == winMap.i) {
							winMap.isScaleChange[y][x] = 1;
							winMap.timer = 1;
						}
					}
				}
				if (winMap.i < kWindowMapHeight + kWindowMapWidth) {
					winMap.i++;
				}
				else {
					winMap.i = 0;
				}
			}
		}


		switch(scene) {
		case TITLE:
			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0 && winMap.isScaleChanging == 0) {
				winMap.isChangeStart = 1;
			}

			if (winMap.isChangeStart == 1 && winMap.isScaleChanging == 0) {
				winMap.isScaleChanging = 1;
				isScaleDown = 0;
				for (int y = 0; y < kWindowMapHeight; y++) {
					for (int x = 0; x < kWindowMapWidth; x++) {
						winMap.isScaleChange[y][x] = 0;
						winMap.timer = 0;
						winMap.scale[y][x].x = 0.0f;
						winMap.scale[y][x].y = winMap.scale[y][x].x;
						winMap.affineMatrix[y][x] = MakeAffineMatrix(winMap.scale[y][x], 0.0f, winMap.pos[y][x]);
						winMap.rect[y][x].leftTop = Transform(winMap.localRect[y][x].leftTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightTop = Transform(winMap.localRect[y][x].rightTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].leftBottom = Transform(winMap.localRect[y][x].leftBottom, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightBottom = Transform(winMap.localRect[y][x].rightBottom, winMap.affineMatrix[y][x]);
					}
				}
				winMap.i = 0;
			}
			if (winMap.isScaleChanging == 0 && winMap.scale[kWindowMapHeight - 1][kWindowMapWidth - 1].x == 1.0f) {
				scene = GAME;
				winMap.isChangeStart = 1;
			}
			break;
		case GAME:
			emitterDead.position = player.center;
			emitterDeade.position = enemy.center;
			isParticle = 0;
			for (int i = 0; i < kParticleNumber; i++)
			{
				if (particle.life[i] == 0)
				{
					int R = 0;
					int G = 0;
					int B = 0;
					R = rand() % 120 + 105;
					G = rand() % 120 + 105;
					B = rand() % 40 + 215;
					particle.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
					particle.position[i] = { 0,0 };
					particle.velocity[i] = { 0,float(rand() % 3) };
					particle.life[i] = 0;
					particle.radius[i] = float(rand() % 7 - 3);
					particle.position[i].x = float(rand() % int(emitter.radius.x * 4 - emitter.radius.x * 2)) + emitter.position.x - emitter.radius.x;
					particle.position[i].y = float(rand() % int(emitter.radius.y * 4 - emitter.radius.y * 2)) + emitter.position.y - emitter.radius.y;
					particle.life[i] = emitter.life;
					break;
				}
			}

			for (int i = 0; i < kParticleNumber; i++)
			{
				if (particle.life[i] > 0)
				{
					particle.life[i]--;
					particle.velocity[i].y += gravity;
					particle.position[i].y += particle.velocity[i].y;
				}
				if (particle.life[i] < 0)
				{
					particle.life[i] = 0;
				}
			}

			if (winMap.isChangeStart == 1 && winMap.isScaleChanging == 0) {
				winMap.isScaleChanging = 1;
				isScaleDown = 1;
				for (int y = 0; y < kWindowMapHeight; y++) {
					for (int x = 0; x < kWindowMapWidth; x++) {
						winMap.isScaleChange[y][x] = 0;
						winMap.timer = 0;
						winMap.scale[y][x].x = 1.0f;
						winMap.scale[y][x].y = winMap.scale[y][x].x;
						winMap.affineMatrix[y][x] = MakeAffineMatrix(winMap.scale[y][x], 0.0f, winMap.pos[y][x]);
						winMap.rect[y][x].leftTop = Transform(winMap.localRect[y][x].leftTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightTop = Transform(winMap.localRect[y][x].rightTop, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].leftBottom = Transform(winMap.localRect[y][x].leftBottom, winMap.affineMatrix[y][x]);
						winMap.rect[y][x].rightBottom = Transform(winMap.localRect[y][x].rightBottom, winMap.affineMatrix[y][x]);
					}
				}
				winMap.i = 0;
			}

			oldPlayer1.worldPos = player.worldPos;
			oldPlayer1.center = player.center;
			oldEnemy = enemy;

			if (player.isAlive == 1)
			{

				player.direction = { 0,0 };
				if (bullet.GetIsShot() == 0) {
					if (keys[DIK_A] || keys[DIK_LEFT]) {
						player.direction.x--;
					}
					if (keys[DIK_D] || keys[DIK_RIGHT]) {
						player.direction.x++;
					}

					if (preKeys[DIK_W] == 0 && keys[DIK_W] != 0 || preKeys[DIK_UP] == 0 && keys[DIK_UP] != 0) {
						player.tempVelo.y = player.jump;
						player.isAir = 1;
					}
				}

				if (player.isAir == 1)
				{
					player.tempVelo.y += player.gravity;
				}

				if (player.isAir == 1 && player.lb.y < enemy.lt.y)
				{
					if (player.rt.x > enemy.lt.x && player.lt.x < enemy.rt.x)
					{
						if (player.lb.y + player.tempVelo.y > enemy.lt.y && bullet.GetIsHit() == 0)
						{
							enemy.isAlive = 0;
						}
					}
				}

				if (player.lt.y < enemy.rb.y && player.lb.y > enemy.rt.y && enemy.isAlive == 1)
				{
					if (player.rt.x > enemy.lt.x && player.lt.x < enemy.rt.x && bullet.GetIsHit() == 0)
					{
						player.isAlive = 0;
					}
				}

				player.normalize = Normalize(player.direction);

				FindVertex(player.worldPos, player.len.x + player.sizeChange.x, player.len.y + player.sizeChange.y, &player.lt, &player.rt, &player.lb, &player.rb);
				FindVertex(oldPlayer1.worldPos, oldPlayer1.len.x + oldPlayer1.sizeChange.x, oldPlayer1.len.y + oldPlayer1.sizeChange.y, &oldPlayer1.lt, &oldPlayer1.rt, &oldPlayer1.lb, &oldPlayer1.rb);
				if (map.mapData[int(player.lb.y + (player.moveSpeed.y * player.direction.y)) / blockSize][int(player.lb.x) / blockSize] == 0 &&
					map.mapData[int(player.rb.y + (player.moveSpeed.y * player.direction.y)) / blockSize][int(player.rb.x) / blockSize] == 0) {
					player.isAir = 1;
				}
				if (player.isAir == 0)
				{
					if (player.direction.x > 0)
					{
						if (map.mapData[int(player.rt.y) / blockSize][int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
							map.mapData[int(player.rb.y - player.tempVelo.y) / blockSize][int(player.rb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
							player.worldPos.x = float(int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize - (player.len.x + player.sizeChange.x) / 2;
							player.direction.x = 0;
						}
					}
					if (player.direction.x < 0)
					{
						if (map.mapData[int(player.lt.y) / blockSize][int(player.lt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
							map.mapData[int(player.lb.y - player.tempVelo.y) / blockSize][int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
							player.worldPos.x = float(int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize + (player.len.x + player.sizeChange.x) / 2 + blockSize;
							player.direction.x = 0;
						}
					}
					if (map.mapData[int(player.lt.y + (player.moveSpeed.y * player.direction.y)) / blockSize][int(player.lt.x) / blockSize] == 1 ||
						map.mapData[int(player.rt.y + (player.moveSpeed.y * player.direction.y)) / blockSize][int(player.rt.x) / blockSize] == 1) {
						player.worldPos.y = float(int(player.rt.y + (player.moveSpeed.y * player.direction.y)) / blockSize) * blockSize + (player.len.y + player.sizeChange.y) / 2 + blockSize;
						player.direction.y = 0;
						player.tempVelo.y = 0;
					}
					if (map.mapData[int(player.lb.y + (player.moveSpeed.y * player.direction.y) + player.tempVelo.y) / blockSize][int(player.lb.x) / blockSize] == 1 ||
						map.mapData[int(player.rb.y + (player.moveSpeed.y * player.direction.y) + player.tempVelo.y) / blockSize][int(player.rb.x) / blockSize] == 1) {
						player.worldPos.y = float(int(player.rb.y + (player.moveSpeed.y * player.direction.y)) / blockSize) * blockSize - (player.len.y + player.sizeChange.y) / 2;
						player.direction.y = 0;
						player.tempVelo.y = 0;
						player.isAir = 0;
					}
				}
				else
				{
					if (player.direction.x > 0)
					{
						if (map.mapData[int(player.rt.y) / blockSize][int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
							map.mapData[int(player.rb.y) / blockSize][int(player.rb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
							player.worldPos.x = float(int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize - (player.len.x + player.sizeChange.x) / 2;
							player.direction.x = 0;
						}
					}
					if (player.direction.x < 0)
					{
						if (map.mapData[int(player.lt.y) / blockSize][int(player.lt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
							map.mapData[int(player.lb.y) / blockSize][int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
							player.worldPos.x = float(int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize + (player.len.x + player.sizeChange.x) / 2 + blockSize;
							player.direction.x = 0;
						}
					}
					if (map.mapData[int(player.lt.y + player.tempVelo.y) / blockSize][int(player.lt.x) / blockSize] == 1 ||
						map.mapData[int(player.rt.y + player.tempVelo.y) / blockSize][int(player.rt.x) / blockSize] == 1) {
						player.worldPos.y = float(int(player.rt.y + (player.moveSpeed.y * player.direction.y)) / blockSize) * blockSize + (player.len.y + player.sizeChange.y) / 2;
						player.direction.y = 0;
						player.tempVelo.y = 0;
						player.velocity.y = 0;
					}
					if (map.mapData[int(player.lb.y + (player.moveSpeed.y * player.direction.y) + player.tempVelo.y) / blockSize][int(player.lb.x) / blockSize] == 1 ||
						map.mapData[int(player.rb.y + (player.moveSpeed.y * player.direction.y) + player.tempVelo.y) / blockSize][int(player.rb.x) / blockSize] == 1) {
						player.worldPos.y = float(int(player.rb.y + (player.moveSpeed.y * player.direction.y) + player.tempVelo.y) / blockSize) * blockSize - (player.len.y + player.sizeChange.y) / 2;
						player.direction.y = 0;
						player.tempVelo.y = 0;
						player.isAir = 0;
					}
				}
				player.velocity.y = player.tempVelo.y;
				MovePlayer(player);

				if (map.mapData[int(enemy.center.y) / blockSize][int(enemy.center.x) / blockSize] == 2) {
					isClear = 1;
				}

				bullet.Update(player.worldPos, player.direction, enemy.center, enemy.isAlive, blockSize, map, keys, preKeys, isClear);

				positionSwap.Update(bullet.GetIsHit(), player.worldPos, enemy.center);

				if (bullet.GetIsHit() == 1) {
					enemy.center = positionSwap.GetSwapBoxPos();
					player.worldPos = positionSwap.GetSwapPlayerPos();
				}

				player.center = player.worldPos;

				FindVertex(player.center, player.len.x + player.sizeChange.x, player.len.y + player.sizeChange.y, &player.lt, &player.rt, &player.lb, &player.rb);
				FindVertex(enemy.center, enemy.len.x, enemy.len.y, &enemy.lt, &enemy.rt, &enemy.lb, &enemy.rb);

				if (enemy.isAlive == 0 && oldEnemy.isAlive == 1)
				{
					player.tempVelo.y *= enemy.e;
				}
				if (enemy.isAlive == 0) {
					enemy.deadTimer++;
				}
				if (enemy.deadTimer > 0 && enemy.deadTimer < 12) {
					for (int i = 0; i < kDeadParticleNumber; i++)
					{
						if (particleDeade.life[i] == 0)
						{
							int R = 0;
							int G = 0;
							int B = 0;
							R = 255;
							G = rand() % 70 + 50;
							B = rand() % 70 + 50;
							particleDeade.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
							particleDeade.radius[i] = 0.2f * float(rand() % 10 + 10);
							particleDeade.moveAngle[i].x = float(rand() % 401 - 200);
							particleDeade.moveAngle[i].y = float(rand() % 401 - 200);
							particleDeade.moveAngle[i] = NormalizeDead(particleDeade.moveAngle[i]);
							particleDeade.life[i] = emitterDead.life;
							break;
						}
					}
					for (int i = 0; i < kDeadParticleNumber; i++)
					{
						if (particle1Deade.life[i] == 0)
						{
							int R = 0;
							int G = 0;
							int B = 0;
							R = 255;
							G = rand() % 70 + 50;
							B = rand() % 70 + 50;
							particle1Deade.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
							particle1Deade.radius[i] = 0.2f * float(rand() % 10 + 10);
							particle1Deade.moveAngle[i].x = float(rand() % 401 - 200);
							particle1Deade.moveAngle[i].y = float(rand() % 401 - 200);
							particle1Deade.moveAngle[i] = NormalizeDead(particle1Deade.moveAngle[i]);
							particle1Deade.life[i] = emitterDead.life;
							break;
						}
					}
				}
				for (int i = 0; i < kDeadParticleNumber; i++)
				{
					if (particleDeade.life[i] > 0)
					{
						particleDeade.startPos[i] = { emitterDeade.position.x + emitterDeade.startLen * particleDeade.moveAngle[i].x, emitterDeade.position.y + emitterDeade.startLen * particleDeade.moveAngle[i].y };
						particleDeade.life[i]--;
						easeOut(particleDeade, emitterDeade);
					}
					if (particleDeade.life[i] < 0)
					{
						particleDeade.life[i] = 0;
					}
					if (particle1Deade.life[i] > 0)
					{
						particle1Deade.startPos[i] = { emitterDeade.position.x + emitterDeade.startLen * particle1Deade.moveAngle[i].x, emitterDeade.position.y + emitterDeade.startLen * particle1Deade.moveAngle[i].y };
						particle1Deade.life[i]--;
						easeOut(particle1Deade, emitterDeade);
					}
					if (particle1Deade.life[i] < 0)
					{
						particle1Deade.life[i] = 0;
					}
				}

			}
			else {
				player.deadTimer++;
				if (player.deadTimer > 0 && player.deadTimer < 12) {
					for (int i = 0; i < kDeadParticleNumber; i++)
					{
						if (particleDead.life[i] == 0)
						{
							int R = 0;
							int G = 0;
							int B = 0;
							R = rand() % 120 + 85;
							G = rand() % 30 + 225;
							B = rand() % 120 + 85;
							particleDead.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
							particleDead.radius[i] = 0.2f * float(rand() % 10 + 10);
							particleDead.moveAngle[i].x = float(rand() % 401 - 200);
							particleDead.moveAngle[i].y = float(rand() % 401 - 200);
							particleDead.moveAngle[i] = NormalizeDead(particleDead.moveAngle[i]);
							particleDead.life[i] = emitterDead.life;
							break;
						}
					}
					for (int i = 0; i < kDeadParticleNumber; i++)
					{
						if (particle1Dead.life[i] == 0)
						{
							int R = 0;
							int G = 0;
							int B = 0;
							R = rand() % 120 + 85;
							G = rand() % 30 + 225;
							B = rand() % 120 + 85;
							particle1Dead.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
							particle1Dead.radius[i] = 0.2f * float(rand() % 10 + 10);
							particle1Dead.moveAngle[i].x = float(rand() % 401 - 200);
							particle1Dead.moveAngle[i].y = float(rand() % 401 - 200);
							particle1Dead.moveAngle[i] = NormalizeDead(particle1Dead.moveAngle[i]);
							particle1Dead.life[i] = emitterDead.life;
							break;
						}
					}
				}

				for (int i = 0; i < kDeadParticleNumber; i++)
				{
					if (particleDead.life[i] > 0)
					{
						particleDead.startPos[i] = { emitterDead.position.x + emitterDead.startLen * particleDead.moveAngle[i].x, emitterDead.position.y + emitterDead.startLen * particleDead.moveAngle[i].y };
						particleDead.life[i]--;
						easeOut(particleDead, emitterDead);
					}
					if (particleDead.life[i] < 0)
					{
						particleDead.life[i] = 0;
					}
					if (particle1Dead.life[i] > 0)
					{
						particle1Dead.startPos[i] = { emitterDead.position.x + emitterDead.startLen * particle1Dead.moveAngle[i].x, emitterDead.position.y + emitterDead.startLen * particle1Dead.moveAngle[i].y };
						particle1Dead.life[i]--;
						easeOut(particle1Dead, emitterDead);
					}
					if (particle1Dead.life[i] < 0)
					{
						particle1Dead.life[i] = 0;
					}
				}
			}
			if (isClear == 1) {
				player.clearTimer++;
				if (player.clearTimer > 125) {
					if (keys[DIK_RETURN]) {
						scene = TITLE;
					}
				}
			}
			if (player.clearTimer % 2 == 0 && isClear == 1) {
				for (int i = 0; i < kDeadParticleNumber; i++)
				{
					if (clearparticleDeade.life[i] == 0)
					{
						int R = 0;
						int G = 0;
						int B = 0;
						R = rand() % 40 + 215;
						G = rand() % 40 + 215;
						B = rand() % 130;
						clearparticleDeade.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
						clearparticleDeade.radius[i] = 0.2f * float(rand() % 10 + 10);
						clearparticleDeade.moveAngle[i].x = float(rand() % 401 - 200);
						clearparticleDeade.moveAngle[i].y = float(rand() % 401 - 200);
						clearparticleDeade.moveAngle[i] = NormalizeDead(clearparticleDeade.moveAngle[i]);
						clearparticleDeade.life[i] = emitterDead.life;
						break;
					}
				}
				for (int i = 0; i < kDeadParticleNumber; i++)
				{
					if (clearparticle1Deade.life[i] == 0)
					{
						int R = 0;
						int G = 0;
						int B = 0;
						R = rand() % 120 + 85;
						G = rand() % 30 + 225;
						B = rand() % 120 + 85;
						clearparticle1Deade.color[i] = (R << 24) + (G << 16) + (B << 8) + 0xFF;
						clearparticle1Deade.radius[i] = 0.2f * float(rand() % 10 + 10);
						clearparticle1Deade.moveAngle[i].x = float(rand() % 401 - 200);
						clearparticle1Deade.moveAngle[i].y = float(rand() % 401 - 200);
						clearparticle1Deade.moveAngle[i] = NormalizeDead(clearparticle1Deade.moveAngle[i]);
						clearparticle1Deade.life[i] = emitterDead.life;
						break;
					}
				}
				
			}
			break;
		}

		for (int i = 0; i < kDeadParticleNumber; i++)
		{
			if (clearparticleDeade.life[i] > 0)
			{
				clearparticleDeade.startPos[i] = { emitterDeade.position.x + 300.0f * clearparticleDeade.moveAngle[i].x, emitterDeade.position.y + 300.0f * clearparticleDeade.moveAngle[i].y };
				clearparticleDeade.life[i]--;
				easeOut(clearparticleDeade, emitterDeade);
			}
			if (clearparticleDeade.life[i] < 0)
			{
				clearparticleDeade.life[i] = 0;
			}
			if (clearparticle1Deade.life[i] > 0)
			{
				clearparticle1Deade.startPos[i] = { emitterDeade.position.x + 300.0f * clearparticle1Deade.moveAngle[i].x, emitterDeade.position.y + 300.0f * clearparticle1Deade.moveAngle[i].y };
				clearparticle1Deade.life[i]--;
				easeOut(clearparticle1Deade, emitterDeade);
			}
			if (clearparticle1Deade.life[i] < 0)
			{
				clearparticle1Deade.life[i] = 0;
			}
		}

		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0 || scene == TITLE)
		{
			player.clearTimer = 0;
			enemy.deadTimer = 0;
			player.deadTimer = 0;
			isClear = 0;
			player.worldPos = { 1 * blockSize + blockSize / 2, 1 * blockSize + blockSize / 2 };
			player.center = player.worldPos;
			player.direction = { 1,0 };
			bullet.Update(player.worldPos, player.direction, enemy.center, enemy.isAlive, blockSize, map, keys, preKeys, isClear);
			player.normalize = { 0,0 };
			player.isAir = 0;
			player.velocity = { 0,0 };
			player.tempVelo = { 0,0 };
			player.isAlive = 1;
			FindVertex(player.center, player.len.x, player.len.y, &player.lt, &player.rt, &player.lb, &player.rb);
			enemy.center = { enemy.worldPos.x,  enemy.worldPos.y };
			enemy.isAlive = 1;
			FindVertex(enemy.center, enemy.len.x, enemy.len.y, &enemy.lt, &enemy.rt, &enemy.lb, &enemy.rb);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		switch (scene) {
		case TITLE:
			Novice::DrawSprite(0, 0, titleTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			break;
		case GAME:
			Novice::DrawSprite(0, 0, bgTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

			/*for (int i = 0; i < kParticleNumber; i++)
			{
				if (particle.life[i] > 0)
				{
					Novice::DrawEllipse(int(particle.position[i].x), int(particle.position[i].y), int(particle.radius[i]), int(particle.radius[i]), 0.0f, particle.color[i], kFillModeSolid);
				}
			}*/

			for (int i = 0; i < mapChipHeight; i++)
			{
				for (int j = 0; j < mapChipWidth; j++)
				{
					if (map.mapData[i][j] == 1)
					{
						Novice::DrawSpriteRect(int(blockSize * j), blockSize * i, 0, 0, blockSize, blockSize, mapTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					if (map.mapData[i][j] == 2)
					{
						Novice::DrawSpriteRect(int(blockSize * j), blockSize * i, 0, 0, 1, 1, block, 32.0f, 32.0f, 0.0f, 0x55AA55FF);
					}
				}
			}

			bullet.Draw();

			/*if (enemy.isAlive == 1)
			{
				Novice::DrawQuad(
					int(enemy.lt.x), int(enemy.lt.y),
					int(enemy.rt.x + 1), int(enemy.rt.y),
					int(enemy.lb.x), int(enemy.lb.y + 1),
					int(enemy.rb.x + 1), int(enemy.rb.y + 1),
					0, 0, blockSize, blockSize, enemyTex, 0xFFFFFFFF);
			}*/

			if(player.isAlive == 1){
				if (bullet.GetDir() > 0)
				{
					Novice::DrawQuad(
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						0, 0, blockSize, blockSize, playerTex, 0xFFFFFFFF);
				}
				else {
					Novice::DrawQuad(
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						32, 0, blockSize, blockSize, playerTex, 0xFFFFFFFF);
				}
			}

			/*for (int i = 0; i < kDeadParticleNumber; i++)
			{
				if (particleDead.life[i] > 60)
				{
					Novice::DrawEllipse(int(particleDead.position[i].x), int(particleDead.position[i].y), int(particleDead.radius[i]), int(particleDead.radius[i]), 0.0f, particleDead.color[i], kFillModeSolid);
				}
				if (particle1Dead.life[i] > 60)
				{
					Novice::DrawEllipse(int(particle1Dead.position[i].x), int(particle1Dead.position[i].y), int(particle1Dead.radius[i]), int(particle1Dead.radius[i]), 0.0f, particle1Dead.color[i], kFillModeSolid);
				}
				if (particleDeade.life[i] > 60)
				{
					Novice::DrawEllipse(int(particleDeade.position[i].x), int(particleDeade.position[i].y), int(particleDeade.radius[i]), int(particleDeade.radius[i]), 0.0f, particleDeade.color[i], kFillModeSolid);
				}
				if (particle1Deade.life[i] > 60)
				{
					Novice::DrawEllipse(int(particle1Deade.position[i].x), int(particle1Deade.position[i].y), int(particle1Deade.radius[i]), int(particle1Deade.radius[i]), 0.0f, particle1Deade.color[i], kFillModeSolid);
				}

				if (clearparticleDeade.life[i] > 40)
				{
					Novice::DrawEllipse(int(clearparticleDeade.position[i].x), int(clearparticleDeade.position[i].y), int(clearparticleDeade.radius[i]), int(clearparticleDeade.radius[i]), 0.0f, clearparticleDeade.color[i], kFillModeSolid);
				}
				if (clearparticle1Deade.life[i] > 40)
				{
					Novice::DrawEllipse(int(clearparticle1Deade.position[i].x), int(clearparticle1Deade.position[i].y), int(clearparticle1Deade.radius[i]), int(clearparticle1Deade.radius[i]), 0.0f, clearparticle1Deade.color[i], kFillModeSolid);
				}
			}*/
			//Novice::DrawSprite(0, 0, setumeiTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			if (player.clearTimer > 120) {
				Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0xAAAAAAAA, kFillModeSolid);
				Novice::DrawSprite(0, 0, clearTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			}
			break;
		}

		for (int y = 0; y < kWindowMapHeight; y++) {
			for (int x = 0; x < kWindowMapWidth; x++) {
				DrawQuadVertex(winMap.rect[y][x], bg2Tex, { float(x * winMap.tileSize),float(y * winMap.tileSize) }, { float(winMap.tileSize),float(winMap.tileSize) }, 0xFFFFFFFF);
			}
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
