#include "game.h"

#include <malloc.h>

#include "gxutils.h"
#include "mathutil.h"
#include "raycast.h"
#include "input.h"

object_t* mapTerrain;
object_t* mapPlane;

player_t players[MAX_PLAYERS];

/* Game settings */
const f32 maxSpeed = 0.3f;
guVector gravity = { 0, -.8f / 60.f, 0 };

void GAME_init(object_t* terrain, object_t* plane) {
	mapTerrain = terrain;
	mapPlane = plane;

	u8 i;
	for (i = 0; i < MAX_PLAYERS; i++) {
		player_t* player = GAME_getPlayerData(i);
		player->isPlaying = FALSE;
		player->isGrounded = FALSE;
	}
}

void GAME_createPlayer(u8 playerId, model_t* hovercraftModel, guVector startPosition) {
	/* Create player hovercraft object and position it */
	player_t* player = GAME_getPlayerData(playerId);
	player->hovercraft = OBJECT_create(hovercraftModel);
	OBJECT_moveTo(player->hovercraft, startPosition.x, startPosition.y, startPosition.z);
	OBJECT_flush(player->hovercraft);

	/* Set player as playing */
	player->isPlaying = TRUE;
}

void GAME_removePlayer(u8 playerId) {
	player_t* player = GAME_getPlayerData(playerId);
	OBJECT_destroy(player->hovercraft);
}

void GAME_updateWorld() {
	//todo Checkpoint logic here
}

void GAME_updatePlayer(u8 playerId) {
	player_t* player = GAME_getPlayerData(playerId);

	/* Data */
	guVector acceleration = {0,0,0}, deacceleration = { 0, 0, 0 };
	guVector jump = { 0, 0.3f, 0 };
	guVector *velocity = &player->velocity;
	guVector *position = &player->hovercraft->transform.position;
	guVector *right = &player->hovercraft->transform.right;
	guVector *playerForward = &player->hovercraft->transform.forward;
	guVector forward, worldUp = {0,1,0};

	/* Get input */
	f32 rot = INPUT_AnalogX(playerId) * .033f;
	f32 accel = INPUT_TriggerR(playerId) * .02f;
	f32 decel = INPUT_TriggerL(playerId) * .033f;

	/* Apply rotation */
	OBJECT_rotateAxis(player->hovercraft, &worldUp, rot);
	OBJECT_flush(player->hovercraft);

	/* calculate forward */
	guVecCross(right, &worldUp, &forward);
	guVecNormalize(&forward);

	/* Calculate physics */
	guVecScale(playerForward, &acceleration, accel);
	guVecScale(playerForward, &deacceleration, -decel);

	/* Apply physics */
	guVecScale(velocity, velocity, 0.95f);
	guVecAdd(velocity, &acceleration, velocity);
	guVecAdd(velocity, &deacceleration, velocity);
	guVecAdd(velocity, &gravity, velocity);
	if (player->isGrounded && INPUT_getButton(playerId, INPUT_BTN_JUMP) == TRUE) {
		guVecAdd(velocity, &jump, velocity);
	}

	/* Calculate collisions */
	u8 otherPlayerId;
	for (otherPlayerId = playerId + 1; otherPlayerId < MAX_PLAYERS; otherPlayerId++) {
		player_t* target = GAME_getPlayerData(otherPlayerId);
		if (target->isPlaying != TRUE) continue;

		/* Check for collision between current player and other*/
		CalculateBounce(player, target);
	}

	/* Limit speed */
	/*if (guVecDotProduct(velocity, velocity) > (maxSpeed*maxSpeed)) {
		guVecNormalize(velocity);
		guVecScale(velocity, velocity, maxSpeed);
	}*/

	/* Move Player */
	OBJECT_move(player->hovercraft, velocity->x, velocity->y, velocity->z);

	/* Collision check*/
	const f32 rayoffset = 200;
	guVector raydir = { 0, -1, 0 };
	guVector raypos = { 0, rayoffset, 0 };
	guVector rayhit, normalhit;
	guVecAdd(&raypos, position, &raypos);
	f32 dist = 0;
	f32 minHeight = mapPlane->transform.position.y;
	guQuaternion rotation;

	/* Raycast track */
	if (Raycast(mapTerrain, &raydir, &raypos, &dist, &normalhit)) {
		/* Get hit position */
		guVecScale(&raydir, &rayhit, dist);
		guVecAdd(&rayhit, &raypos, &rayhit);
		f32 height = rayhit.y;

		if (dist < rayoffset) {
			/* Moved into the terrain, snap */
			guVector f;
			guVecCross(right, &normalhit, &f);
			guVecNormalize(&f);
			QUAT_lookat(&f, &normalhit, &rotation);
			QUAT_slerp(&rotation, &player->hovercraft->transform.rotation, .9f, &rotation);
			OBJECT_moveTo(player->hovercraft, rayhit.x, height, rayhit.z);

			/* Since we hit the ground, reset the gravity*/
			player->isGrounded = TRUE;
			velocity->y = 0.0f;
		} else {
			/* We didn't move into the terrain */
			player->isGrounded = FALSE;

			/* Rotate back to level*/
			QUAT_lookat(&forward, &worldUp, &rotation);
			QUAT_slerp(&rotation, &player->hovercraft->transform.rotation, .9f, &rotation);
		}
	} else {
		/* Ray misses, we're up really high or on water, code below will make use*/
		player->isGrounded = FALSE;

		/* This should be avoided somehow */
		QUAT_lookat(&forward, &worldUp, &rotation);
		QUAT_slerp(&rotation, &player->hovercraft->transform.rotation, .9f, &rotation);
	}

	/* Rotate player again */
	OBJECT_rotateSet(player->hovercraft, &rotation);

	/* Make sure we do not move underwater */
	if (position->y < minHeight) {
		player->isGrounded = TRUE;
		velocity->y = 0.0f;
		OBJECT_moveTo(player->hovercraft, position->x, minHeight, position->z);
	}

	OBJECT_flush(player->hovercraft);
}

