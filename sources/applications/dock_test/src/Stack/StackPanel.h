//=============================================================================
#ifndef __STACK_PANEL_H__
#define __STACK_PANEL_H__
//=============================================================================
#include "../includes.h"
#include "StackManager.h"
//=============================================================================
class StackPanel : public StackHolder
{
	public:
		StackPanel (StackManager *parent, 
				    juce::Component* contents, 
					juce::ToolbarItemFactory* factory,
					bool isOpen, 
					const int height);
		~StackPanel ();
		//=====================================================================
		int getPreferredHeight () { return height; }
		//=====================================================================
		void resized ();
		void paint (juce::Graphics &g);
		//=====================================================================
	private:
		friend class StackManager;
		friend class StackHolder;
		juce::Component *content;
		int height;
};
//=============================================================================
#endif //__STACK_PANEL_H__
//=============================================================================