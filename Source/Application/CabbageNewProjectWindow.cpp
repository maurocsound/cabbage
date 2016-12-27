/*
  Copyright (C) 2016 Rory Walsh

  Cabbage is free software; you can redistribute it
  and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  Cabbage is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with Csound; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
*/

#include "CabbageNewProjectWindow.h"
#include "CabbageContentComponent.h"
#include "../Utilities/CabbageStrings.h"
#include "../Utilities/CabbageUtilities.h"


CabbageProjectWindow::CabbageProjectWindow(CabbageContentComponent* owner, ValueTree valueTree):
    owner(owner),
    valueTree(valueTree),
    newInstrumentButton("newInstrument", this),
    newEffectButton("newEffect", this),
    newCsoundFileButton("newCsound", this)
{
    addAndMakeVisible(newCsoundFileButton);
    addAndMakeVisible(newInstrumentButton);
    addAndMakeVisible(newEffectButton);
    newCsoundFileButton.addListener(this);
    newInstrumentButton.addListener(this);
    newEffectButton.addListener(this);

    //Image instrumentImage = ImageFileFormat::loadFrom(File("Normal.png"));
    const Image instrumentImage = ImageCache::getFromMemory (CabbageBinaryData::InstrumentButton_png, CabbageBinaryData::InstrumentButton_pngSize);
    CabbageUtilities::setImagesForButton(&newInstrumentButton, instrumentImage);

    const Image effectImage = ImageCache::getFromMemory (CabbageBinaryData::EffectButton_png, CabbageBinaryData::EffectButton_pngSize);
    CabbageUtilities::setImagesForButton(&newEffectButton, effectImage);

    const Image csoundImage = ImageCache::getFromMemory (CabbageBinaryData::CsoundFileButton_png, CabbageBinaryData::CsoundFileButton_pngSize);
    CabbageUtilities::setImagesForButton(&newCsoundFileButton, csoundImage);


}

void CabbageProjectWindow::writeNewFile(File fc, String fileText)
{
	fc.replaceWithText(fileText);
	owner->openFile (fc.getFullPathName());
	delete this->getParentComponent();	
}

void CabbageProjectWindow::createNewFile(String type)
{
	
	FileChooser fc ("Select file name and location", File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory));

	if (fc.browseForFileToSave(false))
	{
		String csdText;
		if(type=="newCsound")
			csdText = CabbageStrings::getNewCsoundFileText();
		else if(type=="newEffect")
			csdText = CabbageStrings::getNewCabbageEffectFileText();
		else
			csdText = CabbageStrings::getNewCabbageInstrumentFileText();
			
		if(fc.getResult().existsAsFile())
		{
			CabbageIDELookAndFeel lookAndFeel;
			const int result = CabbageUtilities::showYesNoMessage("Do you wish to overwrite\nexiting file?", &lookAndFeel);
			if(result==0)
			{
				writeNewFile(fc.getResult(), csdText);
//				
//				fc.getResult().replaceWithText(CabbageStrings::getNewCsoundFileText());
//				owner->openFile (fc.getResult().getFullPathName());
//				delete this->getParentComponent();
			}
		}
		else
		{
				writeNewFile(fc.getResult(), csdText);
//			fc.getResult().replaceWithText(CabbageStrings::getNewCsoundFileText());
//			owner->openFile (fc.getResult().getFullPathName());
//			delete this->getParentComponent();
		}
	}	
}

void CabbageProjectWindow::buttonClicked(Button* button)
{
	createNewFile(button->getName());
}