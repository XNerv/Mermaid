//=============================================================================
#ifndef __STACKHOLDER_H__
#define __STACKHOLDER_H__
//=============================================================================
#include "../includes.h"
//=============================================================================
class StackManager;
class StackPanel;
//=============================================================================
#define STACKHEADERHEIGHT 27
//=============================================================================
class StackHolder : public juce::Component,
					private juce::Timer
{
	public:
		StackHolder (StackManager* parent, 
					 StackPanel* panel, 
					 juce::ToolbarItemFactory* factory, 
					 const juce::String& name,
					 bool isOpen);
		~StackHolder ();
		//=====================================================================
		const juce::Rectangle<int> getContentBounds ();
		//=====================================================================
		juce_UseDebuggingNewOperator
	protected:
		//=====================================================================
		void resized ();
		void paint (juce::Graphics &g);
		//=====================================================================
		void mouseDown (const juce::MouseEvent& e);
		void mouseEnter (const juce::MouseEvent &e);
		void mouseExit (const juce::MouseEvent &e);
		//=====================================================================
		bool isOpen ()						{ return stackIsOpen; }
		void setOpen (bool status)			{ stackIsOpen = status; }
		//=====================================================================
	private:
		juce::Toolbar* toolbar;
		juce::ToolbarItemFactory* factory;
		StackManager *manager;
		StackPanel *panel;
		juce::String name;
		bool stackIsOpen;
		bool mouseIsOver;
		//=====================================================================
		int animDur;
		int animStepSize;
		int animStepDur;
		juce::Time animStartTime;
		void timerCallback ();
		//=====================================================================
};
//=============================================================================
#endif //__STACKHOLDER_H__
//=============================================================================