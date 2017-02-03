
#ifndef __SCROLLBARLOOKANDFEEL_H__
#define __SCROLLBARLOOKANDFEEL_H__

#include "../includes.h"

class ScrollBarLookAndFeel : public juce::LookAndFeel_V1
{
	public:
		ScrollBarLookAndFeel ();
		~ScrollBarLookAndFeel ();
		
		virtual void drawScrollbarButton (juce::Graphics &g, juce::ScrollBar &scrollbar, int width, int height, int buttonDirection, bool isScrollbarVertical, bool isMouseOverButton, bool isButtonDown);
		virtual void drawScrollbar (juce::Graphics &g, juce::ScrollBar &scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown);
		virtual juce::ImageEffectFilter* getScrollbarEffect ();
		virtual int getMinimumScrollbarThumbSize (juce::ScrollBar &scrollbar);


		virtual int getDefaultScrollbarWidth ();



		virtual int getScrollbarButtonSize (juce::ScrollBar &scrollbar);
		//=====================================================================

};

#endif //__SCROLLBARLOOKANDFEEL_H__
