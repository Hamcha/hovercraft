/*! \file gxutils.h
 *  \brief Utility functions and wrappers for GX
 */

#ifndef _GXUTILS_H 
#define _GXUTILS_H

#include <gccore.h>
#include "scene.h"

/*! Texture sizes for sprites and fonts */
const f32 TexSize256 = 1.f / 256.f;

/*! \brief Initialize the GX subsystem
 */
void GXU_init();

/*! \brief Load texture from Id
 *  \param[in]  texId  Texture Id, from generated assets
 *  \param[out] texObj Texture object to bind the texture to
 */
void GXU_loadTexture(s32 texId, GXTexObj* texObj);

/*! \brief Set light color
 *  \param view View matrix
 *  \param lightColor Colors of Diffuse, Ambient and Material lights
 */
void GXU_setLight(Mtx view, GXColor lightColor[]);

/*! \brief Get video mode
 *  \return Currently preferred/used video mode
 */
GXRModeObj* GXU_getMode();

/*! \brief Finish rendering and swap buffers
 */
void GXU_done();

/*! \brief Setup player camera (including split screen mode)
 *  \param[in,out] camera      Camera to setup
 *  \param[in]     splitType   Type of split (total number of players)
 *  \param[in]     splitPlayer Player position in split
 */
void GXU_setupCamera(camera_t* camera, u8 splitType, u8 splitPlayer);

#endif