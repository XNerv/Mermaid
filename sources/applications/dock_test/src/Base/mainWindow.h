#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "../includes.h"

class MainWindow : public juce::DocumentWindow
{
	public:
		MainWindow ();
		~MainWindow ();

		void closeButtonPressed ();
		juce_UseDebuggingNewOperator
	private:
};
#endif //__MAINWINDOW_H__