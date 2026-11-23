
#include "DXLookNFeel.h"

#define REG_COLOUR(id, value) setColour(id, value); colourMap.set(#id, id)


DXLookNFeel::DXLookNFeel() {

    REG_COLOUR(TextButton::buttonColourId, secondaryText);
    REG_COLOUR(TextButton::textColourOnId, textColour);
    REG_COLOUR(TextButton::textColourOffId, textColour);

    REG_COLOUR(Slider::rotarySliderOutlineColourId, secondaryText);
    REG_COLOUR(Slider::rotarySliderFillColourId, fillColour);
    REG_COLOUR(Slider::thumbColourId, fillColour);
    REG_COLOUR(Slider::trackColourId, secondaryText);

    REG_COLOUR(Label::textColourId, textColour);

    REG_COLOUR(ToggleButton::textColourId, textColour);
    REG_COLOUR(ToggleButton::tickColourId, fillColour);
    REG_COLOUR(ToggleButton::tickDisabledColourId, secondaryText);

    REG_COLOUR(ComboBox::backgroundColourId, background);
    REG_COLOUR(ComboBox::textColourId, textColour);
    //REG_COLOUR(ComboBox::buttonColourId, secondaryText);
    REG_COLOUR(ComboBox::outlineColourId, borderOutline);

    REG_COLOUR(TextEditor::backgroundColourId, lightBackground);
    REG_COLOUR(TextEditor::textColourId, textColour);
    REG_COLOUR(TextEditor::highlightColourId, fillColour);
    REG_COLOUR(TextEditor::outlineColourId, Colour(0xFF414957));

    REG_COLOUR(PopupMenu::backgroundColourId, lightBackground);
    REG_COLOUR(PopupMenu::textColourId, textColour);
    REG_COLOUR(PopupMenu::highlightedTextColourId, textColour);
    REG_COLOUR(PopupMenu::highlightedBackgroundColourId, fillColour);

    REG_COLOUR(AlertWindow::backgroundColourId, lightBackground);
    REG_COLOUR(AlertWindow::textColourId, textColour);

    REG_COLOUR(DialogWindow::backgroundColourId, background);

    REG_COLOUR(TreeView::backgroundColourId, background);

    REG_COLOUR(ListBox::backgroundColourId, lightBackground);

    REG_COLOUR(DirectoryContentsDisplayComponent::highlightColourId, fillColour);

    REG_COLOUR(DirectoryContentsDisplayComponent::textColourId, textColour);

    REG_COLOUR(ScrollBar::thumbColourId, secondaryText);
}

void DXLookNFeel::loadTheme(const juce::File& themeFile) {
    if (!themeFile.existsAsFile())
        return;

    auto parsed = juce::JSON::parse(themeFile);
    if (auto* obj = parsed.getDynamicObject()) {
        auto readCol = [obj](const juce::Identifier& key, juce::Colour& target) {
            if (obj->hasProperty(key)) {
                // Strip leading '#' if user wrote "#FF16161B" instead of "FF16161B"
                auto hex = obj->getProperty(key).toString().trimCharactersAtStart("#");
                if (hex.length() == 6)
                    hex = "FF" + hex;  // Allow standard 6-digit RGB hex too
                target = juce::Colour::fromString(hex);
            }
        };

        readCol("background", background);
        readCol("lightBackground", lightBackground);
        readCol("fillColour", fillColour);
        readCol("textColour", textColour);
        readCol("secondaryText", secondaryText);
        readCol("borderOutline", borderOutline);
    }
}

void DXLookNFeel::saveDefaultThemeIfMissing(const juce::File& themeFile) {
    if (themeFile.existsAsFile())
        return;

    auto* obj = new juce::DynamicObject();
    obj->setProperty("background", "#" + background.toDisplayString(true));
    obj->setProperty("lightBackground", "#" + lightBackground.toDisplayString(true));
    obj->setProperty("fillColour", "#" + fillColour.toDisplayString(true));
    obj->setProperty("textColour", "#" + textColour.toDisplayString(true));
    obj->setProperty("secondaryText", "#" + secondaryText.toDisplayString(true));
    obj->setProperty("borderOutline", "#" + borderOutline.toDisplayString(true));

    themeFile.replaceWithText(juce::JSON::toString(juce::var(obj), false));
}



Typeface::Ptr DXLookNFeel::getTypefaceForFont(const Font &font) {
    if (font.getTypefaceName() == Font::getDefaultSansSerifFontName())
        return Typeface::createSystemTypefaceFor(BinaryData::NotoSansRegular_ttf, BinaryData::NotoSansRegular_ttfSize);

    return LookAndFeel_V4::getTypefaceForFont(font);
}


void DXLookNFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, const float rotaryStartAngle,
                                   const float rotaryEndAngle, juce::Slider& slider) {
    constexpr float baseDiameter = 20.0f;
    float scale = 1.0f;
    if (auto* topLevel = slider.getTopLevelComponent()) {
        constexpr float refWidth = 860.0f;
        constexpr float refHeight = 520.0f;
        scale = juce::jmin((float)topLevel->getWidth() / refWidth,
                           (float)topLevel->getHeight() / refHeight);
    }

    float diameter = baseDiameter * scale;
    diameter = juce::jmin(diameter, (float)juce::jmin(width, height) - 6.0f);

    const float radius = diameter * 0.5f;
    const float centreX = (float)x + (float)width * 0.5f;
    const float centreY = (float)y + (float)height * 0.5f;

    const float bgPadding = 3.5f * scale;
    const float bgDiameter = diameter + (bgPadding * 1.5f);
    auto bgArea = juce::Rectangle<float>(centreX - bgDiameter * 0.5f, centreY - bgDiameter * 0.5f,
                                         bgDiameter, bgDiameter);

    auto dialArea = juce::Rectangle<float>(centreX - radius, centreY - radius, diameter, diameter);

    g.setColour(DXLookNFeel::secondaryText.withAlpha(0.25f));

    const float arcRadius = radius - 1.5f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f, rotaryStartAngle,
                                rotaryEndAngle, true);

    g.setColour(DXLookNFeel::secondaryText.withAlpha(0.25f));
    g.strokePath(backgroundArc, juce::PathStrokeType(1.5f * scale, juce::PathStrokeType::curved,
                                                     juce::PathStrokeType::rounded));

    const float currentAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    if (currentAngle > rotaryStartAngle) {
        juce::Path valueArc;
        const float arcRadius = radius - 1.5f;
        valueArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f, rotaryStartAngle,
                               currentAngle, true);

        g.setColour(DXLookNFeel::secondaryText.withAlpha(0.75f));
        g.strokePath(valueArc, juce::PathStrokeType(1.5f * scale, juce::PathStrokeType::curved,
                                                    juce::PathStrokeType::rounded));
    }

    juce::Path dialTick;
    const float tickWidth = juce::jmax(1.5f, radius * 0.12f);
    const float tickLength = radius * 0.75f;

    dialTick.addRoundedRectangle(centreX - (tickWidth * 0.5f), centreY - radius + 1.0f, tickWidth,
                                 tickLength, 0.8f);

    auto transform = juce::AffineTransform::rotation(currentAngle, centreX, centreY);

    g.setColour(DXLookNFeel::secondaryText);
    g.fillPath(dialTick, transform);
}

void DXLookNFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, float minSliderPos, float maxSliderPos,
                                   const juce::Slider::SliderStyle style, juce::Slider& slider) {
    using namespace juce;

    auto bounds = Rectangle<float>((float)x, (float)y, (float)width, (float)height);

    if (style == Slider::LinearHorizontal) {
        const float cy = bounds.getCentreY();

        Rectangle<float> rail(bounds.getX() + 5.0f, cy - 2.0f, bounds.getWidth() - 10.0f, 4.0f);

        g.setColour(DXLookNFeel::borderOutline);
        g.fillRoundedRectangle(rail, 2.0f);

        g.setColour(DXLookNFeel::fillColour);
        g.fillRoundedRectangle(rail.withWidth(jmax(0.0f, sliderPos - rail.getX())), 2.0f);

        g.setColour(DXLookNFeel::textColour);
        g.fillEllipse(sliderPos - 5.0f, cy - 5.0f, 10.0f, 10.0f);
    } else {
        LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos,
                                         maxSliderPos, style, slider);
    }
}





void DXLookNFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                   bool isMouseOverButton,
                      bool isButtonDown) {
    auto bounds = button.getLocalBounds().toFloat();

    const bool enabled = button.getToggleState();

    const float height = juce::jmin(bounds.getHeight() * 0.7f, bounds.getWidth() * 0.45f);

    const float width = juce::jmin(bounds.getWidth() * 0.9f, height * 1.85f);

    auto track = juce::Rectangle<float>(bounds.getCentreX() - width * 0.5f,
                                        bounds.getCentreY() - height * 0.5f, width, height);

    juce::Colour trackColour = enabled ? DXLookNFeel::fillColour : DXLookNFeel::background;

    g.setColour(trackColour);
    g.fillRoundedRectangle(track, height * 0.5f);

    const float thumbSize = height * 0.72f;
    const float inset = (height - thumbSize) * 0.5f;

    const float thumbX = enabled ? track.getRight() - thumbSize - inset : track.getX() + inset;

    juce::Rectangle<float> thumb(thumbX, track.getY() + inset, thumbSize, thumbSize);

    g.setColour(enabled? DXLookNFeel::background : DXLookNFeel::textColour);
    g.fillEllipse(thumb);
}




void DXLookNFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                          const juce::Colour& backgroundColour, bool isHighlighted,
                          bool isDown) {
    auto bounds = button.getLocalBounds().reduced(5,0).toFloat();

    g.setColour(DXLookNFeel::lightBackground);
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(DXLookNFeel::borderOutline);
    g.drawRoundedRectangle(bounds.reduced(0.5f), 4.0f, 1.0f);
}

void DXLookNFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isHighlighted,
                    bool isDown) {
    auto bounds = button.getLocalBounds();

    g.setColour(DXLookNFeel::textColour);

    g.setFont(juce::Font(13.0f, juce::Font::bold));

    g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
}
