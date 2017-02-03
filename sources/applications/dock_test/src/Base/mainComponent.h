#ifndef __MAINCOMPONENT_H__
#define __MAINCOMPONENT_H__

#include "../includes.h"

#include "../Dock/DockManager.h"
#include "../Dock/DockPanel.h"
#include "../Dock/DockButtons.h"

class MainWindow;

class MainComponent : public juce::Component
{
	public:
		MainComponent (MainWindow *win);
		~MainComponent ();
	
		void resized();
	
	private:
		MainWindow *mainWindow;
		DockManager *manager;
};
#endif
