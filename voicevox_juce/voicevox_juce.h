/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 voicevox_juce
  vendor:             COCOTONE
  version:            0.0.1
  name:               VOICEVOX wrapper library for JUCE framework.
  description:        Classes for creating VOICEVOX application with JUCE framework.
  website:            https://github.com/cocotone/voicevox_juce
  license:            BSD 3-Clause License
  minimumCppStandard: 17

  dependencies:       juce_core

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define VOICEVOX_JUCE_H_INCLUDED

#include <juce_core/juce_core.h>

//==============================================================================

#include "voicevox_client/voicevox_client.h"
