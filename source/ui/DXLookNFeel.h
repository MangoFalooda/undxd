
#ifndef DXLOOKNFEEL_H_INCLUDED
#define DXLOOKNFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LightedToggleButton : public ToggleButton {
public:
    LightedToggleButton(juce::String name) : ToggleButton(name) {}
};

class DXLookNFeel : public LookAndFeel_V4 {
    HashMap<String, int> colourMap;

public:
    DXLookNFeel();

    Typeface::Ptr getTypefaceForFont(const Font&) override;



    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                       bool isMouseOverButton, bool isButtonDown) override;
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                     bool isHighlighted, bool isDown) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                           const juce::Colour& backgroundColour, bool isHighlighted,
                                           bool isDown) override;


    //static inline const juce::Colour background{0xFF16161B};       
    //static inline const juce::Colour lightBackground{0xFF212028}; 
    //static inline const juce::Colour fillColour{0xFF9999FF};  
    //static inline const juce::Colour textColour{0xFFF0F1F5};     
    //static inline const juce::Colour secondaryText{0xFF8E8D9E};  
    //static inline const juce::Colour borderOutline{0xFF353443}; 

    //static inline const juce::Colour background{0xFF121315};
    //static inline const juce::Colour lightBackground{0xFF1D1F22};
    //static inline const juce::Colour fillColour{0xFF38A8FF};
    //static inline const juce::Colour textColour{0xFFE8E8EA};
    //static inline const juce::Colour secondaryText{0xFF92969D};
    //static inline const juce::Colour borderOutline{0xFF303338};

    static inline juce::Colour background{0xFF16161B};
    static inline juce::Colour lightBackground{0xFF212028};
    static inline juce::Colour fillColour{0xFF9999FF};
    static inline juce::Colour textColour{0xFFF0F1F5};
    static inline juce::Colour secondaryText{0xFF8E8D9E};
    static inline juce::Colour borderOutline{0xFF353443};

    static void loadTheme(const juce::File& themeFile);
    static void saveDefaultThemeIfMissing(const juce::File& themeFile);
  
};



///////////////////////////

class LogoButtonLAF : public juce::LookAndFeel_V4 {
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour, bool isHighlighted,
                              bool isDown) override {
        // no background
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isHighlighted,
                        bool isDown) override {
        auto bounds = button.getLocalBounds().toFloat();

        float fontSize = bounds.getHeight() * 0.72f;
        auto font = juce::Font(juce::FontOptions(fontSize, juce::Font::bold)).withExtraKerningFactor(0.12f);
        g.setFont(font);
        g.setColour(DXLookNFeel::textColour);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centred, false);
    }
};




/////////////////////////////

class DXMidiKeyboard : public juce::MidiKeyboardComponent {
public:
    DXMidiKeyboard(juce::MidiKeyboardState& state, Orientation orientation)
        : MidiKeyboardComponent(state, orientation) {

        setColour(whiteNoteColourId, juce::Colour(0xFFE2E5E9));
        setColour(blackNoteColourId, juce::Colour(0xFF252A32));
        setColour(keySeparatorLineColourId, juce::Colour(0xFF343941));
        setColour(mouseOverKeyOverlayColourId, juce::Colour(0x2239C6B0));
        setColour(keyDownOverlayColourId, juce::Colour(0xFF39C6B0));
        setColour(textLabelColourId, juce::Colour(0xFF606875));
        setColour(shadowColourId, juce::Colours::transparentBlack);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colour(0xFF171A20));
        juce::MidiKeyboardComponent::paint(g);
    }

    void drawWhiteNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area,
                       bool isDown, bool isOver, juce::Colour lineColour,
                       juce::Colour textColour) override {

        auto colour = isDown ? DXLookNFeel::fillColour : juce::Colour(0xFFE2E5E9);

        if (isOver && !isDown)
            colour = DXLookNFeel::fillColour;

        g.setColour(colour);
        g.fillRect(area);

        g.setColour(juce::Colour(0xFF343941));
        g.fillRect(area.getRight() - 0.7f, area.getY(), 0.7f, area.getHeight());

        auto note = getWhiteNoteText(midiNoteNumber);

        if (note.isNotEmpty()) {
            g.setColour(juce::Colour(0xFF606875));
            g.setFont(10.0f);
            g.drawText(note, area.withTrimmedBottom(3.0f), juce::Justification::centredBottom);
        }
    }

    void drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area,
                       bool isDown, bool isOver, juce::Colour noteFillColour) override {

        auto colour = isDown ? DXLookNFeel::fillColour : juce::Colour(0xFF252A32);

        if (isOver && !isDown) colour = DXLookNFeel::fillColour;
        g.setColour(colour);
        auto blackKeyBounds = area.withTop(area.getY() - 6.0f);
        g.fillRoundedRectangle(blackKeyBounds, 4.0f);
    }
};





