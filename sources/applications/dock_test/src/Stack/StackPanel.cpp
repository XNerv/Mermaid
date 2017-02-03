//=============================================================================
#include "StackPanel.h"
//=============================================================================
StackPanel::StackPanel (StackManager *parent, 
						juce::Component* component, 
						juce::ToolbarItemFactory* factory, 
						bool isOpen, const int h)
	: StackHolder (parent, this, factory, component->getName(), isOpen),
	  content (component),
	  height (h)
{
	content->addMouseListener (this, true);
	addAndMakeVisible (content);
}
//=============================================================================
StackPanel::~StackPanel ()
{
	deleteAllChildren ();
}
//=============================================================================
void StackPanel::resized ()
{
	content->setBounds (StackHolder::getContentBounds());
	StackHolder::resized ();
}
//=============================================================================
void StackPanel::paint (juce::Graphics &g)
{
	StackHolder::paint (g);
}
//=============================================================================
