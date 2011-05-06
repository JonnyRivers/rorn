#pragma once

namespace Rorn
{
	namespace Engine
	{
		class Glyph
		{
		public:
			Glyph(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float startU, float startV, float endU, float endV);

			unsigned int GetX() const { return x_; }
			unsigned int GetY() const { return y_; }
			unsigned int GetWidth() const { return width_; }
			unsigned int GetHeight() const { return height_; }
			float GetStartU() const { return startU_; }
			float GetStartV() const { return startV_; }
			float GetEndU() const { return endU_; }
			float GetEndV() const { return endV_; }
		private:
			unsigned int x_;
			unsigned int y_;
			unsigned int width_;
			unsigned int height_;
			float startU_;
			float startV_;
			float endU_;
			float endV_;
		};
	}
}