
#ifndef __DOCKHOLDER_H__
#define __DOCKHOLDER_H__

#include "../includes.h"

class DockManager;
class DockPanel;

class DockHolder : public juce::Component
{
	public:
		DockHolder (DockManager* parent, DockPanel* panel, const int type);
		~DockHolder ();
	
		const juce::Rectangle<int> getContentBounds();
		
		const int getCurrentSize ()			{ return size; }
		
		juce_UseDebuggingNewOperator
	protected:
		friend DockManager;
		
		void resized ();
		void paint (juce::Graphics &g);
		
		void mouseEnter (const juce::MouseEvent &e);
		void mouseMove (const juce::MouseEvent &e);
		void mouseExit (const juce::MouseEvent &e);
		
		bool isMouseInside ()				{ return inside; }
		
		bool isOpen ()						{ return open; }
		void setOpen (bool status)			{ open = status; }
		
		bool isDocked ()					{ return docked; }
		void setDocked (bool status)		{ docked = status; }
		
	private:
		juce::ComponentAnimator animator;
		DockManager *manager;
		DockPanel* panel;
		int orientation;
		int size;
		bool docked;
		bool inside;
		bool open;
		//=====================================================================
		int button;		// default: 15
		int padding;	// default: 3
		//=====================================================================
		void initConstrainer ();
		//=====================================================================
		friend class DockPanel;
		juce::ResizableBorderComponent *resizable;
		juce::ComponentBoundsConstrainer *constrainer;
		//=====================================================================
};
//=============================================================================
#endif //__DOCKHOLDER_H__
//=============================================================================