//////////////////////////////////////


class OperatorPowerLookAndFeel : public juce::LookAndFeel_V4 {
public:

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool highlighted,
                          bool pressed) override {
        auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);
        const bool active = button.getToggleState();
        const auto colour = active ? DXLookNFeel::fillColour : DXLookNFeel::borderOutline;

      const float radius = bounds.getWidth() * 0.28f;

        g.setColour(colour);
        g.drawEllipse(bounds.getCentreX() - radius, bounds.getCentreY() - radius, radius * 2.0f,
                      radius * 2.0f, 2.0f);

        g.setFont(juce::Font(16.0f, juce::Font::bold));
        g.drawText(button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred);
    }
};



class OperatorModeLookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool highlighted,
                          bool pressed) override {
        auto bounds = button.getLocalBounds();

        const bool fixed = button.getToggleState();

        g.setColour(DXLookNFeel::background);
        g.fillRoundedRectangle(bounds.toFloat(), 5.0f);

        auto top = bounds.removeFromTop(bounds.getHeight() / 2);
        auto bottom = bounds;

        g.setFont(juce::Font(10.0f, juce::Font::bold));

        // ratio
        g.setColour(!fixed ? DXLookNFeel::fillColour : DXLookNFeel::background);

        if (!fixed)
            g.fillRoundedRectangle(top.toFloat().reduced(2), 3.0f);

        g.setColour(!fixed ? DXLookNFeel::background : DXLookNFeel::textColour);

        g.drawText("RATIO", top, juce::Justification::centred);

        // fixed
        if (fixed) {
            g.setColour(DXLookNFeel::fillColour);
            g.fillRoundedRectangle(bottom.toFloat().reduced(2), 3.0f);
        }

        g.setColour(fixed ? DXLookNFeel::background : DXLookNFeel::textColour);

        g.drawText("FIXED", bottom, juce::Justification::centred);
    }
};





/////////////////////////////////////////////////

struct ScalingCurveDrawer {
    static void drawCurve(juce::Graphics& g, int curveIndex, juce::Rectangle<float> bounds,
                          juce::Colour colour, bool isRightCurve, float strokeThickness = 1.8f) {
        auto r = bounds.reduced(bounds.getWidth() * 0.18f, bounds.getHeight() * 0.18f);
        juce::Path path;


        if (!isRightCurve) {
            switch (curveIndex) {
            case 0:  // -LIN
                path.startNewSubPath(r.getX(), r.getBottom());
                path.lineTo(r.getRight(), r.getY());
                break;
            case 1:  // -EXP
                path.startNewSubPath(r.getRight(), r.getY());
                path.quadraticTo(r.getX(), r.getY(), r.getX(), r.getBottom());
                break;
            case 2:  // +EXP
                path.startNewSubPath(r.getX(), r.getY());
                path.quadraticTo(r.getX(), r.getBottom(), r.getRight(), r.getBottom());
                break;
            case 3:  // +LIN
                path.startNewSubPath(r.getX(), r.getY());
                path.lineTo(r.getRight(), r.getBottom());
                break;
            default:
                break;
            }
        } else {

            switch (curveIndex) {
            case 0: 
                path.startNewSubPath(r.getX(), r.getY());
                path.lineTo(r.getRight(), r.getBottom());
                
                break;
            case 1: 
                path.startNewSubPath(r.getRight(), r.getBottom());
                path.quadraticTo(r.getRight(), r.getY(), r.getX(), r.getY());
                break;
            case 2: 
                path.startNewSubPath(r.getX(), r.getBottom());
                path.quadraticTo(r.getRight(), r.getBottom(), r.getRight(), r.getY());
                
                break;
            case 3: 
                path.startNewSubPath(r.getX(), r.getBottom());
                path.lineTo(r.getRight(), r.getY());
                
                break;
            default:
                break;
            }
        }

        g.setColour(colour);
        g.strokePath(path, juce::PathStrokeType(strokeThickness, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));
    }
};

