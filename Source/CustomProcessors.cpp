/*
  ==============================================================================

    CustomPlugin.cpp
    Created: 14 Feb 2020 9:07:58am
    Author:  maxime

  ==============================================================================
*/

#include "CustomProcessors.h"

CustomProcessor::CustomProcessor(String name, bool midiEffect) : AudioProcessor()
{
    effectName = name;
}