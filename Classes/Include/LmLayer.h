/*************************************************************************
 LmLayer  - object that contain a sound an img
 and a texte to make intro of each InteractionScene
 *************************************************************************/
#ifndef LMLAYER_H
#define LMLAYER_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

struct LmLayerSeed
{
	std::vector<std::pair<std::string, int>> Images;
	std::string SoundURL;
	std::string Text;
	cocos2d::Color3B ColorText;
	float FontSize;
	float WidthPercent;
	float HeightPercent;
	float SizePercent;

};

class LmLayer: public cocos2d::Layer
{
public:

	/*
	 * 3 parameters
	 * images URL/Anchor
	 * Sound URL
	 * Text
	 */
	LmLayer(const LmLayerSeed &);
	~LmLayer();

	bool init();
	void playSound();
	void pauseSound();

	const std::string& getSSoundUrl() const
	{
		return m_sSoundURL;
	}

private:

	std::vector<std::pair<std::string, int>> m_aImages;
	std::string m_sSoundURL;
	std::string m_sText;
	cocos2d::Color3B m_oColorText;
	float m_fFontSize;
	float m_fWidthPercent;
	float m_fHeightPercent;
	float m_fSizePercent;
};

#endif // LMLAYER_H
