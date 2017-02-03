
#include "DockPanel.h"
#include "DockButtons.h"

DockPanel::DockPanel (DockManager *parent, juce::Component* component, const int position)
	: DockHolder (parent, this, position),
	  content (component)
{
	content->addMouseListener (this, true);
	addAndMakeVisible (content);
}

DockPanel::~DockPanel ()
{
	deleteAllChildren ();
}

void DockPanel::resized ()
{
	content->setBounds (DockHolder::getContentBounds());
	DockHolder::resized ();
}

void DockPanel::paint (juce::Graphics &g)
{
	DockHolder::paint (g);
}

