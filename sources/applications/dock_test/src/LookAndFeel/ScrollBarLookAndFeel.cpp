
#include "ScrollBarLookAndFeel.h"

ScrollBarLookAndFeel::ScrollBarLookAndFeel ()
{
}

ScrollBarLookAndFeel::~ScrollBarLookAndFeel ()
{
}

void ScrollBarLookAndFeel::drawScrollbarButton (juce::Graphics& g,
												juce::ScrollBar& scrollbar,
												int width, int height,
												int buttonDirection,
												bool /*isScrollbarVertical*/,
												bool isMouseOverButton,
												bool isButtonDown)
{
	juce::ColourGradient gradient (juce::Colour(66,66,66), 0.0f, 0.0f,
							 juce::Colour(76,76,76), (float)width, 0.0f,
							 false);
 	juce::FillType fill (gradient);
	g.setFillType (fill);
	g.fillAll ();

    juce::Path p;
	if (buttonDirection == 0) // UP
		p.addTriangle (0.5f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f);
	else
	if (buttonDirection == 1) // RIGHT
		p.addTriangle (0.0f, 0.0f, 0.75f, 0.5f, 0.0f, 1.0f);
	else
	if (buttonDirection == 2) // DOWN
		p.addTriangle (0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.75f);
	else
	if (buttonDirection == 3) // LEFT
		p.addTriangle (1.0f, 0.0f, 1.0f, 1.0f, 0.25f, 0.5f);

	if (isButtonDown)
		g.setColour (juce::Colour(26,26,26));
	else
	if (isMouseOverButton)
		g.setColour (juce::Colour(204,204,204));
	else
		g.setColour (juce::Colour(128,128,128));

	p.applyTransform (p.getTransformToScaleToFit ((float)width/4, (float)height/4, 
												  (float)width/2, (float)height/2, 
												  true, juce::Justification::centred));

	if (buttonDirection == 0 || buttonDirection == 2)
	{
		if (isButtonDown)
			g.fillPath (p, juce::AffineTransform::identity.translation (1.5f, 1.0f));
		else
			g.fillPath (p, juce::AffineTransform::identity.translation (0.5f, 0.0f));
	}
	else
	{
		if (isButtonDown)
			g.fillPath (p, juce::AffineTransform::identity.translation (1.0f, 1.5f));
		else
			g.fillPath (p, juce::AffineTransform::identity.translation (0.0f, 0.5f));
	}
}

void ScrollBarLookAndFeel::drawScrollbar (juce::Graphics& g,
										  juce::ScrollBar& scrollbar,
										  int x, int y,
										  int width, int height,
										  bool isScrollbarVertical,
										  int thumbStartPosition,
										  int thumbSize,
										  bool isMouseOver,
										  bool isMouseDown)
{
	/*
	ColourGradient gradient (Colour(66,66,66), 0.0f, 0.0f,
							 Colour(76,76,76), (float)x+width, (float)y,
							 false);
 	FillType fill (gradient);
	g.setFillType (fill);
	g.fillAll ();

	if (isMouseDown)
		g.setColour (Colour(66,66,66));
	else
		*/
	if (isMouseOver || isMouseDown)
		g.setColour (juce::Colour(204,204,204));
	else
		g.setColour (juce::Colour(128,128,128));

	if (isScrollbarVertical)
	{
		int w = width / 2;
		g.fillRoundedRectangle ((float)x + 1.0f, (float)thumbStartPosition,
							    (float)w * 1.7f, (float)thumbSize, (float)w/2);
	}
	else
	{
		int h = height / 2;
		g.fillRoundedRectangle ((float)thumbStartPosition, (float)y + 1.0f,
								(float)thumbSize, (float)h * 1.7f, (float)h/2);
	}
}

juce::ImageEffectFilter* ScrollBarLookAndFeel::getScrollbarEffect()
{
    return 0;
}

int ScrollBarLookAndFeel::getMinimumScrollbarThumbSize (juce::ScrollBar& scrollbar)
{
    return juce::jmin (scrollbar.getWidth(), scrollbar.getHeight());
}

int ScrollBarLookAndFeel::getDefaultScrollbarWidth()
{
    return 15;
}

int ScrollBarLookAndFeel::getScrollbarButtonSize (juce::ScrollBar& scrollbar)
{
    return 2 + (scrollbar.isVertical() ? scrollbar.getWidth()
                                       : scrollbar.getHeight());
}