class CurveMenuItem : public juce::PopupMenu::CustomComponent {
public:
    CurveMenuItem(int curveIdx, const juce::String& labelText, bool isRightSide)
        : juce::PopupMenu::CustomComponent(true), index(curveIdx), text(labelText),
          isRight(isRightSide) {}

    void getIdealSize(int& width, int& height) override {
        width = 80;
        height = 22;
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        bool isHighlighted = isItemHighlighted();

        g.setColour(DXLookNFeel::background);
        g.fillRect(bounds);

        auto iconArea = bounds.removeFromLeft(bounds.getHeight()).reduced(3.0f);
        ScalingCurveDrawer::drawCurve(g, index, iconArea, DXLookNFeel::fillColour,
            isRight, 1.6f);

        g.setColour(DXLookNFeel::secondaryText);
        g.setFont(juce::Font(juce::FontOptions(10.0f)));
        g.drawText(text, bounds.reduced(4.0f, 0.0f).toNearestInt(),
                   juce::Justification::centredLeft, true);
    }

private:
    int index;
    juce::String text;
    bool isRight;
};

class CurveComboLookAndFeel : public DXLookNFeel {
public:
    CurveComboLookAndFeel(bool rightSide = false) : isRight(rightSide) {}

    void positionComboBoxText(juce::ComboBox& /*box*/, juce::Label& label) override {
        label.setBounds(0, 0, 0, 0);
        label.setVisible(false);
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                      int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                      juce::ComboBox& box) override {
        auto bounds = box.getLocalBounds().toFloat();

        g.setColour(DXLookNFeel::background);
        g.fillRoundedRectangle(bounds, 2.0f);
       
        int selectedIndex = box.getSelectedId() - 1;
        if (selectedIndex < 0)
            selectedIndex = box.getSelectedItemIndex();
        if (selectedIndex < 0)
            selectedIndex = 0;

        auto curveArea = bounds.reduced(2.0f);
        ScalingCurveDrawer::drawCurve(g, selectedIndex, curveArea, DXLookNFeel::fillColour,
                                      isRight, 1.8f);
    }

private:
    bool isRight = false;
};













////////////////////////////////////////////

struct LfoWaveformDrawer {
    static void drawWaveform(juce::Graphics& g, int waveIndex, juce::Rectangle<float> bounds,
                             juce::Colour colour, float strokeThickness = 1.6f) {
        auto r = bounds.reduced(bounds.getWidth() * 0.12f, bounds.getHeight() * 0.16f);
        const float midY = r.getCentreY();
        const float topY = r.getY();
        const float botY = r.getBottom();
        const float midX = r.getCentreX();

        juce::Path path;

        switch (waveIndex) {
        case 0:  
            path.startNewSubPath(r.getX(), midY);
            path.lineTo(r.getX() + r.getWidth() * 0.25f, topY);
            path.lineTo(r.getX() + r.getWidth() * 0.75f, botY);
            path.lineTo(r.getRight(), midY);
            break;

        case 1:  
            path.startNewSubPath(r.getX(), topY + (midY - topY) * 0.5f);
            path.lineTo(midX, botY);
            path.lineTo(midX, topY);
            path.lineTo(r.getRight(), midY + (botY - midY) * 0.5f);
            break;

        case 2:  
            path.startNewSubPath(r.getX(), botY - (botY - midY) * 0.5f);
            path.lineTo(midX, topY);
            path.lineTo(midX, botY);
            path.lineTo(r.getRight(), midY - (midY - topY) * 0.5f);
            break;

        case 3:  
            path.startNewSubPath(r.getX(), botY);
            path.lineTo(r.getX(), topY);
            path.lineTo(midX, topY);
            path.lineTo(midX, botY);
            path.lineTo(r.getRight(), botY);
            path.lineTo(r.getRight(), topY);
            break;

        case 4: 
        {
            path.startNewSubPath(r.getX(), midY);
            const float quarterW = r.getWidth() * 0.25f;
            path.cubicTo(r.getX() + quarterW * 0.5f, topY, r.getX() + quarterW * 1.5f, topY, midX,
                         midY);
            path.cubicTo(midX + quarterW * 0.5f, botY, midX + quarterW * 1.5f, botY, r.getRight(),
                         midY);
            break;
        }

        case 5: 
            path.startNewSubPath(r.getX(), midY);
            path.lineTo(r.getX() + r.getWidth() * 0.18f, midY);
            path.lineTo(r.getX() + r.getWidth() * 0.18f, botY);
            path.lineTo(r.getX() + r.getWidth() * 0.38f, botY);
            path.lineTo(r.getX() + r.getWidth() * 0.38f, midY + (botY - midY) * 0.4f);
            path.lineTo(r.getX() + r.getWidth() * 0.58f, midY + (botY - midY) * 0.4f);
            path.lineTo(r.getX() + r.getWidth() * 0.58f, topY);
            path.lineTo(r.getX() + r.getWidth() * 0.78f, topY);
            path.lineTo(r.getX() + r.getWidth() * 0.78f, midY);
            path.lineTo(r.getRight(), midY);
            break;

        default:
            break;
        }

        g.setColour(colour);
        g.strokePath(path, juce::PathStrokeType(strokeThickness, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));
    }
};

