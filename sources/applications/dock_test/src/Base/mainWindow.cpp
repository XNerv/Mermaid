#include "mainWindow.h"
#include "mainComponent.h"

MainWindow::MainWindow () : juce::DocumentWindow("Adobe-Like Dock", juce::Colours::darkgrey, juce::DocumentWindow::allButtons, true)
{
    MainComponent* const contentComponent = new MainComponent (this);
	setContentOwned (contentComponent, false);
	
	juce::DocumentWindow::setResizable (true, false);
	juce::DocumentWindow::setResizeLimits (800, 600, 8192, 8192);
	juce::DocumentWindow::setFullScreen (true);
	juce::DocumentWindow::setVisible (true);
	
    // don't want the window to take focus when the title-bar is clicked..
    setWantsKeyboardFocus (true);
}

MainWindow::~MainWindow ()
{
	setContentOwned (0, false);
}

void MainWindow::closeButtonPressed()
{
	juce::JUCEApplication::getInstance()->systemRequestedQuit();
}