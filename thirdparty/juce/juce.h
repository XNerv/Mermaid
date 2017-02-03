#ifndef __APPHEADERFILE_ZJ6SKR__
#define __APPHEADERFILE_ZJ6SKR__

// To fix an error when the juce.h is included before windows.h
// To more info see http://www.juce.com/forum/topic/how-include-systems-headers-juce-resolved
#if defined(_MSC_VER)
    #include <windows.h>
#endif

#include "AppConfig.h"  
#include "modules/juce_core/juce_core.h"
#include "modules/juce_audio_basics/juce_audio_basics.h"
#include "modules/juce_audio_devices/juce_audio_devices.h"
#include "modules/juce_audio_formats/juce_audio_formats.h"
#include "modules/juce_audio_processors/juce_audio_processors.h"
#include "modules/juce_audio_utils/juce_audio_utils.h"
#include "modules/juce_box2d/juce_box2d.h"
#include "modules/juce_cryptography/juce_cryptography.h"
#include "modules/juce_data_structures/juce_data_structures.h"
#include "modules/juce_events/juce_events.h"
#include "modules/juce_graphics/juce_graphics.h"
#include "modules/juce_gui_basics/juce_gui_basics.h"
#include "modules/juce_gui_extra/juce_gui_extra.h"
#include "modules/juce_opengl/juce_opengl.h"
#include "modules/juce_video/juce_video.h"
 
#if ! DONT_SET_USING_JUCE_NAMESPACE
 // If your code uses a lot of JUCE classes,  then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_JUCE_NAMESPACE.
 using namespace juce;
#endif

#endif   // __APPHEADERFILE_ZJ6SKR__
