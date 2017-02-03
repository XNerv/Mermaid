
#include "SmoothScrollBar.h"

#define M_PI (3.14159265358979323846)

SmoothScrollBar::SmoothScrollBar (const bool isVertical, const bool buttonsAreVisible) : juce::ScrollBar(isVertical),
     // SW FPS
     sw_LoopInterval (1/*roundToIntAccurate (1000.0 / sw_ScrollMaxFPS)*/),
     // SW OPTIONS
     sw_EnableThisExtension (true),
     sw_PresetSettings (7),
     sw_ScrollDuration (100),
     sw_EnableRelativeStepSize (true),
     sw_EnableAdaptiveStep (true),
     sw_EnableAdaptiveDuration (true),
     sw_EnableSoftEdge (true),
     sw_ScrollMaxFPS (120/*960*/),
     sw_ModifierSmallStep (3),
     sw_ModifierFullPageStep (2),
     sw_ModifierDisableSmoothWheel (0),
     sw_compatibilityDetectionMode (false),
     // SW GLOBALS
     sw_scrollLeft (0),
     sw_scrollLeftLastEvent (0),
     sw_scrollEventTimeStamp (0),
     sw_scrollDeadline (0),
     sw_clientFrame (NULL),
     sw_intervalID (false),
     sw_velocityLastEvent (0),
     sw_velocityCurrent (0),
     sw_scrollCurrentStep (1),
     sw_scrollCurrentDuration (0),
     sw_optimizedProfileCodeExists (false),
     sw_optimizedHandlerExists (false),
     sw_maxTop (0),
     sw_isScrollable (false),
     // SW ADAPTIVE
     sw_AdaptiveStepMinStepFactor (.4),
     sw_AdaptiveStepMaxStepFactor (1.2),
     sw_AdaptiveDurationMinStepFactor (.8),
     sw_AdaptiveDurationMaxStepFactor (2),
     sw_RelativeStepFactor (0.15),
     sw_FullPageFactor (0.9),
     sw_ShortStepFactor (0.1),
     sw_ScrollPx (20),
     sw_AdaptiveStepMinDurationFactor (.7),
     sw_AdaptiveStepMaxDurationFactor (1.5),
     sw_AdaptiveDurationMinDurationFactor (.7),
     sw_AdaptiveDurationMaxDurationFactor (1.5),
     sw_EnableSmoothEventTransition (true),
     sw_SmoothScrollProfile (2),
     sw_TransEnd (1),
     sw_EnableWholePageScrollInForms (false),
     sw_debugMode (false),
     // SW OTHERS
     sw_ASa (0),
     sw_ASb (0),
     sw_ADa (0),
     sw_ADb (0),
     // SW OTHERS
     sw_lastWheelLoopEvent (0),
     sw_dtn_0 (0),
     sw_dtn_1 (0),
     sw_dtn_2 (0)
{
   smoothTimer = new SmoothTimer(this);
   initSmoothWheel ();
}
// =============================================================================
SmoothScrollBar::~SmoothScrollBar ()
{
   deleteAndZero (smoothTimer);
}
// =============================================================================
bool SmoothScrollBar::keyPressed (const juce::KeyPress &key, Component *comp)
{
	return keyPressed (key);
}
// =============================================================================
bool SmoothScrollBar::keyPressed (const juce::KeyPress &key)
{
	bool back = false;
   //if (ScrollBar::isVertical())
   {
      if (key.isKeyCode (key.upKey))
      {
         /*smoothWheelHandler (juce::MouseEvent(juce::MouseInputSource(1, true),
							 juce::Point<int>(0,0),
							 juce::ModifierKeys::getCurrentModifiers(),
							 NULL, NULL,
							 juce::Time::getCurrentTime(),
                             juce::Point<int>(0,0), 
							 juce::Time::getCurrentTime(),
                             1, false), -1);*/
         back = true;
      }
      //else
      if (key.isKeyCode (key.downKey))
      {
        /* smoothWheelHandler (juce::MouseEvent(juce::MouseInputSource(1, true),
							 juce::Point<int>(0,0),
							 juce::ModifierKeys::getCurrentModifiers(),
							 NULL, NULL,
							 juce::Time::getCurrentTime(),
                             juce::Point<int>(0,0), 
							 juce::Time::getCurrentTime(),
                             1, false), +1);*/
         back = true;
      }
   //}
   //else
   //{
	  //else
      if (key.isKeyCode (key.leftKey))
      {
         /*smoothWheelHandler (juce::MouseEvent(juce::MouseInputSource(1, true),
							 juce::Point<int>(0,0),
							 juce::ModifierKeys::getCurrentModifiers(),
							 NULL, NULL,
							 juce::Time::getCurrentTime(),
                             juce::Point<int>(0,0), 
							 juce::Time::getCurrentTime(),
                             1, false), -1);*/
         back = true;
      }
      //else
      if (key.isKeyCode (key.rightKey))
      {
         /*smoothWheelHandler (juce::MouseEvent(juce::MouseInputSource(1, true),
							 juce::Point<int>(0,0),
							 juce::ModifierKeys::getCurrentModifiers(),
							 NULL, NULL,
							 juce::Time::getCurrentTime(),
                             juce::Point<int>(0,0), 
							 juce::Time::getCurrentTime(),
                             1, false), +1);*/
         back = true;
      }
   }
   return back;
}
// =============================================================================
void SmoothScrollBar::mouseWheelMove (const juce::MouseEvent &e, const juce::MouseWheelDetails& d)
{
   double increment = ScrollBar::isVertical() ? d.deltaY: d.deltaX;
   smoothWheelHandler (e, -increment);
}
// =============================================================================
//checks whether the specified modifier key was held when the event was intercepted.
bool SmoothScrollBar::modifierHeld (const juce::MouseEvent &e, const int modifier)
{
    switch (modifier)
   {
       case 1:
         return e.mods.isAltDown();
       case 2:
         return e.mods.isCtrlDown();
       case 3:
         return e.mods.isShiftDown();
    }
    return false;
}
// =============================================================================
bool SmoothScrollBar::acquireEvent (const juce::MouseEvent &e, double detail)
{
   //implicit else: ok. intercepting event. initialize variables, step size etc.
   //sw_intervalID = window.setInterval(sw_smoothWheelLoop,sw_LoopInterval);
   //sw_clientFrame= tmpClientFrame;

   //int viewHeight = getHeight();//clientFrame.innerHeight;
   int viewHeight;
   if (ScrollBar::isVertical())
   {
      viewHeight = getParentComponent()->getHeight();
      if (viewHeight == 0)
         viewHeight = getHeight();
   }
   else
   {
      viewHeight = getParentComponent()->getWidth();
      if (viewHeight == 0)
         viewHeight = getWidth();
   }

    if (modifierHeld (e, sw_ModifierFullPageStep))
      sw_scrollCurrentStep = viewHeight * sw_FullPageFactor;
    else
      sw_scrollCurrentStep = (sw_EnableRelativeStepSize) ?
                        viewHeight * sw_RelativeStepFactor :
                        sw_ScrollPx * (abs(detail));

   if (modifierHeld (e, sw_ModifierSmallStep))
      sw_scrollCurrentStep = sw_scrollCurrentStep * sw_ShortStepFactor;

   sw_maxTop = ScrollBar::getMaximumRangeLimit();

   sw_velocityCurrent = 0;
   smoothTimer->startTimer (sw_LoopInterval);
   sw_intervalID = true;
    return true;
}
// =============================================================================
void SmoothScrollBar::smoothWheelHandler (const juce::MouseEvent &e, double detail)
{
   bool alreadyScrolling = (sw_velocityCurrent != 0);
   double now = juce::Time::getMillisecondCounterHiRes();
   if (!alreadyScrolling)
   {
      //already scrolling, do average 2
      sw_dtn_1=
        sw_dtn_2=
          sw_ScrollDuration * sw_AdaptiveDurationMaxStepFactor;
   }
   else
   {
        sw_dtn_2 = sw_dtn_1;
        sw_dtn_1 = sw_dtn_0;
   }
   sw_dtn_0 = (now - sw_scrollEventTimeStamp);

   //average last 3 event deltas.
   double dtn = (sw_dtn_0 + sw_dtn_1 + sw_dtn_2) / 3;

   // if all settings match, use a pre-configured handler.
   if (sw_optimizedHandlerExists)
   {
      if (!sw_intervalID)
         if (!acquireEvent(e, detail))
            return;

      double sf = sw_ASa * dtn + sw_ASb;
      double df = sw_ADa * dtn + sw_ADb;

      sw_scrollCurrentDuration = 300 * ((df < .8) ? .8 :
                               ((df >  2) ?  2 : df));
      sw_scrollLeft += sw_scrollCurrentStep * ((detail > 0) ? 1 : -1) *
                                    ((sf > 1.2) ? 1.2 :
                                    ((sf <  .4) ?  .4 : sf));
      double st = ScrollBar::getCurrentRangeStart();
      double sd = st + sw_scrollLeft;
      sw_scrollLeft = (sd < 0) ? -st : ((sd > sw_maxTop) ? sw_maxTop - st : sw_scrollLeft);
      sw_scrollEventTimeStamp = sw_velocityCurrent ? sw_lastWheelLoopEvent : now; //now;
      sw_scrollDeadline = sw_scrollEventTimeStamp + sw_scrollCurrentDuration;
      sw_scrollLeftLastEvent = sw_scrollLeft;
      sw_velocityLastEvent = sw_velocityCurrent;
      //e.preventDefault ();
      //e.stopPropagation ();
      return;
   }
   else
   {
      //implicit else, consider all variables explicitly
      if (!sw_intervalID)
         //if not currently scrolling
         if (!sw_EnableThisExtension || !acquireEvent(e, detail))
            //if event should not be intercepted
            return;  //let default handler kick in
         //implicit else: event acquired and vars initialized
      //implicit else: currently scrolling -> modify target and deadline.

      //e.preventDefault ();
      //e.stopPropagation ();
   }

   double suggestedStepFactor = juce::jlimit(sw_AdaptiveStepMinStepFactor,
                              sw_AdaptiveStepMaxStepFactor,
                              sw_ASa * dtn + sw_ASb);
   double suggestedDurationFactor = juce::jlimit(sw_AdaptiveDurationMinStepFactor,
                                 sw_AdaptiveDurationMaxStepFactor,
                                 sw_ADa * dtn + sw_ADb);
   sw_scrollCurrentDuration = sw_ScrollDuration *
                      ((sw_EnableAdaptiveDuration) ? suggestedDurationFactor : 1);
   sw_scrollLeft += ((detail > 0) ? 1 : -1) * sw_scrollCurrentStep *
                ((sw_EnableAdaptiveStep) ? suggestedStepFactor : 1);

   if (sw_EnableSoftEdge)
   {
      double destination = ScrollBar::getCurrentRangeStart() + sw_scrollLeft;
      if (destination < 0)
         sw_scrollLeft = -ScrollBar::getCurrentRangeStart();
      else
      if (destination > ScrollBar::getMaximumRangeLimit())
         sw_scrollLeft = ScrollBar::getMaximumRangeLimit() -
                     ScrollBar::getCurrentRangeStart();
   }

   sw_scrollEventTimeStamp = alreadyScrolling ? sw_lastWheelLoopEvent : now;
   sw_scrollDeadline = sw_scrollEventTimeStamp + sw_scrollCurrentDuration;
   sw_scrollLeftLastEvent = sw_scrollLeft;
   sw_velocityLastEvent = sw_velocityCurrent;
}
// =============================================================================
void SmoothScrollBar::internalCallback ()
{
   double toScroll, currentTime, currentPercentage, timeLeft;

   currentTime = juce::Time::getMillisecondCounterHiRes ();

   if (sw_lastWheelLoopEvent == currentTime)
   {
      //we're back too soon. means previous loop was delayed.
      //smoothTimer->stopTimer ();
      //sw_intervalID = false;
      smoothTimer->startTimer (sw_LoopInterval);
      sw_intervalID = true;
      return;
   }

   sw_lastWheelLoopEvent = currentTime;

   timeLeft = sw_scrollDeadline - currentTime;
   if (timeLeft <= 0)            
      //we're past the deadline. scroll what's left.
      toScroll = sw_scrollLeft;
   else                     
   {
      //calculate next scroll destination according to scroll profile
      currentPercentage = 1 - timeLeft / sw_scrollCurrentDuration;
      toScroll= /*roundDoubleToIntAccurate*/ (sw_scrollLeftLastEvent *
                                    (scrollProfile (currentPercentage) - 1) +
                                    sw_scrollLeft);
      if ((sw_scrollLeft - toScroll) * sw_scrollLeft < 0)
         //if going past destination point
         toScroll = sw_scrollLeft;
   }

   if ((toScroll == 0) && (sw_scrollLeft != 0))
      return;

   ScrollBar::setCurrentRangeStart (ScrollBar::getCurrentRangeStart () + toScroll);
   sw_scrollLeft -= toScroll;

   //no more scroll needed ?
   if (sw_scrollLeft == 0)
   {
      //window.clearInterval (sw_intervalID);
      smoothTimer->stopTimer ();
      sw_intervalID = false;
      sw_velocityCurrent = 0;
      return;
   }
}
// =============================================================================
/**
   Transform percentage of time to percentage of location according to a scroll profile
   it might consider an implicit parameter of the speed of the scroll at the time
   the event was intercepted.
*/
double SmoothScrollBar::scrollProfile (double percentage)
{
   double sw_p, sw_p1, sw_p2, sw_p21, sw_p2v;

   if (sw_optimizedProfileCodeExists)
   {
      sw_p = percentage;
      sw_p1 = 1  - sw_p;
      sw_p2 = sw_p1 * sw_p1;
      sw_p21 = 1 - sw_p2;
      sw_p2v = sw_p2 * sw_velocityLastEvent;
      sw_velocityCurrent = sw_p2v + sw_p21 * 2 * sw_p1;
      return sw_p2v * sw_p + sw_p21 * sw_p21; //location
   }

   //implicit else: non-optimized path, consider all settings explicitly.
   double location, velocity;
   switch (sw_SmoothScrollProfile)
   {
      case 0:
         location = profileLinear (percentage);
         break;
      case 1:
         location = profilePower2 (percentage);
         break;
      case 2:
         location = profilePower3 (percentage);
         break;
      case 3:
         location = profileCosine (percentage);
         break;
      default:
         location = profileLinear (percentage);
         break;
   }
   if (sw_EnableSmoothEventTransition)
   {
      switch (sw_SmoothScrollProfile)
      {
         case 0:
            velocity = profileLinearVelocity (percentage);
            break;
         case 1:
            velocity = profilePower2Velocity (percentage);
            break;
         case 2:
            velocity = profilePower3Velocity (percentage);
            break;
         case 3:
            velocity = profileCosineVelocity (percentage);
            break;
         default:
            velocity = profileLinearVelocity (percentage);
            break;
      }
      location = transition (percentage, profileConstVelocity (percentage), location);
      sw_velocityCurrent= transition (percentage, profileConstVelocityVelocity (percentage), velocity);
   }
   return location;
}
// =============================================================================
/**
   Returns a weighted average of 2 profiles according (non-linear) to percentage.
*/
double SmoothScrollBar::transition (double percentage, double srcProfile, double dstProfile)
{
   double srcPer = ((sw_TransEnd - percentage) / sw_TransEnd) *
               ((sw_TransEnd - percentage) / sw_TransEnd);
   double dstPer = 1 - srcPer;
   return (percentage < sw_TransEnd) ? ((srcPer * srcProfile) + (dstPer * dstProfile)) : dstProfile;
}
// =============================================================================
/**
   various scroll profiles and their velocities (derivatives) functions.
*/
double SmoothScrollBar::profileLinear (double percentage)         { return percentage; }
double SmoothScrollBar::profileLinearVelocity (double percentage)   { return 1; }
double SmoothScrollBar::profilePower2 (double percentage)         { return 1-(1-percentage)*(1-percentage); }
double SmoothScrollBar::profilePower2Velocity (double percentage)   { return 2*(1-percentage); }
double SmoothScrollBar::profilePower3 (double percentage)         { return 1-(1-percentage)*(1-percentage)*(1-percentage); }
double SmoothScrollBar::profilePower3Velocity (double percentage)   { return 3*(1-percentage)*(1-percentage); }
double SmoothScrollBar::profileCosine (double percentage)         { return .5-.5*cos(M_PI*percentage); }
double SmoothScrollBar::profileCosineVelocity (double percentage)   { return .5*sin(M_PI*percentage)*M_PI; }
// using implicit parameter velocityLastEvent.
// this profile does NOT achieve destination point on deadline.
// it represents a linear motion at the speed that was recorded when the event was intercepted.
// this profile is used for transition when a scroll event is intercepted while still scrolling.
double SmoothScrollBar::profileConstVelocity (double percentage)         { return sw_velocityLastEvent*percentage; }
double SmoothScrollBar::profileConstVelocityVelocity (double percentage)   { return sw_velocityLastEvent; }
// =============================================================================
/**
   Initialize variable according to the preset, and determine whether optimization is possible.
*/
struct SmoothWheelPreset
{
   int      ScrollDuration;
   bool   EnableRelativeStepSize;
   bool   EnableAdaptiveStep;
   bool   EnableAdaptiveDuration;
   int      SmoothScrollProfile;
   bool   EnableSmoothEventTransition;
   int      TransEnd;
};
// =============================================================================
const SmoothWheelPreset presets[] =
{
   { 150,  true,  false, false,  1, true,  1 }, // 1
   { 300,  true,  false, false,  1, true,  1 }, // 2
   { 500,  true,  false, false,  1, true,  1 }, // 3 : was default in v0.2
   { 1000, true,  false, false,  1, true,  1 }, // 4
   { 300,  true,  true,  false,  1, true,  1 }, // 5
   { 300,  true,  false, true ,  1, true,  1 }, // 6
   { 300,  true,  true , true ,  1, true,  1 }, // 7 : default with v0.3, v0.4
   { 500,  true,  true , false , 1, true,  1 }, // 8
   { 500,  true,  true , true ,  1, true,  1 }, // 9
   { 250,  false, false, false,  0, false, 1 }, // 10
   { 150,  false, false, false,  2, false, 1 },  // 11
   { 750,  true,  true,  true,   2, true,  1 }   // 12 (MAX)
};
// =============================================================================
void SmoothScrollBar::initSmoothWheel ()
{
   if (sw_PresetSettings >= 1 && sw_PresetSettings <= 11)
   {
      sw_ScrollDuration            = presets[sw_PresetSettings].ScrollDuration;
      sw_EnableRelativeStepSize      = presets[sw_PresetSettings].EnableRelativeStepSize;
      sw_EnableAdaptiveStep         = presets[sw_PresetSettings].EnableAdaptiveStep;
      sw_EnableAdaptiveDuration      = presets[sw_PresetSettings].EnableAdaptiveDuration;
      sw_SmoothScrollProfile         = presets[sw_PresetSettings].SmoothScrollProfile;
      sw_EnableSmoothEventTransition   = presets[sw_PresetSettings].EnableSmoothEventTransition;
      sw_TransEnd                  = presets[sw_PresetSettings].TransEnd;
   }

   initGlobals();

   sw_optimizedProfileCodeExists = (sw_SmoothScrollProfile == 1)            &&
                           (sw_EnableSmoothEventTransition == true)   && 
                           (sw_TransEnd == 1);

   sw_optimizedHandlerExists =
   (
         sw_EnableThisExtension                  &&
         sw_EnableAdaptiveStep                  &&
         sw_EnableAdaptiveDuration               &&
         sw_EnableSoftEdge                     &&
        (sw_ScrollDuration               == 300) &&
        (sw_AdaptiveStepMinStepFactor      == 0.4) &&
        (sw_AdaptiveStepMaxStepFactor      == 1.2) &&
        (sw_AdaptiveDurationMinStepFactor   == .8)   &&
        (sw_AdaptiveDurationMaxStepFactor   == 2)
    );
}
// =============================================================================
void SmoothScrollBar::initGlobals ()
{
   //set some of the adaptive functionality parameters only once in advance.
   sw_ASa = ((sw_AdaptiveStepMinStepFactor - sw_AdaptiveStepMaxStepFactor) /
           (sw_AdaptiveStepMaxDurationFactor - sw_AdaptiveStepMinDurationFactor)) /
            sw_ScrollDuration; //diagonal
   sw_ASb = sw_AdaptiveStepMaxStepFactor - sw_ASa *
          sw_AdaptiveStepMinDurationFactor; //offset
   sw_ADa = ((sw_AdaptiveDurationMaxStepFactor - sw_AdaptiveDurationMinStepFactor)/
           (sw_AdaptiveDurationMaxDurationFactor - sw_AdaptiveDurationMinDurationFactor)) /
            sw_ScrollDuration; //diagonal
   sw_ADb = sw_AdaptiveDurationMinStepFactor - sw_ADa *
          sw_AdaptiveDurationMinDurationFactor; //offset

   //if (sw_intervalID)
      //window.clearInterval (sw_intervalID);

   sw_scrollLeft            = 0;
   sw_scrollLeftLastEvent      = 0;
   sw_scrollEventTimeStamp      = 0;
   sw_scrollDeadline         = 0;
   sw_clientFrame            = NULL;
   sw_intervalID            = NULL;
   sw_velocityLastEvent      = 0;
   sw_velocityCurrent         = 0;
   sw_scrollCurrentStep      = 1;
   sw_scrollCurrentDuration   = 0;
   sw_maxTop               = 0;
   sw_isScrollable            = false;

   return;
}
// =============================================================================
