//=============================================================================
#ifndef __DOCK_PANEL_H__
#define __DOCK_PANEL_H__
//=============================================================================
#include "../includes.h"
#include "DockManager.h"
//=============================================================================
class DockPanel : public DockHolder
{
	public:
		DockPanel (DockManager *parent, 
				   juce::Component* content, 
				   const int position);
		~DockPanel ();
		//=====================================================================
		void resized ();
		void paint (juce::Graphics &g);
		//=====================================================================
	private:
		friend class DockManager;
		friend class DockHolder;
		juce::Component *content;
};
//=============================================================================
#endif //__DOCK_PANEL_H__
//=============================================================================