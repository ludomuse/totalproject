/*
 * LmMenuItemImage.h
 *
 *  Created on: 19 nov. 2015
 *      Author: IHMTEKDev4
 */

#ifndef CLASSES_INCLUDE_LMMENUITEMIMAGE_H_
#define CLASSES_INCLUDE_LMMENUITEMIMAGE_H_

#include "cocos2d.h"

class LmMenuItemImage: public cocos2d::MenuItemImage
{

	public:
		~LmMenuItemImage();

		//static method that return a LmMenuItemImage param=the label of the button
		static LmMenuItemImage* create(const std::string &,
				const cocos2d::ccMenuCallback&);

		const std::string& getSTextButton() const
		{
			return m_sTextButton;
		}

		static const float s_fMaxFontSize;

	private:

		LmMenuItemImage(std::string);

		std::string m_sTextButton;

};

#endif /* CLASSES_INCLUDE_LMMENUITEMIMAGE_H_ */
