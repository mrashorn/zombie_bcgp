#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder
{
	private:
		// A map container from the STL
		// that holds related pairs of String and Texture
		map<string, Texture> m_Textures;

		// A pointer of the same type as the class itself
		// the one and only instance
		static TextureHolder* m_s_Instance;

	public:
		TextureHolder();
		static Texture& GetTexture(string const& filename); // reference to a texture, 
															// without needing an instance of the class TextureHolder

		// Static = can be used without an instance of the class
		// const = cannot be changed inside the function

};

#endif

