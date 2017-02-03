//=============================================================================
#include "StackManager.h"
//=============================================================================
StackManager::StackManager (const bool sbal)
	: scrollBarAtLeft (sbal),
	  isFixedOnTop (true),
	  fixed (0),
	  currentRangeStart (0.0)
{
	setLookAndFeel (&laf);
	addAndMakeVisible (scrollbar = new SmoothScrollBar (true, false));
	scrollbar->addListener (this);
	addAndMakeVisible (viewport = new Component());
}
//=============================================================================
StackManager::~StackManager ()
{
	deleteAllChildren ();
}
//=============================================================================
void StackManager::scrollBarMoved (juce::ScrollBar *sb, const double RangeStart)
{
	currentRangeStart = RangeStart;
	resized_child ();
}
//=============================================================================
void StackManager::resized_child ()
{
	int tmp = 0;
	for (int i=0; i < viewport->getNumChildComponents(); ++i)
		tmp += viewport->getChildComponent(i)->getHeight();

	int y = juce::roundDoubleToInt(-currentRangeStart);
	for (int i=0; i < viewport->getNumChildComponents(); ++i)
	{
		viewport->getChildComponent(i)->setTopLeftPosition (0, y);
		viewport->getChildComponent(i)->setSize (viewport->getWidth(), viewport->getChildComponent(i)->getHeight());
		y += viewport->getChildComponent(i)->getHeight();
	}

	if (tmp > 0)
	{
		static_cast<juce::ScrollBar*>(scrollbar)->setRangeLimits (0.0, tmp);
		static_cast<juce::ScrollBar*>(scrollbar)->setCurrentRange (currentRangeStart, viewport->getHeight());
		static_cast<juce::ScrollBar*>(scrollbar)->setSingleStepSize (1.0);
	}

	repaint();
}
//=============================================================================
void StackManager::resized ()
{
	int barWidth = 15;
	int x1, y1, w1, h1;
	int x2, y2, w2, h2;
	int x3, y3, w3, h3;

	if (scrollBarAtLeft)
	{
		x1 = 0;
		x2 = barWidth;
		x3 = x2;
	}
	else
	{
		x1 = (getWidth() - barWidth) + 1;
		x2 = 0;
		x3 = x2;
	}
	w1 = barWidth - 1;
	w2 = getWidth() - barWidth;
	w3 = w2;

	if (fixed)
	{
		if (isFixedOnTop)
		{
			y1 = fixed->getHeight();
			y2 = fixed->getHeight();
			y3 = 0;
		}
		else
		{
			y1 = 0;
			y2 = 0;
			y3 = getHeight() - fixed->getHeight();
		}
		h1 = getHeight() - fixed->getHeight();
		h2 = getHeight() - fixed->getHeight();
		h3 = fixed->getHeight();
	}
	else
	{
		y1 = 0;
		y2 = 0;
		y3 = 0;
		h1 = getHeight();
		h2 = getHeight();
		h3 = 0;
	}

	if (fixed)
		fixed->setBounds (x3, y3, w3, h3);

	if (scrollbar)
		scrollbar->setBounds (x1, y1, w1, h1);

	if (viewport)
		viewport->setBounds (x2, y2, w2, h2);

	resized_child ();
}
//=============================================================================
void StackManager::paint (juce::Graphics &g)
{
	g.fillAll (juce::Colour(0xff292929));
	
	float barWidth = 15.f;
	float x, y, w, h;

	g.setColour (juce::Colours::black);
	g.fillRect (scrollBarAtLeft ? 0.f : (float)(getWidth() - barWidth), 0.f, barWidth, (float)getHeight());

	if (fixed)
	{
		if (scrollBarAtLeft)
			x = 1.f;
		else
			x = (getWidth() - barWidth) + 3.f;

		if (isFixedOnTop)
			y = 0.f;
		else
			y = (getHeight() - (float)fixed->getHeight()) + 1.f;

		w = barWidth;
		h = (float)fixed->getHeight()-1.f;

		g.setColour (juce::Colour(0xff333333));
		g.drawVerticalLine (juce::roundFloatToInt(x), y, y+h);
		g.drawVerticalLine (juce::roundFloatToInt(x+2), y, y+h);
		g.drawVerticalLine (juce::roundFloatToInt(x+4), y, y+h);
		g.drawVerticalLine (juce::roundFloatToInt(x+6), y, y+h);
		g.drawVerticalLine (juce::roundFloatToInt(x+8), y, y+h);
		g.drawVerticalLine (juce::roundFloatToInt(x+10), y, y+h);
		g.drawHorizontalLine (juce::roundFloatToInt(y), x, x+10);
		g.drawHorizontalLine (juce::roundFloatToInt(y+h-1), x, x+10);
	}
}
//=============================================================================
void StackManager::setFixedComponent (juce::Component *component, juce::ToolbarItemFactory *factory, bool onTop, bool isOpen, const int height)
{
	isFixedOnTop = onTop;
	if (fixed)
		delete fixed;
	addAndMakeVisible (fixed = new StackPanel (this, component, factory, isOpen, height));
	fixed->setSize (viewport->getWidth(), height+STACKHEADERHEIGHT);
	resized ();
}
//=============================================================================
void StackManager::setStackedComponent (juce::Component *component, juce::ToolbarItemFactory *factory, bool isOpen, const int height)
{
	StackPanel *panel = new StackPanel (this, component, factory, isOpen, height);
	stacks.add (panel);
	resized ();
	int currentY = 0;
	if (viewport->getNumChildComponents() > 0)
		currentY = viewport->getChildComponent(viewport->getNumChildComponents()-1)->getBottom();
	viewport->addAndMakeVisible (panel);
	panel->setBounds (0, currentY, viewport->getWidth(), height+STACKHEADERHEIGHT);
}
//=============================================================================