void GAME_renderPlayerView(u8 playerId) {
	/* Setup camera view and perspective */
	player_t* player = GAME_getPlayerData(playerId);
	transform_t target = player->hovercraft->transform;
	camera_t* camera = &player->camera;

	/* Settings */
	const float cameraHeight = 2.5;
	const float cameraDistance = -5.f;
	const float t = 1.f / 10.f;

	/* Calculate camera position*/
	guVector posTemp, targetCameraPos = { 0, cameraHeight, 0 };
	guVecScale(&target.forward, &posTemp, cameraDistance);
	guVecAdd(&targetCameraPos, &posTemp, &targetCameraPos);
	guVecAdd(&target.position, &targetCameraPos, &targetCameraPos);

	/* Lerp between old camera position and target */
	guVector camPos, up = { 0, 1, 0 };
	guVecSub(&targetCameraPos, &camera->position, &camPos);
	guVecScale(&camPos, &camPos, t);
	guVecAdd(&camera->position, &camPos, &camPos);

	/* make sure the camera does not enter the ground*/
	const f32 rayoffset = 400;
	guVector raydir = { 0, -1, 0 };
	guVector raypos = { 0, rayoffset, 0 };
	guVecAdd(&raypos, &camPos, &raypos);
	guVector normalhit;
	f32 dist = 0;
	if (Raycast(mapTerrain, &raydir, &raypos, &dist, &normalhit)) {
		if (dist < (rayoffset + cameraHeight)) {
			/* the camera is lower then it should be, move up*/
			camPos.y += (rayoffset + cameraHeight) - dist;
		}
	}

	/* Create camera matrix */
	Mtx viewMtx;
	guLookAt(viewMtx, &camPos, &up, &target.position);

	GX_LoadProjectionMtx(camera->perspectiveMtx, GX_PERSPECTIVE);
	camera->position = camPos;

	/* Viewport setup */
	GX_SetViewport(camera->offsetLeft, camera->offsetTop, camera->width, camera->height, 0, 1);

	/* Render the player's hovercraft */
	SCENE_renderPlayer(viewMtx);
}

player_t* GAME_getPlayerData(u8 playerId) {
	return &players[playerId];
}