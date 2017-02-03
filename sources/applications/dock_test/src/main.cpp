#include "includes.h"
#include "Base/mainWindow.h"

class JUCEMainApp : public juce::JUCEApplication
{
public:
    JUCEMainApp() : mainWindow(0)
    { 
        /* do nothing */
    }
    ~JUCEMainApp()
    { 
        /* do nothing */
    }
    
    void initialise(const juce::String& commandLine)
    {
        mainWindow = new MainWindow();
    }
    
    void shutdown()
    {
        if (mainWindow != 0)
            juce::deleteAndZero(mainWindow);
    }
    
    void systemRequestedQuit()
    {
        if (mainWindow != 0)
        {
            juce::deleteAndZero(mainWindow);
            quit();
        }
    }
    
    const juce::String getApplicationName()		{ return juce::String("Adobe-Like Dock"); }
    const juce::String getApplicationVersion()	{ return juce::String("1.0"); }
    
    bool moreThanOneInstanceAllowed()
    {
        return true;
    }
    
    void anotherInstanceStarted(const juce::String& commandLine)
    {
    }
    
    void unhandledException(const std::exception *e, const juce::String &sourceFilename, int lineNumber)
    {
    }

private:
    MainWindow* mainWindow;
    
};

START_JUCE_APPLICATION(JUCEMainApp)