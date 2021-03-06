// Copyright (C) XFX Team
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright 
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions and the following disclaimer in the 
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any 
//       contributors may be used to endorse or promote products derived from 
//       this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

extern "C"
{
#include "pbkit.h"
}

#include <Graphics/Color.h>
#include <Graphics/GraphicsDevice.h>
#include <Graphics/Sprite.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/SpriteFont.h>
#include <Graphics/Texture2D.h>
#include <MathHelper.h>
#include <Matrix.h>
#include <Rectangle.h>
#include <Vector2.h>
#include <Vector4.h>

#include <sassert.h>

#include <System/Type.h>

namespace XFX
{
	namespace Graphics
	{
		const Type SpriteBatchTypeInfo("SpriteBatch", "XFX::Graphics::SpriteBatch", TypeCode::Object);

		SpriteBatch::SpriteBatch(GraphicsDevice * const graphicsDevice)
		{
			this->graphicsDevice = graphicsDevice;
		}

		SpriteBatch::~SpriteBatch()
		{
			Dispose(false);
		}

		void SpriteBatch::Begin() 
		{ 
			Begin(SpriteSortMode::Deferred, BlendState::AlphaBlend); 
		}

		void SpriteBatch::Begin(SpriteSortMode_t sortMode, const BlendState& blendState)
		{

		}

		void SpriteBatch::Begin(SpriteSortMode_t sortMode, const BlendState& blendState, const SamplerState& samplerState, const DepthStencilState& depthStencilState, const RasterizerState& rasterizerState)
		{

		}

		void SpriteBatch::Begin(SpriteSortMode_t sortMode, const BlendState& blendState, const SamplerState& samplerState, const DepthStencilState& depthStencilState, const RasterizerState& rasterizerState, Effect* effect)
		{

		}

		void SpriteBatch::Begin(SpriteSortMode_t sortMode, const BlendState& blendState, const SamplerState& samplerState, const DepthStencilState& depthStencilState, const RasterizerState& rasterizerState, Effect* effect, Matrix transformMatrix)
		{
			sassert(!inBeginEndPair, "Begin cannot be called again until End has been successfully called.");

			spriteSortMode = sortMode;

			if (sortMode == SpriteSortMode::Immediate)
			{
				applyGraphicsDeviceSettings();
			}

			inBeginEndPair = true;
		}

		void SpriteBatch::Dispose(bool disposing)
		{
			if (disposing && !IsDisposed())
			{
				// TODO: dispose of resources
			}

			GraphicsResource::Dispose(disposing);
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Rectangle destinationRectangle, const Color color)
		{
			Draw(texture, destinationRectangle, NULL, color, 0.0f, Vector2::Zero, SpriteEffects::None, 0.0f);
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Vector2 position, const Color color)
		{
			Rectangle destination = Rectangle((int)position.X, (int)position.Y, texture->Width, texture->Height);
			Draw(texture, destination, NULL, color, 0.0f, Vector2::Zero, SpriteEffects::None, 0.0f);
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Rectangle destinationRectangle, const Nullable<Rectangle> sourceRectangle, const Color color)
		{
			Draw(texture, destinationRectangle, sourceRectangle, color, 0.0f, Vector2::Zero, SpriteEffects::None, 0.0f);
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Vector2 position, const Nullable<Rectangle> sourceRectangle, const Color color)
		{
			Rectangle destination = Rectangle((int)position.X, (int)position.Y, texture->Width, texture->Height);
			Draw(texture, destination, sourceRectangle, color, 0.0f, Vector2::Zero, SpriteEffects::None, 0.0f); 	
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Rectangle destinationRectangle, const Nullable<Rectangle> sourceRectangle, const Color color, const float rotation, const Vector2 origin, const SpriteEffects_t effects, const float layerDepth)
		{
			Sprite sprite = Sprite(texture, 
				sourceRectangle.HasValue() ? sourceRectangle.getValue() : Rectangle(0, 0, texture->Width, texture->Height), 
				destinationRectangle, 
				color, 
				rotation, 
				origin, 
				effects, 
				layerDepth);

			SpriteList.Add(sprite);

			if (spriteSortMode == SpriteSortMode::Immediate)
			{
				Flush(); 
			}
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Vector2 position, const Nullable<Rectangle> sourceRectangle, const Color color, const float rotation, const Vector2 origin, const Vector2 scale, const SpriteEffects_t effects, const float layerDepth)
		{ 
			int width;
			int height;

			if (sourceRectangle.HasValue())
			{
				width = (int)(sourceRectangle.getValue().Width * scale.X);
				height = (int)(sourceRectangle.getValue().Height * scale.Y);
			}
			else
			{
				width = (int)(texture->Width * scale.X);
				height = (int)(texture->Height * scale.Y);
			}

			Rectangle destination = Rectangle((int)position.X, (int)position.Y, width, height);
			Draw(texture, destination, sourceRectangle, color, rotation, origin, effects, layerDepth); 
		}

