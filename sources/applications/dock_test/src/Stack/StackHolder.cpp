//=============================================================================
#include "StackHolder.h"
#include "StackPanel.h"
//==============================================================================
#define sigmoid(x) (1.0f / (1.0f + exp (-5.5f * 2 * ((x) - 0.5))))
#define sigmoidProgress(x) ((sigmoid (x) - sigmoid (0)) / \
			    (sigmoid (1) - sigmoid (0)))
//==============================================================================
#define interpolate(a,b,val) (((val) * a) + ((1 - (val)) * (b)))
//=============================================================================
StackHolder::StackHolder (StackManager* parent, StackPanel* p,
						  juce::ToolbarItemFactory* f,
						  const juce::String& n, bool isOpen)
	: manager (parent),
	  panel (p),
	  name (n),
	  factory (f),
	  stackIsOpen (isOpen),
	  mouseIsOver (false),
	  animStepDur (10),
	  animDur (500)
{
	addAndMakeVisible (toolbar = new juce::Toolbar ());
	toolbar->setColour (juce::Toolbar::backgroundColourId, juce::Colours::transparentBlack);
	if (factory)
		toolbar->addDefaultItems (*factory);
}
//=============================================================================
StackHolder::~StackHolder ()
{
	if (factory)
		delete factory;
}
//=============================================================================
const juce::Rectangle<int> StackHolder::getContentBounds ()
{
	juce::Rectangle<int> bounds (0, STACKHEADERHEIGHT, getWidth(), getHeight()-STACKHEADERHEIGHT);
	bounds.reduce (8, 6);
	return bounds;
}
//=============================================================================
void StackHolder::resized ()
{
	if (toolbar)
		toolbar->setBounds (manager->getScrollBarAtLeft() ? juce::roundFloatToInt(getWidth()*0.35f) : 0, 
							0, 
							juce::roundFloatToInt(getWidth()*0.65f), 
							STACKHEADERHEIGHT);
}
//=============================================================================
void StackHolder::paint (juce::Graphics &g)
{
	const bool atLeft = manager->getScrollBarAtLeft ();

	g.fillAll (juce::Colour (0xff292929));

	int x, y, w, h;
	x = 0;
	w = getWidth();
	h = getHeight();
	y = (STACKHEADERHEIGHT - 14) / 2;

	const int tw = juce::roundFloatToInt(g.getCurrentFont().getStringWidthFloat(name) * 2.f);
	g.setColour (mouseIsOver ? juce::Colours::white : juce::Colour (194,194,194));
	if (atLeft)
		g.drawText (name, x+20, 0, tw, STACKHEADERHEIGHT, juce::Justification::centredLeft, false);
	else
		g.drawText (name, w-20-tw, 0, tw, STACKHEADERHEIGHT, juce::Justification::centredRight, false);

	const float ss = 8.f;
	      float xx = x + (ss / 2.f) + 2.f;
	const float yy = y + (ss / 2.f);

	if (!atLeft)
	{
		xx = w - 8.f - ss;
	}

	juce::Path op;
	if (stackIsOpen)
	{
		op.addTriangle (xx,  yy,
						xx+ss, yy,
						xx+(ss/2), yy+ss);
	}
	else
	{
		if (atLeft)
			op.addTriangle (xx,  yy,
							xx+ss, yy+(ss/2),
							xx, yy+ss);
		else
			op.addTriangle (xx+ss,  yy,
							xx, yy+(ss/2),
							xx+ss, yy+ss);
	}

	if (mouseIsOver)
		g.setColour (juce::Colours::white);
	else
		g.setColour (juce::Colour(170,170,170));

	g.fillPath (op);

	float xxx, yyy, www, hhh;
	xxx = 4.f;
	yyy = STACKHEADERHEIGHT + 2.f;
	www = getWidth () - 8.f;
	hhh = getHeight () - STACKHEADERHEIGHT - 4.f;

	g.setColour (juce::Colours::black);
	g.drawHorizontalLine	(juce::roundFloatToInt(yyy),			xxx,		xxx+www-1.f);
	g.drawVerticalLine		(juce::roundFloatToInt(xxx),			yyy,		yyy+hhh-1.f);
	g.drawVerticalLine		(juce::roundFloatToInt(xxx+www-2),	yyy,		yyy+hhh-1.f);
	g.drawHorizontalLine	(juce::roundFloatToInt(yyy+hhh-2),	xxx,		xxx+www-2.f);

	g.setColour (juce::Colour (0xff3a3a3a));
	g.drawVerticalLine		(juce::roundFloatToInt(xxx+www-1),	yyy,		yyy+hhh-1.f);
	g.drawHorizontalLine	(juce::roundFloatToInt(yyy+hhh-1),	xxx,		xxx+www-1.f);
	g.drawHorizontalLine	(juce::roundFloatToInt(yyy+1),		xxx+1.f,	xxx+www-2.f);
	g.drawVerticalLine		(juce::roundFloatToInt(xxx+1),		yyy+1.f,	yyy+hhh-2.f);
}
//=============================================================================
void StackHolder::mouseDown (const juce::MouseEvent& e)
{
	panel->content->mouseDown (e);

	if (e.getEventRelativeTo (this).y > STACKHEADERHEIGHT)
		return;

	setOpen (!isOpen());
	repaint ();

	int change = panel->getPreferredHeight();
	animStepSize = juce::roundFloatToInt (change / ((float) animDur / animStepDur));
	animStartTime = juce::Time::getCurrentTime ();
	startTimer (animStepDur);
}
//=============================================================================
void StackHolder::mouseEnter (const juce::MouseEvent& e)
{
	panel->content->mouseEnter (e);
	mouseIsOver = true;
	repaint ();
}
//=============================================================================
void StackHolder::mouseExit (const juce::MouseEvent& e)
{
	panel->content->mouseExit (e);
	mouseIsOver = false;
	repaint ();
}
//=============================================================================
void StackHolder::timerCallback ()
{
	int target = STACKHEADERHEIGHT + 12;
	if (stackIsOpen) 
		target += panel->getPreferredHeight();

	juce::RelativeTime stepTime = juce::Time::getCurrentTime () - animStartTime;
	int elapsed = juce::jlimit (0, animDur, (int)stepTime.inMilliseconds ());

	double progress = sigmoidProgress((double)elapsed / animDur);

	int h = STACKHEADERHEIGHT + 12; 

	if (stackIsOpen)
	{
		h += juce::roundDoubleToInt (progress * panel->getPreferredHeight());
	}
	else
	{
		h += juce::roundDoubleToInt ((1-progress) * panel->getPreferredHeight());
	}

	setSize (getWidth(), h);
	manager->resized ();

	if (elapsed >= animDur) 
		stopTimer ();
}
//=============================================================================