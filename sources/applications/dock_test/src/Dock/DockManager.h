
#ifndef __DOCKMANAGER_H__
#define __DOCKMANAGER_H__

#include "../includes.h"
#include "DockHolder.h"
#include "DockPanel.h"

class DockManager : public juce::Component,
					public juce::ButtonListener,
					public juce::ComponentListener,
					private juce::Timer
{
	public:
		DockManager ();
		~DockManager ();

		bool isDocked (const int position);

		void setPanelComponent (const int position, juce::Component *component);

		juce_UseDebuggingNewOperator
	protected:

		void resized ();
		void paint (juce::Graphics &g);

		void mouseEnter (const juce::MouseEvent &e);
		void mouseExit (const juce::MouseEvent &e);

		void componentMovedOrResized (juce::Component &component, bool wasMoved, bool wasResized);

		void buttonClicked (juce::Button *button);

	private:
		DockHolder *top, *bottom, *center, *left, *right;
		juce::DrawableButton *topButton, *bottomButton, *leftButton, *rightButton;

		DockManager (const DockManager&);
		const DockManager& operator= (const DockManager&);

		juce::ComponentAnimator animator;
		void timerCallback ();
		juce::MouseEvent *lastMouseEvent;

	protected:
		friend class DockHolder;

		void mousePropagation (const juce::MouseEvent &e, const int position);

};

#endif