class LfoMenuItem : public juce::PopupMenu::CustomComponent {
public:
    LfoMenuItem(int waveIdx, const juce::String& labelText)
        : juce::PopupMenu::CustomComponent(true), index(waveIdx), text(labelText) {}

    void getIdealSize(int& width, int& height) override {
        width = 85;
        height = 22;
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        bool isHighlighted = isItemHighlighted();

        g.setColour(isHighlighted ? DXLookNFeel::lightBackground : DXLookNFeel::background);
        g.fillRect(bounds);

        auto iconArea = bounds.removeFromLeft(bounds.getHeight()).reduced(3.0f);
        LfoWaveformDrawer::drawWaveform(
            g, index, iconArea, DXLookNFeel::fillColour,
            1.5f);

        g.setColour(DXLookNFeel::secondaryText);
        g.setFont(juce::Font(juce::FontOptions(10.0f)));
        g.drawText(text, bounds.reduced(4.0f, 0.0f).toNearestInt(),
                   juce::Justification::centredLeft, true);
    }

private:
    int index;
    juce::String text;
};

class LfoComboLookAndFeel : public DXLookNFeel {
public:
    LfoComboLookAndFeel() = default;

    void positionComboBoxText(juce::ComboBox&, juce::Label& label) override {
        label.setBounds(0, 0, 0, 0);
        label.setVisible(false);
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool /*isButtonDown*/,
                      int /*buttonX*/, int /*buttonY*/, int /*buttonW*/, int /*buttonH*/,
                      juce::ComboBox& box) override {
        auto bounds = box.getLocalBounds().toFloat();

        g.setColour(DXLookNFeel::background);
        g.fillRoundedRectangle(bounds, 2.0f);
       
        int selectedIndex = box.getSelectedId() - 1;
        if (selectedIndex < 0)
            selectedIndex = box.getSelectedItemIndex();
        if (selectedIndex < 0)
            selectedIndex = 0;

        auto waveArea = bounds.reduced(2.0f);
        LfoWaveformDrawer::drawWaveform(g, selectedIndex, waveArea, DXLookNFeel::fillColour, 1.8f);
    }
};


///////////////////////////////////////


class SimpleComboBoxLaf : public juce::LookAndFeel_V4 {
public:
    SimpleComboBoxLaf() = default;

    juce::Font getPopupMenuFont() override { return juce::Font(juce::FontOptions(11.0f)); }

