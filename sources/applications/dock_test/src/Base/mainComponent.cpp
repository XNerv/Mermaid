//=============================================================================
#include "mainComponent.h"
#include "mainWindow.h"
//==============================================================================
#include "../Panels/HeaderPanel.h"
#include "../Panels/BottomPanel.h"
#include "../Panels/LeftPanel.h"
#include "../Panels/RightPanel.h"
#include "../Panels/CenterPanel.h"
//==============================================================================
MainComponent::MainComponent (MainWindow *win)
	: mainWindow (win)
{
	addAndMakeVisible (manager = new DockManager ());
	manager->setPanelComponent (CENTER, new CenterPanel);
	manager->setPanelComponent (TOP, new HeaderPanel);
	manager->setPanelComponent (BOTTOM, new BottomPanel);
	manager->setPanelComponent (LEFT, new LeftPanel);
	manager->setPanelComponent (RIGHT, new RightPanel);
}
//==============================================================================
MainComponent::~MainComponent ()
{
	deleteAllChildren ();
}
//==============================================================================
void MainComponent::resized ()
{
	manager->setBounds (0, 0, getWidth(), getHeight());
}
//==============================================================================