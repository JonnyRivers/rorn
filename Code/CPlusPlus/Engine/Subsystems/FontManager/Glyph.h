#pragma once

namespace Rorn
{
	namespace Engine
	{
		class Glyph
		{
		public:
			Glyph(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float startU, float startV, float endU, float endV);

			unsigned int GetX() { return x_; }
			unsigned int GetY() { return y_; }
			unsigned int GetWidth() { return width_; }
			unsigned int GetHeight() { return height_; }
			float GetStartU() { return startU_; }
			float GetStartV() { return startV_; }
			float GetEndU() { return endU_; }
			float GetEndV() { return endV_; }
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