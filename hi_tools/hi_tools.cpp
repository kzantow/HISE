/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licenses for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licensing:
*
*   http://www.hise.audio/
*
*   HISE is based on the JUCE library,
*   which must be separately licensed for closed source applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/


#if USE_VDSP_FFT
#define Point DummyPoint
#define Component DummyComponent
#define MemoryBlock DummyMB
#include <Accelerate/Accelerate.h>
#undef Point
#undef Component
#undef MemoryBlock
#endif

#include <atomic>
#include <float.h>
#include <limits.h>
#include <regex>

#include "hi_tools.h"

#include "hi_binary_data/hi_binary_data.cpp"

#if USE_IPP
#include "hi_tools/IppFFT.cpp"
#endif

#include "hi_tools/CustomDataContainers.cpp"
#include "hi_tools/HiseEventBuffer.cpp"

#include "hi_tools/MiscToolClasses.cpp"

#include "hi_tools/HI_LookAndFeels.cpp"


#include "hi_tools/JavascriptTokeniser.cpp"
#include "hi_tools/MarkdownElements.cpp"
#include "hi_tools/Markdown.cpp"
#include "hi_tools/MarkdownParser.cpp"
#include "hi_tools/VariantBuffer.cpp"
#include "hi_tools/Tables.cpp"

#include "hi_standalone_components/SampleDisplayComponent.cpp"

#include "hi_standalone_components/VuMeter.cpp"
#include "hi_standalone_components/Plotter.cpp"
#include "hi_standalone_components/SliderPack.cpp"
#include "hi_standalone_components/TableEditor.cpp"