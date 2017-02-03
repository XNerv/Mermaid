//=============================================================================
#ifndef __STACKMANAGER_H__
#define __STACKMANAGER_H__
//=============================================================================
#include "../includes.h"
#include "StackHolder.h"
#include "StackPanel.h"
#include "../LookAndFeel/ScrollBarLookAndFeel.h"
#include "../Smoothing/SmoothScrollBar.h"
//=============================================================================
class StackManager : public juce::Component,
					 private juce::ScrollBarListener
{
	ScrollBarLookAndFeel laf;

	public:
		StackManager (const bool scrollBarAtLeft);
		~StackManager ();
		//=====================================================================
		bool getScrollBarAtLeft ()					{ return scrollBarAtLeft; }
		//=====================================================================
		void setFixedComponent (juce::Component *component, 
								juce::ToolbarItemFactory *factory, 
								bool onTop, bool isOpen, 
								const int height);
		//=====================================================================
		void setStackedComponent (juce::Component *component, 
								  juce::ToolbarItemFactory *factory, 
								  bool isOpen, 
								  const int height);
		//=====================================================================
		juce_UseDebuggingNewOperator
	protected:
		friend class StackHolder;
		//=====================================================================
		void resized ();
		void resized_child ();
		void paint (juce::Graphics &g);
		//=====================================================================
	private:
		//=====================================================================
		StackManager (const StackManager&);
		const StackManager& operator= (const StackManager&);
		//=====================================================================
		juce::OwnedArray <StackHolder> stacks;
		juce::Component* viewport;
		StackPanel* fixed;
		bool isFixedOnTop;
		//=====================================================================
		juce::ComponentAnimator animator;
		//=====================================================================
		bool scrollBarAtLeft;
		SmoothScrollBar *scrollbar;
		void scrollBarMoved (juce::ScrollBar *scrollBarThatHasMoved, 
							 double RangeStart);
		double currentRangeStart;
		//=====================================================================
};
//=============================================================================
#endif //__STACKMANAGER_H__
//=============================================================================