		void SpriteBatch::Draw(Texture2D * const texture, const Vector2 position, const Nullable<Rectangle> sourceRectangle, const Color color, const float rotation, const Vector2 origin, const float scale, const SpriteEffects_t effects, const float layerDepth)
		{
			int width;
			int height;

			if (sourceRectangle.HasValue())
			{
				width = (int)(sourceRectangle.getValue().Width * scale);
				height = (int)(sourceRectangle.getValue().Height * scale);
			}
			else
			{
				width = (int)(texture->Width * scale);
				height = (int)(texture->Height * scale);
			}

			Rectangle destination = Rectangle((int)position.X, (int)position.Y, width, height);
			Draw(texture, destination, sourceRectangle, color, rotation, origin, effects, layerDepth);
		}

		void SpriteBatch::DrawString(SpriteFont * const spriteFont, String& text, const Vector2 position, const Color color)
		{
			spriteFont->Draw(text, this, position, color, 0.0f, Vector2::Zero, Vector2::One, SpriteEffects::None, 0.0f); 
		}

		void SpriteBatch::DrawString(SpriteFont * const spriteFont, String& text, const Vector2 position, const Color color, const float rotation, const Vector2 origin, const Vector2 scale, const SpriteEffects_t effects, const float layerDepth)
		{
			spriteFont->Draw(text, this, position, color, rotation, origin, scale, effects, layerDepth);
		}

		void SpriteBatch::DrawString(SpriteFont * const spriteFont, String& text, const Vector2 position, const Color color, const float rotation, const Vector2 origin, const float scale, const SpriteEffects_t effects, const float layerDepth)
		{
			Vector2 vector = Vector2::Zero;
			vector.X = scale; 
			vector.Y = scale; 
			spriteFont->Draw(text, this, position, color, rotation, origin, vector, effects, layerDepth); 
		}

		void SpriteBatch::End() 
		{
			sassert(inBeginEndPair, "Begin must be called successfully before End can be called.");
			
			if (spriteSortMode != SpriteSortMode::Immediate)
			{
				applyGraphicsDeviceSettings();
				Flush();
			}
			
			/*glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();*/

			restoreRenderState(); 

			inBeginEndPair = false;
		}

		const Type& SpriteBatch::GetType()
		{
			return SpriteBatchTypeInfo;
		}

		void SpriteBatch::restoreRenderState()
		{
			
		}
	
		void SpriteBatch::applyGraphicsDeviceSettings() 
		{
			DWORD* p;

			p = pb_begin();

			

			pb_end(p);

            // Set the blend mode 
     //       switch (spriteBlendMode) 
     //       { 
     //           case SpriteBlendMode::AlphaBlend: 
     //               p = pb_begin();
					////Enable blending
					//pb_push(p, NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_ENABLE, 1); p+=2;
					////set blendmode
					//pb_push2(p, NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_SRC, (0x302<<16) | 0x302, (0x303<<16) | 0x303); p+=3;
					////send data to GPU
					//pb_end(p);
     //               break; 
     //           case SpriteBlendMode::Additive:
					//p = pb_begin();
					////Enable blending
					//pb_push(p, NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_ENABLE, 1); p+=2;
					////set blendmode
					//pb_push2(p, NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_SRC, (0x302<<16) | 0x302, (1<<16) | 1); p+=3;
					////send data to GPU
					//pb_end(p);
     //               break; 
     //           case SpriteBlendMode::None: 
     //               p = pb_begin();
					////Disable Blending
					//pb_push(p, NV20_TCL_PRIMITIVE_3D_BLEND_FUNC_ENABLE, 0); p+=2;
					////send data to GPU
					//pb_end(p);
     //               break; 
     //           default: 
     //               return; 
     //       } 
                          
            //glEnable(GL_TEXTURE_2D); 
  
            // Reset the projection matrix and use the orthographic matrix 
            /*int viewPort[4]; 
            glGetIntegerv(GL_VIEWPORT, viewPort); 
            glMatrixMode(GL_PROJECTION); 
            glPushMatrix();
            glLoadIdentity();
			glOrtho(0, device.Viewport_().Width, device.Viewport_().Height, 0, -1, 1); // viewPort[2] = width, viewPort[3] = height 
            glMatrixMode(GL_MODELVIEW); 
            glPushMatrix(); 
            glLoadIdentity();*/
        }

