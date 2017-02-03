//=============================================================================
#include "RightPanel.h"
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
RightPanel::RightPanel ()
	: firstCall (true)
{
	addAndMakeVisible (manager = new StackManager(false));
}
//=============================================================================
RightPanel::~RightPanel ()
{
	deleteAllChildren ();
}
//=============================================================================
void RightPanel::resized ()
{
	manager->setBounds (0, 0, getWidth(), getHeight());
	if (firstCall)
	{
		firstCall = false;
		manager->setFixedComponent (new ZeroComponent("Fixed Panel"), 0, false, true, 200);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #1"), 0, true, 150);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #2"), 0, true, 50);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #3"), 0, true, 100);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #4"), 0, true, 100);
		manager->setStackedComponent (new ZeroComponent("Stacked Panel #5"), 0, true, 50);
	}
}
//=============================================================================
void RightPanel::paint (juce::Graphics &g)
{
	g.fillAll (juce::Colour(0xff292929));
}
//=============================================================================