    void getIdealPopupMenuItemSize(const juce::String& text, bool isSeparator,
                                   int standardMenuItemHeight, int& idealWidth,
                                   int& idealHeight) override {
        juce::LookAndFeel_V4::getIdealPopupMenuItemSize(text, isSeparator, standardMenuItemHeight,
                                                        idealWidth, idealHeight);

        idealHeight = isSeparator ? 6 : 18;
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX,
                      int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override {
        auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat();

        setColour(juce::PopupMenu::backgroundColourId, DXLookNFeel::background);

        g.setColour(DXLookNFeel::lightBackground);
        g.fillRoundedRectangle(bounds, 4.0f);

        g.setColour(DXLookNFeel::borderOutline);
        g.drawRoundedRectangle(bounds.reduced(0.5f), 4.0f, 1.0f);

        auto arrowBox = bounds.removeFromRight(18.0f).reduced(3.0f, 4.0f);
        float arrowW = 6.0f;
        float arrowH = 4.0f;
        float cx = arrowBox.getCentreX();

        juce::Colour arrowColour = box.findColour(juce::ComboBox::arrowColourId);
        if (arrowColour.isTransparent())
            arrowColour = juce::Colour(0xFF9299A5);

        g.setColour(arrowColour);

        juce::Path upArrow;
        float upY = arrowBox.getY() + 1.0f;
        upArrow.addTriangle(cx, upY, cx - (arrowW * 0.5f), upY + arrowH, cx + (arrowW * 0.5f),
                            upY + arrowH);
        g.fillPath(upArrow);

        juce::Path downArrow;
        float downY = arrowBox.getBottom() - 1.0f;
        downArrow.addTriangle(cx, downY, cx - (arrowW * 0.5f), downY - arrowH, cx + (arrowW * 0.5f),
                              downY - arrowH);
        g.fillPath(downArrow);
    }

    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override {
        label.setBounds(4, 1, box.getWidth() - 22, box.getHeight() - 2);
        label.setFont(juce::Font(juce::FontOptions(14.0f)));
        label.setJustificationType(juce::Justification::centredLeft);
    }
};




class StepperComboBox : public juce::ComboBox {
public:
    using juce::ComboBox::ComboBox;

    void mouseDown(const juce::MouseEvent& e) override {
        const int arrowZoneWidth = 18;

        if (e.x >= getWidth() - arrowZoneWidth) {
            int numItems = getNumItems();
            if (numItems <= 1)
                return;

            int currentIndex = getSelectedItemIndex();

            if (e.y < getHeight() / 2)
            {
                int newIndex = (currentIndex <= 0) ? (numItems - 1) : (currentIndex - 1);
                setSelectedItemIndex(newIndex);
            } else 
            {
                int newIndex = (currentIndex >= numItems - 1) ? 0 : (currentIndex + 1);
                setSelectedItemIndex(newIndex);
            }
            return;
        }

        juce::ComboBox::mouseDown(e);
    }
};


/////////////////////////////////////////////////////////

class LayoutToggleLAF : public juce::LookAndFeel_V4 {
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override {
        //auto bounds = button.getLocalBounds().toFloat();

        //auto colour = backgroundColour;

        //if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
        //    colour = colour.brighter(0.15f);

        //g.setColour(colour);
        //g.fillRoundedRectangle(bounds, 4.0f);
    }

    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override {
        // No text.
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override {
        auto bounds = button.getLocalBounds().toFloat();

        const float iconSize = 16.0f;
        auto iconBounds = bounds.withSizeKeepingCentre(iconSize, iconSize);

        auto colour = DXLookNFeel::secondaryText;

        //if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
        //    colour = colour.brighter(0.2f);

        g.setColour(colour);

        if (!button.getToggleState()) {
            // Grid mode: 4 rectangles
            const float gap = 2.0f;
            const float cell = (iconSize - gap) / 2.0f;

            for (int row = 0; row < 2; ++row) {
                for (int col = 0; col < 2; ++col) {
                    g.fillRoundedRectangle(iconBounds.getX() + col * (cell + gap),
                                           iconBounds.getY() + row * (cell + gap), cell, cell,
                                           1.0f);
                }
            }
        } else {
            // Row mode: 2 thick horizontal bars
            const float barHeight = 5.0f;
            const float gap = 4.0f;

            for (int i = 0; i < 2; ++i) {
                g.fillRoundedRectangle(iconBounds.getX(), iconBounds.getY() + i * (barHeight + gap),
                                       iconSize, barHeight, 1.5f);
            }
        }
    }
};

///////////////////////////////////////////////


#endif