		void SpriteBatch::Flush()
        {
			DWORD* p;

			switch (spriteSortMode)
			{
			case SpriteSortMode::BackToFront:
				//SpriteList.Sort(BackToFrontSpriteComparer<Sprite>());
				break;
			case SpriteSortMode::FrontToBack:
				//SpriteList.Sort(FrontToBackSpriteComparer<Sprite>());
				break;
			case SpriteSortMode::Texture: // nothing here?
				break;
			}
			for (int i = 0; i < SpriteList.Count(); i++)
			{
				// Set the color, bind the texture for drawing and prepare the texture source
				if (SpriteList[i].getColor().A() <= 0) continue;
				//glColor4f((float)sprite.Color_().R() / 255f, (float)sprite.getColor().G() / 255f, (float)sprite.getColor().B() / 255f, (float)sprite.getColor().A() / 255f);
				//glBindTexture(GL_TEXTURE_2D, sprite.getTexture().textureId);
				// Setup the matrix
				//if ((SpriteList[i].DestinationRectangle().X != 0) || (SpriteList[i].DestinationRectangle().Y != 0))
				//	glTranslatef(SpriteList[i].DestinationRectangle().X, SpriteList[i].DestinationRectangle().Y, 0.0f); // Position

				//if (SpriteList[i].Rotation() != 0)
				//	glRotatef(MathHelper::ToDegrees(SpriteList[i].Rotation()), 0.0f, 0.0f, 1.0f); // Rotation

				/*if ((SpriteList[i].DestinationRectangle().Width != 0 && SpriteList[i].Origin().X != 0) || (SpriteList[i].DestinationRectangle().Height != 0 && SpriteList[i].Origin().Y != 0))
					glTranslatef(
					// Orientation
					-SpriteList[i].Origin.X * (float)SpriteList[i].DestinationRectangle().Width / (float)SpriteList[i].SourceRectangle().Width,
					-SpriteList[i].Origin.Y * (float)SpriteList[i].DestinationRectangle().Height / (float)SpriteList[i].SourceRectangle().Height, 0.0f);*/
				// Calculate the points on the texture
				float x = (float)SpriteList[i].SourceRectangle().X / (float)SpriteList[i].getTexture()->Width;
				float y = (float)SpriteList[i].SourceRectangle().Y / (float)SpriteList[i].getTexture()->Height;
				float twidth = (float)SpriteList[i].SourceRectangle().Width / (float)SpriteList[i].getTexture()->Width;
				float theight = (float)SpriteList[i].SourceRectangle().Height / (float)SpriteList[i].getTexture()->Height;

				p = pb_begin();
				pb_push1(p, NV20_TCL_PRIMITIVE_3D_BEGIN_END, QUADS); p+=2;
				pb_push(p++, (0x40000000 | NV20_TCL_PRIMITIVE_3D_VERTEX_DATA), 16);

				*((float *)(p++)) = 0.0f;
				*((float *)(p++)) = 0.0f;
				*((float *)(p++)) = x;
				*((float *)(p++)) = y;

				*((float *)(p++)) = SpriteList[i].DestinationRectangle().Width;
				*((float *)(p++)) = 0.0f;
				*((float *)(p++)) = x + twidth;
				*((float *)(p++)) = y;

				*((float *)(p++)) = SpriteList[i].DestinationRectangle().Width;
				*((float *)(p++)) = SpriteList[i].DestinationRectangle().Height;
				*((float *)(p++)) = x + twidth;
				*((float *)(p++)) = y + theight;

				*((float *)(p++)) = 0.0f;
				*((float *)(p++)) = SpriteList[i].DestinationRectangle().Height;
				*((float *)(p++)) = x;
				*((float *)(p++)) = y + theight;

				pb_push(p++, NV20_TCL_PRIMITIVE_3D_BEGIN_END, 1);
				*(p++) = STOP;

				pb_end(p);

				// Draw
				/*glBegin(GL_QUADS);
				{
					glTexCoord2f(x,y + theight);
					glVertex2f(0f, SpriteList[i].DestinationRectangle().Height);
					glTexCoord2f(x + twidth, y + theight);
					glVertex2f(SpriteList[i].DestinationRectangle().Width, SpriteList[i].DestinationRectangle().Height);
					glTexCoord2f(x + twidth,y);
					glVertex2f(SpriteList[i].DestinationRectangle().Width, 0f);
					glTexCoord2f(x,y);
					glVertex2f(0f, 0f);
				}
				glEnd();
				glPopMatrix(); // Finish with the matrix*/
			}
			SpriteList.Clear();  
        }
	}
}
