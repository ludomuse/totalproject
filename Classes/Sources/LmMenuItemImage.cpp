/*
 * LmMenuItemImage.cpp
 *
 *  Created on: 19 nov. 2015
 *      Author: IHMTEKDev4
 */

#include "../Include/LmMenuItemImage.h"

using namespace cocos2d;


LmMenuItemImage::LmMenuItemImage(std::string l_sTextButton)
{
	//primitive type

	//pointer
	m_sTextButton = l_sTextButton;
}

LmMenuItemImage::~LmMenuItemImage()
{

}

LmMenuItemImage* LmMenuItemImage::create(const std::string &l_sTextButton,
		const ccMenuCallback& callback)
{
	LmMenuItemImage* l_pButton = new LmMenuItemImage(l_sTextButton);

	const std::string & normalImage = "Ludomuse/Content/backgroundDevice.png";
	const std::string & selectedImage =
			"Ludomuse/Content/backgroundDevicePressed.png";
	const std::string & disabledImage = "Ludomuse/Content/backgroundDevice.png";


	if (l_pButton->initWithNormalImage(normalImage, selectedImage,
			disabledImage, callback))
	{
		l_pButton->autorelease();

		Size l_oSizeButton = l_pButton->getContentSize();
		int l_iTextLenght = l_sTextButton.length();
		float l_fFontSize = (l_oSizeButton.width/l_iTextLenght)*1.8;

		if(l_fFontSize>l_oSizeButton.height*0.5)
		{
			l_fFontSize=l_oSizeButton.height*0.5;
		}


		//create a label
		auto l_pLabel = Label::createWithTTF(l_sTextButton,
				"Fonts/JosefinSans-Regular.ttf", l_fFontSize);
		l_pLabel->setPosition(l_oSizeButton.width * 0.5,
				l_oSizeButton.height * 0.5);
		l_pLabel->setAlignment(TextHAlignment::CENTER);
		l_pLabel->setMaxLineWidth(l_oSizeButton.width * 0.9);
		l_pButton->addChild(l_pLabel);



		return l_pButton;
	}

	CC_SAFE_DELETE(l_pButton);
	return NULL;
}


