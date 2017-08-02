/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"







//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);

	addAndMakeVisible(rc = new RxComponent());
	//	addAndMakeVisible(som = new Som());
}

MainContentComponent::~MainContentComponent()
{
	rc = nullptr;
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
	if(rc)
		rc->setBounds(0, 0, 496, 400);
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

}
