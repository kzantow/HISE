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
*   Commercial licences for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licencing:
*
*   http://www.hartinstruments.net/hise/
*
*   HISE is based on the JUCE library,
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

PitchwheelModulator::PitchwheelModulator(MainController *mc, const String &id, Modulation::Mode m):
	TimeVariantModulator(mc, id, m),
    targetValue(1.0f),
	Modulation(m),
	intensity(1.0f),
	inverted(false),
	useTable(false),
	table(new MidiTable()),
	smoothTime(200.0f),
	currentValue(1.0f)
{
	this->enableConsoleOutput(false);
	
	parameterNames.add("Inverted");
	parameterNames.add("UseTable");
	parameterNames.add("SmoothTime");

};

PitchwheelModulator::~PitchwheelModulator()
{
};

ProcessorEditorBody *PitchwheelModulator::createEditor(ProcessorEditor *parentEditor)
{
#if USE_BACKEND

	return new PitchWheelEditorBody(parentEditor);

	
#else

	ignoreUnused(parentEditor);
	jassertfalse;

	return nullptr;

#endif
};

float PitchwheelModulator::getAttribute (int parameter_index) const
{
	switch(parameter_index)
	{
	case Parameters::Inverted:
		return inverted ? 1.0f : 0.0f;
	case Parameters::SmoothTime:
		return smoothTime;
	case Parameters::UseTable:
		return useTable ? 1.0f : 0.0f;
	default: 
		jassertfalse;
		return -1.0f;
	}

};

void PitchwheelModulator::setInternalAttribute (int parameter_index, float newValue)
{
	switch(parameter_index)
	{
	case Parameters::Inverted:
		inverted = (newValue != 0.0f); break;
	case Parameters::SmoothTime:
		{
			smoothTime = newValue; 

			smoother.setSmoothingTime(smoothTime);		
			break;
		}
	case Parameters::UseTable:
		useTable = (newValue != 0.0f); break;


	default: 
		jassertfalse;
	}
};


float PitchwheelModulator::calculateNewValue ()
{
	currentValue = (fabsf(targetValue - currentValue) < 0.001) ? targetValue : smoother.smooth(targetValue);
	
	return currentValue;
}

	/** sets the new target value if the controller number matches. */
void PitchwheelModulator::handleMidiEvent (const MidiMessage &m)
{
	
	if(m.isPitchWheel())
	{
		inputValue = m.getPitchWheelValue() / 16383.0f;
		float value;

		if(useTable) value = table->get((int)(inputValue * 127.0f));
		else value = inputValue;

		if(inverted) value = 1.0f - value;

		targetValue = value;

		debugMod(" New Value: " + String(value));
	};
}