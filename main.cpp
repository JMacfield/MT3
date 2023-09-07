#include <Novice.h>
#include "MyMath.h"

const char kWindowTitle[] = "LE2B_02_イソガイユウト_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Ball ball{ 
	.position = { 0.8f,1.2f,0.3f },
	.velocity = { 0.0f,0.0f,0.0f },
	.acceleration = { 0.0f,-9.8f,0.0f },
	.radius = 0.05f,
	.color = WHITE,
	};

	Plane plane{
		.normal{-0.2f,0.9f,-0.3f},
		.distance = 0.0f,
	};

	Capsule capsule{
		capsule.segment.origin = ball.position,
		capsule.segment.diff = {0.0f,0.0f,0.0f},
		capsule.radius = ball.radius,
	};

	float e = 0.8f;
	float deltaTime = 1.0f / 60.0f;

	bool isStart = false;

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

		if(keys[DIK_SPACE]&& preKeys[DIK_SPACE] == 0) {
			isStart = true;
		}

		if (isStart) {
			Vector3 velocity = ball.acceleration * deltaTime;
			ball.velocity += velocity;
			Vector3 position = ball.velocity * deltaTime;
			ball.position += position;

			capsule.segment.diff = ball.position;
			Vector3 center = Multiply(plane.distance, plane.normal);

			Segment capsuleLine = { capsule.segment.origin,capsule.segment.diff };
			Vector3 closestPoint = ClosestPoint(capsuleLine, center);
			Vector3 closestPointToCenter = center - closestPoint;

			float sumRadius = 1 + capsule.radius;

			if (Dot(closestPointToCenter, closestPointToCenter) < sumRadius * sumRadius) {

			}

			if (IsCollision(Sphere{ ball.position,ball.radius }, plane)) {
				ball.velocity = Reflect(ball.velocity, plane.normal) * e;
			}
		}

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

		Matrix4x4 viewportMatrix = MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSphere(ball, viewProjectionMatrix, viewportMatrix, ball.color);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);

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
