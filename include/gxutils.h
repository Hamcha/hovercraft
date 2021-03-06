/*! \file gxutils.h
 *  \brief Utility functions and wrappers for GX
 */

#ifndef _GXUTILS_H 
#define _GXUTILS_H

#include <gccore.h>

/*! Camera structure */
typedef struct {
	guVector position;       /*< Camera position    */
	f32      width,          /*< Viewport width     */
	         height,         /*< Viewport height    */
	         offsetTop,      /*< Viewport Y         */
	         offsetLeft;     /*< Viewport X         */
	Mtx44    perspectiveMtx; /*< Perspective Matrix */
} camera_t;

/* Frame time (1/60 or 1/50 depending on video mode) */
f32 frameTime;

/*! \brief Initialize the GX subsystem
 */
void GXU_init();

void GXU_closeTPL();

/*! \brief Load texture from Id
 *  \param[in]  texId  Texture Id, from generated assets
 *  \param[out] texObj Texture object to bind the texture to
 */
void GXU_loadTexture(s32 texId, GXTexObj* texObj);

/*! \brief Set light color
 *  \param view View matrix
 *  \param lightColor Light color
 *  \param lpos Light position
 */
void GXU_setLight(Mtx view, GXColor lightColor, guVector lpos);

/*! \brief Set Directional light
*  \param view View matrix
*  \param lightColor Colors of Diffuse, Ambient and Material lights
*  \param ldir Light direction in the world
*  \param shininess Specular shininess
*/
void GXU_setDirLight(Mtx view, GXColor lightColor[], guVector ldir, f32 shininess);

/*! \brief Get video mode
 *  \return Currently preferred/used video mode
 */
GXRModeObj* GXU_getMode();

/*! \brief Get current aspect ratio
 *  \return Aspect ratio as width/height
 */
f32 GXU_getAspectRatio();

/*! \brief Finish rendering and swap buffers
 */
void GXU_done();

/*! \brief Setup player camera (including split screen mode)
 *  \param[in,out] camera      Camera to setup
 *  \param[in]     splitType   Type of split (total number of players)
 *  \param[in]     splitPlayer Player position in split
 */
void GXU_setupCamera(camera_t* camera, u8 splitType, u8 splitPlayer);

/*! \brief Load 2D ortho matrix for sprite/font rendering 
 */
void GXU_2DMode();

/*! \brief Wrapper for changing viewport (adds scissors and 2d update)
 */
void GXU_SetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ);

/*! \brief Get frame rate depending on current TV mode
 */
u32 GXU_framerate();

#endif