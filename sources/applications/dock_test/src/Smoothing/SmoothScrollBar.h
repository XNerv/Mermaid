// =============================================================================
#ifndef __JUCE_SMOOTHSCROLLBAR__
#define __JUCE_SMOOTHSCROLLBAR__
// =============================================================================
#include "../includes.h"
// =============================================================================
class SmoothScrollBar;
class SmoothScrollBarListener
{
public:
    /** Destructor. */
    virtual ~SmoothScrollBarListener() {}

    /** Called when a ScrollBar is moved.

        @param scrollBarThatHasMoved    the bar that has moved
        @param newRangeStart            the new range start of this bar
    */
    virtual void scrollBarMoved (SmoothScrollBar* scrollBarThatHasMoved,
                                 double newRangeStart) = 0;
};
// =============================================================================
class  SmoothTimer;
class  SmoothScrollBar : public juce::ScrollBar
{
   SmoothTimer *smoothTimer;

   public:
      SmoothScrollBar (const bool isVertical, const bool buttonsAreVisible = true);
      ~SmoothScrollBar ();
      // =================================================================
      void initGlobals ();
      void initSmoothWheel ();
      bool modifierHeld (const juce::MouseEvent &e, const int modifier);
      bool acquireEvent (const juce::MouseEvent &e, double detail);
      void smoothWheelHandler (const juce::MouseEvent &e, double detail);
      void mouseWheelLoop ();
      // =================================================================
      double scrollProfile (double current);
      double transition (double percentage, double srcProfile, double dstProfile);
      double profileLinear (double percentage);      
      double profileLinearVelocity (double percentage);
      double profilePower2 (double percentage);      
      double profilePower2Velocity (double percentage);
      double profilePower3 (double percentage);      
      double profilePower3Velocity (double percentage);
      double profileCosine (double percentage);      
      double profileCosineVelocity (double percentage);
      double profileConstVelocity (double percentage);
      double profileConstVelocityVelocity (double percentage);
      // =================================================================
	  juce_UseDebuggingNewOperator
   private:
      // **************** CONFIGURATION SECTION ****************
      // Global enable/disable for this extension. default is true.
      // setting it to false will disable this extension completely.
      const bool sw_EnableThisExtension;
      // =================================================================
      // Select one of several preset settings: (default is 7)
      // 0 : don't use preset. use actual variables values.
      // 1 : turbo:  for those who like quickies  (duration=150, non-linear, smooth transitions, relative step)
      // 2 : fast: slower than turbo, faster than the medium   (duration=300, non-linear, smooth transitions, relative step)
      // 3 : medium: [was the default in v0.2] (duration=500, non-linear, smooth transitions, relative step)
      // 4 : nice and easy: yet even slower and smoother   (duration=1000, non-linear, smooth transitions, relative step)
      // 5 : fast + adaptive-step
      // 6 : fast + adaptive-duration
      // 7 :  -> [DEFAULT] fast + adaptive-step + adaptive-duration
      // 8 : medium + adaptive-step
      // 9 : medium + adaptive-step + adattive-duration
      // 10 : same as version 0.1 default (duration=250, linear, no smooth transition, no relative step)
      // 11 : same as version 0.1 with isLinear=false, duration=150 (no smooth transition, no relative step)
      const int sw_PresetSettings;
      // =================================================================
      // Max milisec to complete scroll after last scroll event
      // reasonable values are (fast)100/250/500/1000(slow). default is 300
      // -> this value is overriden if a preset is selected.
      int sw_ScrollDuration;
      // =================================================================
      // If enabled, set step size relative to the scrolled area height by a constant factor.
      // thus, if enabled and the visible window/frame is short, so will be the scroll steps.
      // when enabled, it overrides gecko internal fixed pixels/line constant. default is true.
      // -> this value is overriden if a preset is selected.
      bool sw_EnableRelativeStepSize;
      // =================================================================
      // If enabled, the step size will vary according to the delay between consecutive wheel events.
      // such that fast wheel scroll will cause step size to increase, and vice versa.
      // the desired effect is that when you're reading, and want to scroll, the step is small,
      // but it you're just browsing through the document, with faster wheel action,
      // you'll gain more 'milage' per wheel event. default is true.
      // -> this value is overriden if a preset is selected.
      bool sw_EnableAdaptiveStep;
      // =================================================================
      // If enabled, duration will vary according to the delay between consecutive wheel events,
      // such that fast wheel scroll will cause duration to DEcrease, and vice versa.
      // the desired effect is to make it slower and smoother while reading, but 'snappy' when browsing.
      // default is true.
      // -> this value is overriden if a preset is selected.
      bool sw_EnableAdaptiveDuration;
      // =================================================================
      // If enabled, causes the scroll to 'land gracefully' on the top or bottom edges of the document.
      // default is true.
      bool sw_EnableSoftEdge;
      // =================================================================
      // Limits the frames/sec of the smooth scroll operation.
      // for best visual performance, set this value to your monitor refresh rate. default is 100.
      // reasonable values are 20 to 120
      // note that setting it to high values will make no difference if your cpu isn't up for the task.
      // tips: for faster Gecko rendering, set Desktop color to 16bpp, and turn off font-anti-aliasing
      // IMPORTANT TIP: if possible, set your monitor refresh rate to 100, and ScrollMaxFPS to either 100 or 50.
      // (other values will cause imprecision due to low resolution internal timers)
      const int sw_ScrollMaxFPS;
      // =================================================================
      // IMPORTANT NOTE ABOUT MODIFIER KEYS:
      // by default, mozilla/phoenix/firebird assigns the ctrl key to enlarge/reduce the text size.
      // the alt key is assigned to browse history. shift key is not assigned.
      // in order for a modifier key to work with this extension, it has to be UNassigned first.
      // you can UNassign a modifier key for the mouse wheel as follows:
      // mozilla: edit->preferences->advanced->mouse Wheel -> set it to scroll normally.
      // phoenix/firebird: about:config in the locationbar bar. then change mousewheel.with<xxx>key.action property to 0
      // =================================================================
      // Select a modifier key for fine small scroll steps. default is 3 (SHIFT key).
      // possible values: 0=Disable, 1=ALT, 2=CTRL, 3=SHIFT
      const int sw_ModifierSmallStep;
      // =================================================================
      // Select a modifier key for full page scroll (like PgUp/PgDn). default is 1 (ALT key).
      // possible values: 0=Disable, 1=ALT, 2=CTRL, 3=SHIFT
      const int sw_ModifierFullPageStep;
      // =================================================================
      // Select a modifier key for currently disabling smoothwheel. default is 0 (not assigned).
      // possible values: 0=Disable, 1=ALT, 2=CTRL, 3=SHIFT
      const int sw_ModifierDisableSmoothWheel;
      // =================================================================
      // Curtesy of All-In-One-Gestures author, starting with v0.3, smoothwheel uses a new target detection system
      // (that's the system that decides what part of the screen to scroll when the wheel is scrolled)
      // the new system is able to scroll inside textareas of forms, drop-down menus and more.
      // however, on older Gecko builds, it currently performs less that optimal. So, if you feel that you prefere
      // previous version's (v0.2) detection mechanism, set sw_compatibilityDetectionMode to true.
      // default is false.
      const bool sw_compatibilityDetectionMode;
      // ********* END OF COMMON CONFIGURATION OPTIONS SECTION ***********
      //WHILE THE ABOVE COMMON CONFIGURATIONS ARE MOSTLY ENABLE/DISABLE, THE NEXT
      //VALUES CAN BE USED TO FINE-TUNE THE BEHAVIOUR OF EACH FEATURE
      // =================================================================
      //these 2 values control the step size when EnableAdaptiveStep=true.
      //they define the factors for the smallest and largest steps, respectively. defaults are .4 and 1.2
      //for extreme control (and variation) (requires a steady hand as well), try 0.3 and 3 respectively.
      double sw_AdaptiveStepMinStepFactor;
      double sw_AdaptiveStepMaxStepFactor;
      // =================================================================
      // These 2 values control the duration when EnableAdaptiveDuration=true.
      // they define the factors for the smallest and largest durations, respectively. defaults are .8 and 2
      double sw_AdaptiveDurationMinStepFactor;
      double sw_AdaptiveDurationMaxStepFactor;
      // =================================================================
      // Factor to use when EnableRelativeStepSize is true. default is 0.15 of viewable page height.
      const double sw_RelativeStepFactor;
      // =================================================================
      // This factor configures how much "full page" is when ModifierFullPageStep is used.
      // 1 will result in full page height. 0.5 results in half page, etc.
      // default is 0.9 of full page height.
      const double sw_FullPageFactor;
      // =================================================================
      // Factor for how short the scroll steps are if ModifierSmallStep is used.
      // default is 0.1 of normal step.
      const double sw_ShortStepFactor;
      // =================================================================
      // #pixels per scroll line. relevant only if EnableRelativeStepSize=false.
      // default value is 20 (empirical estimation of internal Gecko value)
      const int sw_ScrollPx;
      // =================================================================
      // the next 4 values control the duration between wheel events that define the
      // adaptive behaviour of the adaptiveStep and adaptiveDurations features.
      // =================================================================
      // defaults are .7 and 1.5
      double sw_AdaptiveStepMinDurationFactor;
      double sw_AdaptiveStepMaxDurationFactor;
      // =================================================================
      // defaults are .7 and 1.5
      double sw_AdaptiveDurationMinDurationFactor;
      double sw_AdaptiveDurationMaxDurationFactor;
      // =================================================================
      //******* it's recommended not to change the next values since it'll prevent optimizations *******/
      // =================================================================
      // Tries to make a smooth transition if new scroll event occures while still scrolling. default is true.
      // -> this value is overriden if a preset is selected.
      // -> changing the default value will prevent code optimizations.
      bool sw_EnableSmoothEventTransition;
      // =================================================================
      // Selects either linear (=0) or various non-linear(=1/2/3) scroll profiles. default is 1.
      // possible values:
      // 0 : Linear
      // 1 : 2nd power (non-linear)
      // 2 : 3rd power (non-linear)
      // 3 : cosine    (non-linear)
      // -> this value is overriden if a preset is selected.
      // -> changing the default value will prevent code optimizations.
      int sw_SmoothScrollProfile;
      // =================================================================
      // Percentage of when to finish transitional profile (from previous velocity to absolute scroll profile)
      // default is 1.
      // -> this value is overriden if a preset is selected.
      // -> changing the default value will prevent code optimizations.
      int sw_TransEnd;
      // =================================================================
      // THIS VALUE IS IRRELEVANT WITH THE NEW (v0.3+) DETECTION MECHANISM
      // Whether or not to enable WHOLE PAGE scroll when mouse wheel is over form textarea. default is false.
      // if set to false, mouse wheel events will NOT be intercepted by this extension while
      // mouse is over a form textarea. instead, the DEFAULT scroll will be issued.
      // if set to true, the WHOLE PAGE will scroll smoothly when mouse is over a form textarea.
      const bool sw_EnableWholePageScrollInForms;
      // =================================================================
      // controls debug mode on/off. default is false.
      bool sw_debugMode;
      //*************** END OF CONFIGURATION SECTION ********************
      //fps converted to ms between scroll events
      const int sw_LoopInterval;
      // =================================================================
      // Global variables (used by more than one function).
      double   sw_scrollLeft;                  // in pixels
      double   sw_scrollLeftLastEvent;            // in pixels
      double   sw_scrollEventTimeStamp;         // timestamp
      double   sw_scrollDeadline;               // timestamp
      void*   sw_clientFrame;                  // holds the object to scroll
      bool   sw_intervalID;                  // holds the interval timer handle. also indicates whether we're currently scrolling.
      double   sw_velocityLastEvent;            //
      double   sw_velocityCurrent;               //
      double   sw_scrollCurrentStep;            // how many pixels to scroll for the current scroll event.
      double   sw_scrollCurrentDuration;         // how many ms to assign for the current scroll event.
      bool   sw_optimizedProfileCodeExists;      // whether optimized code exist according to various variables
      bool   sw_optimizedHandlerExists;         // if all vairables match, there's a pre-set optimized handler for default settings.
      double   sw_maxTop;                     // maximum top to scroll to. =sw_clientFrame.scrollHeight-sw_clientFrame.clientHeight
      bool   sw_isScrollable;               //
      // =================================================================
      // Set some of the adaptive functionality parameters only once in advance.
      double   sw_ASa;
      double   sw_ASb;
      double   sw_ADa;
      double   sw_ADb;
      // =================================================================
      double   sw_lastWheelLoopEvent;
      double   sw_dtn_0;
      double   sw_dtn_1;
      double   sw_dtn_2;
      // =================================================================
   public:
      // =================================================================
      /** override the ScrollBar methods */
	  bool keyPressed (const juce::KeyPress &key, juce::Component *comp);
      bool keyPressed (const juce::KeyPress &key);
      void mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails&);
      // =================================================================
   private:
      // =================================================================
      friend class SmoothTimer;
      void internalCallback ();
      // =================================================================
};
// =============================================================================
class  SmoothTimer : public juce::Timer
{
   public:
      SmoothTimer (SmoothScrollBar *p)
         : parent (p)
      {}

      void timerCallback ()
      {
         parent->internalCallback ();
      }

   private:
      SmoothScrollBar* parent;
};
// =============================================================================
#endif // __JUCE_SMOOTHSCROLLBAR__
// =============================================================================
