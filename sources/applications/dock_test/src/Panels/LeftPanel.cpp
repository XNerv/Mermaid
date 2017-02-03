//=============================================================================
#include "LeftPanel.h"
//=============================================================================
class ZeroComponent : public juce::Component
{
	public:
		ZeroComponent (const juce::String& name)
			: juce::Component (name)
		{}
		void paint (juce::Graphics &g)
		{
			//g.fillAll (Colours::red);
		}
};
//=============================================================================
LeftPanel::LeftPanel ()
	: firstCall (true)
{
	addAndMakeVisible (manager = new StackManager(true));
}
//=============================================================================
LeftPanel::~LeftPanel ()
{
	deleteAllChildren ();
}
//=============================================================================
void LeftPanel::resized ()
{
	manager->setBounds (0, 0, getWidth(), getHeight());
	if (firstCall)
	{
		firstCall = false;
		manager->setFixedComponent (new ZeroComponent("Fixed Panel"), 0, true, true, 200);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #1"), 0, true, 150);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #2"), 0, true, 50);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #3"), 0, true, 100);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #4"), 0, true, 100);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #5"), 0, true, 250);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #6"), 0, true, 500);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #7"), 0, true, 100);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #8"), 0, true, 250);
	}
}
//=============================================================================
void LeftPanel::paint (juce::Graphics &g)
{
	g.fillAll (juce::Colour(0xff292929));
}
//=============================================================================
