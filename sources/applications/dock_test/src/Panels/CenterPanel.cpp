//=============================================================================
#include "CenterPanel.h"
//=============================================================================
CenterPanel::CenterPanel ()
{
}
//=============================================================================
CenterPanel::~CenterPanel ()
{
}
//=============================================================================
void CenterPanel::resized ()
{
}
//=============================================================================
void CenterPanel::paint (juce::Graphics &g)
{
	g.fillAll (juce::Colour(0xff474747));
	g.setColour (juce::Colours::white);
	g.drawText ("MANAGED CENTER PANEL", 10, 10, getWidth()-10, 20, juce::Justification::centredLeft, false);
	g.drawArrow (juce::Line<float>(0, getHeight()/2, getWidth(), getHeight()/2), 1.0f, 12.f, 12.f); 
	g.drawArrow (juce::Line<float>(getWidth(), getHeight()/2, 0, getHeight()/2), 1.0f, 12.f, 12.f); 
	g.drawArrow (juce::Line<float>(getWidth()/2, 0, getWidth()/2, getHeight()), 1.0f, 12.f, 12.f); 
	g.drawArrow (juce::Line<float>(getWidth()/2, getHeight(), getWidth()/2, 0), 1.0f, 12.f, 12.f);
	juce::String currentWidth = juce::String("width=") + juce::String(getWidth()) + juce::String("px");
	g.drawText (currentWidth, 20, (getHeight()/2)-20, getWidth()-20, 20, juce::Justification::centredLeft, false);
	juce::String currentHeight = juce::String("height=") + juce::String(getHeight()) + juce::String("px");
	g.drawText (currentHeight, (getWidth()/2) + 10, 15, (getWidth()/2)-10, 20, juce::Justification::centredLeft, false);
}
//=============================================================================
