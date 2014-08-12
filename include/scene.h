/*! \file scene.h
 *  \brief InGame scene handling
 */

#ifndef _SCENE_H
#define _SCENE_H

#include <ogc/gu.h>

/*! Camera structure */
typedef struct {
	guVector	position;		/*< Camera position    */
	f32			width,			/*< Viewport width     */
				height,			/*< Viewport height    */
				offsetTop,		/*< Viewport Y         */
				offsetLeft;		/*< Viewport X         */
	Mtx			perspectiveMtx; /*< Perspective Matrix */
} camera_t;

/*! Loads the game scene and models */
void SCENE_load();

/*! Renders the scene */
void SCENE_render();

#endif