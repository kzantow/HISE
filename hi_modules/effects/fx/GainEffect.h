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

#ifndef GAINEFFECT_H_INCLUDED
#define GAINEFFECT_H_INCLUDED

namespace hise { using namespace juce;

/** A simple effect that does nothing. */
class EmptyFX : public MasterEffectProcessor
{
public:

	SET_PROCESSOR_NAME("EmptyFX", "Empty")

	EmptyFX(MainController *mc, const String &uid) :
		MasterEffectProcessor(mc, uid)
	{
		finaliseModChains();
	};

	~EmptyFX()
	{};

	void setInternalAttribute(int /*parameterIndex*/, float /*newValue*/) override {};
	float getAttribute(int /*parameterIndex*/) const override { return 0.0f; };

	bool hasTail() const override { return false; };

	int getNumInternalChains() const override { return 0; };
	int getNumChildProcessors() const override { return 0; };

	void setSoftBypass(bool /*shouldBeSoftBypassed*/, bool /*useRamp*//* =true */) override {};

	bool isFadeOutPending() const noexcept override
	{
		return false;
	}

	Processor *getChildProcessor(int /*processorIndex*/) override
	{
		return nullptr;
	};

	const Processor *getChildProcessor(int /*processorIndex*/) const override
	{
		return nullptr;
	};

	ProcessorEditorBody *createEditor(ProcessorEditor *parentEditor)  override;

	void applyEffect(AudioSampleBuffer &/*b*/, int /*startSample*/, int /*numSamples*/)
	{

	}

};

/** A utility effect that allows smooth gain changes, static delays and panning.
	@ingroup effectTypes
	
*/
class GainEffect: public MasterEffectProcessor
{
public:

	SET_PROCESSOR_NAME("SimpleGain", "Simple Gain")

	enum InternalChains
	{
		GainChain = 0,
        DelayChain,
        WidthChain,
		BalanceChain,
		numInternalChains
	};

	enum EditorStates
	{
		GainChainShown = Processor::numEditorStates,
        DelayChainShown,
        WidthChainShown,
		BalanceChainShown,
		numEditorStates
	};

	enum Parameters
	{
		Gain = 0,
        Delay,
        Width,
		Balance,
		numParameters
	};

	GainEffect(MainController *mc, const String &uid);;

    ~GainEffect();

	void setInternalAttribute(int parameterIndex, float newValue) override;;
	float getAttribute(int parameterIndex) const override;;

	void restoreFromValueTree(const ValueTree &v) override;;
	ValueTree exportAsValueTree() const override;

	bool hasTail() const override { return false; };

	Processor *getChildProcessor(int processorIndex) override
    {
        switch(processorIndex)
        {
            case GainChain:	 return gainChain;
            case DelayChain: return delayChain;
            case WidthChain: return widthChain;
			case BalanceChain: return balanceChain;
			default:		 return nullptr;
        }
    };
    
	const Processor *getChildProcessor(int processorIndex) const override
    {
        switch(processorIndex)
        {
            case GainChain:  return gainChain;
            case DelayChain: return delayChain;
            case WidthChain: return widthChain;
			case BalanceChain: return balanceChain;
			default:		 return nullptr;
        }
    };
    
	int getNumInternalChains() const override { return numInternalChains; };
	int getNumChildProcessors() const override { return numInternalChains; };

	ProcessorEditorBody *createEditor(ProcessorEditor *parentEditor)  override;

	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void applyEffect(AudioSampleBuffer &b, int startSample, int numSamples) override;

    void setDelayTime(float newDelayInMilliseconds)
    {
        delay = newDelayInMilliseconds;
        leftDelay.setDelayTimeSeconds(delay/1000.0f);
        rightDelay.setDelayTimeSeconds(delay/1000.0f);
    }
    
private:

	float gain;
    float delay;
	float balance;

	ModulatorChain* gainChain;
    ModulatorChain* delayChain;
    ModulatorChain* widthChain;
	ModulatorChain* balanceChain;

	LinearSmoothedValue<float> smoothedGainL;
	LinearSmoothedValue<float> smoothedGainR;
	Smoother smoother;
	Smoother balanceSmoother;

    MidSideDecoder msDecoder;
    
    DelayLine<16384> leftDelay;
    DelayLine<16384> rightDelay;
  
	JUCE_DECLARE_WEAK_REFERENCEABLE(GainEffect)
};

} // namespace hise

#endif  // GAINEFFECT_H_INCLUDED
